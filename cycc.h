#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// トークンの型を表す値
enum {
  TK_NUM = 256, // 整数トークン
  TK_EOF,       // 入力の終わりを表すトークン
  TK_EQ,        // 比較演算子'=='を表すトークン
  TK_NE,        // 比較演算子'!='を表すトークン
  TK_LE,        // 比較演算子'<='を表すトークン
  TK_GE,        // 比較演算子'>='を表すトークン
};

// トークンの型
typedef struct {
  int ty;      // トークンの型
  int val;     // tyがTK_NUMの場合、その数値
  char *input; // トークン文字列（エラーメッセージ用）
} Token;

// 可変長ベクタ
typedef struct {
  void **data;
  int capacity;
  int len;
} Vector;

// 抽象構文木のノードの型を定義
enum {
  ND_NUM = 256, // 整数のノードの型
  ND_EQ,        // 比較演算子'=='のノードの型
  ND_NE,        // 比較演算子'!='のノードの型
  ND_LE,        // 比較演算子'<='のノードの型('>='の場合は両辺を入れ替えて使用)
};

typedef struct Node {
  int ty;           // 演算子かND_NUM
  struct Node *lhs; // 左辺
  struct Node *rhs; // 右辺
  int val;          // tyがND_NUMの場合のみ使う
} Node;

// 可変長ベクタの為の関数
Vector *new_vector();
void vec_push(Vector *vec, void *elem);
void runtest();

//　レジスタマシンでスタックマシンをエミュレートし、コンパイルする
void gen(Node *node);

// 入力プログラム
extern char *user_input;

// トークナイズした結果のトークン列をこの可変長ベクタに保存
extern Vector *tokens;

// 現在着目しているtoken->dataのインデックス
extern int pos;

// user_input が指している文字列を
// トークンに分割してtokensに保存
void tokenize(char *user_input);

// パーサの関数宣言
Node *expr();