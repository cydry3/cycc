#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// トークンの型を表す値
enum {
  TK_NUM = 256, // 整数トークン
  TK_IDENT,     // 識別子　
  TK_EOF,       // 入力の終わりを表すトークン
  TK_EQ,        // 比較演算子'=='を表すトークン
  TK_NE,        // 比較演算子'!='を表すトークン
  TK_LE,        // 比較演算子'<='を表すトークン
  TK_GE,        // 比較演算子'>='を表すトークン
  TK_RETURN,    // return文
  TK_IF,        // if文を表すトークン
  TK_ELSE,      // elseを表すトークン
  TK_WHIL,      // while文を表すトークン
  TK_FOR,       // for文を表すトークン
  TK_INT,       // intキーワードを表すトークン
};

// トークンの型
typedef struct {
  int ty;      // トークンの型
  int val;     // tyがTK_NUMの場合、その数値
  char *name;  // tyがTK_IDENTの場合、その名前
  char *input; // トークン文字列（エラーメッセージ用）
} Token;

// 可変長ベクタ
typedef struct {
  void **data;
  int capacity;
  int len;
} Vector;

// 変数管理のためのマップ
typedef struct {
  Vector *keys;
  Vector *vals;
} Map;

// 抽象構文木のノードの型を定義
enum {
  ND_NUM = 256, // 整数のノードの型
  ND_IDENT,     // 識別子のノードの型
  ND_EQ,        // 比較演算子'=='のノードの型
  ND_NE,        // 比較演算子'!='のノードの型
  ND_LE,        // 比較演算子'<='のノードの型('>='の場合は両辺を入れ替えて使用)
  ND_RETURN,    // return文の型
  ND_IF,        // if文の型
  ND_IFELSE,    // if-elseの型
  ND_WHIL,      // while文の型
  ND_FOR,       // for文の型
  ND_BLOCK,     // ブロック（複文)の型
  ND_FUNC,      // 関数呼び出しの型
  ND_DEF_FUNC,  // 関数定義の型
};

typedef struct Node {
  int ty;           // 演算子かND_NUM
  struct Node *lhs; // 左辺
  struct Node *rhs; // 右辺
  int val;          // tyがND_NUMの場合のみ使う
  char *name;       // tyがND_IDENTの場合のみ使う
  Vector *block;    // tyがND_BLOCKの場合のみ使う
  Vector *args;     // tyがND_FUNCの場合のみ使う
} Node;

// 可変長ベクタの為の関数
Vector *new_vector();
void vec_push(Vector *vec, void *elem);
void runtest();

// 変数管理のマップの為の関数
Map *new_map();
void map_put(Map *map, char *key, void *val);
void *map_get(Map *map, char *key);

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
void program();

// エラーを報告する関数
// printfと同じ引数を受け取る
void error(char *fmt, ...);

// パースされた複数の関数定義を100個まで格納
extern Node *funcs[100];

// 複数文字のローカル変数の為のマップ
extern Map *var_map;

// 変数の為のカウンター
extern int var_count;

// if文のジャンプ先のラベルをユニークにする為のカウンタ
extern int jmp_label_count;

// 関数呼び出しの為に、作成した仮の関数
int foo();
