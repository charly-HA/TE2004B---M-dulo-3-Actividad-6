// =================================================================
//
// File: exercise01.cpp
// Author(s): Julio David Resendiz Cruz - A01709375, Carlos Hernández Arciniega - A01709003
// Description: This file contains the code to count the number of
//				even numbers within an array. The time this implementation
//				takes will be used as the basis to calculate the
//				improvement obtained with parallel technologies.
//
// =================================================================
//
// Tiempo secuencial: 2412.102 ms
// Tiempo paralelo: 1304.815 ms 
// Speedup: 1.8486 
//
// =================================================================

#include <iostream>
#include <iomanip>
#include <chrono>
#include <pthread.h>
#include "utils.h"

using namespace std;
using namespace std::chrono;

// array size
#define SIZE 500000000
#define NUM_THREADS 4  

// Datos para los hilos
struct Thread {
     int threadId;
    int* array;
    long long SumPar;
    int size;   
};

// Función para la suma  de números pares 
void* sum(void* arg) {
    Thread* data = static_cast<Thread*>(arg);
    long long sum = 0;

    for (int i = data->threadId; i < data->size; i += NUM_THREADS) {
        if (data->array[i] % 2 == 0) {
            sum += data->array[i];
        }
    }
    data->SumPar = sum;
    return nullptr;  
}

int main(int argc, char* argv[]) {
    int *array;
    long long result = 0;
    
    high_resolution_clock::time_point start, end;
    double timeElapsed;

    array = new int[SIZE];
    fill_array(array, SIZE);
    display_array("array", array);

    cout << "Starting...\n";
    timeElapsed = 0;
    for (int j = 0; j < N; j++) {
        start = high_resolution_clock::now();

        // Hilos
        pthread_t threads[NUM_THREADS];
        Thread threadDatos[NUM_THREADS];

        // Iniciar y ejecutar los hilos
        for (int i = 0; i < NUM_THREADS; ++i) {
            threadDatos[i].array = array;
            threadDatos[i].size = SIZE;
            threadDatos[i].threadId = i;
            // Suma parcial
            threadDatos[i].SumPar = 0; 

            pthread_create(&threads[i], nullptr, sum, &threadDatos[i]);
        }

        // Esperar a que todos los hilos terminen su trabajo
        for (int i = 0; i < NUM_THREADS; ++i) {
            pthread_join(threads[i], nullptr);
            result += threadDatos[i].SumPar;
        }

        end = high_resolution_clock::now();
        timeElapsed += 
            duration<double, std::milli>(end - start).count();
    }
    cout << "result = " << result/N << "\n";
    cout << "avg time = " << fixed << setprecision(3) 
        << (timeElapsed / N) << " ms\n";

    delete[] array;
    return 0;
}
