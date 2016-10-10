/*
 * pokedexServer.h
 *
 */

#ifndef POKEDEXSERVER_H_
#define POKEDEXSERVER_H_

#include <string.h>
#include <errno.h>
#include <assert.h>
#include <pthread.h>
#include <stdbool.h>
#include <semaphore.h>
#include <commons/collections/list.h>
#include <commons/string.h>
#include <commons/log.h>
#include "conexiones.h"
#include <commons/bitarray.h>
#include <time.h>
#include <sys/mman.h>
#include <math.h>



int tamanioFileSystem;

void crearArchivo(char* nombreArchivoNuevo,int tamanio,int directorioPadre);
void completarTablaDeAsignaciones(int* tablaDeAsignaciones,int cantidadDeBloquesArchivo,int primerBloque);
int calcularTamanioDeArchivo(FILE* archivoAMapear);
void* mapearArchivoMemoria(FILE* archivo);
int buscarBloqueVacioEnElBitmap();


typedef struct{
	osada_header* header;
	t_bitarray* bitmap;
	osada_file tablaDeArchivos[1024];
	int* tablaDeAsignaciones;
	void* bloquesDeDatos;
}osada_bloqueCentral;

osada_bloqueCentral disco;
int tamanioDisco;
char* rutaDisco;

#endif /* POKEDEXSERVER_H_ */



