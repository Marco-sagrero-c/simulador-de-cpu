#include "swap.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include <sys/select.h>
#include <time.h>
#include "kbhit.h"
#include <ctype.h>
#include <limits.h>
#include "lista.h"

#define QMAX 10

void draw(struct PCB *pcb, int marcos);
void strUpper(char *str);
int procesar_instruccion(char *instruccion, struct PCB *pcb);
// int leer_linea_archivo(FILE *archivo, struct PCB *pcb, struct PCB **lista);
int comandos(char *mensaje, struct PCB **lista, struct PCB **pcb, struct PCB **, struct PCB **);
int shell(char *mensaje, int *j);
//////////////////////////////////
struct PCB *crear_pcb();
void ver_lista(struct PCB *lista);
// void encolar_pcb(struct PCB **lista, struct PCB **pcb);
int programa_cuenta();
// MODULO 2
void ver_terminados(struct PCB *lista);
void mensage(char *mensaje);
void ver_principal(struct PCB *nodo);
int tecla_especial(int *j, int *ch, char *mensaje, struct PCB **lista, struct PCB **PRINCIPAL, struct PCB **terminados, int *rapides, int *TMP, int long *swap, struct PCB **LISTOS, int *marcos);
// MODULO 3
int usuario(struct PCB *lista, struct PCB *Prim);
void Dispacher(struct PCB **lista1);
void actu_U(struct PCB **lista, int us);
void Actualiza_p(struct PCB **lista);
int el_menor(struct PCB *lista);
int error_de_pagina(int pid);

///////
int IncCPU = 60 / QMAX; // Quantum por proceso
int PBase = 60;         // Prioridad base para todos los procesos
int NumUs = 0;          // Cantidad de usuarios para planificar
int W = 0;              // Peso o ponderación de usuarios (inicializado en 0.0)
int usu[100];           // Arreglo con IDs de usuarios, suponiendo un máximo de 100 usuarios

////////////////////modulo 4
void VER_TMS(int Cantidad);
void ver_swap(int long posicion);
void ver_listos(struct PCB *lista);
void liberarMarcosPorPID(int pid);
int obtenerMarco(int pid, int arco);
int leer_linea_archivoBin(struct PCB *pcb, struct PCB **lista);
int buscarProgramaPorNombre(struct PCB *lista, struct PCB *PCBejecucion, struct PCB *PCbnuevo);
int hijo(struct PCB *lista, int padre);
void heredar(int pid, int sobrino, struct PCB **lista, struct PCB **PCB);
/////esta parte del codigo es para la RAM

char Ram[4096] = {0};

void ver_ram();
void mostrar_cap_ram();

int contador_carga = 0; // Contador global para el orden de carga
// Función para manejar el error de página;
int manejar_error_pagina(int pid, int pagina, struct ram *ram, struct tmp *swap);
// Función para seleccionar un marco para reemplazo (algoritmo LRU)
int seleccionar_marco_reemplazo(struct ram *ram);
// Función para guardar una página en el swap
void guardar_en_swap(int pid, int pagina, struct tmp *swap);
// Función para cargar una página desde el swap a la RAM
void cargar_desde_swap(int pid, int pagina, struct ram *marco, struct tmp *swap);
// struct ram RAM= NULL;
/////
int verificar_fallo_pagina(int pid, int pagina);
int copiar_marco_completo_swap_a_ram(int pid, int marco_swap);
int planificador_fifo();

int ver_ram_con = 0;

int main(void)
{

    ///////////////archivo binario (swap)
    char archivoSalida[] = "SWAP2.bin"; // Nombre del archivo binario de salida
    salida = fopen(archivoSalida, "wb+");
    if (salida == NULL)
    {
        perror("Error al crear archivo binario de salida");
        return EXIT_FAILURE;
    }

    for (int i = 0; i < TAMANO_ARREGLO; i++)
    {
        long x = i * 16;
        arreglo[i].lleno = 0;
        arreglo[i].pri = x;
        arreglo[i].ult = x + 16;
        arreglo[i].tmp = i;
    }

    for (int i = 0; i < 8; i++)
    {
        int x = i * 512; // Cada marco ocupa 512 bytes

        ram_arr[i].pri = x;       // Dirección de inicio del marco en RAM
        ram_arr[i].ult = x + 511; // Última dirección dentro del marco

        ram_arr[i].lleno = 0; // Indicar que el marco está vacío
        ram_arr[i].id = -1;   // No tiene proceso asignado
    }

    int TMP = 0;
    int long swap = 0;
    int conta = 0;

    struct PCB *PRINCIPAL = crear_PCB(-1); // este pcb es el que se va a ejecutar en el prosesador -1 VACIO !1 TIENE ALGO
    PRINCIPAL->IR = "--------";
    PRINCIPAL->fileName = "VACIO     ";
    PRINCIPAL->programa = 0;
    struct PCB *lista = NULL;      // ESTA ES LA LISTA DE ESPERA
    struct PCB *AUX = NULL;        // ESTA LISTA ES PARA PODER HACER VARIOS TRABAJOS A LA VEZ
    int qmax = 0;                  // esta es la variable quue controle el q max
    struct PCB *terminados = NULL; // ESTA ES LA LISTA DE LOS QUE YA SE TERMINARON
    struct PCB *LISTOS = NULL;     // ESTA ES LA LISTA DE LOS QUE YA SE TERMINARON
    struct PCB *AUXL = NULL;       // ESTA LISTA ES PARA PODER HACER VARIOS TRABAJOS A LA VEZ
    int rapidez = 10;
    int iteraciones = 0;
    int ch;

    int control = 1;
    char mensaje[80] = {0};
    char copia[80];
    int j = 0;
    //////////////////////////////
    int marcos = 0;

    initscr(); // Inicializa ncurses
    resize_term(300, 300);
    raw();                // Desactiva el búfer de línea
    keypad(stdscr, TRUE); // Habilita la captura de teclas especiales
    noecho();             // No muestra los caracteres ingresados
    strcpy(copia, "esto es un teexto de prueba");
    while (control)
    {
        ch = shell(mensaje, &j);
        iteraciones++;

        control = tecla_especial(&j, &ch, mensaje, &lista, &AUX, &terminados, &rapidez, &TMP, &swap, &LISTOS, &marcos);

        if (strcmp(copia, mensaje))
        {
            if (lista == NULL)
            {
                // draw(PRINCIPAL, 0);
            }
            mensage(mensaje);
            strcpy(copia, mensaje);
            refresh();
        }

        if (iteraciones == rapidez)
        {
            iteraciones = 0;

            if (lista != NULL)
            {
                if (AUX == NULL)
                {
                    // dispacher
                    int menor = el_menor(lista);
                    mvprintw(11, 0, "menor = %d", menor);
                    insertar_PCB(&AUX, extraer_PCB(&lista, menor));
                }

                if (lista != NULL)
                {
                    ver_lista(lista);
                }
                else
                {
                    mvprintw(4, 70, "                                                                                                                                                                                           ");
                    refresh();
                }
            }
            else
            {
                mvprintw(1, 70, "                                                                                                                                                                                ");
                draw(PRINCIPAL, 0);
            }

            if (AUX != NULL)
            {
                if (qmax == IncCPU)
                {
                    insertar_PCB(&lista, extraer_PCB(&AUX, AUX->pid));
                    AUX = NULL;
                    qmax = 0;
                    Dispacher(&lista);
                    // DISPACHER
                }
                else
                {
                    /// AQUI PUEDO PONER EL EROOR DE PAGINA

                    if (leer_linea_archivoBin(AUX, &lista) == 1)
                    {
                        draw(AUX, marcos);
                        ver_principal(AUX);
                    }
                    else
                    {

                        int familia = hijo(lista, AUX->pid);

                        if (familia == 0)
                        {
                            liberarMarcosPorPID(AUX->pid);
                        }
                        else
                        {
                            heredar(AUX->pid, familia, &lista, &AUX);
                        }

                        insertar_PCB(&terminados, extraer_PCB(&AUX, AUX->pid));
                        AUX = NULL;
                        Dispacher(&lista);
                        /////DISPACHER
                    }
                }

                qmax++;
            }
            if (LISTOS != NULL)
            {
                ver_listos(LISTOS);
                AUXL = extraer_PCB(&LISTOS, LISTOS->pid);
                int familia = buscarProgramaPorNombre(lista, AUX, AUXL);

                if (familia != 0)
                {
                    AUXL->familia = familia;
                    insertar_PCB(&lista, AUXL);
                    Dispacher(&lista);
                }
                else
                {
                    conta = bloques(AUXL->programa);
                    if (conta <= espacios_restantes())
                    {
                        fseek(AUXL->programa, 0, SEEK_SET);
                        escribirParteEnArchivoBinario(AUXL->programa, AUXL->pid);
                        insertar_PCB(&lista, AUXL);
                        Dispacher(&lista);
                    }
                    else
                    {
                        insertar_PCB(&LISTOS, AUXL);
                    }
                }
            }
            else
            {
                mvprintw(16, 70, "                                                                                                                  ");
                mvprintw(17, 70, "                                                                                                                  ");
                refresh();
            }
        }

        if (terminados != NULL)
        {
            ver_terminados(terminados);
        }

        if (iteraciones == 200)
        {
            iteraciones = 0;
        }
        mvprintw(12, 0, "         ");
        mvprintw(12, 0, "USU-%d", usuario(lista, AUX));
        mvprintw(12, 8, "         ");
        mvprintw(12, 8, "W = %d", W);

        VER_TMS(TMP);
        ver_swap(swap);
        ver_ram();
        mostrar_cap_ram();

        refresh();

        usleep(10000); // DILEI
    }

    endwin(); // Termina ncurses
    PRINCIPAL->fileName = NULL;
    free(PRINCIPAL->fileName);
    PRINCIPAL->IR = NULL;
    free(PRINCIPAL->IR);

    free(PRINCIPAL);
    vaciar_lista(&lista);
    vaciar_lista(&terminados);

    fclose(salida);

    return EXIT_SUCCESS;
}

void liberarMarcosPorPID(int pid)
{
    for (int i = 0; i < TAMANO_ARREGLO; i++)
    {
        if (arreglo[i].id == pid)
        {
            arreglo[i].lleno = 0; // Marcar como vacío
            arreglo[i].id = 0;    // Reiniciar PID
        }
    }
}

void heredar(int pid, int sobrino, struct PCB **lista, struct PCB **PCB)
{
    for (int i = 0; i < TAMANO_ARREGLO; i++)
    {
        if (arreglo[i].id == pid)
        {
            arreglo[i].id = sobrino; // Marcar como vacío
        }
    }

    struct PCB *actual = *lista;

    while (actual != NULL)
    {
        if (actual->familia == pid)
        {
            actual->familia = sobrino;
        }
        actual = actual->sig;
    }

    //////
    for (int i = 0; i < 8; i++)
    {
        if (pid == ram_arr[i].id)
        {
            ram_arr[i].id = sobrino;
        }
    }

    // ram_arr[marco_ram].id = pid;
}

int hijo(struct PCB *lista, int padre)
{

    while (lista != NULL)
    {
        if (lista->familia == padre)
        {
            return lista->pid;
        }
        lista = lista->sig;
    }
    return 0;
}

int obtenerMarco(int pid, int Marco)
{
    int count = 0;
    for (int i = 0; i < TAMANO_ARREGLO; i++)
    {
        if (arreglo[i].id == pid)
        {
            if (count == Marco)
            {
                return i; // Devuelve el índice del marco correspondiente
            }
            count++;
        }
    }
    return -1; // Si no se encuentra ningún marco para el pid y Marco dados, devuelve -1
}

int el_menor(struct PCB *lista)
{
    // Inicializamos el valor mínimo con el máximo posible
    int minP = INT_MAX;
    int uidmin = -1;

    // Recorremos la lista
    while (lista != NULL)
    {
        // Si el P actual es menor que el mínimo encontrado hasta ahora, actualizamos minP
        if (lista->P < minP)
        {
            minP = lista->P;
            uidmin = lista->pid;
        }
        // Pasamos al siguiente nodo
        lista = lista->sig;
    }
    // Devolvemos el valor mínimo de P
    // MIN P ES EL MINIMO
    return uidmin;
}

void Dispacher(struct PCB **lista1)
{
    struct PCB *AUX1 = *lista1;
    int a = usuario(*lista1, NULL);
    if (a <= 0)
    {
        a = 1;
    }
    W = 100 / a;
    Actualiza_p(lista1);
}

void Actualiza_p(struct PCB **lista)
{
    struct PCB *actual = *lista;
    bool actualizado[100] = {false}; // NUM_USUARIOS es el número total de usuarios posible

    while (actual != NULL)
    {
        actual->KCPU = actual->KCPU / 2;

        if (!actualizado[actual->UID]) // Verifica si el usuario no ha sido actualizado
        {
            usu[actual->UID] = usu[actual->UID] / 2;
            actualizado[actual->UID] = true; // Marca al usuario como actualizado
        }
        actual->KCPUxU = usu[actual->UID];
        actual->P = PBase + (actual->KCPU / 2) + (usu[actual->UID] / (4 * W * 0.01));
        actual = actual->sig;
    }
}

int usuario(struct PCB *lista1, struct PCB *lista2)
{
    int contador = 0;
    int *uidUnicos = NULL; // Arreglo para almacenar los UID únicos
    struct PCB *actual = lista1;

    // Recorrer la primera lista
    while (actual != NULL)
    {
        // Verificar si el UID actual ya está en la lista de UID únicos
        int encontrado = 0;
        for (int i = 0; i < contador; i++)
        {
            if (uidUnicos[i] == actual->UID)
            {
                encontrado = 1;
                break;
            }
        }

        // Si el UID no está en la lista de UID únicos, agregarlo
        if (!encontrado)
        {
            contador++;
            uidUnicos = realloc(uidUnicos, contador * sizeof(int));
            uidUnicos[contador - 1] = actual->UID;
        }

        actual = actual->sig; // Avanzar al siguiente PCB en la lista
    }

    // Recorrer la segunda lista
    actual = lista2;
    while (actual != NULL)
    {
        // Verificar si el UID actual ya está en la lista de UID únicos
        int encontrado = 0;
        for (int i = 0; i < contador; i++)
        {
            if (uidUnicos[i] == actual->UID)
            {
                encontrado = 1;
                break;
            }
        }

        // Si el UID no está en la lista de UID únicos, agregarlo
        if (!encontrado)
        {
            contador++;
            uidUnicos = realloc(uidUnicos, contador * sizeof(int));
            uidUnicos[contador - 1] = actual->UID;
        }

        actual = actual->sig; // Avanzar al siguiente PCB en la lista
    }

    free(uidUnicos); // Liberar la memoria del arreglo de UID únicos
    return contador;
}

int tecla_especial(int *j, int *ch, char *mensaje, struct PCB **lista, struct PCB **PRINCIPAL, struct PCB **terminados, int *rapides, int *TMP, int long *swap, struct PCB **LISTOS, int *marcos)
{

    if (*ch == 10) // SIRVE PARA LEEER COMANDOS
    {
        if (comandos(mensaje, lista, PRINCIPAL, terminados, LISTOS) == -935)
            return 0;
        mensaje[0] = '\0';
        *j = 0;
        mvprintw(14, 2, "                                                                                          ");
        refresh();
    }

    if (*ch == 27) // DETECTA SI LA TECLA PRESIONADA ES ESC
    {
        return 0;
    }

    if (*ch == KEY_BACKSPACE) // DETECTA SI LA TECLA PRESIONADA ES ESC
    {
        if ((*j) > 1)
        {
            (*j)--;
            (*j)--;             // Retroceder en el índice del mensaje
            mensaje[*j] = '\0'; // Eliminar el último carácter del mensaje
        }
    }

    if (*ch == KEY_LEFT) // IZQUIERDA
    {
        if (*rapides > 10)
        {
            (*rapides) -= 10;
        }

        (*j)--; // Retroceder en el índice del mensaje
        mensaje[*j] = '\0';
        mvprintw(13, 2, "                                                      ");
        mvprintw(13, 2, "rapidez %d                        ", *rapides);
        refresh();
    }
    if (*ch == KEY_RIGHT) // DERECHA
    {
        if (*rapides < 210)
        {
            (*rapides) += 10;
        }

        (*j)--;
        mensaje[*j] = '\0';
        mvprintw(13, 2, "                                                         ");
        mvprintw(13, 2, "rapidez %d                       ", *rapides);
        refresh();
    }

    if (*ch == KEY_UP) // DERECHA
    {
        if (*TMP < 4080)
        {
            (*TMP) += 16;
        }

        (*j)--;
        mensaje[*j] = '\0';
        refresh();
    }

    if (*ch == KEY_DOWN) // DERECHA
    {
        if (*TMP > 0)
        {
            (*TMP) -= 16;
        }

        (*j)--;
        mensaje[*j] = '\0';
        refresh();
    }

    if (*ch == KEY_F(2)) // DERECHA
    {
        if (*swap <= 65456)
        {
            (*swap) += 80;
        }

        (*j)--;
        mensaje[*j] = '\0';
        refresh();
    }

    ////

    if (*ch == KEY_F(4)) // DERECHA
    {
        if (*marcos <= 4095)
        {
            (*marcos) += 1;
        }
        (*j)--;
        mensaje[*j] = '\0';
        refresh();
    }

    if (*ch == KEY_F(3)) // DERECHA
    {
        if (*marcos > 0)
        {
            (*marcos) -= 1;
        }

        (*j)--;
        mensaje[*j] = '\0';
        refresh();
    }

    if (*ch == KEY_F(6)) // DERECHA
    {
        if (ver_ram_con > 0)
        {
            ver_ram_con -= 1;
        }

        (*j)--;
        mensaje[*j] = '\0';
        refresh();
    }

    if (*ch == KEY_F(7)) // DERECHA
    {
        if (ver_ram_con < 7)
        {
            ver_ram_con += 1;
        }

        (*j)--;
        mensaje[*j] = '\0';
        refresh();
    }
    ///

    if (*ch == KEY_F(1)) // DERECHA
    {
        if (*swap > 0)
        {
            (*swap) -= 80;
        }

        (*j)--;
        mensaje[*j] = '\0';
        mvprintw(13, 2, "                                                         ");
        mvprintw(13, 2, "rapidez %d                       ", *rapides);
        refresh();
    }

    return 1;
}

void ver_principal(struct PCB *nodo)
{
    struct PCB *actual = nodo;
    mvprintw(0, 70, "-------------------------ejecucion----------------------------------------------------------------------------------------------");
    mvprintw(1, 70, "PID:[   ] FILE:[                ] AX:[   ] BX:[   ] CX:[   ] DX:[   ] PC:[   ] IR:[        ] UID[  ] KCPU[   ] KCPUxU[   ] P[  ]");
    mvprintw(1, 76, "%d", actual->pid);
    mvprintw(1, 86, "%s", actual->fileName);
    mvprintw(1, 109, "%d", actual->AX);
    mvprintw(1, 118, "%d", actual->BX);
    mvprintw(1, 127, "%d", actual->CX);
    mvprintw(1, 136, "%d", actual->DX);
    mvprintw(1, 144, "%d", actual->PC - 1);
    mvprintw(1, 153, "%s", actual->IR);
    mvprintw(1, 167, "%d", actual->UID);
    mvprintw(1, 176, "%d", actual->KCPU);
    mvprintw(1, 188, "%d", actual->KCPUxU);
    mvprintw(1, 195, "%d", actual->P);
    mvprintw(1 + 1, 70, "                                                                                                                                                                                                                 ");
    mvprintw(0, 2, "$> ");
    refresh();
}

void ver_lista(struct PCB *lista)
{

    struct PCB *actual = lista;
    int contador = 3;
    mvprintw(3, 70, "-------------------------LISTOS-------------------------------------------------------------------------");
    while (actual != NULL)
    {
        contador++;
        mvprintw(contador, 70, "PID:[   ] FILE:[                ] AX:[   ] BX:[   ] CX:[   ] DX:[   ] PC:[   ] IR[         ] UID[  ] KCPU[   ] KCPUxU[   ] P[  ]");
        mvprintw(contador, 76, "%d", actual->pid);
        mvprintw(contador, 86, "%s", actual->fileName);
        mvprintw(contador, 109, "%d", actual->AX);
        mvprintw(contador, 118, "%d", actual->BX);
        mvprintw(contador, 127, "%d", actual->CX);
        mvprintw(contador, 136, "%d", actual->DX);
        mvprintw(contador, 144, "%d", actual->PC - 1);
        mvprintw(contador, 152, "%s", actual->IR);
        mvprintw(contador, 167, "%d", actual->UID);
        mvprintw(contador, 176, "%d", actual->KCPU);
        mvprintw(contador, 188, "%d", actual->KCPUxU);
        mvprintw(contador, 195, "%d", actual->P);
        mvprintw(contador + 1, 70, "                                                                                                                                 ");
        mvprintw(0, 2, "$> ");
        actual = actual->sig;
    }
    refresh();
}

void ver_listos(struct PCB *lista)
{

    struct PCB *actual = lista;
    int contador = 16;
    mvprintw(16, 70, "-------------------------ESPERA-------------------------------------------------------------------------");
    while (actual != NULL)
    {
        contador++;
        mvprintw(contador, 70, "PID:[   ] FILE:[                ] AX:[   ] BX:[   ] CX:[   ] DX:[   ] PC:[   ] IR[         ] UID[  ] KCPU[   ] KCPUxU[   ] P[  ]");
        mvprintw(contador, 76, "%d", actual->pid);
        mvprintw(contador, 86, "%s", actual->fileName);
        mvprintw(contador, 109, "%d", actual->AX);
        mvprintw(contador, 118, "%d", actual->BX);
        mvprintw(contador, 127, "%d", actual->CX);
        mvprintw(contador, 136, "%d", actual->DX);
        mvprintw(contador, 144, "%d", actual->PC);
        mvprintw(contador, 152, "%s", actual->IR);
        mvprintw(contador, 167, "%d", actual->UID);
        mvprintw(contador, 176, "%d", actual->KCPU);
        mvprintw(contador, 188, "%d", actual->KCPUxU);
        mvprintw(contador, 195, "%d", actual->P);
        mvprintw(contador + 1, 70, "                                                                                                                                 ");
        mvprintw(0, 2, "$> ");
        actual = actual->sig;
    }
    refresh();
}

void VER_TMS(int cuales)
{

    int contador = cuales + 15;
    int aux = 38;
    mvprintw(37, 0, "-------TMS----------");

    while (cuales <= contador)
    {

        mvprintw(aux, 0, "MARCO:%x  PID:%d LLENO:%d ", cuales, arreglo[cuales].id, arreglo[cuales].lleno);
        // mvprintw(aux + 1, 0, "                                                                      ");
        mvprintw(0, 2, "$> ");
        cuales++;
        aux++;
    }
    refresh();
}

void ver_terminados(struct PCB *lista)
{
    struct PCB *actual = lista;
    int contador = 25;
    mvprintw(25, 70, "-------------------------terminados---------------------------------------------------");
    while (actual != NULL)
    {
        if (contador < 35)
        {
            contador++;
        }
        mvprintw(contador, 70, "PID:[   ] FILE:[                ] AX:[   ] BX:[   ] CX:[   ] DX:[   ] PC:[   ] IR:[        ] UID[  ] KCPU[   ] KCPUxU[   ] P[  ]");
        mvprintw(contador, 76, "%d", actual->pid);
        mvprintw(contador, 86, "%s", actual->fileName);
        mvprintw(contador, 109, "%d", actual->AX);
        mvprintw(contador, 118, "%d", actual->BX);
        mvprintw(contador, 127, "%d", actual->CX);
        mvprintw(contador, 136, "%d", actual->DX);
        mvprintw(contador, 144, "%d", actual->PC - 1);
        mvprintw(contador, 153, "%s", actual->IR);
        mvprintw(contador, 167, "%d", actual->UID);
        mvprintw(contador, 176, "%d", actual->KCPU);
        mvprintw(contador, 188, "%d", actual->KCPUxU);
        mvprintw(contador, 195, "%d", actual->P);
        mvprintw(contador + 1, 70, "                                                                                                ");
        mvprintw(0, 2, "$> ");
        actual = actual->sig;
    }
    refresh();
}

struct PCB *crear_pcb() // dios 1 si crea dios 0 solo cuenta
{
    int a = programa_cuenta();
    return crear_PCB(a);
}

int programa_cuenta()
{
    static int contador = 0; // Se inicializa en la primera llamada y retiene su valor entre llamadas
    contador++;
    return contador;
}

void draw(struct PCB *pcb, int marcos)
{

    mvprintw(1, 0, "---------------------------procesador-------------------------");
    mvprintw(2, 0, "NAME:%s", pcb->fileName);
    mvprintw(3, 0, "           ");
    mvprintw(3, 0, "-AX:[%d]", pcb->AX);
    mvprintw(3, 10, "           ");
    mvprintw(3, 10, "-PC:[%d]", pcb->PC - 1); //
    mvprintw(4, 0, "          ");
    mvprintw(4, 0, "-BX:[%d]", pcb->BX);
    mvprintw(5, 0, "          ");
    mvprintw(5, 0, "-CX:[%d]", pcb->CX);
    mvprintw(6, 0, "          ");
    mvprintw(6, 0, "-DX:[%d]", pcb->DX);
    mvprintw(4, 10, "          ");
    mvprintw(4, 10, "-PID:[%d] ", pcb->pid);
    mvprintw(9, 0, "              ");
    mvprintw(9, 0, "IR:[%s]", pcb->IR);
    /////////////////////////////////////////////////////////
    mvprintw(5, 10, "           ");
    mvprintw(5, 10, "-UDI:[%d]", pcb->UID);
    mvprintw(6, 10, "          ");
    mvprintw(6, 10, "-KCPU:[%d] ", pcb->KCPU);
    mvprintw(7, 0, "          ");
    mvprintw(7, 0, "-KCPUxU:[%d]", pcb->KCPUxU);
    mvprintw(8, 0, "          ");
    mvprintw(8, 0, "-P:[%d]", pcb->P);
    mvprintw(12, 0, "-------------------------MENSAJES-");
    mvprintw(14, 0, "-");
    mvprintw(15, 0, "-");
    /////////////////////////////////////////////////////////TMS

    if (pcb->pid == -1)
    {
        mvprintw(8, 10, "-PAG:[0]");
        for (int i = 2; i < 12; i++)
        {
            mvprintw(i, 23, "                                       ");
        }
    }
    else
    {
        mvprintw(8, 10, "-PAG:[%d]", bloques(pcb->programa));

        mvprintw(2, 23, "-MR--------------TMP--------MV----------");

        for (int i = 3; i < 12; i++)
        {
            if (marcos < bloques(pcb->programa))
            {

                if (pcb->familia != 0)
                {
                    mvprintw(i, 40, "-MR %x     MV %x", obtenerMarco(pcb->familia, marcos), arreglo[marcos].orden);
                }
                else
                {
                    mvprintw(i, 40, "-MR %x     MV %x", obtenerMarco(pcb->pid, marcos), arreglo[marcos].orden);
                }
            }
            else
            {
                mvprintw(i, 40, "-MR %d   MV 0 ", arreglo[marcos].tmp);
            }
            mvprintw(i + 1, 40, "                        ");
            marcos++;
        }
    }

    refresh(); // Actualiza la pantalla
}

void mensage(char *mensaje)
{
    mvprintw(0, 2, "$>                                                  ");
    mvprintw(0, 2, "$> %s", mensaje);
    refresh(); // Actualiza la pantalla
}

void strUpper(char *str)
{
    while (*str)
    {
        *str = toupper(*str);
        str++;
    }
}

void actu_U(struct PCB **lista, int us)
{

    struct PCB *actual = *lista;
    while (actual != NULL)
    {
        if (actual->UID == us)
        {
            actual->KCPUxU = usu[us];
        }

        actual = actual->sig;
    }
}

int buscarProgramaPorNombre(struct PCB *lista, struct PCB *PCBejecucion, struct PCB *PCbnuevo)
{
    struct PCB *actual = lista;

    if (PCBejecucion != NULL)
    {

        if (strcmp(PCbnuevo->fileName, PCBejecucion->fileName) == 0 && PCbnuevo->UID == PCBejecucion->UID)
        {
            return PCBejecucion->pid;
        }
    }

    // Recorrer la lista de PCBs
    while (actual != NULL)
    {
        if (strcmp(actual->fileName, PCbnuevo->fileName) == 0 && PCbnuevo->UID == actual->UID)
        {
            return actual->pid; // Retornar el pid si se encuentra coincidencia
        }
        actual = actual->sig; // Avanzar al siguiente nodo
    }

    return 0; // Retornar -1 si no se encuentra coincidencia
}

int comandos(char *mensaje, struct PCB **lista, struct PCB **pcb, struct PCB **terminados, struct PCB **LISTOS)
{
    char a[80] = {0}, b[80] = {0}, c[5] = {0};
    sscanf(mensaje, "%s %s %s", a, b, c);
    FILE *archivo = NULL; // Archivo abierto

    strUpper(a);

    if (strcmp(a, "LOAD") == 0)
    {
        if ((strcmp(b, "\n") == 0) || !(isdigit(c[0])))
        {
            mvprintw(13, 2, "                                                            ");
            mvprintw(13, 2, "FORMATO INCORRECTO LOAD archivo UsuarioID  ");
            refresh();
            return 0;
        }
        int UID = atoi(c);
        mvprintw(13, 2, "                                                         ");
        mvprintw(13, 2, "se selecciono el archivo %s con usuario %d", b, UID);
        refresh();

        if (archivo = fopen(b, "r"))
        {
            struct PCB *PCB = crear_pcb();
            strcpy(PCB->fileName, b);
            strcpy(PCB->IR, "----------");
            PCB->programa = archivo;
            PCB->UID = UID;
            PCB->P = PBase;              ///
            PCB->KCPUxU = usu[PCB->UID]; // hacer el pcb nuevo
            PCB->marcos = bloques(archivo);

            int parentesco = buscarProgramaPorNombre(*lista, *pcb, PCB);
            int resultado = 1;

            if (parentesco > 0)
            {
                // printf("hay familia  %d ------%d ...",parentesco,PCB->pid);
                PCB->familia = parentesco;
            }
            else
            {
                resultado = escribirParteEnArchivoBinario(PCB->programa, PCB->pid);
            }

            if (resultado == 0)
            {
                insertar_PCB(terminados, PCB);
                mvprintw(13, 2, "                                                ");
                mvprintw(13, 2, "archivo es muy grande %s ", b);
                refresh();
                return 0;
            }
            else if (resultado == 41)
            {
                insertar_PCB(LISTOS, PCB);
                return 1;
            }
            insertar_PCB(lista, PCB);
            Dispacher(lista); //
        }
        else
        {
            mvprintw(13, 2, "                                                ");
            mvprintw(13, 2, "archivo no encontrado %s ", b);
            refresh();
            return 0;
        }

        return 1;
    }
    else if (strcmp(a, "END") == 0)
    {
        mvprintw(13, 2, "                                                                ");
        mvprintw(13, 2, "saliendo........ ");
        refresh();
        getch();
        return -935;
    }
    else if (strcmp(a, "KILL") == 0)
    {
        int auxpid = 0;
        if (isdigit(b[0]))
        {
            auxpid = atoi(b);
        }
        else
        {
            mvprintw(13, 2, "                                                       ");
            mvprintw(13, 2, "FORMATO-> KILL <PID>               ");
            refresh();
            return 0;
        }

        if ((*pcb) == NULL)
        {
            mvprintw(13, 2, "                                                             ");
            mvprintw(13, 2, "no se encontro el pid N             ");
            refresh();
            return 0;
        }

        if ((*pcb)->pid == auxpid)
        {
            struct PCB *eliminar = *pcb;

            int familia = hijo(*lista, eliminar->pid);

            if (familia == 0)
            {
                liberarMarcosPorPID(eliminar->pid);
            }
            else
            {
                heredar(eliminar->pid, familia, lista, pcb);
            }

            insertar_PCB(terminados, eliminar);
            *pcb = NULL;
            Dispacher(lista); //
        }
        else
        {
            if (*lista == NULL)
            {
                mvprintw(13, 2, "                                                             ");
                mvprintw(13, 2, "lista vacia                        ");
                refresh();
                return 0;
            }

            struct PCB *eliminar = extraer_PCB(lista, auxpid);
            if (eliminar == NULL)
            {
                mvprintw(13, 2, "                                                                    ");
                mvprintw(13, 2, "no se encontro el pid              ");
            }
            else
            {
                ver_lista(*lista);

                int familia = hijo(*lista, eliminar->pid);

                if ((*pcb)->familia == eliminar->pid)
                {
                    heredar(eliminar->pid, (*pcb)->pid, lista, pcb);
                }
                else if (familia == 0)
                {
                    liberarMarcosPorPID(eliminar->pid);
                }
                else
                {
                    heredar(eliminar->pid, familia, lista, pcb);
                }

                insertar_PCB(terminados, eliminar);
                Dispacher(lista);
            }
        }
    }
    else
    {
        mvprintw(13, 2, "                                                                                ");
        mvprintw(13, 2, "opcion no valida ");
        refresh();
        return 1;
    }
}

int shell(char *mensaje, int *j)
{
    int ch = 0;
    if (kbhit())
    {
        do
        {
            ch = getch(); // Obtener la tecla presionada
        } while (kbhit());
        mensaje[(*j)++] = ch;
        int tr = *j;
        if (tr > 79)
        { // no deja que el mensage supere los 80 caraterers no tiene mensage
            mensaje[0] = '\0';
            *j = 0;
        }
        mensaje[tr] = '\0';
    }
    return ch;
}

void mostrar_mensaje(const char *mensaje)
{
    mvprintw(14, 2, "                                                      ");
    mvprintw(14, 2, "%s", mensaje);
    mvprintw(0, 2, "$>");
    refresh();
}

// Función para obtener el valor de un registro o número
int obtener_valor(const char *registro_fuente, struct PCB *pcb)
{
    if (isdigit(registro_fuente[0]))
    {
        return atoi(registro_fuente);
    }

    // Arreglo de punteros a los registros para simplificar el acceso
    int *registros[] = {&pcb->AX, &pcb->BX, &pcb->CX, &pcb->DX};
    const char *nombres_registros[] = {"AX", "BX", "CX", "DX"};

    for (int i = 0; i < 4; i++)
    {
        if (strcmp(registro_fuente, nombres_registros[i]) == 0)
        {
            return *registros[i];
        }
    }

    mostrar_mensaje("registro no valido");
    return -1; // Valor inválido
}

int procesar_instruccion(char *instruccion, struct PCB *pcb)
{
    char operacion[5];
    char registro[5];
    char registro_fuente[5];

    // Parsear la instrucción
    sscanf(instruccion, "%s %s %s", operacion, registro, registro_fuente);

    // Convertir a mayúsculas
    strUpper(operacion);
    strUpper(registro);
    strUpper(registro_fuente);

    // Manejar la operación END
    if (strcmp(operacion, "END") == 0)
    {
        mostrar_mensaje("se encontro un end");
        return 3; // Código de finalización
    }

    // Validar el registro destino
    if (isdigit(registro[0]))
    {
        mostrar_mensaje("registro no valido");
        return 1; // Código de error
    }

    // Obtener el valor del registro fuente
    int valor = obtener_valor(registro_fuente, pcb);
    if (valor == -1)
    {
        return 1; // Código de error
    }

    // Arreglo de punteros a los registros para simplificar el acceso
    int *registros[] = {&pcb->AX, &pcb->BX, &pcb->CX, &pcb->DX};
    const char *nombres_registros[] = {"AX", "BX", "CX", "DX"};

    // Buscar el registro destino
    int *registro_destino = NULL;
    for (int i = 0; i < 4; i++)
    {
        if (strcmp(registro, nombres_registros[i]) == 0)
        {
            registro_destino = registros[i];
            break;
        }
    }

    if (registro_destino == NULL)
    {
        mostrar_mensaje("registro no valido");
        return 1; // Código de error
    }

    // Realizar la operación correspondiente
    if (strcmp(operacion, "MOV") == 0)
    {
        *registro_destino = valor;
    }
    else if (strcmp(operacion, "ADD") == 0)
    {
        *registro_destino += valor;
    }
    else if (strcmp(operacion, "SUB") == 0)
    {
        *registro_destino -= valor;
    }
    else if (strcmp(operacion, "MUL") == 0)
    {
        *registro_destino *= valor;
    }
    else if (strcmp(operacion, "DIV") == 0)
    {
        if (valor == 0)
        {
            mostrar_mensaje("division por cero no valido");
            return 1; // Código de error
        }
        *registro_destino /= valor;
    }
    else if (strcmp(operacion, "INC") == 0)
    {
        (*registro_destino)++;
    }
    else if (strcmp(operacion, "DEC") == 0)
    {
        (*registro_destino)--;
    }
    else
    {
        mostrar_mensaje("operacion no valida");
        return 1; // Código de error
    }

    return 0; // Éxito
}

void ver_swap(int long posicion)
{

    int long copia = posicion;
    int long contador = posicion + 15;
    int aux2 = 1;
    char *linea = NULL;

    mvprintw(37, 32, "-------SWAP---------------------------------------------------------");
    while (aux2 <= 5)
    {
        int aux = 38;
        int long hexa = posicion;
        posicion = copia;
        contador = posicion + 15;

        while (posicion <= contador)
        {
            // strcpy(linea, leer_linea_cadena(posicion * 32));
            char *linea = leer_linea_cadena(posicion * 32);
            int pos = 12 + (15 * aux2);
            if (linea)
            {
                mvprintw(aux, pos, "                   ");
                mvprintw(aux, pos, "%lx-> %s ", hexa, linea);
                // mvprintw(aux + 1, 35, "                          ");
                mvprintw(0, 2, "$> ");
                posicion++;
                aux++;
            }
            else if (hexa >= 65536)
            {
                {
                    mvprintw(aux, pos, "XXX->--------------- ");
                    mvprintw(0, 2, "$> ");
                    posicion++;
                    aux++;
                }
            }
            else
            {
                mvprintw(aux, pos, "%ld->///////////// ", hexa);
                mvprintw(0, 2, "$> ");
                posicion++;
                aux++;
            }
            hexa++;
        }
        aux2++;
        copia += 16;
    }
    refresh();
}

void mostrar_cap_ram()
{
    mvprintw(37, 170, "--RAM: Estado de Marcos -"); // Título

    int pos_y = 38;  // Fila inicial para mostrar los marcos
    int pos_x = 172; // Columna inicial

    // Recorrer cada marco en la RAM
    for (int j = 0; j < 8; j++)
    {
        if (ram_arr[j].lleno)
        {
            // Marco lleno: mostrar PID y página
            mvprintw(pos_y + j, pos_x, "- %d: PID %d  N-marco %d", j + 1, ram_arr[j].id, ram_arr[j].num_marco);
        }
        else
        {
            // Marco vacío
            mvprintw(pos_y + j, pos_x, "- %d: Libre", j + 1);
        }
    }

    mvprintw(0, 2, "$> "); // Restaurar el prompt
    refresh();             // Actualizar la pantalla
}

/// ////

void ver_ram()
{
    mvprintw(37, 110, "------ Contenido de RAM ------"); // Título

    int pos_y = 38;  // Fila inicial para mostrar el contenido
    int pos_x = 110; // Columna inicial

    int inicio = ver_ram_con * 512;

    // Recorrer la RAM en bloques de 32 bytes
    for (int i = 0; i < 16; i++) // Mostramos 16 líneas (16 * 32 = 512 bytes)
    {
        mvprintw(pos_y, pos_x, "%d %03d:", ver_ram_con, i);

        mvprintw(pos_y, pos_x + 15, "|");
        for (int j = 0; j < 32; j++)
        {
            int index = inicio + ((i * 32) + j); // Calcular posición en la RAM
            char c = Ram[index];

            if (c >= 32 && c <= 126)
            {
                mvprintw(pos_y, pos_x + 11 + j, "%c", c);
            }
            else
            {
                mvprintw(pos_y, pos_x + 11 + j, "."); // Caracteres no imprimibles
            }
        }
        mvprintw(pos_y, pos_x + 49, "|"); // Cierre de la línea

        pos_y++; // Mover a la siguiente fila
    }

    mvprintw(0, 2, "$> "); // Restaurar el prompt
    refresh();             // Actualizar la pantalla
}

int leer_linea_archivoBin(struct PCB *pcb, struct PCB **lista)
{
    char *cadena = NULL;
    int MARCO;
    int pid_proseso;

    if (pcb->familia != 0) // verifica si tiene familirares si si lo hace con el de la familia sino lo hace con el propio
    {
        MARCO = obtenerMarco(pcb->familia, pcb->PC / 16);
        pid_proseso = pcb->familia;
    }
    else
    {
        MARCO = obtenerMarco(pcb->pid, pcb->PC / 16);
        pid_proseso = pcb->pid;
    }

    long linea = arreglo[MARCO].pri + pcb->PC % 16; // liena de donde lee el archivo  ,tambien la voy a usar para leer hacer el fallo de pag
    //

    mvprintw(15, 2, "                                                      ");
    mvprintw(15, 2, "pid ->%d MARCO->%d", pid_proseso, MARCO);
    mvprintw(0, 2, "$>");
    refresh();

    if (verificar_fallo_pagina(pid_proseso, MARCO) != 0)
    {
        copiar_marco_completo_swap_a_ram(pid_proseso, MARCO);
    }

    int end = 0;
    // actualizar
    cadena = leer_linea_cadena(linea * 32);

    strcpy(pcb->IR, cadena);
    end = procesar_instruccion(cadena, pcb);
    pcb->PC++;
    pcb->KCPU += 12;
    usu[pcb->UID] += 12;
    pcb->KCPUxU = usu[pcb->UID];
    actu_U(lista, pcb->UID);

    if (end == 1)
    {
        return 0;
    }
    if (end == 3)
    {
        return 0;
    }

    return 1;
}

int verificar_fallo_pagina(int pid, int pagina)
{
    for (int i = 0; i < 8; i++)
    {
        // VERIFICA SI EN LA RAM ESTA LA PAGINA QUE OCUP
        if (ram_arr[i].id == pid && pagina == ram_arr[i].num_marco)
        {
            return 0; // No hay fallo de página
        }
    }
    return 1; // Hay fallo de página
}

int encontrar_marco_libre_ram()
{
    for (int i = 0; i < 8; i++)
    {
        if (ram_arr[i].lleno == 0)
        {
            return i; // Devuelve el índice del primer marco libre
        }
    }
    return -1; // No hay marcos libres
}

///
int copiar_marco_completo_swap_a_ram(int pid, int marco_swap)
{
    // Verificar si el marco en swap es válido
    if (marco_swap < 0 || marco_swap >= 4096 || arreglo[marco_swap].pri == -1)
    {
        return -1; // Marco inválido
    }

    // Encontrar un marco libre en la RAM
    int marco_ram = encontrar_marco_libre_ram();
    if (marco_ram == -1)
    {
        planificador_fifo(); // Llamar al planificador FIFO si no hay espacio
        marco_ram = encontrar_marco_libre_ram();
        if (marco_ram == -1)
        {
            return -1; // No hay espacio en RAM
        }
    }
    // Guardar los marcos donde se copian las líneas
    char buffer[32];

    for (int linea = 0; linea < 16; linea++)
    {
        // Calcular la posición en el archivo swap
        long posicion_swap = arreglo[marco_swap].pri + (linea * 32); // Cada línea ocupa 32 bytes

        // Mover el puntero del archivo a la posición deseada
        fseek(salida, posicion_swap, SEEK_SET);

        // Leer 32 bytes directamente en el buffer
        size_t bytes_leidos = fread(buffer, sizeof(char), 32, salida);

        // Asegurar que el buffer siempre tenga 32 caracteres
        if (bytes_leidos < 32)
        {
            memset(buffer + bytes_leidos, '0', 32 - bytes_leidos); 
        }

        // Copiar el buffer directamente a la RAM
        memcpy(&Ram[marco_ram * 512 + linea * 32], buffer, 32);
    }
    
    ram_arr[marco_ram].lleno = 1;
    ram_arr[marco_ram].id = pid;
    ram_arr[marco_ram].num_marco = marco_swap;

    return 0; // Retorna 0 si todas las líneas fueron copiadas correctamente
}

int planificador_fifo()
{
    static int cuenta = 0; // Variable estática para llevar el índice del marco a reemplazar

    // Reiniciar el marco actual en la RAM
    ram_arr[cuenta].lleno = 0; // Marcar el marco como libre
    ram_arr[cuenta].id = 0;    // Reiniciar el ID del proceso

    // Mover al siguiente marco (comportamiento circular)
    cuenta++;
    if (cuenta == 8)
    {
        cuenta = 0; // Reiniciar el contador si alcanza el número de marcos
    }
    return cuenta;
}
