#ifndef CRAWLER_H
#define CRAWLER_H

#include "indexPage.h"

typedef struct listNode{
  char addr[MAX_ADDR_LENGTH];
  Node* trie;
  int totalTerms;
  struct listNode *next;
} List;

int contains(const struct listNode *pNode, const char *addr);

void insertBack(struct listNode *pNode, const char *addr, int urlLength);

void destroyList(struct listNode *pNode);

int getLink(const char* srcAddr, char* link, const int maxLinkLength);

struct listNode* search(const char* url, struct listNode* root);

#endif