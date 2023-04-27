#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "crawler.h"
#include "indexPage.h"



int main(int argc, char** argv) {
	if (argc < 3) {
		fprintf(stderr, "Usage: ./webSearch <fileUrl> <maxPages> <optionalSeed>\n");
		return -1;
	}

	FILE * file = fopen(argv[1], "r");
	if (file == NULL) {
		fprintf(stderr, "Unable to open file.\n");
		return -1;
	}

	int maxPages = atoi(argv[2]);
	if (maxPages < 1) {
		fprintf(stderr, "Max pages must be greater than 0");
		return -1;
	}

	if (argc > 3) {
		srand(atol(argv[3]));
	}
	else {

		srand(time(NULL));
	}

	List* list = malloc(sizeof(List));
	for (int i = 0; i < MAX_ADDR_LENGTH; i++) {
    	list->addr[i] = '\0';
  	}
	list->next=NULL;
	list->totalTerms = 0;
	list->trie = NULL;
	int pagesIndexed = 0;

	char newline[MAX_ADDR_LENGTH * 2];
	newline[MAX_ADDR_LENGTH * 2 - 1] = '\0';

	char url[MAX_ADDR_LENGTH];
	url[MAX_ADDR_LENGTH - 1] = '\0';

	printf("Indexing...\n");
	while (fgets(newline, MAX_ADDR_LENGTH * 2, file) != NULL && pagesIndexed < maxPages) {
		int hopLimit, hopCount = 0;
		sscanf(newline, "%s %d", url, &hopLimit);

		while (1) {
			if (!contains(list, url)) {
				printf("%s\n", url);
				insertBack(list, url, strlen(url));
				pagesIndexed++;
			}
			hopCount++;

			if (hopCount <= hopLimit && pagesIndexed < maxPages) {
				char newurl[MAX_ADDR_LENGTH];
				int result = getLink(url, newurl, MAX_ADDR_LENGTH);
				newurl[MAX_ADDR_LENGTH - 1] = '\0';

				if (!result) {
					break;
				}
				strncpy(url, newurl, MAX_ADDR_LENGTH);
			}
			else {
				break;
			}
		}
	}
	fclose(file);

	destroyList(list);
	
	return 0;
}