#include "cycc.h"

// 型のサイズを探す
void find_var_size(Node *node, int *size) {
  if (node == NULL)
    return;
  find_var_size(node->lhs, size);
  if (node->ty == ND_IDENT) {
    Type *t = map_get(var_map, node->name);
    if (t == NULL)
      t = map_get(gl_var_map, node->name);
    *size = base_type_size(t);
    return;
  }
  find_var_size(node->rhs, size);
}

// 型のサイズを表す文字列を返す
// メモリアクセス時のサイズ明記
char *word_size(int size) {
  switch (size) {
  case 4:
    return "DWORD PTR ";
  case 1:
    return "BYTE PTR ";
  }
  return "";
}

// 型のサイズを表す文字列を返す
// レジスタのエイリアス名
char *rdi_size(int size) {
  switch (size) {
  case 4:
    return "edi";
  case 1:
    return "dil";
  }
  return "rdi";
}
char *rax_size(int size) {
  switch (size) {
  case 4:
    return "eax";
  case 1:
    return "al";
  }
  return "rax";
}

// 配列型のベースの型のサイズを返す
// 例えば、int[10]ならint(4bytes)
int base_type_size_of_array(Type *t) {
  for (Type *p = t->ptrof; p != NULL; p = p->ptrof)
    t = p;
  return base_type_size(t);
}

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

  if (node->ty == ND_LITER) {
    printf("  lea rax, .LC%d[rip]\n", node->liter);
    printf("  push rax\n");
    return;
  }

  if (node->ty != ND_IDENT)
    error("代入の左辺値が変数ではありません");

  char *var_name = node->name;

  // 配列の為のポインタ型への変換
  Type *t;
  t = map_get(var_map, var_name);
  if (t != NULL) {
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
    return;
  }

  // グローバル変数を参照
  t = map_get(gl_var_map, var_name);
  if (t != NULL) {
    if (t->ty != PTR) {
      printf("  lea rax, %s[rip]\n", var_name);
      printf("  push rax\n");
      return;
    }

    t = t->ptrof;
    if (t != NULL) {
      if (t->ty != ARRAY) {
        printf("  lea rax, %s[rip]\n", var_name);
        printf("  push rax\n");
        return;
      }

      if (t->ty == ARRAY) {
        const int ptr_size = 8;
        printf("  lea rax, %s[rip+%d]\n", var_name, ptr_size);
        printf("  push rax\n");
        printf("  lea rax, %s[rip]\n", var_name);
        printf("  pop rdi\n");
        printf("  mov [rax], rdi\n");
        printf("  push rax\n");
        return;
      }
    }
  }
}

//　レジスタマシンでスタックマシンをエミュレートし、コンパイルする
void gen(Node *node) {
  if (node->ty == ND_NUM) {
    printf("  mov eax, %d\n", node->val);
    printf("  push rax\n");
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
    int jmp_label = jmp_label_count++;
    gen(node->lhs);
    printf("  pop rax\n");
    printf("  cmp rax, 0\n");
    printf("  je .Lend%03d\n", jmp_label);
    gen(node->rhs);
    printf(".Lend%03d:\n", jmp_label);
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

  if (node->ty == ND_LITER) {
    gen_lval(node);
    return;
  }

  if (node->ty == ND_ADDRESS) {
    gen_lval(node->rhs);
    return;
  }

  if (node->ty == ND_IDENT) {
    gen_lval(node);
    printf("  pop rax\n");

    int size = 0;
    find_var_size(node, &size);
    char *w_siz = word_size(size);
    char *r_siz = rax_size(size);
    if (size == 1)
      printf("  movsx rax, %s[rax]\n", w_siz);
    else
      printf("  mov %s, %s[rax]\n", r_siz, w_siz);

    printf("  push rax\n");
    return;
  }

  if (node->ty == ND_FUNC) {
    char *func_name = node->name;
    if (node->args->len != 0) {
      int before_label = jmp_label_count++;
      int after_label = jmp_label_count++;

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
      printf("  mov rbx, 0\n");
      printf("  mov rax, [rsp]\n");
      printf("  mov r10, %d\n", (16 - 1)); // x % 2**n == x & 2**n-1
      printf("  and rax, r10\n");          // 剰余をAND演算で行う
      printf("  cmp rax, 0\n");
      printf("  jne .Lend%03d\n", before_label);
      printf("  add rsp, rax\n");
      printf("  mov rbx, rax\n");
      printf(".Lend%03d:\n", before_label);

      // 関数呼び出し
      printf("  mov al, 0\n");
      printf("  call %s\n", func_name);
      printf("  mov rdi, rax\n");

      // スタックポインタが16の倍数であるかを判定し、揃える
      // 関数呼び出し後の処理
      // 揃っていた場合は0が、そうでない場合は1がスタックに積まれているハズ
      printf("  mov rax, rbx\n");
      printf("  cmp rax, 0\n");
      printf("  jne .Lend%03d\n", after_label);
      printf("  sub rsp, rax\n");
      printf(".Lend%03d:\n", after_label);

      printf("  mov rax, rdi\n");
      printf("  push rax\n");
    } else {
      printf("  call %s\n", func_name);
      printf("  push rax\n");
    }
    return;
  }

  // グローバル変数定義
  if (node->ty == ND_DEF_VAR) {
    char *label = node->name;
    printf("%s:\n", label);
    Type *t = map_get(gl_var_map, label);
    int size = base_type_size(t);
    if (t != NULL) {
      t = t->ptrof;
      if (t != NULL)
        if (t->ty == ARRAY) {
          int base_size = base_type_size_of_array(t);
          size += (base_size * t->array_size);
        }
    }
    printf("  .zero %d\n", size);
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

    int size = 0;
    find_var_size(node->lhs, &size);
    char *w_siz = word_size(size);
    char *r_siz = rdi_size(size);
    printf("  mov %s[rax], %s\n", w_siz, r_siz);
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
    if (t == NULL)
      t = (Type *)map_get(gl_var_map, node->lhs->name);

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
