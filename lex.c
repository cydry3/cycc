#include "cycc.h"

// 入力プログラム
char *user_input;

// 入力プログラムのファイル名
char *filename;

// トークンの為の関数
Token *new_token() {
  Token *token = malloc(sizeof(Token));
  return token;
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

// トークナイズした結果のトークン列をこの可変長ベクタに保存
Vector *tokens;

// 文字列リテラル
Vector *literals;
// 文字列リテラルを識別するカウント、literals count
int liter_count;

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

    // 行コメントをスキップ
    if (strncmp(p, "//", 2) == 0) {
      p += 2;
      while (*p != '\n')
        p++;
      continue;
    }

    // ブロックコメントをスキップ
    if (strncmp(p, "/*", 2) == 0) {
      char *q = strstr(p + 2, "*/");
      if (!q)
        error_at(p, "コメントが閉じられていません");
      p = q + 2;
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

