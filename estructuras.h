#ifndef ESTRUCTURAS_H_INCLUDED
#define ESTRUCTURAS_H_INCLUDED

#define NOMBRE_GRUPO "Niandu_"

// Estructura sugerida, su uso es opcional.
typedef struct
{
    unsigned char b;
    unsigned char g;
    unsigned char r;

}t_pixel;

typedef struct
{
    int tamArchivo;
    unsigned int tamEncabezado;
    unsigned char *encabezado;
    int ancho;
    int alto;
    unsigned short profundidad;

}t_metadata;

typedef struct
{
    t_metadata metadata;
    t_pixel **pixeles;
}t_imagen;

typedef int (*t_func_normal)(char *argv[]);

typedef int (*t_func_porc)(char *argv[], int valor);

typedef struct
{
    const char* nombre_funcion;
    const t_func_normal funcion;

}t_modif_normal;

typedef struct
{
    const char* nombre_funcion;
    const t_func_porc funcion;

}t_modif_porc;

typedef struct
{
    t_func_normal normal;
    t_func_porc porc;
    int valor;

}t_funcion_encontrada;

#endif // ESTRUCTURAS_H_INCLUDED
