
#include "lista.h"
//#include <stdio.h>
#include <stdlib.h>

struct PCB *crear_PCB(int pid) {
    struct PCB *nodo = (struct PCB *)malloc(sizeof(struct PCB));
    if (nodo != NULL) {
        nodo->pid = pid;
        nodo->sig = NULL;
        nodo->ant = NULL;
        nodo->AX = 0;
        nodo->BX = 0;
        nodo->CX = 0;
        nodo->DX = 0;
        nodo->PC = 0;
        nodo->IR = malloc(100 * sizeof(char));
        nodo->fileName = malloc(100 * sizeof(char));
        nodo->programa=NULL;
        //modulo 6
        nodo->UID=0;
        nodo->KCPU=0;
        nodo->KCPUxU=0;    //nuevos parametros
        nodo->P=0;    // nuevo parametro
        //////
        nodo->familia=0; /// para saber si esta compartiendo espacio con  alguien 
    }
    return nodo;
}






void insertar_PCB(struct PCB **inicio, struct PCB *nodo){

  struct PCB * act= NULL;

  if (*inicio==NULL)
    *inicio = nodo ;
  else {
    act=*inicio;
    //recorrido para encontrar el ultimo elementoi
    while(act -> sig != NULL){
      act = act -> sig;
    }
    act-> sig = nodo;
    nodo ->ant = act;
  
}
}




/* 
void insertar_PCB(struct PCB **inicio, struct PCB *nodo) {
    struct PCB *actual = *inicio;
    struct PCB *previo = NULL;

    while (actual != NULL && actual->pid < nodo->pid) {
        previo = actual;
        actual = actual->sig;
    }

    if (previo == NULL) {
        nodo->sig = *inicio;
        if (*inicio != NULL) {
            (*inicio)->ant = nodo;
        }
        *inicio = nodo;
    } else {
        previo->sig = nodo;
        nodo->ant = previo;
        nodo->sig = actual;
        if (actual != NULL) {
            actual->ant = nodo;
        }
    }
}
*/
/* 
struct PCB *extraer_PCB(struct PCB **inicio, int pid) {
    struct PCB *actual = *inicio;
    struct PCB *previo = NULL;

    while (actual != NULL && actual->pid != pid) {
        previo = actual;
        actual = actual->sig;
    }

    if (actual == NULL) {
        return NULL;
    } else if (previo == NULL) {
        *inicio = actual->sig;
        if (*inicio != NULL) {
            (*inicio)->ant = NULL;
        }
    } else {
        previo->sig = actual->sig;
        if (actual->sig != NULL) {
            actual->sig->ant = previo;
        }
    }

    actual->ant = NULL;
    actual->sig = NULL;
    return actual;
}
*/

struct PCB *extraer_PCB(struct PCB **inicio, int pid) {
    struct PCB *actual = *inicio;
    struct PCB *previo = NULL;

    // Recorrer la lista para encontrar el nodo con el pid especificado
    while (actual != NULL && actual->pid != pid) {
        previo = actual;
        actual = actual->sig;
    }

    // Si el nodo no se encuentra, retornar NULL
    if (actual == NULL) {
        return NULL;
    }

    // Si el nodo es el primero en la lista
    if (previo == NULL) {
        *inicio = actual->sig; // Actualizar el inicio de la lista
        if (*inicio != NULL) {
            (*inicio)->ant = NULL; // Actualizar el puntero 'ant' del nuevo primer nodo
        }
    } else {
        // Eliminar el nodo enlazando el nodo anterior con el siguiente
        previo->sig = actual->sig;
        if (actual->sig != NULL) {
            actual->sig->ant = previo;
        }
    }

    // Si el nodo actual era el último nodo, actualizamos la lista a NULL si está vacía
    if (*inicio == NULL) {
        // Si eliminamos el último nodo, ya hemos ajustado *inicio a NULL, no es necesario hacer nada más
    } else {
        struct PCB *temp = *inicio;
        while (temp != NULL) {
            if (temp->sig == NULL) {
                // Si encontramos que el último nodo es NULL, y si estamos eliminando el último nodo,
                // se actualizará la lista a NULL.
                if (temp == actual) {
                    *inicio = NULL;
                }
                break;
            }
            temp = temp->sig;
        }
    }

    // Desconectar el nodo encontrado
    actual->ant = NULL;
    actual->sig = NULL;

    // Retornar el nodo extraído
    return actual;
}












void eliminar_PCB(struct PCB *nodo) {
    if (nodo->programa==0)
    {
        printf("archivo cerrado \n ");
        fclose(nodo->programa);
    }
    free(nodo->IR);
    free(nodo->fileName);
    free(nodo);
}

void imprimir_lista(struct PCB *inicio) {
    struct PCB *actual = inicio;
    printf("NULL");
    while (actual != NULL) {
        printf("<-%d->", actual->pid);
        actual = actual->sig;
    }
    printf("null \n");
}

void vaciar_lista(struct PCB **cola) {
    struct PCB *actual = *cola;
    while (actual != NULL) {
        struct PCB *siguiente = actual->sig;
        eliminar_PCB(actual);
        actual = siguiente;
    }
    *cola = NULL;
}







///////////////////////////////lista pala 

/* 
void insertarAlPrincipio(struct pala **cabeza, char *dato) {
    // Verificar si el dato ya existe en la lista
    struct pala *actual = *cabeza;
    while (actual != NULL) {
        if (strcmp(actual->dato, dato) == 0) {
            // El dato ya existe, no es necesario agregarlo nuevamente
            return;
        }
        actual = actual->siguiente;
    }

    // Crear un nuevo nodo
    struct pala *nuevoNodo = (struct pala *)malloc(sizeof(struct pala));
    if (nuevoNodo == NULL) {
        return;
    }

    // Asignar el dato al nuevo nodo
    nuevoNodo->dato = strdup(dato);
    if (nuevoNodo->dato == NULL) {
        free(nuevoNodo);
        return;
    }

    // Establecer el siguiente del nuevo nodo como la cabeza actual
    nuevoNodo->siguiente = *cabeza;

    // Actualizar la cabeza para que apunte al nuevo nodo
    *cabeza = nuevoNodo;
}


int liberarLista(struct pala *cabeza) {
    int contador = 0; // Contador para registrar cuántas veces se libera memoria
    struct pala *actual = cabeza;
    while (actual != NULL) {
        struct pala *temp = actual;
        actual = actual->siguiente;
        free(temp->dato);
        free(temp);
        contador++; // Incrementar el contador cada vez que se libera memoria
    }
    return contador; // Devolver el valor del contador al final de la función
}
*/