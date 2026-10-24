#ifndef FUNCIONES_GRUPO_H_INCLUDED
#define FUNCIONES_GRUPO_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "funciones_diaz.h"
#include "funciones_paz.h"
#include "funciones_segovia.h"
#include "funciones_lara.h"
#include "funciones_gramajo.h"
#include "funciones_sanchez.h"
#include "compartido.h"
#include "estructuras.h"

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
