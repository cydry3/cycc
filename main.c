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
  // 文字列リテラルの可変長ベクタを初期化する
  literals = new_vector();
  // 文字列リテラルを識別するカウントを初期化する
  liter_count = 0;
  // ローカル変数のマップを初期化する
  var_map = new_map();
  // ローカル変数の個数カウンタを初期化する
  var_count = 1;
  // グローバル変数のマップを初期化する
  gl_var_map = new_map();

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

  // データ・セグメントを生成
  printf(".data\n");
  for (int i = 0; code[i]; i++)
    if (code[i]->ty == ND_DEF_VAR)
      gen(code[i]);

  // 文字列リテラルを生成
  for (int i = 0; i < literals->len; i++) {
    printf(".LC%d:\n", ((Token *)literals->data[i])->liter);
    printf("  .string %s\n", ((Token *)literals->data[i])->name);
  }

  // テキスト・セグメントを生成
  printf(".text\n");
  for (int i = 0; code[i]; i++)
    if (code[i]->ty == ND_DEF_FUNC)
      gen(code[i]);

  return 0;
}
