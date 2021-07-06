#include <stdio.h>
#include <stdlib.h>

typedef struct _Node {
	int element;
	struct _Node *next;
	struct _Node *pre;
} Node;

Node *head = NULL;

Node *insertionnode(Node * node, int key)
{
	if (node == NULL) {
		printf("Node is NULL\n");
		Node *newnode = malloc(sizeof(Node));
		newnode->element = key;
		newnode->next = NULL;
		newnode->pre = NULL;
		return newnode;
	} else if (node->element < key) {
		if (node->next == NULL) {
			// 2
			printf("node->next is NULL\n");
			Node *newnode = malloc(sizeof(Node));
			newnode->element = key;
			newnode->pre = node;
			newnode->next = NULL;
			node->next = newnode;
			return newnode;
		}
		else {		// 앞뒤에 무슨 코드가 없어야 하고 그 경우를 잘 나눠서 생각해야함
			printf("node->next is not NULL, recursing\n");
			insertionnode(node->next, key);
		}
	} else {
		// 5
		printf("Found node: %d, trying to put key: %d\n", node->element, key);
		Node * newnode=malloc(sizeof(Node));
		newnode->element=key;
		newnode->pre=node->pre;
		newnode->next=node;
		node->pre->next=newnode;
		node->pre=newnode;
		return newnode;
	}

}

void deletionnode(Node * node, int key)
{
	if (node == NULL)
		return;
	if (node->element == key) {
		if (node->next != NULL) {
			if (node->pre == NULL) {
				head = node->next;
				head->pre = NULL;
				free(node);
				return;
			} else {
				printf("node: %d\n", node->element);
				printf("node->pre: %d\n", node->pre->element);
				printf("node->next: %d\n", node->next->element);
				(node->pre)->next = node->next;
				(node->next)->pre = node->pre;
				free(node);
				return;
			}
		} else {
			(node->pre)->next = NULL;
			free(node);
			return;
		}
	} else {
		deletionnode(node->next, key);
	}
}

void printnodes(Node * node)
{
	if (node != NULL) {
		printf("%d\n", node->element);
		printnodes(node->next);
	}
}

int main()
{
	head = insertionnode(head, 1);
	printf("print++\n");
	printnodes(head);
	printf("print--\n");
	insertionnode(head, 3);
	printf("print++\n");
	printnodes(head);
	printf("print--\n");
	insertionnode(head, 2);
	printf("print++\n");
	printnodes(head);
	printf("print--\n");
	deletionnode(head, 2);
	printf("print++\n");
	printnodes(head);
	printf("print--\n");
	insertionnode(head, 4);
	printf("print++\n");
	printnodes(head);
	printf("print--\n");
}
