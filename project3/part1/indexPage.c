#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "indexPage.h"

struct trieNode* indexPage(const char* url, int* totalTerms)
{
    char buffer[1024*1024];
    int bytesRead = getText(url, buffer, sizeof(buffer));
    struct trieNode *rootToReturn = malloc(sizeof(struct trieNode));
    newNode(rootToReturn);
    rootToReturn->count = -2;

    int wordStarted = -1;
    char *word = malloc(bytesRead + 1);
    int j = 0;


    for (int i = 0; i < bytesRead; i++) {
        char c = buffer[i];
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
            c = tolower(c);

            if (wordStarted == -1) {
                wordStarted = i;
            }

            word[j] = c;
            j++;
            continue;
        }
        if (wordStarted != -1) {
            word[j] = '\0';
            int wordLength = i - wordStarted;
            *totalTerms += 1;
            addWordOccurrence(word, wordLength, rootToReturn->alphabet[word[0] - 'a'], 0);
            printf("\t%s\n", word);
            
            wordStarted = -1;
            j = 0;
        }
    }
    free(word);

    return rootToReturn;
}

void newNode(Node *node) {
  for (int i = 0; i < 26; i++) {
    node->alphabet[i] = malloc(sizeof(Node));
    node->alphabet[i]->letter = 'a' + i;
    node->alphabet[i]->count = -1;
    for (int j = 0; j < 26; j++) {
        node->alphabet[i]->alphabet[j] = NULL;
    }
  }
}

int addWordOccurrence(const char* word, const int wordLength, Node *node, int cursor)
{
    if (node->count == -1) {
        node->count = 0;
        newNode(node);
    }

    int endOfWord = cursor + 1 == wordLength;
    if (endOfWord) {
        node->count++;
    }
    else {
        cursor++;
        addWordOccurrence(word, wordLength, node->alphabet[*(word + cursor) - 'a'], cursor);
    }
    return 0;
}

int freeTrieMemory(Node *node)
{
    if (node == NULL) {
        return 0;
    }
    
    for (int i = 0; i < 26; i++) {
        freeTrieMemory(node->alphabet[i]);
    }

    free(node);
    return 0;
}

int getText(const char* srcAddr, char* buffer, const int bufSize){
    FILE *pipe;
    int bytesRead;

    snprintf(buffer, bufSize, "curl -s \"%s\" | python3 getText.py", srcAddr);

    pipe = popen(buffer, "r");
    if(pipe == NULL){
        fprintf(stderr, "ERROR: could not open the pipe for command %s\n",
                buffer);
        return 0;
    }

    bytesRead = fread(buffer, sizeof(char), bufSize-1, pipe);
    buffer[bytesRead] = '\0';

    pclose(pipe);

    return bytesRead;
}