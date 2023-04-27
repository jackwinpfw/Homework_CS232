#ifndef INDEXPAGE_H
#define INDEXPAGE_H

#define MAX_ADDR_LENGTH 1000

typedef struct trieNode {
    char letter;
    int count;

    struct trieNode *alphabet[26];
} Node;

Node* indexPage(const char* url, int* totalTerms);

void newNode(Node *node);

int addWordOccurrence(const char* word, const int wordLength, Node *node, int cursor);

int freeTrieMemory(Node *node);

int getText(const char* srcAddr, char* buffer, const int bufSize);

#endif