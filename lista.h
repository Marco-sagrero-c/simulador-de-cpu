#ifndef _LISTA_H_
#define _LISTA_H_
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct PCB
{
    //...
    int pid;
    struct PCB *sig;
    struct PCB *ant;
    int AX, BX, CX, DX, PC;
    char *IR;
    /// 
    int marcos;
    int familia;
    /// 
    int UID,KCPU,KCPUxU,P;// nuevo parametro

    
    char *fileName;
    FILE *programa;
};

struct pala {
    char *dato;
    struct pala *siguiente;
};




struct PCB *crear_PCB(int);
void insertar_PCB(struct PCB **, struct PCB *);
struct PCB *extraer_PCB(struct PCB **, int);
void eliminar_PCB(struct PCB *);
void imprimir_lista(struct PCB *);
void vaciar_lista(struct PCB **);




////////////////////////////
void mostrar_mensaje(const char *mensaje);
int obtener_valor(const char *registro_fuente, struct PCB *pcb) ;
//////////////////////////////////////////////////////////////////////////
//void insertarAlPrincipio(struct pala **cabeza, char *dato);
//int liberarLista(struct pala *cabeza) ;

#endif
