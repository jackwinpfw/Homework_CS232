/* File: indexPage.c */
/* Author: Excell Pepple */
/* Date: March, 2023 */

/* This program indexes a web page, printing out the counts of words on that page */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

typedef struct trieNode {
    char letter;
    int count;

    struct trieNode *alphabet[26];
} Node;

struct trieNode* indexPage(const char* url);

void newNode(Node **node);

int addWordOccurrence(const char* word, const int wordLength, Node *node, int cursor);

void printTrieContents(char *word, const Node *node, int cursor);

int freeTrieMemory(Node *node);

int getText(const char* srcAddr, char* buffer, const int bufSize);



int main(int argc, char** argv){
    if (argc < 2){
        printf("Illegal arguments Exception\n");
        printf("Usage: indexPage <url>\n");
        return 1;
    }
    struct trieNode *root = indexPage(argv[1]);
    char letters[100];
    printTrieContents(letters, root, 0);
    freeTrieMemory(root);

    return 0;
}

struct trieNode* indexPage(const char* url)
{

    char * buffer = malloc(sizeof(char) * 2048);
    int content = getText(url, buffer, 2048);
    if ( content < 0){
        printf("No text found!");
        free(buffer);
        return NULL;
    }
    printf("%s\n", url);
    struct trieNode* rt = malloc(sizeof(Node));
	rt->count = -2;

	newNode(&rt);

    char * invalid = "\t\n 01123456789_.,:;-?!()[]{}\'\"";
    char *words = strtok(buffer, invalid);

    while(words != NULL) {
        for (int i = 0; i < words[i]; i++){
            words[i] = tolower(words[i]);
            if(!(words[i]> 'a' && words[i] <'z')) continue;
        }
        if (words[0] != '\0'){
            printf("\t%s\n", words);
            addWordOccurrence(words, strlen(words), rt, -1);
        }
        words = strtok(NULL, invalid);
    }
    free(buffer);
    return rt;
}

void newNode(Node **node) {
  for (int i = 0; i < 26; i++) {
    (*node)->alphabet[i] = malloc(sizeof(Node));
    (*node)->alphabet[i]->letter = 'a' + i;
    (*node)->alphabet[i]->count = -1;
  }
}

int addWordOccurrence(const char* word, const int wordLength, Node *node, int cursor)
{
    // If the current node was an unused letter, "initialize" it
    if (node->count == -1) {
        node->count = 0;
        newNode(&node);
    }

    // If the next cursor position is past the length of the word, increase the count of the current letter
    int endOfWord = cursor + 1 == wordLength;
    if (endOfWord) {
        node->count++;
    }
    else {
        // Calls this function with the node of the next letter in word
        cursor++;
        addWordOccurrence(word, wordLength, node->alphabet[*(word + cursor) - 'a'], cursor);
    }
    return 0;
}

void printTrieContents(char *buffer, const Node *node, int cursor)
{
    // For each letter in the node's alphabet
    for (int i = 0; i < 26; i++) {
        // Ignore letters with a count of -1
        if (node->alphabet[i]->count < 0) {
            continue;
        }

        /*
          If a letter in the current node's alphabet is used:
              add the letter at the buffer's cursor position,
              add a null character to end the string
              print the current word and its count
              call this function with the letter and the next cursor position
        */
        if (node->alphabet[i]->count > 0) {
            buffer[cursor] = node->alphabet[i]->letter;
            buffer[cursor + 1] = '\0';
            printf("%s: %d\n", buffer, node->alphabet[i]->count);
            printTrieContents(buffer, node->alphabet[i], cursor + 1);
            continue;
        }

        /*
          If a letter in the current node's alphabet is part of a prefix for another word:
              add the letter at the buffer's cursor position
              call this function with the letter and the next cursor position
        */
        buffer[cursor] = node->alphabet[i]->letter;
        printTrieContents(buffer, node->alphabet[i], cursor + 1);
    }
}

int freeTrieMemory(Node *node)
{
    // For each letter in the node's alphabet
    for (int i = 0; i < 26; i++) {
        // Free the letter in the node's alphabet if its count is -1 (what I use for unused letters)
        if (node->alphabet[i]->count == -1) {
            free(node->alphabet[i]);
            continue;
        }

        // Go down the trie
        freeTrieMemory(node->alphabet[i]);
    }

    free(node);
    return 0;
}




/* You should not need to modify this function */
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
