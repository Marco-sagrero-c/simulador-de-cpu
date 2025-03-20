#ifndef SWAP_H
#define SWAP_H

#include <stdio.h>


#define TAMANIO_LINEA 32 // Tamaño máximo esperado de una línea
#define TAMANO_ARREGLO 4096

struct tmp
{
    int lleno;
    int orden;
    long pri, ult;
    int tmp;
    int id;
};

struct ram {
    int pri ,ult;
    int lleno;      // Indica si el marco está ocupado
    int orden;      // Orden de la página dentro del proceso
    int num_marco;  // Número del marco en la RAM
    int id;         // ID del proceso dueño del marco
    int contador;   // Contador para el orden de carga (FIFO)
};




extern struct ram ram_arr[8];
extern FILE *salida;
extern struct tmp arreglo[TAMANO_ARREGLO];


void escribir(FILE *entrada, int numBloque);
int bloques(FILE *archivo);
int encontrarBloqueVacio();
int escribirParteEnArchivoBinario(FILE *entrada, int id);
long contarLineas(FILE *archivo);
int leer_linea_archivoB(long int posicion);
char* leer_linea_cadena(long int posicion);
int espacios_restantes(void);











#endif // SWAP_H

