#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <float.h>
#include <limits.h>
#include <time.h>

typedef struct node{	

	float cost;
	bool *g;
	int ngps;
	int *path;
	int level;
	int nactors;
	int actorIndex;
	struct node *left, *right;
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

typedef struct stack{
	
	struct stack *next;
	Node *no;
	
} Stack;

void popAndStack(Stack **s, bool add);
void printResult(int *path, int size, ActorList *list);
float bound(Node *no, ActorList *list, bool left, bool a, bool *newGroups, bool *oldGroups, int n_groups);
bool isInAllGroups(int ngroups, int *list, int size, ActorList *actorL);
//float bound(Node *no, ActorList *list, bool left, bool a);

int main(int argc, char *argv[ ]){
		
	//Variaveis usadas no input
	int visited = 0;
	int verified = 0;
	bool parameter_a = false;
	int *all_groups;
	int n_groups = 0, n_actors = 0, n_chars = 0;
	int i,j, k;
	bool *groupList;
	//Verificação de argumentos
	if(argc > 2){
		printf("O numero de argumentos e maior do que o esperado!\n");
		printf("Continuando o programa supondo que o primeiro parametro seja -a\n");
	}
	if(argc == 2){
		if(strcmp(argv[1], "-a%0")){		
			parameter_a = true;
		}
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

	#ifdef DEBUG
		clock_t start = clock();
		double time_spent;
	#endif
	
	//Gerando a raiz da árvore e as variaveis necessarias
	Stack *sa = malloc(sizeof(Stack));
	sa->next = NULL;
	sa->no = NULL;
	int *reacopy = NULL;
	Node *optimal = malloc(sizeof(Node));
	optimal->cost = FLT_MAX;
	Node *aux;
	Node *root = malloc(sizeof(Node));
	root->g = malloc(sizeof(bool)*n_groups);
	for(i = 0; i < n_groups; i++){
		root->g[i] = false;
	}
	root->actorIndex = 0;
	root->cost = 0;
	root->ngps = 0;
	root->level = 1;
	root->used = true;
	root->nactors = 0;
	root->path = NULL;
	sa->no = root;
	
	//percorrendo os nos
	while(sa != NULL){
		verified++;
		aux = sa->no;
		if(parameter_a && aux->ngps == n_groups){
			if(aux->nactors == n_chars){
				if(optimal->cost > aux->cost)
					optimal = aux;
				popAndStack(&sa, false);
			}
			else{
				float simCost = aux->cost;
				int left = n_chars - aux->ngps;
				float cheaper = FLT_MAX;
				bool flaga = false;
				int bestactor;
				while(aux->nactors != n_chars){
					for(i = 0; i < n_actors; i++){
						flaga = false;
						for(j = 0; j < aux->nactors; j++){
							if(i+1 == aux->path[j]){
								flaga = true;
							}
						}
						if(flaga == false){
							if(actorsList->actors[i]->cost < cheaper){
								cheaper = actorsList->actors[i]->cost;
								bestactor = i+1;
							}
						}
					}
					left++;
					aux->nactors++;
					aux->path = realloc(aux->path, sizeof(int)*aux->nactors);
					aux->path[aux->nactors-1] = bestactor;
					aux->cost += cheaper;
					cheaper = FLT_MAX;
				}
				if(optimal->cost > aux->cost){
					optimal = aux;
				}
				popAndStack(&sa, false);
			}
		}
		else if(optimal->cost < aux->cost){
		//	printf("REMOVENDO DA STACK\n");
			popAndStack(&sa, false);
		}
		else if(aux->nactors == n_chars){
			if(aux->ngps == n_groups){
				if(optimal->cost > aux->cost)
					optimal = aux;
			}
			//	printf("REMOVENDO DA STACK\n");
				popAndStack(&sa, false);
		}
		else if(!parameter_a && aux->nactors + n_actors - (aux->level-1) < n_chars){
			popAndStack(&sa, false);
		}
		else if(aux->level > n_actors){
			popAndStack(&sa, false);
		}
		else{
			visited++;
			aux->left = malloc(sizeof(Node));
			aux->right = malloc(sizeof(Node));
			if(aux == root){
				aux->left->path = NULL;
				aux->right->path = NULL;
			}
			aux->right->nactors = aux->nactors;
			aux->right->path = aux->path;
			aux->right->level = aux->level + 1;
			aux->right->g = malloc(sizeof(bool)*n_groups);
			aux->right->ngps = 0;
			for(i = 0; i < n_groups; i++){
				aux->right->g[i] = false;
				if(aux->g[i]){
					aux->right->g[i] = true;
					aux->right->ngps++;
				}
			}
			aux->right->cost = bound(aux, actorsList, false, parameter_a, aux->right->g, aux->g, n_groups);
			
//			aux->right->cost = bound(aux, actorsList, false, parameter_a);
			
			aux->left->nactors = aux->nactors + 1;
			aux->left->path = malloc(aux->left->nactors*sizeof(int));
			aux->left->level = aux->level + 1;
			aux->left->g = malloc(sizeof(bool)*n_groups);
			aux->left->ngps = 0;
			for(i = 0; i < n_groups; i++){
				aux->left->g[i] = false;
				if(aux->g[i]){
					aux->left->g[i] = true;
					aux->left->ngps++;	
				}
			}
			for(i = 0; i < actorsList->actors[aux->left->level-2]->groupsList->size; i++){
				if(aux->left->g[actorsList->actors[aux->left->level-2]->groupsList->groups[i]-1] == false){
					aux->left->ngps++;
					aux->left->g[actorsList->actors[aux->left->level-2]->groupsList->groups[i]-1] = true;
				}
			}
			//Copiando a array do caminho do no anterior para o novo
			for(k = 0; k < aux->nactors; aux->left->path[k] = aux->path[k], k++);
			if(aux->left->nactors == 9){
				if(aux->left->path[0] == 1 && aux->left->path[1] == 2 && aux->left->path[2] == 3 && aux->left->path[3] == 4 && aux->left->path[4] == 5 && aux->left->path[5] == 17 && aux->left->path[6] == 18 && aux->left->path[7] == 19){
					printf("asidcasdj");
				}
			}
			aux->left->path[aux->left->nactors - 1] = aux->left->level - 1;
			aux->left->cost = bound(aux, actorsList, true, parameter_a, aux->left->g, aux->g, n_groups);
//			aux->left->cost = bound(aux, actorsList, true, parameter_a);
			//printf("ADICIONANDO A STACK\n");
			popAndStack(&sa, true);
		}
		
	}
	
	printf("\n");
	if(optimal->cost == FLT_MAX){
		printf("Inviavel");
	}
	else
		printResult(optimal->path, n_chars, actorsList);
	#ifdef DEBUG
		printf("\n\nNOS VERIFICADOS = %d\n", verified);
		printf("NOS PERCORRIDOS = %d\n", visited);
		clock_t end = clock();
		time_spent += (double)(end - start) / CLOCKS_PER_SEC;
		printf("TIME: %f seconds\n", time_spent);
	#endif
	return 0;
}

void popAndStack(Stack **s, bool add){
	Stack *auxl = malloc(sizeof(Stack));
	Stack *auxr = malloc(sizeof(Stack));
	if(add){	
		auxl->no = (*s)->no->left;
		auxr->no = (*s)->no->right;
		auxl->next = auxr;
		auxr->next = (*s)->next;
		*s = auxl;
	}
	else{
		*s = (*s)->next;
	}		
}

void printResult(int *path, int size, ActorList *list){
	int i, sum = 0;
	
	for(i = 0; i < size; i++){
		printf("%d ", path[i]);
		sum = sum + list->actors[path[i]-1]->cost;
	}
	printf("\n%d",sum);
	
	return;
}

float bound(Node *no, ActorList *list, bool left, bool a, bool *newGroups, bool *oldGroups, int n_groups){
	
	if(left){
		if(a){
//			printf("Using group into consideration\n");
			int i;
			bool flag = false;
			for(i = 0; i < n_groups; i++){
				if(newGroups[i] == true && oldGroups[i] == false){
					flag = true;
				}
			}
			if(flag == true)
				return no->cost + list->actors[(no->left->level-1)-1]->cost;
			return FLT_MAX;
//			printf("Number of new groups = %d\n", total);
//			printf("Old cost = %f\nCost without -a = %f\nCost with -a = %f\n", no->cost, no->cost + list->actors[(no->left->level-1)-1]->cost, no->cost + list->actors[(no->left->level-1)-1]->cost*(1-(float)n_groups/total));
		}
	//lazy bound
		return no->cost + list->actors[(no->left->level-1)-1]->cost;
	}
	return no->cost;
}

bool isInAllGroups(int ngroups, int *list, int size, ActorList *actorL){
	int i, j;
	//Checando se todos os grupos foram escolhido
	bool groups[ngroups];
	for(i = 0; i < ngroups; groups[i] = false, i++);
	for(i = 0; i < size; i++){
		for (j = 0; j < actorL->actors[list[i]-1]->groupsList->size; j++){
			groups[actorL->actors[list[i]-1]->groupsList->groups[j]-1] = true;
		}
	}
	for(i = 0; i < ngroups; i++){
		if(groups[i] == false)
			return false;
	}
	return true;
}

