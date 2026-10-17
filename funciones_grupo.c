/*
    Integrantes del grupo. En caso de ser un grupo de dos integrantes, no completar el �ltimo campo.
    Si alguno de los integrantes del grupo dejara la materia, completar de todos modos sus datos, aclarando que no entrega.
    -----------------
    Apellido:
    Nombre:
    DNI:
    Entrega:
    -----------------
    Apellido:
    Nombre:
    DNI:
    Entrega:
    -----------------
    Apellido:
    Nombre:
    DNI:
    Entrega:
    -----------------
	Apellido:
    Nombre:
    DNI:
    Entrega:
    -----------------
*/

#include "funciones_grupo.h"
#include "estructuras.h"

int solucion(int argc, char* argv[])
{

    t_funcion_encontrada encontrada; // ESTRUCTURA DONDE SE GUARDARA LA FUNCION ENCONTRADA Y SU VALOR HALLADO PARA SER EJECUTADA

    //Validar argumentos minimos: bmpmanipuleitor.exe <funcionalidad> <archivo origen>
    if(argc < ARGUMENTOS_MIN)
    {
        printf("Argumentos insuficientes\nSintaxis: %s <funcionalidad> <archivo.bmp> <funcionalidad2> ... <funcionalidadN>", *argv);
        return 1;
    }

    //Validar que el tercer argumento sea un archivo .bmp
    if(!esArchivoBMP(argv[POS_ARCHIVO]))
    {
        printf("El archivo indicado no es un archivo .bmp.\nSintaxis: %s <funcionalidad> <archivo.bmp> <funcionalidad2> ... <funcionalidadN>", *argv);
        return 1;
    }

    //Validar que el segundo argumento sea una funcionalidad valida
    int cod = esModificacion(argc, argv, 1, &encontrada);
    if(cod != VALIDA)
    {
        validarModif(cod);
        return 1;
    }

    //Ejecutamos la funcion que viene con el segundo argumento.
    ejecutarFuncion(argc, *(argv+1), argv,&encontrada);

    //Ejecutamos cada funcion que sigue despues del tercer argumento
    for(int i = 3; i < argc; i++)
    {
        if(esModificacion(argc, argv, i,&encontrada) == VALIDA && !esRepetida(argv, i))
        {
            ejecutarFuncion(argc, *(argv+i), argv, &encontrada);
        }
    }

    return 0;
}

bool esArchivoBMP(const char *ruta)
{
    unsigned char firma[2];
    size_t bytesleidos_firma;
    unsigned short profundidad;
    unsigned int compresion;
    char *inicioformato;

    inicioformato = strrchr(ruta, '.');
    if(!inicioformato)
        return false;

    if(strcmpi(inicioformato, ".bmp") == 0)
    {
        FILE *archivo = fopen(ruta,"rb");
        if(!archivo)
            return false;

        bytesleidos_firma = fread(firma,1,2,archivo);
        fseek(archivo, 28, SEEK_SET);
        fread(&profundidad, 2, 1, archivo);
        fseek(archivo,30, SEEK_SET);
        fread(&compresion, 4, 1, archivo);
        fclose(archivo);

        if(bytesleidos_firma!= 2 || profundidad != 24 || compresion != 0)
            return false;
        if(*firma != 'B' || *(firma+1) != 'M')
            return false;

        return true;
    }

    return false;
}

int esModificacion(int argc, char *argv[], int pos, t_funcion_encontrada *func)
{
    int cant_normales;
    int cant_porc ;
    int cant_dobl;

    const t_modif_normal *ptr_normal;
    const t_modif_normal *fin_normal;

    const char *nombre = *(argv+pos);

    func->normal=NULL;
    func->porc=NULL;

    const t_modif_normal funciones_normales[] = {
    {"--negativo", pixelNegativo},
    {"--escala-de-grises", pixelGrises},
    {"--espejar-horizontal", pixelEspHoriz},
    {"--espejar-vertical", pixelEspVert}
    };

    const t_modif_porc *ptr_porc;
    const t_modif_porc *fin_porc;

    const t_modif_porc func_porc[] = {
    {"--aumentar-contraste=",pixelMasContrast},
    {"--reducir-contraste=",pixelMenosContrast},
    {"--tonalidad-azul=",pixelTonAzul},
    {"--tonalidad-verde=",pixelTonVerde},
    {"--tonalidad-roja=",pixelTonRojo},
    {"--recortar=",pixelRecortar}
    };


    cant_normales = sizeof(funciones_normales) / sizeof(*funciones_normales);
    cant_porc = sizeof(func_porc) / sizeof(*func_porc);

    //Verificar si es una funcion normal

    fin_normal = funciones_normales + cant_normales;

    for(ptr_normal = funciones_normales ; ptr_normal< fin_normal ; ptr_normal++)
    {
        if(strcmp(nombre,ptr_normal->nombre_funcion)==0)
        {
            func->normal = ptr_normal->funcion;
            return VALIDA;
        }
    }

    //Verificar si es una funcion con porcentaje.
    fin_porc = func_porc + cant_porc;

    for(ptr_porc = func_porc; ptr_porc < fin_porc; ptr_porc++)
    {
        size_t largo_func = strlen(ptr_porc->nombre_funcion);

        if(strncmp(nombre, ptr_porc->nombre_funcion, largo_func) == 0)
        {
            int valor;
            char basura;

            //Puntero al texto en la posicion justo despues del '='
            const char *nombre_valor = nombre + largo_func;

            int leidos = sscanf(nombre_valor, "%d%c", &valor, &basura);
            if(leidos == 1 && valor >= 0 && valor <= 100)
            {
                func->porc = ptr_porc->funcion;
                func->valor = valor;
                return VALIDA;
            }
            else
                return VALOR_INC;
        }
    }

    //Verificar si es una funcion que necesita dos imagenes -- TO DO


    return MODIF_INC;
}

void validarModif(int cod)
{
    switch(cod)
    {
    case MODIF_INC:
        printf("La modificacion que ingresaste no existe.\n");
        break;

    case VALOR_INC:
        printf("Solo se pueden ingresar valores entre 0 y 100.\n");
        break;

    case FALTA_IMAG:
        printf("Para usar la funcion concatenar se necesitan de dos imagenes.\n");
        break;
    }
}

void ejecutarFuncion(int argc, const char *nombre, char *argv[],t_funcion_encontrada *func)
{

    if(func->normal != NULL)
        func->normal(argv);
    else if(func->porc != NULL)
        func->porc(argv,func->valor);
    else
        printf("Justo esa funcion la hice todavia, proba otra.\n");
}

bool esRepetida(char *argv[], int pos)
{
    for(int i = pos - 1; i > 0; i--)
    {
        if(strcmp(argv[pos], argv[i]) == 0)
            return true;
    }

    return false;
}
