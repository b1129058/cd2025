#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    TYPE_TOKEN,
    IF_TOKEN,
    ELSE_TOKEN,
    WHILE_TOKEN,
    MAIN_TOKEN,
    ID_TOKEN,
    LITERAL_TOKEN,
    EQUAL_TOKEN,
    LESSEQUAL_TOKEN,
    GREATEREQUAL_TOKEN,
    LESS_TOKEN,
    GREATER_TOKEN,
    ASSIGN_TOKEN,
    SEMICOLON_TOKEN,
    PLUS_TOKEN,
    MINUS_TOKEN,
    LEFTPAREN_TOKEN,
    RIGHTPAREN_TOKEN,
    LEFTBRACE_TOKEN,
    RIGHTBRACE_TOKEN,
    UNKNOWN_TOKEN
} TokenType;

typedef struct Node {
    char *lexeme;
    TokenType type;
    struct Node *next;
} Node;

Node* createNode(const char *lexeme, TokenType type) {
    Node *new_node = (Node*)malloc(sizeof(Node));
    if (!new_node) {
        perror("malloc failed");
        exit(1);
    }
    new_node->lexeme = strdup(lexeme);
    if (!new_node->lexeme) {
        perror("strdup failed");
        exit(1);
    }
    new_node->type = type;
    new_node->next = NULL;
    return new_node;
}

int my_isalpha(int c) {
    return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

int my_isdigit(int c) {
    return (c >= '0' && c <= '9');
}

int my_isalnum(int c) {
    return my_isalpha(c) || my_isdigit(c);
}

int isWhitespace(int c) {
    return (c == ' ' || c == '\t' || c == '\n');
}

TokenType matchKeyword(const char *s) {
    if (strcmp(s, "int") == 0)
        return TYPE_TOKEN;
    if (strcmp(s, "if") == 0)
        return IF_TOKEN;
    if (strcmp(s, "else") == 0)
        return ELSE_TOKEN;
    if (strcmp(s, "while") == 0)
        return WHILE_TOKEN;
    if (strcmp(s, "main") == 0)
        return MAIN_TOKEN;
    return ID_TOKEN;
}

TokenType getToken(FILE *fp, char *buf) {
    int c;
    while ((c = fgetc(fp)) != EOF && isWhitespace(c))
        ;
    if (c == EOF)
        return UNKNOWN_TOKEN;
    
    if (my_isalpha(c)) {
        int i = 0;
        buf[i++] = (char)c;
        while ((c = fgetc(fp)) != EOF && (my_isalnum(c) || c == '_'))
            buf[i++] = (char)c;
        buf[i] = '\0';
        if (c != EOF)
            ungetc(c, fp);
        return matchKeyword(buf);
    }
    
    if (my_isdigit(c)) {
        int i = 0;
        buf[i++] = (char)c;
        while ((c = fgetc(fp)) != EOF && (my_isdigit(c) || c == '.'))
            buf[i++] = (char)c;
        buf[i] = '\0';
        if (c != EOF)
            ungetc(c, fp);
        return LITERAL_TOKEN;
    }
    
    if (c == '=') {
        int d = fgetc(fp);
        if (d == '=') {
            strcpy(buf, "==");
            return EQUAL_TOKEN;
        } else {
            if (d != EOF)
                ungetc(d, fp);
            strcpy(buf, "=");
            return ASSIGN_TOKEN;
        }
    }
    
    if (c == '<') {
        int d = fgetc(fp);
        if (d == '=') {
            strcpy(buf, "<=");
            return LESSEQUAL_TOKEN;
        } else {
            if (d != EOF)
                ungetc(d, fp);
            strcpy(buf, "<");
            return LESS_TOKEN;
        }
    }
    
    if (c == '>') {
        int d = fgetc(fp);
        if (d == '=') {
            strcpy(buf, ">=");
            return GREATEREQUAL_TOKEN;
        } else {
            if (d != EOF)
                ungetc(d, fp);
            strcpy(buf, ">");
            return GREATER_TOKEN;
        }
    }
    
    if (c == '(') { strcpy(buf, "("); return LEFTPAREN_TOKEN; }
    if (c == ')') { strcpy(buf, ")"); return RIGHTPAREN_TOKEN; }
    if (c == '{') { strcpy(buf, "{"); return LEFTBRACE_TOKEN; }
    if (c == '}') { strcpy(buf, "}"); return RIGHTBRACE_TOKEN; }
    if (c == ';') { strcpy(buf, ";"); return SEMICOLON_TOKEN; }
    if (c == '+') { strcpy(buf, "+"); return PLUS_TOKEN; }
    if (c == '-') { strcpy(buf, "-"); return MINUS_TOKEN; }
    
    buf[0] = (char)c;
    buf[1] = '\0';
    return UNKNOWN_TOKEN;
}

const char* tokenName(TokenType t) {
    switch (t) {
        case TYPE_TOKEN: return "TYPE_TOKEN";
        case IF_TOKEN: return "IF_TOKEN";
        case ELSE_TOKEN: return "ELSE_TOKEN";
        case WHILE_TOKEN: return "WHILE_TOKEN";
        case MAIN_TOKEN: return "MAIN_TOKEN";
        case ID_TOKEN: return "ID_TOKEN";
        case LITERAL_TOKEN: return "LITERAL_TOKEN";
        case EQUAL_TOKEN: return "EQUAL_TOKEN";
        case LESSEQUAL_TOKEN: return "LESSEQUAL_TOKEN";
        case GREATEREQUAL_TOKEN: return "GREATEREQUAL_TOKEN";
        case LESS_TOKEN: return "LESS_TOKEN";
        case GREATER_TOKEN: return "GREATER_TOKEN";
        case ASSIGN_TOKEN: return "ASSIGN_TOKEN";
        case SEMICOLON_TOKEN: return "SEMICOLON_TOKEN";
        case PLUS_TOKEN: return "PLUS_TOKEN";
        case MINUS_TOKEN: return "MINUS_TOKEN";
        case LEFTPAREN_TOKEN: return "LEFTPAREN_TOKEN";
        case RIGHTPAREN_TOKEN: return "RIGHTPAREN_TOKEN";
        case LEFTBRACE_TOKEN: return "LEFTBRACE_TOKEN";
        case RIGHTBRACE_TOKEN: return "RIGHTBRACE_TOKEN";
        default: return "UNKNOWN_TOKEN";
    }
}

int main(void) {
    FILE *fp = fopen("2025.c", "r");
    if (!fp) {
        perror("Error opening file");
        return 1;
    }
    
    Node *head = NULL, *tail = NULL;
    char buffer[256];
    while (!feof(fp)) {
        TokenType t = getToken(fp, buffer);
        if (feof(fp))
            break;
        if (buffer[0] == '\0' || t == UNKNOWN_TOKEN)
            continue;
        Node *node = createNode(buffer, t);
        if (!head)
            head = tail = node;
        else {
            tail->next = node;
            tail = node;
        }
    }
    fclose(fp);
    
    Node *cur = head;
    while (cur) {
        printf("%s : %s\n", cur->lexeme, tokenName(cur->type));
        cur = cur->next;
    }
    
    cur = head;
    while (cur) {
        Node *tmp = cur;
        cur = cur->next;
        free(tmp->lexeme);
        free(tmp);
    }
    
    return 0;
}
