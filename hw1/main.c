#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Token types
typedef enum {
    TYPE_TOKEN,        // int
    MAIN_TOKEN,        // main
    IF_TOKEN,          // if
    ELSE_TOKEN,        // else
    WHILE_TOKEN,       // while
    ID_TOKEN,          // identifier
    LITERAL_TOKEN,     // integer number
    EQUAL_TOKEN,       // ==
    GREATEREQUAL_TOKEN,// >=
    LESSEQUAL_TOKEN,   // <=
    GREATER_TOKEN,     // >
    LESS_TOKEN,        // <
    LEFTPAREN_TOKEN,   // (
    REFTPAREN_TOKEN,   // )
    LEFTBRACE_TOKEN,   // {
    REFTBRACE_TOKEN,   // }
    ASSIGN_TOKEN,      // =
    SEMICOLON_TOKEN,   // ;
    PLUS_TOKEN,        // +
    MINUS_TOKEN        // -
} TokenType;

typedef struct Node {
    char* token;
    TokenType type;
    struct Node *next;
} Node;

// Function to check if a character is whitespace
int is_whitespace(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

// Function to check if a character is a digit
int is_digit(char c) {
    return c >= '0' && c <= '9';
}

// Function to check if a character is a letter
int is_letter(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

// Function to check if a string is all digits
int is_all_digits(const char* str) {
    while (*str) {
        if (!is_digit(*str)) return 0;
        str++;
    }
    return 1;
}

// Function to check if a string is a keyword
TokenType check_keyword(const char* str) {
    if (strcmp(str, "int") == 0) return TYPE_TOKEN;
    if (strcmp(str, "main") == 0) return MAIN_TOKEN;
    if (strcmp(str, "if") == 0) return IF_TOKEN;
    if (strcmp(str, "else") == 0) return ELSE_TOKEN;
    if (strcmp(str, "while") == 0) return WHILE_TOKEN;
    return ID_TOKEN;
}

Node* createNode(const char* token, TokenType type) {
    Node *node = malloc(sizeof(Node));
    if (node != NULL) {
        node->token = strdup(token);
        node->type = type;
        node->next = NULL;
    }
    return node;
}

void addToken(Node **head, const char* token, TokenType type) {
    if (*head == NULL) {
        *head = createNode(token, type);
        return;
    }
    
    Node *temp = *head;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = createNode(token, type);
}

void printList(Node *head) {
    Node *temp = head;
    while(temp != NULL) {
        const char* type_str;
        switch (temp->type) {
            case TYPE_TOKEN: type_str = "TYPE_TOKEN"; break;
            case MAIN_TOKEN: type_str = "MAIN_TOKEN"; break;
            case IF_TOKEN: type_str = "IF_TOKEN"; break;
            case ELSE_TOKEN: type_str = "ELSE_TOKEN"; break;
            case WHILE_TOKEN: type_str = "WHILE_TOKEN"; break;
            case ID_TOKEN: type_str = "ID_TOKEN"; break;
            case LITERAL_TOKEN: type_str = "LITERAL_TOKEN"; break;
            case EQUAL_TOKEN: type_str = "EQUAL_TOKEN"; break;
            case GREATEREQUAL_TOKEN: type_str = "GREATEREQUAL_TOKEN"; break;
            case LESSEQUAL_TOKEN: type_str = "LESSEQUAL_TOKEN"; break;
            case GREATER_TOKEN: type_str = "GREATER_TOKEN"; break;
            case LESS_TOKEN: type_str = "LESS_TOKEN"; break;
            case LEFTPAREN_TOKEN: type_str = "LEFTPAREN_TOKEN"; break;
            case REFTPAREN_TOKEN: type_str = "REFTPAREN_TOKEN"; break;
            case LEFTBRACE_TOKEN: type_str = "LEFTBRACE_TOKEN"; break;
            case REFTBRACE_TOKEN: type_str = "REFTBRACE_TOKEN"; break;
            case ASSIGN_TOKEN: type_str = "ASSIGN_TOKEN"; break;
            case SEMICOLON_TOKEN: type_str = "SEMICOLON_TOKEN"; break;
            case PLUS_TOKEN: type_str = "PLUS_TOKEN"; break;
            case MINUS_TOKEN: type_str = "MINUS_TOKEN"; break;
            default: type_str = "UNKNOWN_TOKEN"; break;
        }
        printf("%s : %s\n", temp->token, type_str);
        temp = temp->next;
    }
}

void freeList(Node *head) {
    Node *temp;
    while(head != NULL) {
        temp = head;
        head = head->next;
        free(temp->token);
        free(temp);
    }
}

Node* createListFromFile(FILE *fptr) {
    Node *head = NULL;
    char buffer[256];
    int buffer_index = 0;
    char c;
    int in_comment = 0;  // 0: not in comment, 1: in single line comment, 2: in multi-line comment
    int in_preprocessor = 0;  // 是否在預處理指令中

    while ((c = fgetc(fptr)) != EOF) {
        // 處理註解
        if (in_comment == 0 && c == '/') {
            char next = fgetc(fptr);
            if (next == '/') {
                in_comment = 1;
                continue;
            } else if (next == '*') {
                in_comment = 2;
                continue;
            } else {
                ungetc(next, fptr);
            }
        }
        
        if (in_comment == 1) {
            if (c == '\n') {
                in_comment = 0;
            }
            continue;
        }
        
        if (in_comment == 2) {
            if (c == '*') {
                char next = fgetc(fptr);
                if (next == '/') {
                    in_comment = 0;
                    continue;
                }
                ungetc(next, fptr);
            }
            continue;
        }

        // 處理預處理指令
        if (c == '#') {
            in_preprocessor = 1;
            continue;
        }
        
        if (in_preprocessor) {
            if (c == '\n') {
                in_preprocessor = 0;
            }
            continue;
        }

        if (is_whitespace(c)) {
            if (buffer_index > 0) {
                buffer[buffer_index] = '\0';
                TokenType type;
                if (is_all_digits(buffer)) {
                    type = LITERAL_TOKEN;
                } else {
                    type = check_keyword(buffer);
                }
                addToken(&head, buffer, type);
                buffer_index = 0;
            }
            continue;
        }

        // Handle operators and special symbols
        if (c == '=') {
            if (buffer_index > 0) {
                buffer[buffer_index] = '\0';
                TokenType type;
                if (is_all_digits(buffer)) {
                    type = LITERAL_TOKEN;
                } else {
                    type = check_keyword(buffer);
                }
                addToken(&head, buffer, type);
                buffer_index = 0;
            }
            char next = fgetc(fptr);
            if (next == '=') {
                addToken(&head, "==", EQUAL_TOKEN);
            } else {
                addToken(&head, "=", ASSIGN_TOKEN);
                ungetc(next, fptr);
            }
            continue;
        }

        if (c == '>') {
            if (buffer_index > 0) {
                buffer[buffer_index] = '\0';
                TokenType type;
                if (is_all_digits(buffer)) {
                    type = LITERAL_TOKEN;
                } else {
                    type = check_keyword(buffer);
                }
                addToken(&head, buffer, type);
                buffer_index = 0;
            }
            char next = fgetc(fptr);
            if (next == '=') {
                addToken(&head, ">=", GREATEREQUAL_TOKEN);
            } else {
                addToken(&head, ">", GREATER_TOKEN);
                ungetc(next, fptr);
            }
            continue;
        }

        if (c == '<') {
            if (buffer_index > 0) {
                buffer[buffer_index] = '\0';
                TokenType type;
                if (is_all_digits(buffer)) {
                    type = LITERAL_TOKEN;
                } else {
                    type = check_keyword(buffer);
                }
                addToken(&head, buffer, type);
                buffer_index = 0;
            }
            char next = fgetc(fptr);
            if (next == '=') {
                addToken(&head, "<=", LESSEQUAL_TOKEN);
            } else {
                addToken(&head, "<", LESS_TOKEN);
                ungetc(next, fptr);
            }
            continue;
        }

        if (c == '(') {
            if (buffer_index > 0) {
                buffer[buffer_index] = '\0';
                TokenType type;
                if (is_all_digits(buffer)) {
                    type = LITERAL_TOKEN;
                } else {
                    type = check_keyword(buffer);
                }
                addToken(&head, buffer, type);
                buffer_index = 0;
            }
            addToken(&head, "(", LEFTPAREN_TOKEN);
            continue;
        }

        if (c == ')') {
            if (buffer_index > 0) {
                buffer[buffer_index] = '\0';
                TokenType type;
                if (is_all_digits(buffer)) {
                    type = LITERAL_TOKEN;
                } else {
                    type = check_keyword(buffer);
                }
                addToken(&head, buffer, type);
                buffer_index = 0;
            }
            addToken(&head, ")", REFTPAREN_TOKEN);
            continue;
        }

        if (c == '{') {
            if (buffer_index > 0) {
                buffer[buffer_index] = '\0';
                TokenType type;
                if (is_all_digits(buffer)) {
                    type = LITERAL_TOKEN;
                } else {
                    type = check_keyword(buffer);
                }
                addToken(&head, buffer, type);
                buffer_index = 0;
            }
            addToken(&head, "{", LEFTBRACE_TOKEN);
            continue;
        }

        if (c == '}') {
            if (buffer_index > 0) {
                buffer[buffer_index] = '\0';
                TokenType type;
                if (is_all_digits(buffer)) {
                    type = LITERAL_TOKEN;
                } else {
                    type = check_keyword(buffer);
                }
                addToken(&head, buffer, type);
                buffer_index = 0;
            }
            addToken(&head, "}", REFTBRACE_TOKEN);
            continue;
        }

        if (c == ';') {
            if (buffer_index > 0) {
                buffer[buffer_index] = '\0';
                TokenType type;
                if (is_all_digits(buffer)) {
                    type = LITERAL_TOKEN;
                } else {
                    type = check_keyword(buffer);
                }
                addToken(&head, buffer, type);
                buffer_index = 0;
            }
            addToken(&head, ";", SEMICOLON_TOKEN);
            continue;
        }

        if (c == '+') {
            if (buffer_index > 0) {
                buffer[buffer_index] = '\0';
                TokenType type;
                if (is_all_digits(buffer)) {
                    type = LITERAL_TOKEN;
                } else {
                    type = check_keyword(buffer);
                }
                addToken(&head, buffer, type);
                buffer_index = 0;
            }
            addToken(&head, "+", PLUS_TOKEN);
            continue;
        }

        if (c == '-') {
            if (buffer_index > 0) {
                buffer[buffer_index] = '\0';
                TokenType type;
                if (is_all_digits(buffer)) {
                    type = LITERAL_TOKEN;
                } else {
                    type = check_keyword(buffer);
                }
                addToken(&head, buffer, type);
                buffer_index = 0;
            }
            addToken(&head, "-", MINUS_TOKEN);
            continue;
        }

        // Handle identifiers and numbers
        if (is_letter(c) || is_digit(c)) {
            buffer[buffer_index++] = c;
        }
    }

    // Handle any remaining token in the buffer
    if (buffer_index > 0) {
        buffer[buffer_index] = '\0';
        TokenType type;
        if (is_all_digits(buffer)) {
            type = LITERAL_TOKEN;
        } else {
            type = check_keyword(buffer);
        }
        addToken(&head, buffer, type);
    }

    return head;
}

int main() {

    int cd2025 = 5;
    int cd2025_ = 5;

    if (cd2025 == 5) {
        cd2025_ = 0;
    } else {
        cd2025_ = 1+2+(3+4)+5;
    }

    while (cd2025_+cd2025) {
        printf("%d\n", cd2025_+cd2025);
        cd2025 = cd2025-1;
    }

    FILE *fptr = fopen("main.c", "r");
    if (fptr == NULL) { 
        perror("Failed to open file");
        return 1;
    }
    
    Node *list = createListFromFile(fptr);
    fclose(fptr);
    
    printList(list);
    freeList(list);
    
    return 0;
} 