/*
 * pokedexServer.c
 *
 */

#include "pokedexServer.h"

osada_header* miFileSystem;
osada_file* tablaDeArchivos[2048];
t_bitarray* mapaDeBits;

int main(int argc, char **argv) {

	char *logFile = NULL;
	inicializarBloqueCentral();
	//assert(("ERROR - No se pasaron argumentos", argc > 1)); // Verifica que se haya pasado al menos 1 parametro, sino falla

	/*//Parametros
	int i;
	for( i = 0; i < argc; i++){
		if (strcmp(argv[i], "") == 0){
			logFile = argv[i+1];
			printf("Log File: '%s'\n",logFile);
		}
	}*/

	//Creo el archivo de Log
	//logPokedex = log_create(logFile, "POKEDEXCLIENT", 0, LOG_LEVEL_TRACE);

	/*
	char* rutaArchivoDePrueba="/home/utnso/Escritorio/PruebaDeMapeo.txt";
	FILE* archivoDePrueba=fopen(rutaArchivoDePrueba,"r+");
	int tamanio=calcularTamanioDeArchivo(archivoDePrueba);
	void* archivoMapeado=malloc(tamanio);
	archivoMapeado=mapearArchivoMemoria(archivoDePrueba);
	char* frase=malloc(15);
	memcpy(frase,archivoMapeado,15);
	printf("%s",frase);
	printf("%i",tamanio);
	 */
}


////////////////////////////FUNCIONES PROPIAS DEL FILESYSTEM/////////////////////////////////////
void leerArchivo(unsigned char nombreDelArchivo[17]){
	FILE* discoAbierto = fopen(rutaDisco,"r+");
	osada_file infoArchivo;
	disco = (osada_bloqueCentral*)mapearArchivoMemoria(discoAbierto);
	int i = 0;
	while(disco->tablaDeArchivos[i].file_size != (-1)){
		if(disco->tablaDeArchivos[i].fname == nombreDelArchivo){
			infoArchivo = disco->tablaDeArchivos[i];
		}
		i++;
	}
	int cantidadDeBloquesArchivo = ceill(infoArchivo.file_size / OSADA_BLOCK_SIZE);
	int secuenciaArchivo[cantidadDeBloquesArchivo];
	int j = infoArchivo.first_block;
	secuenciaArchivo[0] = j;
	int h = 1;
	while(disco->tablaDeAsignaciones[j] != (-1)){
		secuenciaArchivo[h] = disco->tablaDeAsignaciones[j];
		j = disco->tablaDeAsignaciones[j];
		h++;
	}
	int k = 0;
	char* archivo = malloc(cantidadDeBloquesArchivo*8);
	while(secuenciaArchivo[k] != NULL){
		memcpy(archivo+OSADA_BLOCK_SIZE*k,
				disco->bloquesDeDatos+secuenciaArchivo[k]*OSADA_BLOCK_SIZE,OSADA_BLOCK_SIZE );
		k++;
	}
	//TODO Enviar archivo al cliente
}

void crearArchivo(char* nombreArchivoNuevo,int tamanio,int directorioPadre){
	time_t tiempo;
	struct tm* tm;
	osada_file* nuevoArchivo=malloc(sizeof(osada_file));
	strcpy(&nuevoArchivo->fname,nombreArchivoNuevo);
	nuevoArchivo->file_size=tamanio;
	nuevoArchivo->first_block=-buscarBloqueVacioEnElBitmap();
	nuevoArchivo->state=REGULAR;
	tiempo=time(NULL);
	tm=localtime(&tiempo);
	nuevoArchivo->lastmod=tm->tm_mday*10000+tm->tm_mon*100+tm->tm_year;
	nuevoArchivo->parent_directory=directorioPadre;
	char* comandoCreacion=string_new();
	char* rutaPokedex;//falta definir la ruta donde se va a guardar PREGUNTAR
	comandoCreacion=string_from_format("touch %s",rutaPokedex);
	system(comandoCreacion);
	//Esto no va lo dejo porque probablemente se use en escribir o modificar archivo
	/*FILE* archivoAbierto=fopen(rutaFileSystem,"r+");
	int tamanioAReservar;
	tamanioAReservar=calcularTamanioDeArchivo(archivoAbierto);
	int cantidadDeBloquesArchivos=ceil(tamanio/OSADA_BLOCK_SIZE);
	completarTablaDeAsignaciones(tablaDeAsignaciones,cantidadDeBloquesArchivos,nuevoArchivo->first_block);
	void* archivoMapeado=malloc(tamanioAReservar);
	archivoMapeado=mapearArchivoMemoria(archivoAbierto);
	 */
}


void crearArchivo2(char* direccionDisco){



}

void EscribirOModificar(unsigned char nombreDelArchivo[17]){
	FILE* discoAbierto = fopen(rutaDisco,"r+");
		osada_file infoArchivo;
		disco = (osada_bloqueCentral*)mapearArchivoMemoria(discoAbierto);
		int i = 0;
		while(disco->tablaDeArchivos[i].file_size != (-1)){
			if(disco->tablaDeArchivos[i].fname == nombreDelArchivo){
				infoArchivo = disco->tablaDeArchivos[i];
			}
			i++;
		}
		int cantidadDeBloquesArchivo = ceill(infoArchivo.file_size / OSADA_BLOCK_SIZE);
		int secuenciaArchivo[cantidadDeBloquesArchivo];
		int j = infoArchivo.first_block;
		secuenciaArchivo[0] = j;
		int h = 1;
		while(disco->tablaDeAsignaciones[j] != (-1)){
			secuenciaArchivo[h] = disco->tablaDeAsignaciones[j];
			j = disco->tablaDeAsignaciones[j];
			h++;
		}
}

void borrarArchivos(){

}

void crearDirectorio(){

}

void borrarDirectoriosVacios(){

}

void renombrarArchivo(){

}

/////////////////////////////FUNCIONES SECUNDARIAS//////////////////////////////////////
void seteoInicialTablaDeAsignaciones(int* tablaDeAsignaciones){
	int i;
	for(i=0;i>=2048;i++){
		tablaDeAsignaciones[i]=-1;
	}
}

void* mapearArchivoMemoria(FILE* archivo){
	int tamanio = calcularTamanioDeArchivo(archivo);
	int descriptorArchivo;
	descriptorArchivo = fileno(archivo);
	lseek(descriptorArchivo, 0, SEEK_SET);
	void* archivoMapeado=malloc(tamanio);
	archivoMapeado = mmap(NULL, tamanio, PROT_READ | PROT_WRITE, MAP_SHARED, descriptorArchivo, 0);
	return archivoMapeado;
}


int calcularTamanioDeArchivo(FILE* archivoAMapear){
	fseek(archivoAMapear, 0, SEEK_END);
	int tamanio=ftell(archivoAMapear);
	return tamanio;
}


void inicializarBloqueCentral(){
	int cantidadDeBloquesTotal = tamanioDisco / OSADA_BLOCK_SIZE;
	int cantidadDeBits = cantidadDeBloquesTotal / 8;
	int cantidadDeBytesAMalloquear = cantidadDeBits / 8;
	int fileDescriptor;
	FILE* archivoDisco = fopen(rutaDisco,'r+');
	fileDescriptor = fileno(archivoDisco);
	lseek(fileDescriptor,0,SEEK_SET);
	char* bitmap = malloc(sizeof(cantidadDeBytesAMalloquear));
	bitmap = mmap(NULL, disco->header->bitmap_blocks , PROT_READ|PROT_WRITE, MAP_FILE|MAP_SHARED, fileDescriptor, OSADA_BLOCK_SIZE);
	disco->bitmap = bitarray_create(bitmap, cantidadDeBits);
	munmap(bitmap,disco->header->bitmap_blocks*OSADA_BLOCK_SIZE);
	disco=(osada_bloqueCentral*)mapearArchivoMemoria(archivoDisco);
	disco->header->version=1;
	disco->header->fs_blocks=tamanioDisco/OSADA_BLOCK_SIZE;
	disco->header->bitmap_blocks=sizeof(disco->header->fs_blocks/8);
	disco->header->allocations_table_offset=disco->header->bitmap_blocks+1025;
	int cantidadDeBloquesTablaDeAsignaciones=(tamanioDisco-1-disco->header->bitmap_blocks-1024)*4/OSADA_BLOCK_SIZE;
	disco->header->data_blocks=cantidadDeBloquesTablaDeAsignaciones-disco->header->bitmap_blocks-disco->header->fs_blocks-1-1024;
	int i;
	int bloquesAdministrativos=1+1024+disco->header->bitmap_blocks+cantidadDeBloquesTablaDeAsignaciones;
	for(i=0;i<bloquesAdministrativos;i++){
		bitarray_set_bit(disco->bitmap,i);
	}
	seteoInicialTablaDeAsignaciones(disco->tablaDeAsignaciones);
	int j;
	for(j=0;j<2048;j++){
		disco->tablaDeArchivos[j].file_size=-1;
	}
}

int buscarBloqueVacioEnElBitmap(){
	int i;
	for(i=0;i<miFileSystem->bitmap_blocks;i++){
		if(bitarray_test_bit(mapaDeBits,i)==0){
			bitarray_set_bit(mapaDeBits,i);
			return i;
		}

	}
	return -1;
}

void completarTablaDeAsignaciones(int* tablaDeAsignaciones,int cantidadDeBloquesArchivo,int primerBloque){
	int i;
	int primerBloqueVacio=buscarBloqueVacioEnElBitmap();
	tablaDeAsignaciones[primerBloque]=primerBloqueVacio;
	for(i=1;i>cantidadDeBloquesArchivo;i++){
		if(i==cantidadDeBloquesArchivo){
			tablaDeAsignaciones[primerBloqueVacio]=-2;
		}
		tablaDeAsignaciones[primerBloqueVacio]=buscarBloqueVacioEnElBitmap();
		primerBloqueVacio=buscarBloqueVacioEnElBitmap();

	}

}

void copiarArchivoNuevoEnMemoria(void* fsMapeado,int* tablaDeAsignaciones,int primerBloque,int cantidadDeBloquesArchivo){
	int offset=sizeof(osada_header);
	int i;
	for(i=0;i<cantidadDeBloquesArchivo;i++){
		offset=+tablaDeAsignaciones[primerBloque]*OSADA_BLOCK_SIZE;
		//memcpy(fsMapeado+offset);
	}
}

void seteoInicialFS(char* rutaFS){
	tamanioFileSystem=1024;
	miFileSystem->fs_blocks=tamanioFileSystem/OSADA_BLOCK_SIZE;
	miFileSystem->version=1;
	miFileSystem->bitmap_blocks=miFileSystem->fs_blocks/8/OSADA_BLOCK_SIZE;
	miFileSystem->allocations_table_offset=1+miFileSystem->bitmap_blocks+1024;
	int tamanioTablaDeAsignaciones=(miFileSystem->fs_blocks-1-miFileSystem->bitmap_blocks-1024)*4/OSADA_BLOCK_SIZE;
	miFileSystem->data_blocks=miFileSystem->fs_blocks-1-miFileSystem->bitmap_blocks-tamanioTablaDeAsignaciones;
	int tablaDeAsignaciones[miFileSystem->data_blocks];
	char* mapa=malloc(miFileSystem->bitmap_blocks/8);
	mapaDeBits=bitarray_create(mapa,miFileSystem->bitmap_blocks);
	FILE* archivoFs;
	archivoFs=fopen(rutaFS,"r+");
	int tamanioAReservar;
	tamanioAReservar=calcularTamanioDeArchivo(archivoFs);
	void* archivoMapeado=malloc(tamanioAReservar);
	archivoMapeado=mapearArchivoMemoria(archivoFs);
	//copiar las estructuras administrativas dentro del archivo del fs
}
