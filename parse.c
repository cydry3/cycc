#include "cycc.h"

// 変数の型のサイズを返す　
// 変数をマップする際に使う
int base_type_size(Type *t) {
  int base;
  switch (t->ty) {
  case PTR:
    base = 8;
    break;
  case INT:
    base = 4;
    break;
  case CHAR:
    base = 1;
    break;
  default:
    error("型のサイズが不明です");
  }
  return base;
}

// トークンの為の関数
Token *new_token() {
  Token *token = malloc(sizeof(Token));
  return token;
}

// 入力プログラム
char *user_input;

// 入力プログラムのファイル名
char *filename;

// トークナイズした結果のトークン列をこの可変長ベクタに保存
Vector *tokens;
// 現在着目しているtoken->dataのインデックス
int pos;

// 文字列リテラル
Vector *literals;
// 文字列リテラルを識別するカウント、literals count
int liter_count;

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
  if (filename == NULL) {
    int pos = loc - user_input;
    fprintf(stderr, "%s\n", user_input);
    fprintf(stderr, "%*s", pos, ""); // pos個空白を出力
    fprintf(stderr, "^ %s\n", msg);
    exit(1);
  }

  // locが含まれている行の開始地点と終了地点を取得
  // アドレスを比較
  char *line = loc; // 先頭
  while (user_input < line && line[-1] != '\n')
    line--;

  char *end = loc; // 終点
  while (*end != '\n')
    end++;

  // 全体の何行目であるかを調べる
  int line_num = 1;
  for (char *p = user_input; p < line; p++)
    if (*p == '\n')
      line_num++;

  // 見つかった行を表示する
  int indent = fprintf(stderr, "%s:%d: ", filename, line_num);
  fprintf(stderr, "%.*s\n", (int)(end - line), line);

  // エラー箇所を'^'で指し示す
  int pos = loc - line + indent;
  fprintf(stderr, "%*s", pos, "");
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

    // ブロック
    if (*p == '{' || *p == '}') {
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

    // else
    if (strncmp(p, "else", 4) == 0 && !is_alnum(p[4])) {
      Token *token = new_token();
      token->ty = TK_ELSE;
      token->input = p;
      vec_push(tokens, token);
      i++;
      p += 4;
      continue;
    }

    // while文
    if (strncmp(p, "while", 5) == 0 && !is_alnum(p[5])) {
      Token *token = new_token();
      token->ty = TK_WHIL;
      token->input = p;
      vec_push(tokens, token);
      i++;
      p += 5;
      continue;
    }

    // for文
    if (strncmp(p, "for", 3) == 0 && !is_alnum(p[3])) {
      Token *token = new_token();
      token->ty = TK_FOR;
      token->input = p;
      vec_push(tokens, token);
      i++;
      p += 3;
      continue;
    }

    // int キーワード
    if (strncmp(p, "int", 3) == 0 && !is_alnum(p[3])) {
      Token *token = new_token();
      token->ty = TK_INT;
      token->input = p;
      vec_push(tokens, token);
      i++;
      p += 3;
      continue;
    }

    // char キーワード
    if (strncmp(p, "char", 4) == 0 && !is_alnum(p[4])) {
      Token *token = new_token();
      token->ty = TK_CHAR;
      token->input = p;
      vec_push(tokens, token);
      i++;
      p += 4;
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

    // sizeof演算子
    if (strncmp(p, "sizeof", 6) == 0 && !is_alnum(p[6])) {
      Token *token = new_token();
      token->ty = TK_SIZEOF;
      token->input = p;
      vec_push(tokens, token);
      i++;
      p += 6;
      continue;
    }

    if (*p == '+' || *p == '-' || *p == '*' || *p == '(' || *p == ')' ||
        *p == '/' || *p == '=' || *p == ',' || *p == '&' || *p == '[' ||
        *p == ']') {
      Token *token = new_token();
      token->ty = *p;
      token->input = p;
      vec_push(tokens, token);
      i++;
      p++;
      continue;
    }

    // 文字列リテラル
    if (*p == '"') {
      bp = p;
      p++;
      for (; *p != '"'; p++)
        ;
      p++;

      Token *token = new_token();
      token->ty = TK_LITER;
      token->input = bp;
      token->name = token_dup(bp, (p - bp));
      token->liter = liter_count++;
      vec_push(tokens, token);

      vec_push(literals, token);
      i++;
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
Node *decl();
Type *read_decl_type();
Node *read_decl_func_tail();
Node *read_decl_var_tail();
Node *stmt();
Node *var_decl();
Node *expr();
Node *assign();
Node *equiality();
Node *relational();
Node *add();
Node *mul();
Node *unary();
Node *term();
Node *ident();

Node *init_node() {
  Node *node;
  node = malloc(sizeof(Node));
  node->ty = 0;
  node->lhs = NULL;
  node->rhs = NULL;
  node->val = 0;
  node->name = NULL;
  node->block = NULL;
  node->args = NULL;
  node->deref = 0;
  node->liter = 0;
}

Node *new_node(int ty, Node *lhs, Node *rhs) {
  Node *node = init_node();
  node->ty = ty;
  node->lhs = lhs;
  node->rhs = rhs;
  return node;
}

Node *new_node_num(int val) {
  Node *node = init_node();
  node->ty = ND_NUM;
  node->val = val;
  return node;
}

Node *new_node_ident(char *name) {
  Node *node = init_node();
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

// トークンを消費せず、その型が期待したものかを確認する
int current(int ty) {
  if (((Token *)(tokens->data[pos]))->ty != ty)
    return 0;
  return 1;
}

// 次のトークンを先読みし、期待したトークンかを確認する
// トークンは消費しない
int forward(int ty) {
  int next_pos = pos + 1;
  if (((Token *)(tokens->data[next_pos]))->ty != ty)
    return 0;
  return 1;
}

// ポインタの為の関数
// nの数だけリファレンスするポインタを返す
Type *pointer(Type *t, int n) {
  if (n <= 0)
    return t;
  Type *tmp = malloc(sizeof(Type));
  tmp->ty = PTR;
  tmp->ptrof = pointer(t, n - 1);
  return tmp;
}

// 配列の為の関数
// 配列を読み、変数の型を返す
Type *array(Type *t) {
  if (!consume('[')) {
    return t;
  }

  int siz = ((Token *)tokens->data[pos++])->val;
  Type *tmp = malloc(sizeof(Type));
  tmp->ty = ARRAY;
  tmp->array_size = siz;
  if (!consume(']'))
    error_at((((Token *)(tokens->data[pos]))->input),
             "開きカッコに対する閉じカッコがありません");

  tmp->ptrof = array(t);
  return tmp;
}

// 配列の為の関数
// 配列の型の変数を読み、要素数の計を返す
int array_size_of(Type *t) {
  if (t->ty != ARRAY)
    return 1;
  int siz = t->array_size;
  return siz * array_size_of(t->ptrof);
}

// 配列の為の関数
// 構文木を辿り、sizeof演算子、&アドレス演算子のオペランド以外の
// 配列型をポインタ型に変換する
void array_as_pointer(Node *node) {
  if (node == NULL)
    return;

  if (node->ty == ND_ADDRESS)
    return;

  array_as_pointer(node->lhs);
  // 配列型の場合は、ポインタ型にすべて置き換えてしまう
  if (node->ty == ND_IDENT) {
    char *var_name = node->name;
    Type *t = (Type *)map_get(var_map, var_name);
    if (t != NULL) {
      if (t->ty == ARRAY) {
        t->ty = PTR;
        map_put(var_map, var_name, (void *)t);
      }
    }
  }
  array_as_pointer(node->rhs);
}

// パースされた複数のトップレベルの定義を100個まで格納
Node *code[100];

// パーサ
//
// 生成規則:
// program = decl*
// decl = func
// 	| var_decl ";"
// func = "int" ident "(" ( var_decl? (, var_decl)* )? ")" "{" stmt* "}"
// stmt = expr ";"
// 	| "{" stmt* "}"
// 	| "return" expr ";"
// 	| "if" "(" expr ")" stmt ("else" stmt)?
// 	| "while" "(" expr ")" stmt
// 	| "for" "(" expr? ";" expr? ":"  expr? ")" stmt
// 	| var_decl ";"
// var_decl = ("int" | "char") ( "*" )? ident ( ( "[" num "]" )* )?
// expr = assign
// assign = equiality ("=" assign)?
// equiality = relational ("==" relational | "!=" relational)*
// relational = add ("<" add | "<=" add | ">=" add | ">" add)*
// add = mul ("+" mul | "-" mul)*
// mul  = unary ("*" unary | "/" unary)*
// unary = "sizeof" unary
// 	 | ("+" | "-")? term
// 	 | ( ("*")* )? ( ident | "(" expr ")" )
// 	 | ("&")? ident
// term = num
//	| ident ("(" expr? (, expr)* ")")? ( "[" ( num | ident ) "]" )?
//	| "(" expr ")"
//	| "\"" character* "\""
//
void program() {
  int i = 0;
  while (((Token *)tokens->data[pos])->ty != TK_EOF) {
    code[i++] = decl();
  }
  code[i] = NULL;
}

Node *decl() {
  Node *node;

  // 型の部分　
  Type *ty = read_decl_type();

  // 識別子
  if (((Token *)(tokens->data[pos]))->ty == TK_IDENT) {
    char *label = ((Token *)tokens->data[pos])->name;
    if (forward('(')) {
      pos++;
      node = read_decl_func_tail(label);

    } else if ((forward('[')) || (forward(';'))) {
      pos++;
      node = read_decl_var_tail(ty, label);
    }
  } else
    error_at(((Token *)(tokens->data[pos]))->input, "識別子がありません");

  return node;
}

Type *read_decl_type() {
  // int/char キーワード
  Type *ty = malloc(sizeof(Type));
  if (consume(TK_INT)) {
    ty->ty = INT;
  } else if (consume(TK_CHAR)) {
    ty->ty = CHAR;
  } else {
    error_at((((Token *)(tokens->data[pos]))->input),
             "'int'/'char'キーワードがありません");
  }
  ty->ptrof = NULL;

  // ポインタの部分
  int ptr_count = 0;
  while (consume('*'))
    ptr_count++;

  return pointer(ty, ptr_count);
}

Node *read_decl_func_tail(char *label) {
  Node *node = new_node_ident(label);
  node->ty = ND_DEF_FUNC;
  node->args = new_vector();

  // 仮引数
  if (!consume('('))
    error_at((((Token *)(tokens->data[pos]))->input),
             "仮引数の開きカッコがありません");

  if (!consume(')')) {
    vec_push(node->args, (void *)var_decl());

    while (consume(','))
      vec_push(node->args, (void *)var_decl());

    if (!consume(')'))
      error_at((((Token *)(tokens->data[pos]))->input),
               "仮引数の閉じカッコがありません");
  }

  // ブロック
  if (consume('{')) {
    node->block = new_vector();

    while (!forward('}')) {
      vec_push(node->block, (void *)stmt());
      if (consume('}'))
        break;
    }
  } else {
    error_at((((Token *)(tokens->data[pos]))->input),
             "'{'ではないトークンです");
  }

  return node;
}

Node *read_decl_var_tail(Type *ty, char *label) {
  // 配列の部分
  ty = array(ty);

  if (ty->ty == ARRAY) {
    ty = pointer(ty, 1);
  }
  if (!consume(';'))
    error_at((((Token *)(tokens->data[pos]))->input),
             "';'ではないトークンです");
  Node *node = init_node();
  node->name = label;
  node->ty = ND_DEF_VAR;
  map_put(gl_var_map, label, (void *)ty);
  return node;
}

Node *stmt() {
  Node *node = init_node();

  if (consume(TK_IF)) {
    node->ty = ND_IF;
    if (!consume('('))
      error_at((((Token *)(tokens->data[pos]))->input),
               "'('ではないトークンです");
    node->lhs = expr();
    if (!consume(')'))
      error_at((((Token *)(tokens->data[pos]))->input),
               "')'ではないトークンです");
    node->rhs = stmt();

    // if文がelseを持つ場合, nodeの型を変更する
    if (consume(TK_ELSE)) {
      node->ty = ND_IFELSE;

      Node *rhs_node = init_node();
      rhs_node->lhs = node->rhs;
      rhs_node->rhs = stmt();
      node->rhs = rhs_node;
    }

    return node;
  }

  if (consume(TK_WHIL)) {
    node->ty = ND_WHIL;
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

  if (consume(TK_FOR)) {
    node->ty = ND_FOR;

    Node *node_lhs = init_node();
    node->lhs = node_lhs;
    Node *node_rhs = init_node();
    node->rhs = node_rhs;

    if (!consume('('))
      error_at((((Token *)(tokens->data[pos]))->input),
               "'('ではないトークンです");

    // expr? ; expr? ; expr?)
    if (!forward(';')) {
      node->lhs->lhs = expr();
    }
    if (!consume(';'))
      error_at((((Token *)(tokens->data[pos]))->input),
               "')'ではないトークンです");

    if (!forward(';')) {
      node->lhs->rhs = expr();
    }
    if (!consume(';'))
      error_at((((Token *)(tokens->data[pos]))->input),
               "')'ではないトークンです");

    if (!forward(')')) {
      node->rhs->lhs = expr();
    }
    if (!consume(')'))
      error_at((((Token *)(tokens->data[pos]))->input),
               "')'ではないトークンです");

    node->rhs->rhs = stmt();

    return node;
  }

  // int あるいはcharキーワードによる変数定義
  if ((current(TK_INT)) || (current(TK_CHAR))) {
    node = var_decl();

    if (!consume(';'))
      error_at((((Token *)(tokens->data[pos]))->input), "';'がありません");
    return node;
  }

  if (consume('{')) {
    node->ty = ND_BLOCK;
    node->block = new_vector();

    while (!forward('}')) {
      vec_push(node->block, (void *)stmt());

      if (consume('}'))
        break;
    }

    return node;
  }

  if (consume(TK_RETURN)) {
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

Node *var_decl() {
  // 型の部分　
  Type *ty = read_decl_type();

  Node *node = init_node();
  // ポインタの部分
  int ptr_count = 0;
  while (consume('*'))
    ptr_count++;

  // 識別子の部分
  node->ty = ND_IDENT;

  char *var_name = ((Token *)tokens->data[pos++])->name;
  node->name = var_name;

  Type *var = pointer(ty, ptr_count);
  var->offset = 8 * ++var_count;

  int base = base_type_size(ty);
  int siz = 1;

  // 配列の部分
  var = array(var);
  if (var->ty == ARRAY) {
    siz *= array_size_of(var);
  }

  // サイズを1byte単位から8bytes単位に換算
  siz = base * siz;
  int add_siz = siz / 8;
  if ((siz % 8) > 0)
    add_siz++;
  var_count += add_siz;
  if (var->ty == ARRAY) {
    var->offset = var_count * 8;
    var = pointer(var, 1);
    var->offset = ++var_count * 8;
  }

  map_put(var_map, var_name, (void *)var);

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

Node *sizeof_op(Node *node) {
  switch (node->ty) {
  case '=': // assign
    node = sizeof_op(node->lhs);
    break;
  case ND_EQ: // equiality
  case ND_NE:
  case '<': // relational
  case ND_LE:
    node = new_node_num(4);
    break;
  case '+': // add
  case '-':
  case '*': // mul
  case '/':
    node = sizeof_op(node->lhs);
    break;
  case ND_ADDRESS: // unary
    node = new_node_num(8);
    break;
  case ND_DEREF: {
    Type *t = map_get(var_map, node->rhs->name);
    if (((Type *)t->ptrof)->ty == PTR)
      node = new_node_num(8);
    if (((Type *)t->ptrof)->ty == INT)
      node = new_node_num(4);
  } break;
  case ND_NUM: // term
    node = new_node_num(4);
    break;
  case ND_IDENT: {
    Type *t = map_get(var_map, node->name);
    if (t->ty == PTR)
      node = new_node_num(8);
    if (t->ty == INT)
      node = new_node_num(4);
  } break;
  default:
    error_at((((Token *)(tokens->data[pos]))->input),
             "構文木に紐づく型がわかりません");
  }
  return node;
}

Node *unary() {
  if (consume(TK_SIZEOF)) {
    Node *node = unary();
    node = sizeof_op(node);
    return node;
  }

  if (consume('+'))
    return term();
  if (consume('-'))
    return new_node('-', new_node_num(0), term());

  if (consume('*')) {
    Node *node;
    int derefer_count = 1;
    while (consume('*'))
      derefer_count++;
    if (consume('(')) {
      node = expr();
      if (!consume(')'))
        error_at((((Token *)(tokens->data[pos]))->input),
                 "閉じカッコがありません");
    } else {
      node = ident();
    }
    node = new_node(ND_DEREF, NULL, node);
    node->deref = derefer_count;
    return node;
  }
  if (consume('&'))
    return new_node(ND_ADDRESS, NULL, ident());

  return term();
}

// グローバル変数の為のマップ
Map *gl_var_map;

// 複数文字のローカル変数の為のマップ
Map *var_map;

// ローカル変数の為のカウンター
int var_count;

// termはexprやmulのように左結合でない
Node *term() {
  Node *node = init_node();
  // 開きカッコからはじまるなら、"(" expr ")"
  if (consume('(')) {
    node = expr();
    if (!consume(')'))
      error_at((((Token *)(tokens->data[pos]))->input),
               "開きカッコに対する閉じカッコがありません");
    return node;
  }

  // 上記でない場合は、numかidentが来るハズ
  if (((Token *)(tokens->data[pos]))->ty == TK_NUM) {
    node = new_node_num(((Token *)tokens->data[pos++])->val);
    return node;
  }

  if (((Token *)(tokens->data[pos]))->ty == TK_IDENT) {
    node = ident();

    // 関数であるか判定
    if (consume('(')) {
      node->ty = ND_FUNC;
      node->args = new_vector();

      if (consume(')')) {
        return node;
      }

      vec_push(node->args, (void *)expr());
      for (; consume(',');)
        vec_push(node->args, (void *)expr());

      if (!consume(')'))
        error_at((((Token *)(tokens->data[pos]))->input),
                 "開きカッコに対する閉じカッコがありません");
    }

    // 配列の添字であるか判定
    if (consume('[')) {
      Node *index = init_node();
      Token *token = tokens->data[pos++];
      if (token->ty == TK_NUM)
        index = new_node_num(token->val);
      else if (token->ty == TK_IDENT)
        index = new_node_ident(token->name);
      else
        error_at((((Token *)(tokens->data[pos]))->input),
                 "添字[数]がありません");

      if (!consume(']'))
        error_at((((Token *)(tokens->data[pos]))->input),
                 "開きカッコに対する閉じカッコがありません");

      node = new_node(ND_DEREF, NULL, new_node('+', node, index));
    }

    return node;
  }

  if (((Token *)(tokens->data[pos]))->ty == TK_LITER) {
    node->ty = ND_LITER;
    node->liter = ((Token *)(tokens->data[pos++]))->liter;
    return node;
  }

  error_at(((Token *)(tokens->data[pos]))->input,
           "数値でも識別子でも文字列リテラルでもないトークンです");
}

Node *ident() {
  if (!(((Token *)(tokens->data[pos]))->ty == TK_IDENT))
    error_at(((Token *)(tokens->data[pos]))->input, "識別子がありません");
  char *var_name = ((Token *)tokens->data[pos])->name;
  if (((map_get(var_map, var_name) == NULL) &&
       map_get(gl_var_map, var_name) == NULL) &&
      !forward('(')) {
    error_at((((Token *)(tokens->data[pos]))->input),
             "定義された変数ではありません");
  }
  pos++;
  Node *node = new_node_ident(var_name);
  return node;
}
