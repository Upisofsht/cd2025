#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    char ch;
    int count;
    struct Node *next;
}Node;

Node* createNode(char ch) {
    Node *node = malloc(sizeof(Node));
    if (node != NULL) {
        node->ch = ch;
        node->count = 0;
        node->next = NULL;
    }
    return node;
}

void addChar(Node *head, char c) {
    if (head == NULL) {
        Node *new_node = createNode(c);
        new_node->count++;
        head = new_node;
    }
}

int main(void) {
    FILE *fptr = fopen("main.c", "r");
    if (fptr == NULL) { 
        perror("Failed to open file");
        return 1;
    }
    int ch;
    while ((ch = fgetc(fptr)) != EOF) {
        putchar(ch);
    }
    fclose(fptr);
    return 0;
}
