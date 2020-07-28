#include "p1_threads.h"
// This file includes functions that actually perform the
// computation. You can create a thread with these function
// or directly call this function

void merge(vector<Student> &student, int l, int m, int r){
	
	int i, j, k;
	int n1 = m - l + 1;
	int n2 = r - m;

	vector<Student> left, right;
	Student L, R;

	for(i = 0; i < n1; i++){
		L.id = student[l + i].id;
		L.grade = student[l + i].grade;
		left.push_back(L);
	} 
    for (j = 0; j < n2; j++){
    	R.id = student[m + 1 + j].id;
		R.grade = student[m + 1 + j].grade;
		right.push_back(R); 
	}

	i = 0;
	j = 0;
	k = l;

	while(i < n1 && j < n2){
		if(left[i].grade >= right[j].grade){
			student[k] = left[i];
			i++;
		}else{
			student[k] = right[j];
			j++;
		}
		k++;
	}

	while(i < n1){
		student[k] = left[i];
		i++; 
        k++;
	}

	while (j < n2){ 
		student[k] = right[j];
        j++; 
        k++;
    } 
}

void mergesort(vector<Student> &student, int l, int r){

	if(l < r){
		int m = l + (r-l)/2;
		mergesort(student, l, m);
		mergesort(student, m+1, r);
		merge(student, l, m, r);
	}

}

void *merge_sort(void *arg){

	threadData *td = (threadData*)arg;
	int threadpart = td->part++;
	int l = threadpart * (td->size / td->threads);
	int r = (threadpart + 1) * (td->size / td->threads) - 1;

	if(l < r){
		int m = l + (r-l)/2;
		mergesort(*td->student, l, m);
		mergesort(*td->student, m+1, r);
		merge(*td->student, l, m, r);
	}
}
