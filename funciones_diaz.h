#ifndef FUNCIONES_DIAZ_H_INCLUDED
#define FUNCIONES_DIAZ_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "estructuras.h"
#include "compartidas.h"

#define POS_ARCHIVO 2

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

int imagenCargar(t_imagen *img, const char *nombre);
int imagenGuardar(t_imagen *img, const char *nombre);
void imagenLiberar(t_imagen *img);


unsigned char pixelValorMasContrast(unsigned char valorCanal, double factor);


#endif // FUNCIONES_DIAZ_H_INCLUDED
