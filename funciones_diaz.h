#ifndef FUNCIONES_DIAZ_H_INCLUDED
#define FUNCIONES_DIAZ_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "compartido.h"

int pixelMasContrast(char *argv[], int valor, int pos);
int pixelMenosContrast(char *argv[], int valor, int pos);

unsigned char pixelValorContrast(unsigned char valorCanal, double factor);


#endif // FUNCIONES_DIAZ_H_INCLUDED
