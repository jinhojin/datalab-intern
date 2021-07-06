#include <stdio.h>
#include <stdlib.h>

typedef struct _node {
	int data;
	struct _node *prev;
	struct _node *next;
	
}node;




node* append(node *list, int input_data){
	if(list -> next == NULL){
		node * new_node =(node*) malloc(sizeof(node));
		new_node -> data = input_data;
		list -> next = new_node;
		new_node-> prev = list;
		new_node -> next = NULL;
	}

	else{
		node *current_node;
		current_node = list;
		while( (current_node -> next) -> data <= input_data){
			current_node = current_node -> next;
			if(current_node -> next == NULL){
				break;
			}
			
		}
		if(current_node -> next != NULL){

			node * new_node =(node*) malloc(sizeof(node));
			new_node -> data = input_data;
		
			new_node -> next = current_node -> next;
			(current_node -> next) -> prev = new_node;
		
			current_node -> next = new_node;
			new_node -> prev = current_node;
		}
		else{
			node * new_node =(node*) malloc(sizeof(node));
                        new_node -> data = input_data;

                        new_node -> next = NULL;

                        current_node -> next = new_node;
                        new_node -> prev = current_node;
		}
	}	

}


void delete(node *list, int input_data){
	node *current_node;
        current_node = list;
        if ( current_node -> next == NULL){
		free(list);
		list ==NULL;
	}
	else{
        	while( (current_node -> next) -> data < input_data){
        		current_node = current_node -> next;
        	}
		if( (current_node -> next) -> data == input_data){
			if((current_node -> next) -> next != NULL){
				current_node -> next = (current_node -> next) -> next;
				(current_node -> next) -> prev = current_node;
			}
			else{
				current_node -> next = NULL;
			}
		}
		else{
			printf("data is not in the linked list");
		}
	}
}

void show_nodes(node *list){
	node *current_node;
	current_node = list;
	if (current_node == NULL){
		printf("data is not in here");
	}
	else{

		while( (current_node -> next) != NULL){
			printf("[");
			printf("%d", current_node -> data);
			printf("]-");
			current_node = current_node -> next;
		}
		printf("[");
		printf("%d", current_node -> data);
		printf("]\n");
	}
}
int main(){
	
	node* head_node =(node*) malloc(sizeof(node));
	head_node -> next = NULL;
	head_node -> prev = NULL;
	
	int i;
        int j;
        int count = 0;
        while (i != 4) {
                printf("1. insert key\n");
                printf("2. delete key\n");
                printf("3. print list\n");
		printf("4.exit\n");
                printf("select menu:");
                scanf("%d", &i);
                if (i == 1) {
                        printf("write any integer:");
                        scanf("%d", &j);
                        append(head_node, j);
                        printf("\n\n");
                } else if (i == 2) {
                        printf("write any integer:");
                        scanf("%d", &j);
                        delete(head_node,j);
                        printf("\n\n");
                } else if (i == 3) {
			show_nodes(head_node);
			printf("\n\n");
		} else if (i == 4) {
			return 0;
		} else {
			printf("wrong integer");
		}

	}
}

