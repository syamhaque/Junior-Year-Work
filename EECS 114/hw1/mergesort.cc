/**
 *  \file mergesort.cc
 *	Mohammed Haque 62655407
 	1/23/19
 *  \brief Implement your mergesort in this file.
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "sort.hh"

void
mySort (int N, keytype* A)
{

	int curr, l, m, r;
	for(curr = 1; curr <= N-1; curr = 2*curr){
		for(l = 0; l < N-1; l += 2*curr){  
				m = l + curr - 1;
				if((l + 2*curr - 1) > (N-1)){
					r = N-1;
				}else{
					r = l + 2*curr - 1;
				}
        		//merge
        		int i, j, k;
				int n1 = m - l + 1;
				int n2 = r - m;

				int L[n1], R[n2];

				for(i = 0; i < n1; i++){
					L[i] = A[l + i];
				} 
			    for (j = 0; j < n2; j++){ 
			        R[j] = A[m + 1+ j]; 
				}

				i = 0;
				j = 0;
				k = l;

				while(i < n1 && j < n2){
					if(L[i] <= R[j]){
						A[k] = L[i];
						i++;
					}else{
						A[k] = R[j];
						j++;
					}
					k++;
				}

				while(i < n1){
					A[k] = L[i];
					i++; 
			        k++;
				}

				while (j < n2) 
			    { 
			        A[k] = R[j]; 
			        j++; 
			        k++; 
			    } 
        	
        }
	}

}

/* eof */
