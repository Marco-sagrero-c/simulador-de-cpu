#include "lista.h"
#include "swap.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

FILE *salida;
struct tmp arreglo[TAMANO_ARREGLO];
struct ram ram_arr[8];
//int contador_carga = 0;


int encontrarBloqueVacio()
{
    for (int i = 0; i < TAMANO_ARREGLO; i++)
    {
        if (!arreglo[i].lleno)
        {
            return i; // Devuelve el índice del primer bloque vacío encontrado
        }
    }
    return -1; // Si no se encuentra ningún bloque vacío, devuelve -1
}


int escribirParteEnArchivoBinario(FILE *entrada, int id)
{
    char linea[TAMANIO_LINEA]; // Buffer para almacenar cada línea
    int longitud_linea;

    // Comprobar si el archivo es NULL
    if (entrada == NULL)
    {
        perror("Error: archivo de entrada es NULL");
        return 0; // Error, el archivo de entrada es NULL
    }

    int numBloque;
    int conta = bloques(entrada);

    // Verificar si el archivo es demasiado grande
    if (conta > 4096)
    {
        return 0; // Indicar que el archivo es demasiado grande
    }

    // Verificar si hay suficientes espacios disponibles
    if (conta > espacios_restantes())
    {
        fseek(entrada, 0, SEEK_SET);
        return 41; // Indicar que no hay suficientes espacios
    }

    // Escribir las partes en los bloques vacíos
    for (int i = 0; i < conta; i++)
    {
        numBloque = encontrarBloqueVacio();
        if (numBloque == -1)
        {
            fseek(entrada, 0, SEEK_SET);
            return 0; // No se encontró un bloque vacío, algo salió mal
        }

        escribir(entrada, numBloque);
        arreglo[numBloque].lleno = 1;
        arreglo[numBloque].id = id;
        arreglo[numBloque].orden = i;
    }
    fseek(entrada, 0, SEEK_SET);

    return 1; // Indicar que la operación fue exitosa
}

void escribir(FILE *entrada, int numBloque)
{
    char linea[TAMANIO_LINEA]; // Buffer para almacenar cada línea
    int longitud_linea;

    long posicion = numBloque * 16 * TAMANIO_LINEA;
    fseek(salida, posicion, SEEK_SET);

    // Escribir la primera parte en el bloque
    for (int i = 0; i < 16; i++)
    {
        if (fgets(linea, sizeof(linea), entrada) != NULL)
        {
            longitud_linea = strlen(linea);
            fwrite(linea, sizeof(char), longitud_linea, salida);
            if (longitud_linea < TAMANIO_LINEA)
            {
                char cero = 0;
                int espacios_restantes = TAMANIO_LINEA - longitud_linea;
                for (int j = 0; j < espacios_restantes; j++)
                {
                    fwrite(&cero, sizeof(char), 1, salida);
                }
            }
        }
        else
        {
            break; // Si no hay más líneas en el archivo, terminar el bucle
        }
    }
}


int espacios_restantes(void)
{
    int espacios_vacios = 0;
    for (int i = 0; i < TAMANO_ARREGLO; i++)
    {
        if (arreglo[i].lleno == 0)
        {
            espacios_vacios++;
        }
    }
    return espacios_vacios;
}




long contarLineas(FILE *archivo)
{
    if (archivo == NULL)
    {
        //perror("Error: archivo es NULL");
        return -1; // Error, el archivo es NULL
    }

    long contador = 1; // Contador de líneas
    char caracter;

    // Leer el archivo caracter por caracter hasta llegar al final
    while ((caracter = fgetc(archivo)) != EOF)
    {
        if (caracter == '\n' ) // Si el caracter es un salto de línea, incrementar el contador
        {
            contador++;
        }
    }

    // Restablecer el puntero del archivo al inicio
    fseek(archivo, 0, SEEK_SET);

    return contador;
}



int bloques(FILE *archivo)
{
    int contador;
    long aux = contarLineas(archivo);
    contador = aux / 16;
    if (aux % 16 != 0)
    {
        contador++; // Incrementar contador solo si hay líneas adicionales
    }
    //printf("contador %d \t",contador);
    return contador;
}



int leer_linea_archivoB(long int posicion)
{
    if (salida != NULL)
    {
        fseek(salida, posicion, SEEK_SET); // Establecer la posición de lectura en el archivo

        char buffer[32]; // Tamaño máximo de datos a leer
        int bytes_leidos = 0;
        int i = 0;

        // Leer hasta que se encuentre un salto de línea o un carácter nulo
        while ((bytes_leidos = fread(&buffer[i], sizeof(char), 1, salida)) > 0)
        {
            if (buffer[i] == '\n' || buffer[i] == '\0')
            {
                // Si encuentra un salto de línea o un carácter nulo, detener la lectura
                buffer[i] = '\0'; // Terminar la cadena en el punto de detección

                if (strcmp(buffer, "end") == 0)
                {
                    // printf("Se encontró la palabra 'end'. Finalizando ejecución.\n");
                    i++;
                    return 0; // Terminar la ejecución
                }

                // Analizar la línea utilizando sscanf
                char comando[10];
                char registro[10];
                int valor;
                sscanf(buffer, "%s %s %d", comando, registro, &valor);
                printf("Comando: %s, Registro: %s, Valor: %d\n", comando, registro, valor);

                break;
            }
            i++;
        }

        return 1; // Indica que se leyó una línea
    }
    else
    {
        // printf("Error al abrir el archivo.\n");
        return 0; // Indica que hubo un error al abrir el archivo
    }
}




char *leer_linea_cadena(long int posicion)
{
    static char buffer[TAMANIO_LINEA]; // Tamaño máximo de datos a leer

    if (salida != NULL)
    {
        fseek(salida, posicion, SEEK_SET); // Establecer la posición de lectura en el archivo

        int bytes_leidos = 0;
        int i = 0;

        // Leer hasta que se encuentre un salto de línea o un carácter nulo
        while ((bytes_leidos = fread(&buffer[i], sizeof(char), 1, salida)) > 0)
        {
            if (buffer[i] == '\n' || buffer[i] == '\0')
            {
                // Si encuentra un salto de línea o un carácter nulo, detener la lectura
                buffer[i] = '\0'; // Terminar la cadena en el punto de detección

                return buffer; // Devolver la cadena leída
            }
            i++;
        }

        if (i == 0)
        {
            return NULL; // No se leyó nada, devolver NULL
        }

        buffer[i] = '\0'; // Terminar la cadena
        return buffer;
    }
    else
    {
        return NULL; // Indica que hubo un error al abrir el archivo
    }
}

//////////////////////////////////////////////////////////////////////// RAM 



