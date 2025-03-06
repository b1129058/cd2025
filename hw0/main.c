#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
	char data;
	int count;
	struct Node *next;
} Node;

Node* create_node(char c) {
	Node* new_node = (Node*)malloc(sizeof(Node));
	if (new_node == NULL) {
		perror("malloc failed");
		exit(1);
	}
	new_node->data = c;
	new_node->count = 1;
	new_node->next = NULL;
	return new_node;
}

int main(void) {
	FILE *fp = fopen(__FILE__, "r");
	if (fp == NULL) {
		perror("Error opening file");
		return 1;
	}

	Node *head = NULL, *tail = NULL;
	Node *ascii[256] = {0};

	char ch;
	while ((ch = fgetc(fp)) != EOF) {
		if (ascii[(unsigned char)ch] != NULL) {
			ascii[(unsigned char)ch]->count++;
		} else {
			Node *new_node = create_node(ch);
			if (head == NULL) {
				head = tail = new_node;
			} else {
				tail->next = new_node;
				tail = new_node;
			}
			ascii[(unsigned char)ch] = new_node;
		}
	}
	fclose(fp);

	Node *current = head;
	while (current != NULL) {
		if (current->data == '\n')
			printf("'\\n' : %d\n", current->count);
		else if (current->data == ' ')
			printf("' ' : %d\n", current->count);
		else if (current->data == '\t')
			printf("'\\t' : %d\n", current->count);
		else
			printf("%c : %d\n", current->data, current->count);
		current = current->next;
	}

	//bonus
	char target = 'a';

	//linear search O(n)
	current = head;
	int index = 0;
	Node *found = NULL;
	while (current != NULL) {
		if (current->data == target) {
			found = current;
			break;
		}
		index++;
		current = current->next;
	}
	if (found != NULL) {
		printf("Linear search: char '%c': %d at linked list node %d.\n", target, found->count, index);
	} else {
		printf("Linear search: char '%c' does not exist.\n", target);
	}
	
	// Hash table search O(1)
	if (ascii[(unsigned char)target] != NULL) {
		printf("Hash table search: char '%c': %d.\n", target, ascii[(unsigned char)target]->count);
	} else {
		printf("Hash table search: char '%c' does not exist.\n", target);
	}
	
	printf("Linear search: O(n)\nHash table O(1)");

	//release mem
	current = head;
	while (current != NULL) {
		Node *temp = current;
		current = current->next;
		free(temp);
	}

	return 0;
}
