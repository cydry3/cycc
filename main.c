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

  // トークンの可変長ベクタを初期化する
  tokens = new_vector();
  // 変数のマップを初期化する
  var_map = new_map();
  // 変数の個数カウンタを初期化する
  var_count = 0;

  // ユーザの入力
  user_input = argv[1];
  // トークナイズする
  tokenize(user_input);
  // パースする
  program();

  // アセンブリの前半分
  printf(".intel_syntax noprefix\n");
  printf(".global main\n");
  printf("main:\n");

  // プロローグ
  // 変数の領域を確保する
  printf("  push rbp\n");
  printf("  mov rbp, rsp\n");
  printf("  sub rsp, %d\n", var_count * 8);

  // コードを生成
  for (int i = 0; code[i]; i++) {
    gen(code[i]);

    printf("  pop rax\n");
  }

  // エピローグ
  printf("  mov rsp, rbp\n");
  printf("  pop rbp\n");
  printf("  ret\n");
  return 0;
}
