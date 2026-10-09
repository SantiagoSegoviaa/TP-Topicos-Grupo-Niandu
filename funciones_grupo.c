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
    /*
        Aquí deben hacer el código que solucione lo solicitado.
	*/

    //Validar argumentos minimos: bmpmanipuleitor.exe <funcionalidad> <archivo origen>
    if(argc < ARGUMENTOS_MIN)
    {
        printf("Argumentos insuficientes\nSintaxis: %s <funcionalidad> <archivo.bmp> <funcionalidad2> ... <funcionalidadN>", argv[0]);
        return 1;
    }

    //Validar que el segundo argumento sea una funcionalidad valida
    int cod = esModificacion(argc, argv, 1);
    validarModif(cod);

    //Validar que el tercer argumento sea un archivo .bmp
    if(!esArchivoBMP(argv[POS_ARCHIVO]))
    {
        printf("El archivo indicado no es un archivo .bmp.\nSintaxis: %s <funcionalidad> <archivo.bmp> <funcionalidad2> ... <funcionalidadN>", argv[0]);
        return 1;
    }

    //Ejecutamos la funcion que viene con el segundo argumento.
    ejecutarFuncion(argc, argv[1], argv);

    //Ejecutamos cada funcion que sigue despues del tercer argumento
    for(int i = 3; i < argc; i++)
    {
        if(esModificacion(argc, argv, i) == VALIDA && !esRepetida(argv, i))
        {
            ejecutarFuncion(argc, argv[i], argv);
        }
    }

    return 0;
}

bool esArchivoBMP(const char *nombre)
{
    int i = 0;
    int pos_punto = -1;

    while(nombre[i] != '\0')
    {
        if(nombre[i] == '.')
        {
            pos_punto = i;
        }

        i++;
    }

    if(pos_punto == -1)
        return false;

    if(strcasecmp(&nombre[pos_punto], ".bmp") == 0)
        return true;

    return false;
}

int esModificacion(int argc, char *argv[], int pos)
{
    const char *nombre = argv[pos];
    const char *func_normales[] = {"escala-de-grises",
                                    "espejar-horizontal",
                                    "espejar-vertical",
                                    "rotar-derecha",
                                    "rotar-izquierda",
                                    "negativo",
                                    "copia-prueba"
                                        };

    int cant_normales = sizeof(func_normales) / sizeof(func_normales[0]);

    const char *func_porc[] = {"aumentar-contraste=",
                                "reducir-contraste=",
                                "tonalidad-azul=",
                                "tonalidad-verde=",
                                "tonalidad-roja=",
                                "recortar=",
                                "achicar="
                                    };

    int cant_porc = sizeof(func_porc) / sizeof(func_porc[0]);

    const char *func_dobl[] = {"concatenar-horizontal",
                               "concatenar-vertical"
                                };

    int cant_dobl = sizeof(func_dobl) / sizeof(func_dobl[0]);

    //Verificar si es una funcion normal
    for(int i = 0; i < cant_normales; i++)
    {
        if(strcmp(nombre, func_normales[i]) == 0)
            return VALIDA;
    }

    //Verificar si es una funcion con porcentaje.
    for(int i = 0; i < cant_porc; i++)
    {
        size_t largo_func = strlen(func_porc[i]);

        if(strncmp(nombre, func_porc[i], largo_func) == 0)
        {
            int valor;
            char basura;

            //Puntero al texto en la posicion justo despues del '='
            const char *nombre_valor = nombre + largo_func;

            int leidos = sscanf(nombre_valor, "%d%c", &valor, &basura);
            if(leidos == 1 && valor > 0 && valor <= 100)
                return VALIDA;
            else
                return VALOR_INC;
        }
    }

    //Verificar si es una funcion que necesita dos imagenes
    for(int i = 0; i < cant_dobl; i++)
    {
        if(strcmp(nombre, func_dobl[i]) == 0)
        {
            //Verificar que los argumentos tenga dos archivos .bmp
            int contador = 0;
            for(int j = 1; j < argc; j++)
            {
                if(esArchivoBMP(argv[j]))
                {
                    contador++;
                }
            }

            //printf("[DEBUG] Funcion: %s | Total archivos .bmp detectados: %d\n", nombre, contador);

            if(contador == 2)
                return VALIDA;
            else
                return FALTA_IMAG;
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

void ejecutarFuncion(int argc, const char *nombre, char *argv[])
{
    if(strcmp(nombre, "negativo") == 0)
    {
        pixelNegativo(argv);
    }
    else if(strcmp(nombre, "escala-de-grises") == 0)
    {
        pixelGrises(argv);
    }
    else if(strcmp(nombre, "espejar-horizontal") == 0)
    {
        pixelEspHoriz(argv);
    }
    else if(strcmp(nombre, "espejar-vertical") == 0)
    {
        pixelEspVert(argv);
    }
    else if(strncmp(nombre, "aumentar-contraste=", strlen("aumentar-contraste=")) == 0)
    {
        const char *nombre_valor = nombre + strlen("aumentar-contraste=");
        int valor;

        sscanf(nombre_valor, "%d", &valor);

        pixelMasContrast(argv, valor);
    }
    else if(strncmp(nombre, "reducir-contraste=", strlen("reducir-contraste=")) == 0)
    {
        const char *nombre_valor = nombre + strlen("reducir-contraste=");
        int valor;

        sscanf(nombre_valor, "%d", &valor);

        pixelMenosContrast(argv, valor);
    }
    else if(strncmp(nombre, "tonalidad-roja=", strlen("tonalidad-roja=")) == 0)
    {
        const char *nombre_valor = nombre + strlen("tonalidad-roja=");
        int valor;

        sscanf(nombre_valor, "%d", &valor);

        pixelTonRojo(argv, valor);
    }
    else if(strncmp(nombre, "tonalidad-azul=", strlen("tonalidad-azul=")) == 0)
    {
        const char *nombre_valor = nombre + strlen("tonalidad-azul=");
        int valor;

        sscanf(nombre_valor, "%d", &valor);

        pixelTonAzul(argv, valor);
    }
    else if(strncmp(nombre, "tonalidad-verde=", strlen("tonalidad-verde=")) == 0)
    {
        const char *nombre_valor = nombre + strlen("tonalidad-verde=");
        int valor;

        sscanf(nombre_valor, "%d", &valor);

        pixelTonVerde(argv, valor);
    }
    else if(strncmp(nombre, "recortar=", strlen("recortar=")) == 0)
    {
        const char *nombre_valor = nombre + strlen("recortar=");
        int valor;

        sscanf(nombre_valor, "%d", &valor);

        pixelRecortar(argv, valor);
    }
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
