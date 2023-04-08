#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Node {
  char* data;         // string data to be stored
  struct Node* next;  // node pointer to the next node
} Node;

void addNode(Node** head, char* data) {
  Node* newNode = malloc(sizeof(Node));   // allocate memory for new Node
  //strcpy(newNode->data, data);  // set new node's data to be passed in data
  newNode->data = data;
  newNode->next = NULL;  // set new next Node to be tail

  if(*head == NULL) {    // if no node in list
    *head = newNode;     // head is also the newNode
  } else {
    Node* tail = *head;  // starting at head (:= tail->only exists within fnc), 
    while(tail->next != NULL) {  // traverse list to find,
      tail = tail->next;         // last node
    }
    tail->next = newNode;    // connect last node to the newNode to be new tail
  }
}

void printList(Node* node) {
  while (node != NULL) {
    printf("%s ", node->data);
    node = node->next;
  }
  printf("\n");
}

void processFile(Node* pattern, char* file, int color, int numLines) {
  FILE *fp;
  Node* head = pattern;
  fp = fopen(file, "r");
  if (fp == NULL) {
    perror("");
    exit(EXIT_FAILURE);
  }
  char line[1024];
  int lineNum = 1;
  while (fgets(line, sizeof(line), fp)) {
    while (pattern != NULL) {
      if (strstr(line, pattern->data) != NULL) {
        if (numLines == 1) {
          printf("%d: ", lineNum);
        }
        printf("%s", line);
      }
      pattern = pattern->next;
    }
    pattern = head;
    lineNum++;
  }
  fclose(fp);
}

int main(int argc, char* argv[]) {
  // forloop -> run through each argv[i], so each -p argv[i+1] gets put in 
  // linked list 
  // -c -n triggers once for all, repeats don't matter
  // -h halts anyother tags, (only) prints usage 
  
  if (argc < 2 || strcmp(argv[0], "sgrep") == 0) {
    printf("Error\n");
    return 1;
  }
  
  int color = 0;
  int numLines = 0;
  
  Node* patterns = NULL;
  Node* fileNames = NULL;

  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-h") == 0) {
      printf("Usage: ./sgrep_ref [-n] [-c] [-h] [-p PATTERN]... FILE...\n");
      exit(0);
    }
    else if (strcmp(argv[i], "-n") == 0) {
      numLines = 1;
    }
    else if (strcmp(argv[i], "-c") == 0) {
      color = 1;
    }
    else if (strcmp(argv[i], "-p") == 0) {
      addNode(&patterns, argv[i + 1]);
      i++;
    }
    else {
      addNode(&fileNames, argv[i]);
    }
  }

  while (fileNames != NULL) {
    processFile(patterns, fileNames->data, color, numLines);
    fileNames = fileNames->next;
  }

  return 0;
}