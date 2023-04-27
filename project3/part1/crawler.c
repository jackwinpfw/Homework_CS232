#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "crawler.h"

int contains(const struct listNode *pNode, const char *addr){
  if (pNode == NULL) {
    return 0;
  }

  if (!strcmp(pNode->addr, addr)) {
    return 1;
  }

  return contains(pNode->next, addr);
}

void insertBack(struct listNode *pNode, const char *addr, int urlLength){
  if (pNode->next != NULL) {
    return insertBack(pNode->next, addr, urlLength);
  }
  struct listNode *newNode;
  newNode = malloc(sizeof(struct listNode));
  if(newNode == NULL){
    fprintf(stderr, "ERROR: could not allocate memory\n");
    return;
  }

  for (int i = urlLength; i < MAX_ADDR_LENGTH; i++) {
    newNode->addr[i] = '\0';
  }
  newNode->totalTerms = 0;
  newNode->next = NULL;
  newNode->trie = indexPage(addr, &(newNode->totalTerms));
  strncpy(newNode->addr, addr, MAX_ADDR_LENGTH);
  pNode->next = newNode;
}

void destroyList(struct listNode *pNode){
  if (pNode->next != NULL) {
    destroyList(pNode->next);
  }

  freeTrieMemory(pNode->trie);

  free(pNode);
}
  
int getLink(const char* srcAddr, char* link, const int maxLinkLength){
  const int bufSize = 1000;
  char buffer[bufSize];

  int numLinks = 0;

  FILE *pipe;

  snprintf(buffer, bufSize, "curl -s \"%s\" | python3 getLinks.py", srcAddr);

  pipe = popen(buffer, "r");
  if(pipe == NULL){
    fprintf(stderr, "ERROR: could not open the pipe for command %s\n", buffer);
    return 0;
  }

  fscanf(pipe, "%d\n", &numLinks);

  if(numLinks > 0){
    int linkNum;
    double r = (double)rand() / ((double)RAND_MAX + 1.0);

    for(linkNum=0; linkNum<numLinks; linkNum++){
      fgets(buffer, bufSize, pipe);
      
      if(r < (linkNum + 1.0) / numLinks){
		    break;
      }
    }

    strncpy(link, buffer, maxLinkLength);
    link[maxLinkLength-1] = '\0';
    
    {
      char* pNewline = strchr(link, '\n');
      if(pNewline != NULL){
		    *pNewline = '\0';
      }
    }
  }

  pclose(pipe);

  if(numLinks > 0){
    return 1;
  }
  else{
    return 0;
  }
}

struct listNode* search(const char* url, struct listNode* root){
  //If root does not contain anything
  if (root == NULL){
    return NULL;
  }

  // If strings are equal, if does not pass
  if (strcmp(url, root->addr)) {
    return search(url, root->next);
  }
  
  return root;
}