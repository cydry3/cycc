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

// 入力プログラム
char *user_input;

// 可変長ベクタ
typedef struct {
  void **data;
  int capacity;
  int len;
} Vector;

// 可変長ベクタの為の関数
Vector *new_vector() {
  Vector *vec = malloc(sizeof(Vector));
  vec->data = malloc(sizeof(void *) * 16);
  vec->capacity = 16;
  vec->len = 0;
  return vec;
}
void vec_push(Vector *vec, void *elem) {
  if (vec->capacity == vec->len) {
    vec->capacity *= 2;
    vec->data = realloc(vec->data, sizeof(void *) * vec->capacity);
  }
  vec->data[vec->len++] = elem;
}

// 可変長ベクタの為のテストコード
void expect(int line, int expected, int actual) {
  if (expected == actual)
    return;
  fprintf(stderr, "%d: %d expected, but got %d\n", line, expected, actual);
  exit(1);
}
void runtest() {
  Vector *vec = new_vector();
  expect(__LINE__, 0, vec->len);

  for (int i = 0; i < 100; i++)
    vec_push(vec, (void *)((long)i));

  expect(__LINE__, 100, vec->len);
  expect(__LINE__, 0, (long)vec->data[0]);
  expect(__LINE__, 50, (long)vec->data[50]);
  expect(__LINE__, 99, (long)vec->data[99]);

  printf("OK\n");
  return 0;
}

// トークナイズした結果のトークン列をこの配列に保存
// 100個以上のトークンは来ないものとする　
Token tokens[100];
// 現在着目しているtokenのインデックス
int pos;

// エラーを報告する関数
// printfと同じ引数を受け取る
void error(char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");
  exit(1);
}

// エラー箇所を報告する
void error_at(char *loc, char *msg) {
  int pos = loc - user_input;
  fprintf(stderr, "%s\n", user_input);
  fprintf(stderr, "%*s", pos, ""); // pos個空白を出力
  fprintf(stderr, "^ %s\n", msg);
  exit(1);
}

// user_input が指している文字列を
// トークンに分割してtokensに保存
void tokenize(char *user_input) {
  char *p = user_input;

  int i = 0;
  while (*p) {
    // すきっぷ空白文字
    if (isspace(*p)) {
      p++;
      continue;
    }

    // 比較演算子
    if (strncmp(p, "==", 2) == 0) {
      tokens[i].ty = TK_EQ;
      tokens[i].input = p;
      i++;
      p += 2;
      continue;
    } else if (strncmp(p, "!=", 2) == 0) {
      tokens[i].ty = TK_NE;
      tokens[i].input = p;
      i++;
      p += 2;
      continue;
    } else if (strncmp(p, "<=", 2) == 0) {
      tokens[i].ty = TK_LE;
      tokens[i].input = p;
      i++;
      p += 2;
      continue;
    } else if (strncmp(p, ">=", 2) == 0) {
      tokens[i].ty = TK_GE;
      tokens[i].input = p;
      i++;
      p += 2;
      continue;
    }

    if (*p == '<' || *p == '>') {
      tokens[i].ty = *p;
      tokens[i].input = p;
      i++;
      p++;
      continue;
    }

    if (*p == '+' || *p == '-' || *p == '*' || *p == '(' || *p == ')' ||
        *p == '/') {
      tokens[i].ty = *p;
      tokens[i].input = p;
      i++;
      p++;
      continue;
    }

    if (isdigit(*p)) {
      tokens[i].ty = TK_NUM;
      tokens[i].input = p;
      tokens[i].val = strtol(p, &p, 10);
      i++;
      continue;
    }

    error_at(p, "トークナイズできません");
  }

  tokens[i].ty = TK_EOF;
  tokens[i].input = p;
}

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

// パーサの関数宣言
Node *expr();
Node *equiality();
Node *relational();
Node *add();
Node *mul();
Node *unary();
Node *term();

Node *new_node(int ty, Node *lhs, Node *rhs) {
  Node *node = malloc(sizeof(Node));
  node->ty = ty;
  node->lhs = lhs;
  node->rhs = rhs;
  return node;
}

Node *new_node_num(int val) {
  Node *node = malloc(sizeof(Node));
  node->ty = ND_NUM;
  node->val = val;
  return node;
}

// トークンを消費するとともに、その型が期待したものかを確認する
int consume(int ty) {
  if (tokens[pos].ty != ty)
    return 0;
  pos++;
  return 1;
}

// パーサ
//
// 生成規則:
// expr = equiality
// equiality = relational ("==" relational | "!=" relational)*
// relational = add ("<" add | "<=" add | ">=" add | ">" add)*
// add = mul ("+" mul | "-" mul)*
// mul  = unary ("*" unary | "/" unary)*
// unary = ("+" | "-")? term
// term = num | "(" expr ")"
//
Node *expr() {
  Node *node = equiality();
  return node;
}

Node *equiality() {
  Node *node = relational();

  for (;;) {
    if (consume(TK_EQ))
      node = new_node(ND_EQ, node, relational());
    else if (consume(TK_NE))
      node = new_node(ND_NE, node, relational());
    else
      return node;
  }
}

Node *relational() {
  Node *node = add();

  for (;;) {
    if (consume('<'))
      node = new_node('<', node, add());
    else if (consume(TK_LE))
      node = new_node(ND_LE, node, add());
    else if (consume(TK_GE))
      node = new_node(ND_LE, add(), node);
    else if (consume('>'))
      // 両辺を入れ替え, 'LESS'のノードを作る
      node = new_node('<', add(), node);
    else
      return node;
  }
}

Node *add() {
  Node *node = mul();

  for (;;) {
    if (consume('+'))
      node = new_node('+', node, mul());
    else if (consume('-'))
      node = new_node('-', node, mul());
    else
      return node;
  }
}

Node *mul() {
  Node *node = unary();

  for (;;) {
    if (consume('*'))
      node = new_node('*', node, unary());
    else if (consume('/'))
      node = new_node('/', node, unary());
    else
      return node;
  }
}

Node *unary() {
  if (consume('+'))
    return term();
  if (consume('-'))
    return new_node('-', new_node_num(0), term());
  return term();
}

// termはexprやmulのように左結合でない
Node *term() {
  // 開きカッコからはじまるなら、"(" expr ")"
  if (consume('(')) {
    Node *node = expr();
    if (!consume(')'))
      error_at(tokens[pos].input, "開きカッコに対する閉じカッコがありません");
    return node;
  }
  // 上記でない場合は、numが来るハズ
  if (tokens[pos].ty == TK_NUM)
    return new_node_num(tokens[pos++].val);

  error_at(tokens[pos].input, "数値でも開きカッコでもないトークンです");
}

//　レジスタマシンでスタックマシンをエミュレートし、コンパイルする
void gen(Node *node) {
  if (node->ty == ND_NUM) {
    printf("  push %d\n", node->val);
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

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "引数の個数が正しくありません\n");
    return 1;
  }

  if (strncmp(argv[1], "-test", 5) == 0) {
    runtest();
    return 0;
  }

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
