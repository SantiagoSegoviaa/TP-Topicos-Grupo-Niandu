#ifndef FUNCIONES_DIAZ_H_INCLUDED
#define FUNCIONES_DIAZ_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "estructuras.h"
#include "compartidas.h"

int pixelNegativo(char *argv[],int pos);
int pixelGrises(char *argv[], int pos);
int pixelEspHoriz(char *argv[], int pos);
int pixelEspVert(char *argv[], int pos);
int pixelMasContrast(char *argv[], int valor, int pos);
int pixelMenosContrast(char *argv[], int valor, int pos);
int pixelTonRojo(char *argv[], int valor, int pos);
int pixelTonAzul(char *argv[], int valor, int pos);
int pixelTonVerde(char *argv[], int valor, int pos);
int pixelRecortar(char *argv[], int valor, int pos);

unsigned char pixelValorContrast(unsigned char valorCanal, double factor);


#endif // FUNCIONES_DIAZ_H_INCLUDED
