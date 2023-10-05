// File: exercise03.cpp
// Author(s): Julio David Resendiz Cruz - A01709375, Carlos Hernández Arciniega - A01709003
// Description: This file contains the code that implements the
//				enumeration sort algorithm. The time this 
//				implementation takes ill be used as the basis to 
//				calculate the improvement obtained with parallel 
//				technologies.
// =================================================================
// Tiempo secuencial: 62.635 ms
// Tiempo paralelo: 30.254 ms
// Speedup: 2.0703
// =================================================================

#include <iostream>
#include <pthread.h>
#include <iomanip>
#include <chrono>
#include <cstring>
#include "utils.h"

using namespace std;
using namespace std::chrono;

#define SIZE 10000

const int NUM_THREADS = 4;

struct ThreadDatos {
    int* arr;
    int start;
    int end;
};
//funcion para ordenar el arreglo, implementando thread
void* enumerationSort(void* arg) {
    ThreadDatos* data = static_cast<ThreadDatos*>(arg);
    int* arr = data->arr;
    int n = data->end - data->start + 1;

    for (int i = data->start; i < data->end; i++) {
        for (int j = i + 1; j < n; j++) {
            if (arr[i] > arr[j]) {
                std::swap(arr[i], arr[j]);
            }
        }
    }

    pthread_exit(NULL);
}

//Función principal del programa que implementa el ordenamiento del arreglo utilizando el 
//método de Enumeration Sort en paralelo utilizando pthread.
void parallelEnumerationSort(int arr[], int size) {
    pthread_t threads[NUM_THREADS];
    ThreadDatos threadData[NUM_THREADS];

    int chunkSize = size / NUM_THREADS;
    int start = 0;
    int end = chunkSize;

    for (int i = 0; i < NUM_THREADS; i++) {
        threadData[i].arr = arr;
        threadData[i].start = start;
        threadData[i].end = end;

        pthread_create(&threads[i], NULL, enumerationSort, &threadData[i]);

        start = end;
        end += chunkSize;
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
}

int main(int argc, char* argv[]) {
	int *origin, *copy, result;
	// These variables are used to keep track of the execution time.
	high_resolution_clock::time_point start, end;
	double timeElapsed;

	origin = new int[SIZE];
	random_array(origin, SIZE);
	display_array("before", origin);

	copy = new int[SIZE];

	cout << "Starting...\n";
	timeElapsed = 0;
	for (int j = 0; j < N; j++) {
		memcpy(copy, origin, SIZE * sizeof(int));

		start = high_resolution_clock::now();
          parallelEnumerationSort(copy, SIZE);
		end = high_resolution_clock::now();
		timeElapsed += 
			duration<double, std::milli>(end - start).count();
	}
	display_array("after", copy);
	cout << "avg time = " << fixed << setprecision(3) 
		 << (timeElapsed / N) <<  " ms\n";

	delete [] origin;
	delete [] copy;
	
	return 0;
}
