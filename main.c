#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct node{	

	float value;
	struct node *son;

} Node;

typedef struct actor{
	
	int cost;
	int groups[];

} Actor;


int main(int argc, char *argv[ ]){
	
	//Variaveis
	bool parameter_a = false;
	int *all_groups;
	Actor *actors;
	int n_grupos = 0, n_atores = 0, n_personagens = 0;
	
	//Verificação de argumentos
	if(argc > 2)
		printf("O numero de argumentos e maior do que o esperado");
	if(argc == 2){
		if(!strcmp(argv[1], "-a"))
			parameter_a = true;
		else{
			
			return 1;
		}
	}
	
	//Tratamento do input
	scanf("%d %d %d", &n_grupos, &n_atores, &n_personagens);
	printf("%d %d %d", n_grupos, n_atores, n_personagens);
	
	return 0;
}

Actor* startActor(int size){

}
