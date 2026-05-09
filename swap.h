#ifndef SWAP_H
#define SWAP_H

#include <stdio.h>


#define TAMANIO_LINEA 32 // Tamaño máximo esperado de una línea
#define TAMANO_ARREGLO 4096
#define TAMANO_MARCOS 4 // ES EL TAMANO DE LOS MARCOS ANTES DE 16 HAORA DE 4
#define TAMANO_RAM 16

#define TAMANO_B1_B2 8
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

    //////reloj
    int bit_retraso; // solo es para saber si se usa del vit de 0 y 1
    int puntero;// es el apuntador para ver quien sigue para el reloj
    /////
    int uso;
    ///////////car 
    int referencia ;// bit r 
    int tipo ; // en que lista esta
    int hospedaje; // la utilizo para saber cuando mandar una pagina a t2
    /////
    int accesos;
    
};




extern struct ram ram_arr[TAMANO_RAM];
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

