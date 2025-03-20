# simulador-de-cpu
# simulador-de-cpu


Descripción del Código de lista .c

El código implementa una lista enlazada doble para gestionar estructuras PCB (Process Control Block). Cada PCB representa un proceso y contiene información relevante como el ID del proceso (pid), registros de CPU (AX, BX, CX, DX), punteros a instrucciones (IR), nombre de archivo (fileName), y otros campos adicionales relacionados con la gestión de procesos.

Las funciones principales permiten:

    Crear un nuevo PCB.

    Insertar un PCB en la lista.

    Extraer un PCB de la lista según su pid.

    Eliminar un PCB y liberar su memoria.

    Imprimir la lista de PCB.

    Vaciar la lista completa y liberar toda la memoria asociada.

Estructura del Código
1. Estructura PCB

La estructura PCB (definida en lista.h) contiene los siguientes campos:

    pid: Identificador único del proceso.

    sig y ant: Punteros al siguiente y anterior nodo en la lista enlazada.

    AX, BX, CX, DX: Registros de CPU simulados.

    PC: Contador de programa.

    IR: Puntero a una cadena de caracteres que representa la instrucción actual.

    fileName: Nombre del archivo asociado al proceso.

    programa: Puntero a un archivo (no utilizado en este código).

    UID, KCPU, KCPUxU, P, familia: Campos adicionales para gestión avanzada de procesos.

2. Funciones Principales
crear_PCB(int pid)

    Descripción: Crea y devuelve un nuevo nodo PCB con el pid especificado.

    Uso: Inicializa un nuevo proceso con valores predeterminados y reserva memoria para los campos dinámicos (IR y fileName).

    Parámetros:

        pid: Identificador del proceso.

    Retorno: Puntero al nuevo PCB creado.

insertar_PCB(struct PCB **inicio, struct PCB *nodo)

    Descripción: Inserta un PCB en la lista enlazada.

    Uso: Añade un nodo al final de la lista.

    Parámetros:

        inicio: Puntero al inicio de la lista.

        nodo: Puntero al PCB que se va a insertar.

extraer_PCB(struct PCB **inicio, int pid)

    Descripción: Extrae un PCB de la lista según su pid.

    Uso: Busca y desconecta un nodo de la lista sin liberar su memoria.

    Parámetros:

        inicio: Puntero al inicio de la lista.

        pid: Identificador del proceso a extraer.

    Retorno: Puntero al PCB extraído, o NULL si no se encuentra.

eliminar_PCB(struct PCB *nodo)

    Descripción: Libera la memoria asociada a un PCB.

    Uso: Elimina un nodo y libera sus recursos dinámicos (IR, fileName y el propio nodo).

    Parámetros:

        nodo: Puntero al PCB que se va a eliminar.

imprimir_lista(struct PCB *inicio)

    Descripción: Imprime la lista de PCB en la consola.

    Uso: Muestra los pid de los procesos en la lista.

    Parámetros:

        inicio: Puntero al inicio de la lista.

vaciar_lista(struct PCB **cola)

    Descripción: Vacía la lista completa y libera toda la memoria asociada.

    Uso: Elimina todos los nodos de la lista y libera sus recursos.

    Parámetros:

        cola: Puntero al inicio de la lista.

Uso de la Librería

El código utiliza las siguientes librerías:

    #include "lista.h": Incluye la definición de la estructura PCB y posiblemente otras funciones relacionadas con la lista.

    #include <stdlib.h>: Proporciona funciones para gestión de memoria dinámica (malloc, free).



    ///////////////////////////////////////////////////////////////////////////////

    Descripción del Código

El código implementa un sistema de gestión de memoria y archivos que permite:

    Encontrar bloques de memoria vacíos en un arreglo predefinido.

    Escribir partes de un archivo en bloques de memoria.

    Leer líneas de un archivo binario y procesar su contenido.

    Contar líneas en un archivo y calcular el número de bloques necesarios para almacenarlo.

    Gestionar la memoria RAM simulada mediante un arreglo.

El código utiliza estructuras de datos como arreglo y ram_arr para simular la memoria y el almacenamiento de bloques de datos.
Estructuras y Variables Globales
Estructuras

    struct tmp:

        Representa un bloque de memoria en el arreglo.

        Campos:

            lleno: Indica si el bloque está ocupado.

            id: Identificador asociado al bloque.

            orden: Orden del bloque dentro de un archivo.

    struct ram:

        Representa un bloque de memoria RAM.

        Campos: (No se especifican en el código proporcionado, pero se asume que almacena datos).

Variables Globales

    FILE *salida: Puntero al archivo binario de salida.

    struct tmp arreglo[TAMANO_ARREGLO]: Arreglo que simula bloques de memoria.

    struct ram ram_arr[8]: Arreglo que simula la memoria RAM.

Funciones Principales
int encontrarBloqueVacio()

    Descripción: Encuentra el primer bloque vacío en el arreglo.

    Uso: Devuelve el índice del primer bloque no ocupado.

    Retorno: Índice del bloque vacío, o -1 si no hay bloques disponibles.

int escribirParteEnArchivoBinario(FILE *entrada, int id)

    Descripción: Escribe partes de un archivo en bloques de memoria.

    Uso: Divide el archivo en bloques y los almacena en el arreglo.

    Parámetros:

        entrada: Archivo de entrada a dividir.

        id: Identificador asociado al archivo.

    Retorno:

        1: Operación exitosa.

        0: Error (archivo demasiado grande o sin bloques disponibles).

        41: No hay suficientes espacios en el arreglo.

void escribir(FILE *entrada, int numBloque)

    Descripción: Escribe un bloque de datos en el archivo binario de salida.

    Uso: Almacena 16 líneas del archivo de entrada en un bloque específico.

    Parámetros:

        entrada: Archivo de entrada.

        numBloque: Índice del bloque donde se escribirá.

int espacios_restantes()

    Descripción: Cuenta los bloques vacíos en el arreglo.

    Uso: Devuelve el número de bloques disponibles.

    Retorno: Número de bloques vacíos.

long contarLineas(FILE *archivo)

    Descripción: Cuenta el número de líneas en un archivo.

    Uso: Calcula el tamaño del archivo en líneas.

    Retorno: Número de líneas, o -1 si el archivo es NULL.

int bloques(FILE *archivo)

    Descripción: Calcula el número de bloques necesarios para almacenar un archivo.

    Uso: Divide el número de líneas entre 16 (tamaño del bloque).

    Retorno: Número de bloques necesarios.

int leer_linea_archivoB(long int posicion)

    Descripción: Lee una línea de un archivo binario y la procesa.

    Uso: Busca comandos en el archivo y los ejecuta.

    Parámetros:

        posicion: Posición en el archivo donde comenzar la lectura.

    Retorno:

        1: Línea leída y procesada.

        0: Fin del archivo o error.

char *leer_linea_cadena(long int posicion)

    Descripción: Lee una línea de un archivo binario y la devuelve como cadena.

    Uso: Obtiene una línea específica del archivo.

    Parámetros:

        posicion: Posición en el archivo donde comenzar la lectura.

    Retorno: Cadena leída, o NULL si hay un error.

Uso de Librerías

    #include "lista.h": Incluye definiciones de estructuras y funciones relacionadas con la lista.

    #include "swap.h": Incluye definiciones relacionadas con la gestión de memoria.

    #include <stdlib.h>: Proporciona funciones para gestión de memoria dinámica.

    #include <string.h>: Proporciona funciones para manipulación de cadenas.

    #include <time.h>: Proporciona funciones para manejo de tiempo (no se usa en el código proporcionado).
    # simulador-de-cpu
