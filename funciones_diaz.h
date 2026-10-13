#ifndef FUNCIONES_DIAZ_H_INCLUDED
#define FUNCIONES_DIAZ_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "estructuras.h"
#include "compartidas.h"

void pixelNegativo(char *argv[]);
void pixelGrises(char *argv[]);
void pixelEspHoriz(char *argv[]);
void pixelEspVert(char *argv[]);
void pixelMasContrast(char *argv[], int valor);
void pixelMenosContrast(char *argv[], int valor);
void pixelTonRojo(char *argv[], int valor);
void pixelTonAzul(char *argv[], int valor);
void pixelTonVerde(char *argv[], int valor);
void pixelRecortar(char *argv[], int valor);

unsigned char pixelValorContrast(unsigned char valorCanal, double factor);


#endif // FUNCIONES_DIAZ_H_INCLUDED
