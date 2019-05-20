#include "cycc.h"

// １文字変数'a'~'z'を左辺値として評価する
void gen_lval(Node *node) {
  if (node->ty != ND_IDENT)
    error("代入の左辺値が変数ではありません");

  int offset = ('z' - node->name + 1) * 8;
  printf("  mov rax, rbp\n");
  printf("  sub rax, %d\n", offset);
  printf("  push rax\n");
}

//　レジスタマシンでスタックマシンをエミュレートし、コンパイルする
void gen(Node *node) {
  if (node->ty == ND_NUM) {
    printf("  push %d\n", node->val);
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

  if (node->ty == ND_IDENT) {
    gen_lval(node);
    printf("  pop rax\n");
    printf("  mov rax, [rax]\n");
    printf("  push rax\n");
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

  switch (node->ty) {
  case '+':
    printf("  add rax, rdi\n");
    break;
  case '-':
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
