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
	int n_groups = 0, n_actors = 0, n_chars = 0;
	int i;
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
	scanf("%d %d %d", &n_groups, &n_actors, &n_chars);
	//alocando dinamicamente os atores
	Actor actors = malloc(sizeof(Actor)*n_actors);
	//passando por cada ator e definindo seus custos e grupos
	for(i = 0; i < n_atores; i++){
		int aux_n_groups
		scanf("%d %d", &actors[i]->cost, &aux_n_groups);
		&actors[i]->groups = malloc(sizeof(int)*aux_n_groups);
		
	}
	return 0;
}

Actor* startActor(int size){

}
