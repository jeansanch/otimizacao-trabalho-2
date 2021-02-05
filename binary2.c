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
	struct node *father, *left, *right;
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
float bound(Node *no, int ngroups, ActorList *list, int nchars);
float newGroupTotal(bool *actual, Groups *newGroups);
bool containGroup(Groups *groups, int need);
bool isGroupsTrue(bool *groups, int size);
bool isInAllGroups(int ngroups, int *list, int size, ActorList *actorL);
int actorsInGroup(int *groupList, int size);
int sumOfCosts(float *groups, int size);
void printResult(int *path, int size, ActorList *list);

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
		aux->left = malloc(sizeof(Node));
		aux->right = malloc(sizeof(Node));
		aux->left->father = aux;
		aux->right->father = aux;
		aux->left->nactors = aux->nactors + 1;
		if(aux->left->path != NULL)
			aux->left->path = NULL;
		aux->left->path = realloc(aux->path, sizeof(int)*aux->left->nactors);
		aux->left->level = aux->level + 1;
		aux->left->path[aux->left->nactors - 1] = aux->left->level - 1;
		aux->left->cost = bound(aux->left, n_groups, actorsList, n_chars);
		printf("CUSTO ESQUERDA CALCULADO = %f", aux->left->cost);
		
		aux->right->nactors = aux->nactors;
		if(aux->right->path != NULL)
			aux->right->path = NULL;
		aux->right->path = aux->path;
		aux->right->level = aux->level + 1;
		aux->right->cost = bound(aux->right, n_groups, actorsList, n_chars);
		printf("CUSTO DIREITA CALCULADO = %f", aux->right->cost);
		
		if(aux->left->cost == -1){
			if (aux->right->cost == -1){
				printf("Inviavel");
				return 0;
			}
			else{
				aux = aux->son[i];
			}
		}
		else if(aux->right->cost == -1){
			aux = aux->left;
		}
		else{
			printf("E: %f\nD: %f\n", aux->left->cost, aux->right->cost);
			if(aux->left->cost == aux->right->cost){
				if (aux->left->nactors == n_chars){
					if(isInAllGroups(n_groups, aux->path, aux->nactors, actorsList)){
						aux = aux->left;
						printf("Esquerda!\n");
					}
					else{
						aux = aux->right;
						printf("Direita!\n");
					}
					break;
				}
				aux = aux->left;
					printf("Esquerda!\n");
			}
			else if(aux->left->cost < aux->right->cost){
				aux = aux->left;
				printf("Esquerda!\n");
			}
		 	else{
		 		aux = aux->right;
		 		printf("Direita!\n");
			}
		}
	}
	
	if(isInAllGroups(n_groups, aux->path, aux->nactors, actorsList)){
		printResult(aux->path, aux->nactors, actorsList);
		return 0;
	}
	printf("Inviavel");
	return 0;
}

void printResult(int *path, int size, ActorList *list){
	int i, sum;
	
	for(i = 0; i < size; i++){
		printf("%d ", path[i]);
		sum = list->actors[path[i]]->cost;
	}
	printf("\n%d",sum);
}

float bound(Node *no, int ngroups, ActorList *list, int nchars){
	
	float vgroups[ngroups];
	bool bgroups[ngroups];
	int agroups[ngroups];
	float menor;
	int amenor;
	int i, j;
	float aux, aux1, aux2;
	//Limpando lixo da memoria
	for(i = 0; i < ngroups; i++){
		vgroups[i] = FLT_MAX;
		bgroups[i] = false;
		agroups[i] = -1;
	}
	//calculo inicial com grupos do path
	for(i = 0; i < ngroups; i++){
		menor = FLT_MAX;
		amenor = -1;
		for(j = 0; j < no->nactors; j++){
//			list->actors[no->path[j]]->groupsList[]
			if(containGroup(list->actors[no->path[j]-1]->groupsList, i + 1)){
				aux1 = list->actors[no->path[j]-1]->cost;
				aux2 = pow(10, newGroupTotal(bgroups, list->actors[no->path[j]-1]->groupsList));
				aux = aux1/aux2;
				if(aux < menor){
					menor = aux;
					amenor = j;
				}
			}
		}
		
		if(amenor != -1){
			for(j = 0; j < list->actors[amenor]->groupsList->size; j++){
				if(bgroups[list->actors[amenor]->groupsList->groups[j] - 1] == true){
					if(list->actors[amenor]->cost < vgroups[list->actors[amenor]->groupsList->groups[j] - 1]){
						vgroups[list->actors[amenor]->groupsList->groups[j] - 1] = list->actors[amenor]->cost;
						agroups[list->actors[amenor]->groupsList->groups[j] - 1] = amenor;
					}
				}
				else{
					bgroups[list->actors[amenor]->groupsList->groups[j] - 1] = true;
					vgroups[list->actors[amenor]->groupsList->groups[j] - 1] = list->actors[amenor]->cost;
					agroups[list->actors[amenor]->groupsList->groups[j] - 1] = amenor;
				}
			}
		}
	}
	
	//previsao de melhor grupo
	for(i = 0; i < ngroups; i++){
		menor = FLT_MAX;
		amenor = -1;
			for(j = no->level-1; j < list->size; j++){
				if(containGroup(list->actors[j]->groupsList, i + 1)){
					aux = list->actors[j]->cost/pow(10, newGroupTotal(bgroups, list->actors[j]->groupsList));
					if(aux < menor){
						menor = aux;
						amenor = j;
					}
				}
			}
			if(amenor != -1){
				for(j = 0; j < list->actors[amenor]->groupsList->size; j++){
					if(bgroups[list->actors[amenor]->groupsList->groups[j] - 1] == true){
						if(list->actors[amenor]->cost < vgroups[list->actors[amenor]->groupsList->groups[j] - 1]){
							vgroups[list->actors[amenor]->groupsList->groups[j] - 1] = list->actors[amenor]->cost;
							agroups[list->actors[amenor]->groupsList->groups[j] - 1] = amenor;
						}
					}
					else{
						bgroups[list->actors[amenor]->groupsList->groups[j] - 1] = true;
						vgroups[list->actors[amenor]->groupsList->groups[j] - 1] = list->actors[amenor]->cost;
						agroups[list->actors[amenor]->groupsList->groups[j] - 1] = amenor;
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
	int total = 0, i;
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

bool isInAllGroups(int ngroups, int *list, int size, ActorList *actorL){
	int i, j;
	//Checando se todos os grupos foram escolhido
	bool groups[ngroups];
	for(i = 0; i < ngroups; groups[i] = false, i++);
	for(i = 0; i < size; i++){
		for (j = 0; j < actorL->actors[list[i]]->groupsList->size; j++){
			groups[actorL->actors[list[i]]->groupsList->groups[j]] = true;
		}
	}
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
