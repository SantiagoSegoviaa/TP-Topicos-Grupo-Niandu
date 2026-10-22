#ifndef COMPARTIDO_H_INCLUDED
#define COMPARTIDO_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

#define TAM_RUTA 260

int ImagenCargar(t_imagen *img, const char *nombre);
int ImagenGuardar(t_imagen *img, const char *nombre);
void ImagenLiberar(t_imagen *img);
int ImagenCrear(const t_imagen* imgModelo, t_imagen* imgDestino, int alto, int ancho);
int GeneraRuta(const char *ruta, const char* operacion, char *destino);

#endif // COMPARTIDO_H_INCLUDED
