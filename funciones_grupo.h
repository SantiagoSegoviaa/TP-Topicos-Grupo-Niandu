#ifndef FUNCIONES_GRUPO_H_INCLUDED
#define FUNCIONES_GRUPO_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "funciones_diaz.h"
#include "funciones_paz.h"
#include "estructuras.h"
#define ARGUMENTOS_MIN 3

#define MODIF_INC 0
#define VALIDA 1
#define VALOR_INC 2
#define FALTA_IMAG 3

#define POS_ARCHIVO 2

/*
Aqu� deben hacer los includes de sus archivos individuales, por ejemplo:
#include "funciones_rufino.h"
 */

int solucion(int argc, char* argv[]);
bool esArchivoBMP(const char *nombre);
int esModificacion(char *argv[], int pos, t_funcion_encontrada *func);
void validarModif(int cod);
void ejecutarFuncion(char *argv[], t_funcion_encontrada *func);
bool esRepetida(char *argv[], int pos);


#endif // FUNCIONES_GRUPO_H_INCLUDED
