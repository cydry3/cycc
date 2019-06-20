#include "cycc.h"

// 指定したファイルの内容を返す
char *read_file(char *path) {
  FILE *fp = fopen(path, "r");
  if (!fp)
    error("cannot open %s: %s", path, strerror(errno));

  // fileの長さを調べる
  if (fseek(fp, 0, SEEK_END) == -1)
    error("s: seek: %s", path, strerror(errno));
  size_t size = ftell(fp);
  if (fseek(fp, 0, SEEK_SET) == -1)
    error("s: fseek: %s", path, strerror(errno));

  //　ファイル内容の読み込み
  char *buf = malloc(size + 2);
  fread(buf, size, 1, fp);

  // ファイルを必ず"\n\0"で終わらす
  if (size == 0 || buf[size - 1] != '\n')
    buf[size++] = '\n';
  buf[size] = '\0';
  fclose(fp);
  return buf;
}

int main(int argc, char **argv) {
  if (argc < 2) {
    fprintf(stderr, "引数の個数が正しくありません\n");
    return 1;
  }

  if (strncmp(argv[1], "-test", 5) == 0) {
    runtest();
    return 0;
  }

  // ユーザの入力
  if (strncmp(argv[1], "-file", 5) == 0) {
    user_input = read_file(argv[2]);
  } else {
    user_input = argv[1];
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
