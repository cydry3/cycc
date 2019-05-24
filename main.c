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
  var_count = 1;

  // ユーザの入力
  user_input = argv[1];
  // トークナイズする
  tokenize(user_input);
  // パースする
  program();

  // ジャンプ先ラベルのカウンタを初期化する
  jmp_label_count = 0;

  // アセンブリの前半分
  printf(".intel_syntax noprefix\n");
  printf(".global main\n");

  // コードを生成
  for (int i = 0; funcs[i]; i++)
    gen(funcs[i]);

  return 0;
}
