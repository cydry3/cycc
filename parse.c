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

// トークンを構成する文字か判定
int is_alnum(char c) {
  return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') ||
         ('0' <= c && c <= '9') || (c == '_');
}

// トークンを複製する
char *token_dup(const char *p, int siz) {
  char *tmp = malloc(siz + 1);
  strncpy(tmp, p, siz);
  tmp[siz] = '\0';
  return tmp;
}

// user_input が指している文字列を
// トークンに分割してtokensに保存
void tokenize(char *user_input) {
  char *p = user_input;
  char *bp;

  int i = 0;
  while (*p) {
    // 空白文字をスキップ
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

    // return文
    if (strncmp(p, "return", 6) == 0 && !is_alnum(p[6])) {
      Token *token = new_token();
      token->ty = TK_RETURN;
      token->input = p;
      vec_push(tokens, token);
      i++;
      p += 6;
      continue;
    }

    // if文
    if (strncmp(p, "if", 2) == 0 && !is_alnum(p[2])) {
      Token *token = new_token();
      token->ty = TK_IF;
      token->input = p;
      vec_push(tokens, token);
      i++;
      p += 2;
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

    if ('a' <= *p && *p <= 'z' && !is_alnum(*(p + 1))) {
      Token *token = new_token();
      token->ty = TK_IDENT;
      token->input = p;
      token->name = token_dup(p, 1);
      vec_push(tokens, token);
      i++;
      p++;
      continue;
    }

    if (('a' <= *p && *p <= 'z') || ('A' <= *p && *p <= 'Z')) {
      bp = p;
      for (; is_alnum(*p); p++)
        ;

      Token *token = new_token();
      token->ty = TK_IDENT;
      token->input = p;
      token->name = token_dup(bp, (p - bp));
      vec_push(tokens, token);
      i++;
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
void program();
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

Node *new_node_ident(char *name) {
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

// パースされた複数のステートメントを100個まで格納
Node *code[100];

// パーサ
//
// 生成規則:
// program = stmt*
// stmt = expr ";"
// 	| "return" expr ";"
// 	| "if" "(" expr ")" stmt
// expr = assign
// assign = equiality ("=" assign)?
// equiality = relational ("==" relational | "!=" relational)*
// relational = add ("<" add | "<=" add | ">=" add | ">" add)*
// add = mul ("+" mul | "-" mul)*
// mul  = unary ("*" unary | "/" unary)*
// unary = ("+" | "-")? term
// term = num | ident | "(" expr ")"
//
void program() {
  int i = 0;
  while (((Token *)tokens->data[pos])->ty != TK_EOF)
    code[i++] = stmt();
  code[i] = NULL;
}

Node *stmt() {
  Node *node;

  if (consume(TK_IF)) {
    node = malloc(sizeof(Node));
    node->ty = ND_IF;
    if (!consume('('))
      error_at((((Token *)(tokens->data[pos]))->input),
               "'('ではないトークンです");
    node->lhs = expr();
    if (!consume(')'))
      error_at((((Token *)(tokens->data[pos]))->input),
               "')'ではないトークンです");
    node->rhs = stmt();
    return node;
  }

  if (consume(TK_RETURN)) {
    node = malloc(sizeof(Node));
    node->ty = ND_RETURN;
    node->lhs = expr();
  } else {
    node = expr();
  }

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

// 複数文字のローカル変数の為のマップ
Map *var_map;

// 変数の為のカウンター
int var_count;

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
    char *var_name = ((Token *)tokens->data[pos++])->name;
    Node *node = new_node_ident(var_name);

    if (map_get(var_map, var_name) == NULL) {
      map_put(var_map, var_name, (void *)var_count);
      var_count++;
    }

    return node;
  }

  error_at(((Token *)(tokens->data[pos]))->input,
           "数値でも開きカッコでもないトークンです");
}
