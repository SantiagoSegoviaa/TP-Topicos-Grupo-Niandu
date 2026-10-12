#ifndef FUNCIONES_DIAZ_H_INCLUDED
#define FUNCIONES_DIAZ_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define POS_ARCHIVO 2

int pixelNegativo(char *argv[]);
int pixelGrises(char *argv[]);
int pixelEspHoriz(char *argv[]);
int pixelEspVert(char *argv[]);
int pixelMasContrast(char *argv[], int valor);
int pixelMenosContrast(char *argv[], int valor);
int pixelTonRojo(char *argv[], int valor);
int pixelTonAzul(char *argv[], int valor);
int pixelTonVerde(char *argv[], int valor);
int pixelRecortar(char *argv[], int valor);

unsigned char pixelValorMasContrast(unsigned char valorCanal, double factor);


#endif // FUNCIONES_DIAZ_H_INCLUDED
