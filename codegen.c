#include "cycc.h"

// if文のジャンプ先のラベルをユニークにする為のカウンタ
int jmp_label_count;

// 変数を左辺値として評価する
void gen_lval(Node *node) {
  if (node->ty == ND_DEREF) {
    gen(node->rhs);

    // ２つ以上のデリファレンス'*'
    for (int n = node->deref - 1; n > 0; n--) {
      printf("  pop rax\n");
      printf("  mov rax, [rax]\n");
      printf("  push rax\n");
    }
    return;
  }

  if (node->ty != ND_IDENT)
    error("代入の左辺値が変数ではありません");

  char *var_name = node->name;

  // 配列の為のポインタ型への変換
  Type *t = map_get(var_map, var_name);
  int var_offset;
  if (t->ty == PTR) {
    int ptr_offset = t->offset;
    t = t->ptrof;
    if (t->ty == ARRAY) {
      printf("  lea rax, [rbp-%d]\n", t->offset);
      printf("  push rax\n");
      printf("  lea rax, [rbp-%d]\n", ptr_offset);
      printf("  pop rdi\n");
      printf("  mov [rax], rdi\n");
    }
    var_offset = ptr_offset;
  } else {
    var_offset = t->offset;
  }

  printf("  mov rax, rbp\n");
  printf("  sub rax, %d\n", var_offset);
  printf("  push rax\n");
}

//　レジスタマシンでスタックマシンをエミュレートし、コンパイルする
void gen(Node *node) {
  if (node->ty == ND_NUM) {
    printf("  push %d\n", node->val);
    return;
  }

  if (node->ty == ND_BLOCK) {
    for (int i = 0; i < node->block->len; i++) {
      gen((Node *)node->block->data[i]);
      printf("  pop rax\n");
    }
    return;
  }

  if (node->ty == ND_RETURN) {
    gen(node->lhs);
    printf("  pop rax\n");
    printf("  mov rsp, rbp\n");
    printf("  pop rbp\n");
    printf("  ret\n");
    return;
  }

  if (node->ty == ND_IF) {
    gen(node->lhs);
    printf("  pop rax\n");
    printf("  cmp rax, 0\n");
    printf("  je .Lend%03d\n", jmp_label_count);
    gen(node->rhs);
    printf(".Lend%03d:\n", jmp_label_count);
    jmp_label_count++;
    return;
  }

  if (node->ty == ND_IFELSE) {
    int else_label = jmp_label_count++;
    int end_label = jmp_label_count++;

    gen(node->lhs);
    printf("  pop rax\n");
    printf("  cmp rax, 0\n");
    printf("  je .Lelse%03d\n", else_label);
    gen(node->rhs->lhs);
    printf("  jmp .Lend%03d\n", end_label);
    printf(".Lelse%03d:\n", else_label);
    gen(node->rhs->rhs);
    printf(".Lend%03d:\n", end_label);
    return;
  }

  if (node->ty == ND_WHIL) {
    int begin_label = jmp_label_count++;
    int end_label = jmp_label_count++;

    printf(".Lbegin%03d:\n", begin_label);
    gen(node->lhs);
    printf("  pop rax\n");
    printf("  cmp rax, 0\n");
    printf("  je .Lend%03d\n", end_label);
    gen(node->rhs);
    printf("  jmp .Lbegin%03d\n", begin_label);
    printf(".Lend%03d:\n", end_label);
    return;
  }

  if (node->ty == ND_FOR) {
    int begin_label = jmp_label_count++;
    int end_label = jmp_label_count++;

    gen(node->lhs->lhs);
    printf(".Lbegin%03d:\n", begin_label);
    gen(node->lhs->rhs);
    printf("  pop rax\n");
    printf("  cmp rax, 0\n");
    printf("  je .Lend%03d\n", end_label);
    gen(node->rhs->rhs);
    gen(node->rhs->lhs);
    printf("  jmp .Lbegin%03d\n", begin_label);
    printf(".Lend%03d:\n", end_label);
    return;
  }

  if (node->ty == ND_DEREF) {
    gen(node->rhs);
    printf("  pop rax\n");
    printf("  mov rax, [rax]\n");
    printf("  push rax\n");

    // ２つ以上のデリファレンス'*'
    for (int n = node->deref - 1; n > 0; n--) {
      printf("  pop rax\n");
      printf("  mov rax, [rax]\n");
      printf("  push rax\n");
    }
    return;
  }

  if (node->ty == ND_ADDRESS) {
    gen_lval(node->rhs);
    return;
  }

  if (node->ty == ND_IDENT) {
    gen_lval(node);
    printf("  pop rax\n");
    printf("  mov rax, [rax]\n");
    printf("  push rax\n");
    return;
  }

  if (node->ty == ND_FUNC) {
    char *func_name = node->name;
    if (node->args->len != 0) {
      int else_label_before = jmp_label_count++;
      int end_label_before = jmp_label_count++;
      int end_label_after = jmp_label_count++;

      // 引数を評価
      int argc = node->args->len;
      char *regs[6] = {"rdi", "rsi", "rdx", "rcx", "r8", "r9"};
      for (int i = 0; (i < argc) && (i < 6); i++) {
        gen((Node *)node->args->data[i]);
        printf("  pop rax\n");
        printf("  mov %s, rax\n", regs[i]);
      }

      // スタックポインタが16の倍数であるかを判定し、揃える
      // 関数呼び出し前の処理
      // 揃っていた場合は0を、そうでない場合は1をスタックに積む
      printf("  mov rax, [rsp]\n");
      printf("  mov r10, %d\n", (16 - 1)); // x % 2**n == x & 2**n-1
      printf("  and rax, r10\n");          // 剰余をAND演算で行う
      printf("  cmp rax, 0\n");
      printf("  jne .Lelse%03d\n", else_label_before);
      printf("  push 0\n");
      printf("  push 0\n");
      printf("  jmp .Lend%03d\n", end_label_before);
      printf(".Lelse%03d:\n", else_label_before);
      printf("  push 1\n");
      printf(".Lend%03d:\n", end_label_before);

      // 関数呼び出し
      printf("  mov rax, %d\n", argc);
      printf("  call %s\n", func_name);
      printf("  push rax\n");

      // スタックトップを一時RDIへ退避
      printf("  pop rax\n");
      printf("  mov rdi, rax\n");

      // スタックポインタが16の倍数であるかを判定し、揃える
      // 関数呼び出し後の処理
      // 揃っていた場合は0が、そうでない場合は1がスタックに積まれているハズ

      printf("  pop rax\n");
      printf("  cmp rax, 0\n");
      printf("  jne .Lend%03d\n", end_label_after);
      printf("  pop rax\n");
      printf(".Lend%03d:\n", end_label_after);

      // スタックトップだった値をRDIから復帰
      printf("  mov rax, rdi\n");
      printf("  push rax\n");
    } else {
      printf("  call %s\n", func_name);
      printf("  push rax\n");
    }
    return;
  }

  // 関数定義
  if (node->ty == ND_DEF_FUNC) {
    char *func_label = node->name;
    printf("%s:\n", func_label);

    // プロローグ
    printf("  push rbp\n");
    printf("  mov rbp, rsp\n");

    // 変数の領域を確保する
    printf("  sub rsp, %d\n", var_count * 8);

    // レジスタが持つ引数の値を、仮引数を表す変数にストア
    int argc = node->args->len;
    char *regs[6] = {"rdi", "rsi", "rdx", "rcx", "r8", "r9"};
    for (int i = 0; (i < argc) && (i < 6); i++) {
      char *var_name = ((Node *)node->args->data[i])->name;

      // 配列の為のポインタ型への変換
      Type *t = map_get(var_map, var_name);
      int var_offset;
      if (t->ty == PTR) {
        int ptr_offset = t->offset;
        t = t->ptrof;
        if (t->ty == ARRAY) {
          printf("  lea rax, [rbp-%d]\n", t->offset);
          printf("  push rax\n");
          printf("  lea rax, [rbp-%d]\n", ptr_offset);
          printf("  pop rdi\n");
          printf("  mov [rax], rdi\n");
        }
        var_offset = ptr_offset;
      } else {
        var_offset = t->offset;
      }
      char *reg_name = regs[i];

      printf("  mov rax, rbp\n");
      printf("  sub rax, %d\n", var_offset);
      printf("  mov [rax], %s\n", reg_name);
    }

    // コードを生成
    for (int i = 0; i < node->block->len; i++) {
      gen((Node *)node->block->data[i]);
      printf("  pop rax\n");
    }

    // エピローグ
    printf("  mov rsp, rbp\n");
    printf("  pop rbp\n");
    printf("  ret\n");
    return;
  }

  if (node->ty == '=') {
    gen_lval(node->lhs);
    gen(node->rhs);

    printf("  pop rdi\n");
    printf("  pop rax\n");
    printf("  mov [rax], rdi\n");
    printf("  push rdi\n");
    return;
  }

  gen(node->lhs);
  gen(node->rhs);

  printf("  pop rdi\n");
  printf("  pop rax\n");

  // ポインタの加算・減算の為のシフト量を計算
  int is_pointer = 0;
  int siz = 0;
  if (node->lhs->name != NULL) {
    Type *t = (Type *)map_get(var_map, node->lhs->name);

    if ((t != NULL) && (t->ty == PTR)) {
      is_pointer = 1;
      if (((Type *)(t->ptrof))->ty == ARRAY) {
        t = t->ptrof;
      }

      if (((Type *)(t->ptrof))->ty == PTR) {
        siz = 3; // 3bits 左シフト量(8bytes)
      }
      if (((Type *)(t->ptrof))->ty == INT) {
        siz = 2; // 2bits 左シフト量(4bytes)
      }
    }
  }

  switch (node->ty) {
  case '+':
    // ポインタの加算の為
    if (is_pointer)
      printf("  shl rdi, %d\n", siz);

    printf("  add rax, rdi\n");
    break;
  case '-':
    // ポインタの減算の為
    if (is_pointer)
      printf("  shl rdi, %d\n", siz);

    printf("  sub rax, rdi\n");
    break;
  case '*':
    // imulはrax*引数レジスタし、結果の上位64bitはRDXに、下位64bitはRAXにセットする（仕様
    printf("  imul rdi\n");
    break;
  case '/':
    // RAXの64bitを128bitに伸長し、RDXとRAXに。
    printf("  cqo\n");
    // idivはRDXとRAXをとって128bitと見なして、それを引数レジスタで除算
    printf("  idiv rdi\n");
    break;
  case ND_EQ:
    // 比較した結果をフラグレジスタにセット
    printf("  cmp rax, rdi\n");
    // フラグレジスタの値を、al(RAX下位8bitの別名)へセット
    printf("  sete al\n");
    // 上位56bitをゼロで埋めてセットする
    printf("  movzb rax, al\n");
    break;
  case ND_NE:
    printf("  cmp rax, rdi\n");
    // フラグレジスタの値を、al(RAX下位8bitの別名)へNOT EQUALであるかをセット
    printf("  setne al\n");
    printf("  movzb rax, al\n");
    break;
  case ND_LE:
    printf("  cmp rax, rdi\n");
    // フラグレジスタの値を、al(RAX下位8bitの別名)へ
    // LESS THAN OR EQUALであるかをセット
    printf("  setle al\n");
    printf("  movzb rax, al\n");
    break;
  case '<':
    printf("  cmp rax, rdi\n");
    // フラグレジスタの値を、al(RAX下位8bitの別名)へ
    // LESS THANであるかをセット
    printf("  setl al\n");
    printf("  movzb rax, al\n");
    break;
  }

  printf("  push rax\n");
}
