#include "cycc.h"

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "引数の個数が正しくありません\n");
    return 1;
  }

  if (strncmp(argv[1], "-test", 5) == 0) {
    runtest();
    return 0;
  }

  // 可変長ベクタを初期化する
  tokens = new_vector();
  user_input = argv[1];
  // トークナイズする
  tokenize(user_input);
  // パースする
  Node *node = expr();

  // アセンブリの前半分
  printf(".intel_syntax noprefix\n");
  printf(".global main\n");
  printf("main:\n");

  // コードを生成
  gen(node);

  printf("  pop rax\n");
  printf("  ret\n");
  return 0;
}
