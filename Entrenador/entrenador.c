/*
 * entrenador.c
 *
 */

#include "entrenador.h"

int main(int argc, char **argv) {

	char *logFile = NULL;
	//char* logFile = "/home/utnso/git/tp-2016-2c-SegmentationFault/Entrenador/logEntrenador";

	//assert(("ERROR - No se pasaron argumentos", argc > 1)); // Verifica que se haya pasado al menos 1 parametro, sino falla

	//Parametros
	int i;
	for (i = 0; i < argc; i++) {
		if (i == 0) {
			datosEntrenador.nombre = argv[i + 1];
			printf("Nombre Entrenador: '%s'\n", datosEntrenador.nombre);
		}
		if (i == 1) {
			datosEntrenador.rutaPokedex = argv[i + 1];
			printf("Ruta Pokedex: '%s'\n", datosEntrenador.rutaPokedex);
		}
	}

	//assert(("ERROR - No se paso el nombre del entrenador como argumento", datosEntrenador.nombre != NULL));

	//assert(("ERROR - No se paso el archivo de log como argumento", logFile != NULL));//Verifies if was passed the Log file as parameter, if DONT FAILS

	//Creo el archivo de Log
	//logEntrenador = log_create(logFile, "ENTRENADOR", 0, LOG_LEVEL_TRACE);

	datosEntrenador.hojaDeViaje = list_create();

	//Levanto los datos del metadata de Entrenador

	getMetadataEntrenador();
	//CONFIGURACION DEL ENTRENADOR


	//faltan los objetivos


	return EXIT_SUCCESS;

}


//Funcion que levanta los datos del entrenador

void getMetadataEntrenador() {

	t_config* configEntrenador = malloc(sizeof(t_config));
	configEntrenador->path = string_from_format("/home/utnso/Pokedex/Entrenadores/%s/metadata","Red");
	configEntrenador = config_create(configEntrenador->path);

	datosEntrenador.nombre = config_get_string_value(configEntrenador, "nombre");
	datosEntrenador.simbolo = config_get_string_value(configEntrenador, "simbolo");
	datosEntrenador.cantVidas = config_get_int_value(configEntrenador, "vidas");
	char** hojaDeViaje = config_get_array_value(configEntrenador, "hojaDeViaje");

	printf("El nombre del Entrenador es: %s \n", datosEntrenador.nombre);
	printf("El simbolo que representa al Entrenador es: %s \n",datosEntrenador.simbolo);
	printf("La cantidad de vidas del Entrenador es: %d \n", datosEntrenador.cantVidas);

	int i = 0;
	while (hojaDeViaje[i] != NULL) {
		t_mapa* mapa=malloc(sizeof(t_mapa));
		mapa->nombreMapa = hojaDeViaje[i];

		printf("El mapa que debe recorrer el datosEntrenador: %s \n",
				mapa->nombreMapa);

		char* strConcat = string_new();
		string_append(&strConcat, "obj[");
		string_append(&strConcat, mapa->nombreMapa);
		string_append(&strConcat, "]");

		//entrenador->mapa->objetivos=config_get_array_value(configEntrenador,"obj[PuebloPaleta]");

		mapa->objetivos = config_get_array_value(configEntrenador, strConcat);
		int j = 0;
		while (mapa->objetivos[j] != NULL) {

			if (mapa->objetivos[j + 1] != NULL) {
				printf("%s, ", mapa->objetivos[j]);

			} else {
				printf("%s \n", mapa->objetivos[j]);
			}

			j++;

		}

		t_config* configMapa = malloc(sizeof(t_config));

		configMapa->path = string_from_format("/home/utnso/Pokedex/Mapas/%s/metadata",mapa->nombreMapa);
		configMapa = config_create(configMapa->path);
		mapa->ip = config_get_string_value(configMapa, "IP");
		mapa->puerto = config_get_int_value(configMapa,"Puerto");
		printf("La IP del mapa %s es: %s \n", mapa->nombreMapa,mapa->ip);
		printf("El puerto del mapa %s es: %d \n", mapa->nombreMapa,mapa->puerto);

		list_add(datosEntrenador.hojaDeViaje, (void*)mapa);

		i++;
	}

	printf("La cantidad de mapas a recorrer es: %d \n", datosEntrenador.hojaDeViaje->elements_count);

	recorrerEPrintearLista(datosEntrenador.hojaDeViaje);



}

void recorrerEPrintearLista(t_list* unaLista){
	int i;
	t_mapa* unMapa=malloc(sizeof(t_mapa));
	for(i=0;i<unaLista->elements_count;i++){
		unMapa=(t_mapa*)list_get(unaLista,i);
		printf("%s \n",unMapa->nombreMapa);
		printf("%s \n",unMapa->ip);
		printf("%i \n",unMapa->puerto);
	}
}
//Cambia la posicion del entrenador segun determine el mapa.

char* avanzarPosicion(char* posicionInicial,char* posicionDestino){
 char* miPosicion=string_new();
 char* posicionQueQuieroLlegar=string_new();
 string_append(&miPosicion,posicionInicial);
 string_append(&posicionQueQuieroLlegar,posicionDestino);
 char** posicionXY;
 char** posicionDestinoXY;
 posicionXY=string_split(miPosicion,";");
 int posicionX=atoi(posicionXY[0]);
 int posicionY=atoi(posicionXY[1]);

 posicionDestinoXY=string_split(posicionQueQuieroLlegar,";");
 int posicionXDestino=atoi(posicionDestinoXY[0]);
 int posicionYDestino=atoi(posicionDestinoXY[1]);
 if(posicionX>posicionXDestino){
  posicionX--;
 }
 if(posicionX<posicionXDestino){
  posicionX++;
 }
 if(posicionY>posicionYDestino){
  posicionY--;
 }
 if(posicionY<posicionYDestino){
  posicionY++;
 }
 char* nuevaPosicion=string_new();
 string_append_with_format(&nuevaPosicion,"%i",posicionX);
 string_append(&nuevaPosicion,";");
 string_append_with_format(&nuevaPosicion,"%i",posicionY);
 strcpy(miPosicion,nuevaPosicion);
 return miPosicion;

}

void chequearVidas(t_entrenador* unEntrenador){
 if(unEntrenador->cantVidas==0){
  printf("Te quedaste sin vidas \n");
  //borrarDirectorioDeBill();
  //borra sus medallas
  shutdown(socketEntrenador,2);
 }else{
  unEntrenador->cantVidas--;
  printf("Perdiste una vida, te queda:%i \n",unEntrenador->cantVidas);
 }
}

void chequearObjetivos(t_entrenador* unEntrenador,char pokemon){
	t_mapa* mapaEnElQueEstoy=(t_mapa*)list_get(unEntrenador->hojaDeViaje,unEntrenador->mapaActual);

		int j=0;
		while(!(mapaEnElQueEstoy->objetivos[j]==pokemon)){
		j++;
		}
		mapaEnElQueEstoy->objetivos[j]=0;

		if(mapaEnElQueEstoy->objetivos[j+1]==NULL){
			if(list_size(unEntrenador->hojaDeViaje)==unEntrenador->mapaActual){
			//copiarMedallaDelMapa();
			printf("Eres un maestro pokemon completaste la aventura");
			//GenerarReporteDeAventura();
			}else{
			//copiarMedallaDelMapa();
			//conectarseConElSiguienteMapa();
			}
		}else{
			//AvisarAlMapaQueDeboSeguirAtrapandoPokemon();
		}
}
void avanzarPosicionInt(int* actualX, int* actualY, int destinoX, int destinoY){
	int posicionX = *actualX;
	int posicionY = *actualY;

	if (posicionY == destinoY || (!alternateFlag)){
		if (posicionX > destinoX) {
			posicionX--;
			alternateFlag = true;
		} else if (posicionX < destinoX) {
			posicionX++;
			alternateFlag = true;
		}
	} else if(posicionX == destinoX || alternateFlag) {
		if (posicionY > destinoY) {
			posicionY--;
			alternateFlag = false;
		} else if (posicionY < destinoY) {
			posicionY++;
			alternateFlag = false;
		}
	}
	*actualX = posicionX;
	*actualY = posicionY;
}


void conectarseAlMapa(t_mapa* unMapa){

	if(conectarseA(unMapa->ip,unMapa->puerto) == 0){
		printf("Conexion realizada con exito \n");
	}
	else{
		printf("Conexion fracaso \n");
		exit(-1);
	}
}

char* solicitarUbicacionPokenest(char pokemon){

	//Solicito la posicion de mi proximo objetivo
	if(enviar(socketDeMapa, &pokemon ,sizeof(char))!= -1){
		printf("Datos enviados satisfactoriamente \n");
	}
	else{
		printf("No se han podido enviar todos los datos \n");
	}

	//Recibo la respuesta del mapa

	int tamanioPokenest = 0;
	//Recibo el tamaño de la respuesta

	if(recibir(socketEntrenador, &tamanioPokenest, sizeof(int)) > 0 ){
		printf("Se recibio el tamanio correctamente \n");
	}
		else{
				printf("El tamanio recibido es distinto al esperado \n");
		}



	char* posicionPokenest= malloc(tamanioPokenest);
	if(recibir(socketEntrenador, posicionPokenest,tamanioPokenest) > 0){
		printf("Se recibio el tamanio correctamente \n");
	}else{
		printf("Se recibio un tamanio distinto al esperado \n");
	}

	return posicionPokenest;
}

void avanzarPasosDisponibles(int pasosDisponibles, t_entrenador* unEntrenador, char* posicionPokenest){
	char* posicionesAvanzadas = malloc(sizeof(char));
	int pasosRealizados = 0;
	while(pasosRealizados < pasosDisponibles){

		posicionesAvanzadas = avanzarPosicion(unEntrenador->posicion, posicionPokenest);
		//Recibe por parametro pokenestX, poknestY
		//avanzarPosicionInt(&unEntrenador->posx, &unEntrenador->posy, pokenestX, pokenestY);
		//la funcion actualiza directamente la pos x;y del entrenador
		//if(unEntrenador->posx == pokenestX && unEntrenador->posy == pokenestY){
		if(string_equals_ignore_case(unEntrenador->posicion, posicionPokenest)){
			printf("Se alcanzo la posicion de la Pokenest");
			//informar al mapa que ya llegaste.
		}else{
			pasosRealizados++;

		}

	}

}

/*
int recibirPasosDisponibles(char* posicion){
	int pasosDisponibles;








	if(recibir((char*)pasosDisponibles,socketDeMapa,sizeof(int)))
			{
			printf("No se recibieron los pasos disponibles");
			exit(-1);
			}else{
				return pasosDisponibles;
			}

}


void interactuarConMapa(t_entrenador* unEntrenador){

	t_mapa* mapa = malloc(sizeof(t_mapa));
	t_mapa* mapa = list_get(unEntrenador->hojaDeViaje,unEntrenador->mapaActual);

	conectarseAlMapa(mapa);
	int i = 0;
	while(mapa->objetivos[i] != NULL){
		char* posicionPokenest = malloc(sizeof(char));
		posicionPokenest = solicitarUbicacionPokenest(mapa->objetivos[i]);
		//recibirQuantumDisponible()
		avanzarPasosDisponibles()


*/
