#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <float.h>
#include <math.h>
#include <limits.h>


typedef struct node{	

	float cost;
	bool *g;
	int *path;
	int level;
	int nactors;
	int actorIndex;
	struct node *father;
	struct node **son;
	bool used;

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

bool isNotInPath(int *path, int actorIndex, int limit);
bool isInAllGroups(int ngroups, int *list, int limit);
float numberOfGroups(bool *groups, int size);
float bound(Node *no, int ngroups, ActorList *list, int nchars);
float newGroupTotal(bool *actual, Groups *newGroups);
bool containGroup(Groups *groups, int need);
bool isGroupsTrue(bool *groups, int size);
bool isInAllGroups(int ngroups, int *list, ActorList actorL, int limit);
int main(int argc, char *argv[ ]){
		
	//Variaveis usadas no input
	bool parameter_a = false;
	int *all_groups;
	int n_groups = 0, n_actors = 0, n_chars = 0;
	int i,j;
	bool *groupList;
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
	
	groupList = malloc(sizeof(bool)*n_groups);
	
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
	root->actorIndex = 0;
	root->cost = 0;
	root->father = NULL;
	root->level = 1;
	root->used = true;
	root->nactors = 0;
	root->path = NULL;
	aux = root;
	
	while(aux->nactors != n_chars){
		aux->son = malloc(sizeof(Node)*2);
		
		aux->son[0]->nactors = aux->nactors + 1;
		aux->son[0]->path = realloc(aux->son[i]->path, sizeof(int)*aux->son[0]->nactors);
		aux->son[0]->level = aux->level + 1;
		aux->son[0]->path[aux->son[0]->nactors - 1] = aux->son[0]->level - 1;
		aux->son[0]->cost = bound(aux->son[0], n_groups, actorsList, n_chars);
		
		aux->son[1]->nactors = aux->nactors;
		aux->son[1]->path = aux->path;
		aux->son[1]->level = aux->level + 1;
		aux->son[1]->cost = bound(aux->son[1], n_groups, actorsList, n_chars);
		
		if(aux->son[0]->cost == -1){
			if (aux->son[1]->cost == -1){
				printf("Inviavel");
				return 0;
			}
			else{
				aux = aux->son[i];
			}
		}
		else if(aux->son[1]->cost == -1){
			aux = aux->son[0];
		}
		else{
			aux = aux->son[0]->cost < aux->son[1]->cost ? aux->son[0] : aux->son[1];
		}
	}
	
	
	return 0;
}

float bound(Node *no, int ngroups, ActorList *list, int nchars){
	
	float vgroups[ngroups];
	bool bgroups[ngroups];
	int agroups[ngroups];
	float menor;
	int amenor;
	int aux, i, j;
	
	//calculo inicial com grupos do path
	for(i = 0; i < ngroups; i++){
		menor = FLT_MAX;
		amenor = -1;
		for(j = 0; j < no->nactors; j++){
//			list->actors[no->path[j]]->groupsList[]
			if(containGroup(list->actors[no->path[j]]->groupsList, i)){
				aux = list->actors[no->path[j]]->cost/pow(10, newGroupTotal(bgroups, list->actors[no->path[j]]->groupsList));
				if(aux < menor){
					menor = aux;
					amenor = j;
				}
			}
		}
		
		for(j = 0; j < list->actors[amenor]->groupsList->size; j++){
			if(bgroups[list->actors[amenor]->groupsList->groups[j]] == true){
				if(list->actors[amenor]->cost < vgroups[list->actors[amenor]->groupsList->groups[j]]){
					vgroups[list->actors[amenor]->groupsList->groups[j]] = list->actors[amenor]->cost;
					agroups[list->actors[amenor]->groupsList->groups[j]] = amenor;
				}
			}
			else{
				bgroups[list->actors[amenor]->groupsList->groups[j]] = true;
				vgroups[list->actors[amenor]->groupsList->groups[j]] = list->actors[amenor]->cost;
				agroups[list->actors[amenor]->groupsList->groups[j]] = amenor;
			}
		}
	}
	
	//previsao de melhor grupo
	for(i = 0; i < ngroups; i++){
		menor = FLT_MAX;
		amenor = -1;
		if(bgroups == false){
			for(j = no->level; j < list->size; j++){
				if(containGroup(list->actors[j]->groupsList, i)){
					aux = list->actors[j]->cost/pow(10, newGroupTotal(bgroups, list->actors[j]->groupsList));
					if(aux < menor){
						menor = aux;
						amenor = j;
					}
				}
			}
			for(j = 0; j < list->actors[amenor]->groupsList->size; j++){
				if(bgroups[list->actors[amenor]->groupsList->groups[j]] == true){
					if(list->actors[amenor]->cost < vgroups[list->actors[amenor]->groupsList->groups[j]]){
						vgroups[list->actors[amenor]->groupsList->groups[j]] = list->actors[amenor]->cost;
						agroups[list->actors[amenor]->groupsList->groups[j]] = amenor;
					}
				}
				else{
					bgroups[list->actors[amenor]->groupsList->groups[j]] = true;
					vgroups[list->actors[amenor]->groupsList->groups[j]] = list->actors[amenor]->cost;
					agroups[list->actors[amenor]->groupsList->groups[j]] = amenor;
				}
			}
		}
	}
	
	if(isGroupsTrue(bgroups, ngroups)){
		if(actorsInGroup(agroups, ngroups) <= nchars){
			return sumOfCosts(vgroups, ngroups);
		}
		return -1;		
	}
	return -1;
	
}

float newGroupTotal(bool *actual, Groups *newGroups){
	int i;
	float diff = 0;
	for(i = 0; i < newGroups->size; i++){
		if(actual[newGroups->groups[i]] == false)
			diff += 1;
	}
	return diff;
}

bool containGroup(Groups *groups, int need){
	int i;
	for(i = 0; i < groups->size; i++){
		if(groups->groups[i] == need)
			return true;	
	}
	return false;
}

int sumOfCosts(float *groups, int size){
	int total, i;
	for(i = 0; i < size; i++){
		total += groups[i];
	}
	return total;
}

int actorsInGroup(int *groupList, int size){
	bool listActors[size];
	int i, total = 0;
	for(i = 0; i < size; i++){
		listActors[groupList[i]] = true;
	}
	for(i = 0; i < size; i++){
		listActors[i] ? total++ : 0;
	}
	return total;
}

bool isGroupsTrue(bool *groups, int size){
	int i;
	for(i = 0; i < size; i++){
		if(groups[i] == false)
			return false;
	}
	return true;
}

float numberOfGroups(bool *groups, int size){
	int i;
	float total = 0;
	for(i = 0; i < size; i++){
		if(groups[i] == true){
			total += 1;
		}
	}
	return total;
}


bool isInAllGroups(int ngroups, int *list, ActorList actorL, int limit){
	int i;
	//Checando se todos os grupos foram escolhidos
	for(i = 0; i < ngroups; i++){
		if(groups[i] == false)
			return false;
	}
	return true;
}

bool isNotInPath(int *path, int actorIndex, int limit){
	int i;
	bool flag = false;
	for(i = 0; i < limit; i++){
		if(path[i] == actorIndex)
			flag = true;
	}
	return flag;
}
