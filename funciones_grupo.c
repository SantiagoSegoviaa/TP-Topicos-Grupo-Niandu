/*
    Integrantes del grupo. En caso de ser un grupo de dos integrantes, no completar el último campo.
    Si alguno de los integrantes del grupo dejara la materia, completar de todos modos sus datos, aclarando que no entrega.
    -----------------
    Apellido: Segovia
    Nombre: Santiago Ezequiel
    DNI: 46704508
    Entrega: Si
    -----------------
    Apellido: Diaz
    Nombre: Juan Pablo
    DNI: 44871263
    Entrega: Si
    -----------------
    Apellido: Paz
    Nombre: Facundo Bautista
    DNI: 44641005
    Entrega: Si
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

    t_funcion_encontrada *encontradas; //ARRAY DE ESTRUCTURAS DONDE SE GUARDARAN LAS FUNCIONES ENCONTRADAS Y SU VALOR HALLADO PARA SER EJECUTADAS
    t_funcion_encontrada encontrada; // ESTRUCTURA DONDE SE GUARDARA LA FUNCION ENCONTRADA Y SU VALOR HALLADO PARA SER EJECUTADA
    bool hay_funcion = false;

    int pos_imagenes[2] = {0, 0}; // ARREGLO QUE GUARDA LA POSICION DE LAS IMAGENES A CONCATENAR O PROCESAR
    int i,cant_encontradas = 0,cod;

    for(i = 1 ; i < argc ; i++)
    {
        if(TieneExtensionBMP(*(argv+i)))
        {
            cod = esArchivoBMP(*(argv+i));
            if(cod != OK)
                informarError(cod, *(argv+i));
            else
            {
                if(*pos_imagenes == 0)
                    *pos_imagenes = i;
                else if(*(pos_imagenes+1) == 0)
                {
                    *(pos_imagenes+1) = i;
                }
                else
                    informarError(ERR_IMAGEN_EXTRA, *(argv+i));
            }
        }
    }
    if(*pos_imagenes == 0)
    {
        printf("Las imagenes no fueron ingresadas o no fueron validas.\n");
        return 1;
    }

    encontradas = (t_funcion_encontrada*)malloc(sizeof(t_funcion_encontrada));
    if(encontradas == NULL)
    {
        informarError(ERR_MEMORIA, NULL);
        return 1;
    }

    for(i = 1 ; i < argc ; i++)
    {
        if(!TieneExtensionBMP(*(argv+i)))
        {
            cod = esModificacion(argv,i,&encontrada);
            if(cod != OK)
                informarError(cod, *(argv+i));

            else if(!esRepetida(argv,i))
            {
                if(!hay_funcion)
                {
                    *encontradas = encontrada;
                    cant_encontradas++;
                    hay_funcion = true;
                }
                else
                {
                    t_funcion_encontrada *temp = (t_funcion_encontrada*)realloc(encontradas, sizeof(t_funcion_encontrada) * (cant_encontradas + 1));
                    if(temp == NULL)
                    {
                        informarError(ERR_MEMORIA, NULL);
                        free(encontradas);
                        return 1;
                    }
                    *(temp+cant_encontradas) = encontrada;
                    encontradas = temp;
                    cant_encontradas++;
                }
            }
        }
    }

    if(!hay_funcion)
    {
        printf("No se encontraron funciones validas para ejecutar.\n");
        free(encontradas);
        return 1;
    }
    else
    {

        for(i = 0 ; i < cant_encontradas ; i++)
        {
            if(*(pos_imagenes+1) != 0)
            {
                if((encontradas+i)->dobl != NULL)
                {
                    (encontradas+i)->pos1 = *pos_imagenes;
                    (encontradas+i)->pos2 = *(pos_imagenes+1);
                    ejecutarFuncion(argv,(encontradas+i));
                }
                else
                {
                    (encontradas+i)->pos1 = *pos_imagenes;
                    ejecutarFuncion(argv,(encontradas+i));
                    (encontradas+i)->pos1 = *(pos_imagenes+1);
                    ejecutarFuncion(argv,(encontradas+i));
                }
            }
            else if((encontradas+i)->dobl != NULL)
            {
                informarError(ERR_FALTA_IMAG, "Funcion de concatenacion");
            }
            else
            {
                (encontradas+i)->pos1 = *pos_imagenes;
                ejecutarFuncion(argv,(encontradas+i));
            }
        }
    }
    free(encontradas);

    return 0;
}
int TieneExtensionBMP(const char *ruta)
{
    char *inicioformato;

    inicioformato = strrchr(ruta, '.');

    return inicioformato != NULL && strcmpi(inicioformato, ".bmp") == 0;
}

int esArchivoBMP(const char *ruta)
{
    unsigned char firma[2];
    size_t bytesleidos_firma;
    unsigned short profundidad;
    unsigned int compresion;


    FILE *archivo = fopen(ruta,"rb");
    if(!archivo)
        return ERR_NO_EXISTE;

    bytesleidos_firma = fread(firma,1,2,archivo);
    fseek(archivo, 28, SEEK_SET);
    fread(&profundidad, 2, 1, archivo);
    fseek(archivo,30, SEEK_SET);
    fread(&compresion, 4, 1, archivo);
    fclose(archivo);

    if(bytesleidos_firma!= 2 || profundidad != 24 || compresion != 0)
        return ERR_PROFUNDIDAD;
    if(*firma != 'B' || *(firma+1) != 'M')
        return ERR_NO_ES_BMP;

    return OK;

}

int esModificacion(char *argv[], int pos, t_funcion_encontrada *func)
{
    int cant_normales;
    int cant_porc;
    int cant_dobl;
    //int cant_dobl;

    const t_modif_normal *ptr_normal;
    const t_modif_normal *fin_normal;

    const char *nombre = *(argv+pos);

    func->normal=NULL;
    func->porc=NULL;
    func->dobl=NULL;

    const t_modif_normal funciones_normales[] = {
    {"--negativo", pixelNegativo},
    {"--escala-de-grises", pixelGrises},
    {"--espejar-horizontal", pixelEspHoriz},
    {"--espejar-vertical", pixelEspVert},
    {"--rotar-derecha", pixelRotarDerecha}
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

    const t_modif_dobl *ptr_dobl;
    const t_modif_dobl *fin_dobl;

    const t_modif_dobl func_dobl[] = {
    {"--concatenar-horizontal",pixelConcatenarHoriz},
    {"--concatenar-vertical",pixelConcatenarVert}
    };


    cant_normales = sizeof(funciones_normales) / sizeof(*funciones_normales);
    cant_porc = sizeof(func_porc) / sizeof(*func_porc);
    cant_dobl = sizeof(func_dobl) / sizeof(*func_dobl);

    //Verificar si es una funcion normal

    fin_normal = funciones_normales + cant_normales;

    for(ptr_normal = funciones_normales ; ptr_normal< fin_normal ; ptr_normal++)
    {
        if(strcmp(nombre,ptr_normal->nombre_funcion)==0)
        {
            func->normal = ptr_normal->funcion;
            return OK;
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
                return OK;
            }
            else
                return ERR_VALOR_INC;
        }
    }

    //Verificar si es una funcion que necesita dos imagenes -- TO DO
    fin_dobl = func_dobl + cant_dobl;
    for(ptr_dobl = func_dobl; ptr_dobl < fin_dobl; ptr_dobl++)
    {
        if(strcmp(nombre,ptr_dobl->nombre_funcion)==0)
        {
            func->dobl = ptr_dobl->funcion;
            return OK;
        }
    }

    return ERR_MODIF_INC;
}

void informarError(int cod, const char *arg)
{
    switch(cod)
    {
    case ERR_NO_EXISTE:
        printf("\"%s\": el archivo no existe.\n", arg);
        break;

    case ERR_NO_ES_BMP:
        printf("\"%s\": no es un archivo BMP.\n", arg);
        break;

    case ERR_PROFUNDIDAD:
        printf("\"%s\": no es de 24 bits.\n", arg);
        break;

    case ERR_COMPRESION:
        printf("\"%s\": tiene compresion, no se soporta.\n", arg);
        break;

    case ERR_MODIF_INC:
        printf("\"%s\": la modificacion no existe.\n", arg);
        break;

    case ERR_VALOR_INC:
        printf("\"%s\": el valor debe estar entre 0 y 100.\n", arg);
        break;

    case ERR_FALTA_IMAG:
        printf("\"%s\": se necesitan 2 imagenes validas.\n", arg);
        break;

    case ERR_EJECUCION:
        printf("\"%s\": no se pudo aplicar.\n", arg);
        break;
    case ERR_MEMORIA:
        printf("Hubo un error y no se pudo reservar memoria.\n");
        break;
    case ERR_IMAGEN_EXTRA:
        printf("\"%s\": se ingresaron mas de 2 imagenes validas, se procesaran las primeras 2.\n", arg);
        break;
    }
}
void ejecutarFuncion(char *argv[], t_funcion_encontrada *func)
{

    if(func->normal != NULL)
        func->normal(argv,func->pos1);
    else if(func->porc != NULL)
        func->porc(argv,func->valor,func->pos1);
    else if(func->dobl != NULL)
        func->dobl(argv,func->pos1,func->pos2);
}

bool esRepetida(char *argv[], int pos)
{
    int i;
    for(i = pos - 1; i > 0; i--)
    {
        if(strcmp(*(argv+pos), *(argv+i)) == 0)
            return true;
    }

    return false;
}
