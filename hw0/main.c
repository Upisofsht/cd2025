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
        node->count = 1;
        node->next = NULL;
    }
    return node;
}

void addChar(Node **head, char c) {
    if (*head == NULL) {
        *head = createNode(c);
        return;
    }
    
    Node *temp = *head;
    while (temp != NULL) {
        if (temp->ch == c) {
            temp->count++;
            return;
        }
        if (temp->next == NULL) {
            temp->next = createNode(c);
            return;
        }
        temp = temp->next;
    }
}
Node* createListFromFile(FILE *fptr) {
    Node *head = NULL;
    int ch;
    while ((ch = fgetc(fptr)) != EOF) {
        addChar(&head, (char)ch);
    }
    return head;
}

void printList(Node *head) {
    Node *temp = head;
    while(temp != NULL) {
        temp = temp->next;
    }
}

void freeList(Node *head) {
    Node *temp;
    while(head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

int main() {
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
