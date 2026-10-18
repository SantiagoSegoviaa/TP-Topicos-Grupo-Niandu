#ifndef FUNCIONES_GRUPO_H_INCLUDED
#define FUNCIONES_GRUPO_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "funciones_diaz.h"
#include "funciones_paz.h"
#include "funciones_segovia.h"
#include "estructuras.h"

#define OK               0
#define ERR_NO_EXISTE    1
#define ERR_NO_ES_BMP    2
#define ERR_PROFUNDIDAD  3
#define ERR_COMPRESION   4
#define ERR_MODIF_INC    5
#define ERR_VALOR_INC    6
#define ERR_FALTA_IMAG   7
#define ERR_MEMORIA      8
#define ERR_EJECUCION    9
#define ERR_IMAGEN_EXTRA 10


/*
Aqu� deben hacer los includes de sus archivos individuales, por ejemplo:
#include "funciones_rufino.h"
 */
int TieneExtensionBMP(const char *ruta);
void informarError(int cod, const char *arg);
int solucion(int argc, char* argv[]);
int esArchivoBMP(const char *nombre);
int esModificacion(char *argv[], int pos, t_funcion_encontrada *func);
void ejecutarFuncion(char *argv[], t_funcion_encontrada *func);
bool esRepetida(char *argv[], int pos);


#endif // FUNCIONES_GRUPO_H_INCLUDED
