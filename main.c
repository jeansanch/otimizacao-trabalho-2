#include <stdio.h>
#include <stdlib.h>



typedef struct node
{	
	float value;
	struct node *son;
} Node;

typedef struct actor
{
	int cost;
	int groups[];
} Actor;


int main(int argc, char *argv[ ]){

	if(argc > 1)
		printf("O número de argumentos é maior do que o esperado");

	int *AllGroups;

	return 0;
}

Actor* startActor(int size){

}
