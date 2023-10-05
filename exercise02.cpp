// =================================================================
//
// File: exercise02.cpp
// Author(s):Julio David Resendiz Cruz - A01709375, Carlos Hernández Arciniega - A01709003
// Description: This file contains the code to brute-force all
//				prime numbers less than MAXIMUM. The time this
//				implementation takes will be used as the basis to
//				calculate the improvement obtained with parallel
//				technologies.
// =================================================================
// Tiempo secuencial: 816.423 ms
// Tiempo paralelo: 416.892 ms 
// Speedup: 1.95835
//
// =================================================================

#include <iostream>
#include <iomanip>
#include <pthread.h>
#include <chrono>
#include <cmath>
#include "utils.h"

using namespace std;
using namespace std::chrono;

#define MAXIMUM 5000000 //5e6
#define ll long long
/*
CADA HILO SE VA A ENCARGAR DE REALIZAR LA SUMA DE UN RANGO DE NUMEROS
*/

// Cantidad de hilos que se van a utilizar
const int THREADS = 8;


// Esta es la estructura en donde se van a almacenar
// los datos que necesitamos para cada hilo
// En este caso, el rango de numero que se van a usar por hilo
//  y el resultado de dicha suma
typedef struct {
	int start, end;
	ll result;
} Block;

// Funcion para verificar si un numero es primo o no
bool es_primo(int);

// Esta es la funcion que va a realizar cada hilo
void* task(void*);

int main(int argc, char* argv[]) {
	ll suma = 0;

	pthread_t ptid[THREADS]; // Creamos un arreglo de id's para los hilos
	Block blocks[THREADS]; // Creamos un arreglo de los bloques que definimos anteriormente para cada hilo
	int block_size;

	block_size = MAXIMUM / THREADS; // La cantidad de numeros o el rango de numeros que cada rango va a analizar

	for(int i = 0; i < THREADS; i++){ // En este cilco vamos a definir el rango de numero que le toca a cada hilo
		blocks[i].start = i * block_size;
		blocks[i].end = (i != (THREADS - 1))? (i + 1) * block_size : MAXIMUM + 1;
	}

	// These variables are used to keep track of the execution time.
	high_resolution_clock::time_point start, end;
	double timeElapsed;

	cout << "Starting...\n";
	timeElapsed = 0;
	for (int j = 0; j < N; j++) {
		suma = 0;
		start = high_resolution_clock::now();

		// call the implemented function
		for(int i = 0; i < THREADS; i++){ // EN este ciclo vamos a crear los hilos que definimos
			// Parametros: id, NULL, funcion que va a realizar, bloque de memoria
			pthread_create(&ptid[i], NULL, task, &blocks[i]);
		}
		
		for(int i = 0; i < THREADS; i++){ // En este ciclo vamos a esperar a que cada hilo termine su funcion y sumas el resultado de cada hilo a la respuesta
			pthread_join(ptid[i], NULL);
			suma += blocks[i].result;
		}

		end = high_resolution_clock::now();
		timeElapsed += 
			duration<double, std::milli>(end - start).count();
	}
	cout << "result = " << suma << "\n";
	cout << "avg time = " << fixed << setprecision(3) 
		 << (timeElapsed / N) <<  " ms\n";

	return 0;
}

bool es_primo(int n){
	if (n < 2) return false;
	
	for(int i = 2; i <= sqrt(n); i++){
		if (n % i == 0) return false;
	}

	return true;
}


void* task(void* params){
	Block* b;
	ll acum;

	b = (Block*) params;
	acum = 0;

	for(int i = b->start; i < b->end; i++){
		if (es_primo(i)){
			acum += i;
		}
	}
	b->result = acum;
	pthread_exit(0);
}