#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <float.h>

typedef struct node{	

	float cost;
	int *path;
	int level;
	int actorIndex;
	struct node *father;
	struct node **son;

} Node;

typedef struct groups{
	
	int size;
	int *groups;
	
} Groups;

typedef struct actor{
	
	int cost;
	Groups *groupsList;

} Actor;

typedef struct actorList{

	int size;
	Actor **actors;
	
} ActorList;

int main(int argc, char *argv[ ]){
		
	//Variaveis usadas no input
	bool parameter_a = false;
	int *all_groups;
	int n_groups = 0, n_actors = 0, n_chars = 0;
	int i,j;
	//Verificação de argumentos
	if(argc > 2)
		printf("O numero de argumentos e maior do que o esperado");
	if(argc == 2){
		if(strcmp(argv[1], "-a"))
			parameter_a = true;
		else{
			
			return 1;
		}
	}
	
	//Tratamento do input
	scanf("%d %d %d", &n_groups, &n_actors, &n_chars);

	int aux_n_groups, aux_cost;

	ActorList *actorsList = malloc(sizeof(ActorList));
	actorsList->actors = malloc(sizeof(Actor)*n_actors);
	actorsList->size = n_actors;
	
	//Passando por cada ator e definindo seus custos e grupos
	for(i = 0; i < n_actors; i++){
		Actor *actor;
		Groups *group;
		scanf("%d %d", &aux_cost , &aux_n_groups);
		
		actor = malloc(sizeof(Actor));
		actor->cost = aux_cost;
		group = malloc(sizeof(Groups));
		group->groups = malloc(sizeof(Groups)*aux_n_groups);
		group->size = aux_n_groups;
		
		for(j = 0; j < aux_n_groups; j++){
			scanf("%d", &group->groups[j]);
		}
		actor->groupsList = group;
		actorsList->actors[i] = actor;
	}
	
	//Gerando a raiz da árvore
	Node *aux;
	Node *root = malloc(sizeof(Node));
	root->actorIndex = -1;
	root->cost = 0;
	root->father = NULL;
	root->level = 1;
	aux = root;
	
	//malloc para número de filhos otimizados
	int n_filhos = n_actors-((n_chars-1)-(aux->level-1));
	aux->son = malloc(sizeof(Node)*n_filhos);
	
	for(i = 0; i < n_filhos; i++){
		aux->son[i]->actorIndex = i;
		aux->son[i]->cost = 0;
		aux->son[i]->level = aux->level+1;
		aux->son[i]->path[] = realloc(aux->son[i]->path[], sizeof((int)*aux->level));
		aux->son[i]->path[aux->level-1] = i;
		for(j = 0; j < 0; j++){
			aux->son[i]->cost = aux->son[i]->cost + actorList->actors[j]->cost*n_groups/actorList->actors[j]->groupsList->size;
		}
	}
	
	float aux;
	float bound = FLT_MAX;
	//soma de todos os personagens - personagem atual - personagens do caminho
	for(i = 0; i < n_filhos; i++){
		for(j = 0; j < n_actors; j++){
			if(i != j)
				aux = 
		}
	}
	
	return 0;
}

