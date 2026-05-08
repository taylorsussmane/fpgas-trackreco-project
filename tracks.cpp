#include "tracks.h"
#include <stdio.h>
#include <iostream>
#include "dbscan.h"



void read_data(){


}

void swap(int *arr, int i, int j){
    int temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
}

void sort_objects(int arr[N]){
    for (int i = 0; i < N - 1; i++){
        for (int j = 0; j < N - i - 1; j++){
#pragma HLS PIPELINE II=2
	   if (arr[j] > arr[j + 1]){
                swap(arr, j, j + 1);
	    }
        }
    }
}

void clustering(data[N]){
//receives an array of classes
    for (int i=0; i<N; ++i){
	event = data[i];
	layer_id = event.LayerID;
	x = event.X;
	y = event.Y;
	z = event.Z;
	t = event.Time; //or charge
		


    }
}

void kalman(){


}


void tracks(dint16_t A[N], dint16_t B[N], dint16_t C[N]){

read_data;
clustering;
kalman;

return

}
