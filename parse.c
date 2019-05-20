#include "cycc.h"

// トークンの為の関数
Token *new_token() {
  Token *token = malloc(sizeof(Token));
  return token;
}

// 入力プログラム
char *user_input;

// トークナイズした結果のトークン列をこの可変長ベクタに保存
Vector *tokens;
// 現在着目しているtoken->dataのインデックス
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

    // ステートメント
    if (*p == ';') {
      Token *token = new_token();
      token->ty = *p;
      token->input = p;
      vec_push(tokens, token);
      i++;
      p++;
      continue;
    }

    // 比較演算子
    if (strncmp(p, "==", 2) == 0) {
      Token *token = new_token();
      token->ty = TK_EQ;
      token->input = p;
      vec_push(tokens, token);
      i++;
      p += 2;
      continue;
    } else if (strncmp(p, "!=", 2) == 0) {
      Token *token = new_token();
      token->ty = TK_NE;
      token->input = p;
      vec_push(tokens, token);
      i++;
      p += 2;
      continue;
    } else if (strncmp(p, "<=", 2) == 0) {
      Token *token = new_token();
      token->ty = TK_LE;
      token->input = p;
      vec_push(tokens, token);
      i++;
      p += 2;
      continue;
    } else if (strncmp(p, ">=", 2) == 0) {
      Token *token = new_token();
      token->ty = TK_GE;
      token->input = p;
      vec_push(tokens, token);
      i++;
      p += 2;
      continue;
    }

    if (*p == '<' || *p == '>') {
      Token *token = new_token();
      token->ty = *p;
      token->input = p;
      vec_push(tokens, token);
      i++;
      p++;
      continue;
    }

    if (*p == '+' || *p == '-' || *p == '*' || *p == '(' || *p == ')' ||
        *p == '/' || *p == '=') {
      Token *token = new_token();
      token->ty = *p;
      token->input = p;
      vec_push(tokens, token);
      i++;
      p++;
      continue;
    }

    if (isdigit(*p)) {
      Token *token = new_token();
      token->ty = TK_NUM;
      token->input = p;
      token->val = strtol(p, &p, 10);
      vec_push(tokens, token);
      i++;
      continue;
    }

    if ('a' <= *p && *p <= 'z') {
      Token *token = new_token();
      token->ty = TK_IDENT;
      token->input = p;
      vec_push(tokens, token);
      i++;
      p++;
      continue;
    }

    error_at(p, "トークナイズできません");
  }

  Token *token = new_token();
  token->ty = TK_EOF;
  token->input = p;
  vec_push(tokens, token);
}

// パーサの関数宣言
Node *stmt();
Node *expr();
Node *assign();
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

Node *new_node_ident(char name) {
  Node *node = malloc(sizeof(Node));
  node->ty = ND_IDENT;
  node->name = name;
  return node;
}

// トークンを消費するとともに、その型が期待したものかを確認する
int consume(int ty) {
  if (((Token *)(tokens->data[pos]))->ty != ty)
    return 0;
  pos++;
  return 1;
}

// パーサ
//
// 生成規則:
// stmt = expr ";"
// expr = assign
// assign = equiality ("=" assign)?
// equiality = relational ("==" relational | "!=" relational)*
// relational = add ("<" add | "<=" add | ">=" add | ">" add)*
// add = mul ("+" mul | "-" mul)*
// mul  = unary ("*" unary | "/" unary)*
// unary = ("+" | "-")? term
// term = num | ident | "(" expr ")"
//
Node *stmt() {
  Node *node = expr();
  if (!consume(';'))
    error_at((((Token *)(tokens->data[pos]))->input),
             "';'ではないトークンです");
  return node;
}

Node *expr() {
  Node *node = assign();
  return node;
}

Node *assign() {
  Node *node = equiality();
  if (consume('='))
    node = new_node('=', node, assign());
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
      error_at((((Token *)(tokens->data[pos]))->input),
               "開きカッコに対する閉じカッコがありません");
    return node;
  }
  // 上記でない場合は、numかidentが来るハズ
  if (((Token *)(tokens->data[pos]))->ty == TK_NUM)
    return new_node_num(((Token *)tokens->data[pos++])->val);
  else if (((Token *)(tokens->data[pos]))->ty == TK_IDENT) {
    return new_node_ident(*((Token *)tokens->data[pos++])->input);
  }

  error_at(((Token *)(tokens->data[pos]))->input,
           "数値でも開きカッコでもないトークンです");
}
