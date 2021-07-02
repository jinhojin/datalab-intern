#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _Node {
	int element;
	struct _Node *right;
	struct _Node *left;
} Node;

Node * root=NULL; // root에서 시작해서 재귀 해주고 , 계속 갱신해주니 루트는 전역변수로 빼기

Node *insertionnode(Node * node, int key)
{
	if (node == NULL) {
		Node *newnode = malloc(sizeof(Node)); // 추가할 때 말록하기
		newnode->element = key;
		newnode->right = NULL;
		newnode->left = NULL;
		return newnode;
	} else if (node->element > key) {
		node->left = insertionnode(node->left, key); // 계속 재귀해가며 추가할 위치 찾기
		return node; // 함수가 노드를 반환하는 함수이니 리턴 노드 포인터 해주기
	} else if (node->element < key) {
		node->right = insertionnode(node->right, key);
		return node;
	} else {
		printf("error");
		return NULL; // 아무것도 아닐 때의 포인터인 널 반환
	}
}

void printnodes(Node * node)
{
	if (node != NULL) {
		printnodes(node->left); // inorder traversal로 찾아서 출력하는 코드임 !! 최소값가져오는 것이라고 생각하면 됨
		printf("%d\n", node->element);
		printnodes(node->right);
	}
}

void  deletionnode(Node *pnode, Node *node, int key)
{
	if (node != NULL) {
		if (node->element != key) { // 내가 삭제할 키랑 현재 노드의 키가 같지 않을 때
			if (node->element>key){
				deletionnode(node, node->left, key); // 루트보다 내가 찾는 키가 작으면 왼쪽으로 가서 재귀
			} else if (node->element<key){
				deletionnode(node, node->right, key); // 반대일 때는 오른쪽으로 가서 재귀
			}
		} else { // 현재 노드랑 내가 지울 노드의 키가 같을 때
			if (node->left == NULL && node->right != NULL) { // 그 노드의 위치에 대한 경우의수 4가지 준비하고
				if (pnode == NULL) { // 지울 노드가 루트일 때는 루트 자식들로 루트를 대체하고 ( 포인터 느낌) 그리고 원래 루트는 프리시켜준 다음 루트노드 위치 갱신 후 함수 끝내주기
					node = node->right;
					free(root);
					root = node;
					return;
				} else if (pnode->right == node) {
					pnode->right = node->right;
				} else if (pnode->left == node) {
					pnode->left = node->right;
				} else
					printf("error");
				free(node); // 아닌 경우에 말록 프리
			} else if (node->left != NULL && node->right == NULL) {
				if (pnode == NULL) {
					node = node->left;
					free(root);
					root = node;
					return;
				} else if (pnode->right == node) {
					pnode->right = node->left;
				} else if (pnode->left == node) {
					pnode->left = node->left;
				} else
					printf("error");
				free(node);	
			} else if (node->left != NULL && node->right != NULL) {
				if (pnode == NULL) {
					node = node->left;
					free(root);
					root=node;
					return;
				} else if (pnode->right == node) {
					pnode->right = node->left;
				} else if (pnode->left == node) {
					node->left->right=node->right;
					pnode->left = node->left;
				} else
					printf("error");
				free(node);
			} else{
				if (pnode==NULL){
					free(root);
					root=NULL; // 루트 밖에 안남은 경우에는 루트를 널로 해준 후 함수끝내기
					return ;
				}
				if (pnode->right==node){
					pnode->right=NULL;
				}
				if (pnode->left==node) {
					pnode->left=NULL;
				}
				free(node);
			}
	    }
	}
	return;
}

int main()
{

	root = insertionnode(root, 3);
	insertionnode(root, 2);
	insertionnode(root, 5);
	printf("print++\n");
	printnodes(root);
	printf("print--\n");
	deletionnode(NULL, root, 3);
	printnodes(root);
}
