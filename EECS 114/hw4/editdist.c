/**
 *  \file editdist.c
 *  Mohammed Haque 62655407
    3/17/19
 *  \brief Implement your edit distance
 **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>

//COMMENTED OUT CODE WAS ATTEMPT TO MAKE ARRAY z USING A LINKED LIST

// struct zNode
// {
// 	char s;
// 	struct zNode *next;
// 	struct zNode *prev;
// }; typedef struct zNode zNode;

// zNode *head;

// zNode *createNewNode(char s){

// 	zNode *z = (zNode*)malloc(sizeof(zNode));
// 	assert(z != NULL);
// 	z->s = s;
// 	z->prev = NULL;
// 	z->next = NULL;

// 	return z;
// }

// void insertNodeTail(char s){

// 	zNode *temp = head;
// 	zNode *z = createNewNode(s);
// 	if(head == NULL){
// 		head = z;
// 		return;
// 	}
// 	while(temp->next != NULL) temp = temp->next;
// 	temp->next = z;
// 	z->prev = temp;

// }

// void insertNodeAfter(zNode *z, char s){

// 	if(z == NULL) return;
// 	zNode *q = (zNode*)malloc(sizeof(zNode));
// 	assert(q != NULL);
// 	q->s = s;
// 	q->next = z->next;
// 	z->next = q;

// 	free(q);

// }

// void removeNode(zNode *z){

// 	if(head == NULL){
// 		return;
// 	}
// 	if(head == z){
// 		head = z->next;
// 	}
// 	if(z->next != NULL){
// 		z->next->prev = z->prev;
// 	}
// 	if(z->prev != NULL){
// 		z->prev->next = z->next;
// 	}
	
// 	free(z);

// 	return;
// }

// int min(int ins, int del, int rep){

// 	if(ins < del && ins < rep)	return ins;
// 	else if(del < ins && del < rep)	return del;
// 	else	return rep;

// }

// void printZ(){

// 	zNode *temp = head;
// 	while(temp != NULL){
// 		printf("%c",temp->s);
// 		temp = temp->next;
// 	}
// 	printf("\n");
// }

void backTrace(char *x, char *y, int m, int n, int **operation){

	int i = 0, j = 0, k = 0, curr = 0;
	int cost = 0, totalCost = 0;
	char opDone[10];
	// zNode *temp = head;
	// zNode *z = temp;
	char *z = (char*)malloc(sizeof(char)*(m+n));		//array z to show transformation
	strncpy(z, x, m);			//initialize z with x
	// printZ();

	//print initial parts of table
	printf(" Operation | Cost | Total | z\n");
	printf(" initial   | %4d | %4d  | ", cost, totalCost);
	printf("%s\n", z);

	while(i < m || j < n){		//loop until end of operations
	
		if((operation[i][j] == 0) && i != m && j != n){			//if operation is right, move diagonally down in operation array and go to the next character in z
			z[curr] = x[i];
			i++;
			j++;
			cost = 0;
			strcpy(opDone, "right");
			curr++;
		}
		else if((operation[i][j] == 4) && i != m && j != n){	//if operation is replace, move diagonally down in operation array and replace the current character in z
			z[curr] = y[j];
			// insertNodeAfter(temp, y[j]);
			// removeNode(temp);
			i++;
			j++;
			cost = 4;
			strcpy(opDone, "replace");
			curr++;
		}
		else if((operation[i][j] == 3) && j != n){ 				//if operation is insert, move down in operation array and insert y[j] in current position of z
			z[curr] = y[j];
			// insertNodeAfter(temp, y[j]);
			j++;
			cost = 3;
			strcpy(opDone, "insert");
			curr++;
		}
		else if((operation[i][j] == 2) && i != m){				//if operation is delete, move right in operation array and delete current character of z
			// removeNode(temp);
			i++;
			cost = 2;
			strcpy(opDone, "delete");
		}
	
		// z = temp->next;
		totalCost += cost;		//increment total cost

		//print operation, cost, total cost, and z
		printf("%10s | %4d | %4d  | ", opDone, cost, totalCost);
		for(k = 0; k < curr; k++){
			printf("%c",z[k]);
			// temp = temp->next;
		}
		printf("_");
		for(k = i; k < m; k++){
			printf("%c",x[k]);
			// temp = temp->next;
		}
		printf("\n");
		// temp = z;
	}

	free(z);

} 

int editDistance(char *x, char *y, int m, int n){

	int i, j, minCost;
	int DP[m+1][n+1];		//array to find final edit distance cost
	int **operation = (int**)malloc(sizeof(int*)*(m+1));	//array to hold operations	
	assert(operation != NULL);
	// head = NULL;
	//initialization and allocation
	for(i = 0; i <= m; i++){
		operation[i] = (int*)malloc(sizeof(int)*(n+1));
		assert(operation[i] != NULL);
	}

	DP[m][n] = 0;
	operation[m][n] = 'f';

	for(i = 0; i <= m; i++){		//delete m-i characters in x to insert to y
		DP[i][n] = (m-i)*2;
		operation[i][n] = 2;
		// insertNodeTail(x[i]);
	}
	for(j = 0; j <= n; j++){		//insert n-j characters in y 
		DP[m][j] = (n-j)*3;
		operation[m][j] = 3;
	}

	for(i = m-1; i >= 0; i--){
		for(j = n-1; j >= 0; j--){

			int insertCost = 3 + DP[i][j+1];						//total cost for insert
			int deleteCost = 2 + DP[i+1][j];						//total cost for delete
			int reprigCost = 4*(x[i] != y[j]) + DP[i+1][j+1];		//total cost for replace / right
			minCost = reprigCost;		//set minimum cost to replace since its highest

			if(x[i] == y[j]){					//if x==y, set minCost to perform right operation
				minCost = reprigCost;
				operation[i][j] = 0;
			}
			else{								//if x!=y, set minCost to perform replace operation
				minCost = reprigCost;
				operation[i][j] = 4;
			}
			if(minCost > insertCost){			//if minimum cost > insert cost, set minCost to perform insert operation
				minCost = insertCost;
				operation[i][j] = 3;
			}		
			if(minCost > deleteCost){			//if minimum cost > delete cost, set minCost to performe delete operation
				minCost = deleteCost;
				operation[i][j] = 2;
			}	
			DP[i][j] = minCost;			//set cost to minimum cost
		}
	}

	//ATTEMPT TO RETRACE OPERATIONS SEPARATELY, DECIDED TO DO IT AT THE SAME TIME AS PERFORMANCE
	// for(i = m-1; i >= 0; i--){
	// 	for(j = n-1; j >= 0; j--){

	// 		int insertCost = 3 + DP[i][j+1];
	// 		int deleteCost = 2 + DP[i+1][j];
	// 		int replaceCost = 4 + DP[i+1][j+1];

	// 		// if(i == 0 || j == 0)	operation[i][j] = 'f';

	// 		if(x[i] == y[j]){						//right
	// 			operation[i][j] = 'm';
	// 		}
	// 		else if(DP[i][j] == replaceCost){	//replace
	// 			operation[i][j] = 'r';
	// 		}
	// 		else if(DP[i][j] == insertCost){		//insert
	// 			operation[i][j] = 'i';
	// 		}		
	// 		else if(DP[i][j] == deleteCost){		//delete
	// 			operation[i][j] = 'd';
	// 		}
	// 		// printf("%c", operation[i][j]);
	// 	}
	// }

	if(m < 100 && n < 100)	backTrace(x, y, m, n, operation);		//perform backTrace only for part 1

	//free memory
	for(i = 0; i <= m; i++){
		free(operation[i]);
	}
	free(operation);

	return DP[0][0];
}

int main(){

	int i, j;
	int m = 0, n = 0;
	char *x, *y;
	FILE *input1, *input2, *input3;

	//finding part 1 cost and printing out corresponding sequence of transformation operations
	m = strlen("electrical engineering"); n = strlen("computer science");
	x = (char*)malloc(sizeof(char) * m + 1);
	y = (char*)malloc(sizeof(char) * n + 1);
	strcpy(x, "electrical engineering"); strcpy(y, "computer science");
	printf("part 1 cost = %d\n", editDistance(x, y, m, n));
	free(x);
	free(y);

	//finding edit distance of input 1
	input1 = fopen("input1.txt", "r");
	fscanf(input1, "%d\n", &m);
	x = (char*)malloc(sizeof(char) * m + 1);
	for(i = 0; i <= m; i++){
		x[i] = fgetc(input1);
	}
	fscanf(input1, "%d\n", &n);
	y = (char*)malloc(sizeof(char) * n + 1);
	for(j = 0; j <= n; j++){
		y[j] = fgetc(input1);
	}
	printf("input 1 cost = %d\n", editDistance(x, y, m, n));
	free(x);
	free(y);
	fclose(input1);

	//finding edit distance of input 2
	input2 = fopen("input2.txt", "r");
	fscanf(input2, "%d\n", &m);
	x = (char*)malloc(sizeof(char) * m + 1);
	for(i = 0; i <= m; i++){
		x[i] = fgetc(input2);
	}
	fscanf(input2, "%d\n", &n);
	y = (char*)malloc(sizeof(char) * n + 1);
	for(j = 0; j <= n; j++){
		y[j] = fgetc(input2);
	}
	printf("input 2 cost = %d\n", editDistance(x, y, m, n));
	free(x);
	free(y);
	fclose(input2);

	//finding edit distance of input 3
	input3 = fopen("input3.txt", "r");
	fscanf(input3, "%d\n", &m);
	x = (char*)malloc(sizeof(char) * m + 1);
	for(i = 0; i <= m; i++){
		x[i] = fgetc(input3);
	}
	fscanf(input3, "%d\n", &n);
	y = (char*)malloc(sizeof(char) * n + 1);
	for(j = 0; j <= n; j++){
		y[j] = fgetc(input3);
	}
	printf("input 3 cost = %d\n", editDistance(x, y, m, n));
	free(x);
	free(y);
	fclose(input3);

}
