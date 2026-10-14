#ifndef COMPARTIDAS_H_INCLUDED
#define COMPARTIDAS_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "estructuras.h"

#define POS_ARCHIVO 2

#define SUBCARPETA "tests"

int imagenCargar(t_imagen *img, const char *nombre);
int imagenGuardar(t_imagen *img, const char *nombre);
void imagenLiberar(t_imagen *img);
int imagenCrear(const t_imagen* imgModelo, t_imagen* imgDestino, int alto, int ancho);

#endif // COMPARTIDAS_H_INCLUDED
