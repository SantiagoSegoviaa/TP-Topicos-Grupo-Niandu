/*
    Integrantes del grupo. En caso de ser un grupo de dos integrantes, no completar el último campo.
    Si alguno de los integrantes del grupo dejara la materia, completar de todos modos sus datos, aclarando que no entrega.
    -----------------
    Apellido: Segovia
    Nombre: Santiago
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

    t_funcion_encontrada *encontradas; // ESTRUCTURA DONDE SE GUARDARA LA FUNCION ENCONTRADA Y SU VALOR HALLADO PARA SER EJECUTADA
    t_funcion_encontrada encontrada;
    bool hay_funcion = false;

    int pos_imagenes[2] = {0, 0}; // ARREGLO QUE GUARDA LA POSICION DE LAS IMAGENES A CONCATENAR
    int i,cant_encontradas = 0,cod_modif;

    for(i = 1 ; i < argc ; i++)
    {
        if(strrchr(*(argv+i), '.') != NULL)
        {
            if(!esArchivoBMP(*(argv+i)))
            {
                printf("El archivo %s no es un BMP valido.\n", *(argv+i));
            }
            else
            {
                if(*pos_imagenes == 0)
                    *pos_imagenes = i;
                else
                    *(pos_imagenes+1) = i;
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
        printf("Hubo un error y no se pudo reservar memoria.\n");
        return 1;
    }   

    for(i = 1 ; i < argc ; i++)
    {
        if(!esArchivoBMP(*(argv+i)))    
        {
            cod_modif = esModificacion(argv,i,&encontrada);
            if(cod_modif != VALIDA)
                validarModif(cod_modif);

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
                        printf("Hubo un error y no se pudo reservar memoria.\n");
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
        printf("No se ingresaron funciones validas.\n");
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
                printf("Se salteo una funcion de concatenacion debido a que no se poseen 2 imagenes validas\n");
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
    fin_dobl = func_dobl + cant_dobl;
    for(ptr_dobl = func_dobl; ptr_dobl < fin_dobl; ptr_dobl++)
    {
        if(strcmp(nombre,ptr_dobl->nombre_funcion)==0)
        {
            func->dobl = ptr_dobl->funcion;
            return VALIDA;
        }
    }

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

void ejecutarFuncion(char *argv[], t_funcion_encontrada *func)
{

    if(func->normal != NULL)
        func->normal(argv,func->pos1);
    else if(func->porc != NULL)
        func->porc(argv,func->valor,func->pos1);
    else if(func->dobl != NULL)
        func->dobl(argv,func->pos1,func->pos2);
    else
        printf("Justo esa funcion la hice todavia, proba otra.\n");
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
