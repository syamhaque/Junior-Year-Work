/**
 *  \file insertion-sort.cc
 *  Mohammed Haque 62655407
    1/23/19
 *  \brief Implement your insertion sort in this file.
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "sort.hh"

void
mySort (int N, keytype* A)
{
  int i, j, key, location;

  for(i = 1; i<N; i++){
    
    key = A[i];
    int l = 0;
    int r = i;

    while (l < r) { 
      int m = (l+r)/2; 
    
      if (key <= A[m]){
        r = m;
      }
      else{
        l = m+1;
      }
    }

    for (j = i; j > r; j--){
      A[j] = A[j-1];
    }
    
    A[r] = key;
  }
}

/* eof */
