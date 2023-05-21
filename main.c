#include "hashMap.h"
#include "list.h"
#include "heap.h"
#include "stack.h"
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 100
#define BARRAS printf("------------------------------------------\n")

//INTEGRANTES: SAMIRA BECERRA Y FRANCISCA SILVA 
// hashMap, cola de prioridad, pila, lista
typedef struct List {
  char nombre[31];
  int prioridad;
} List;

typedef struct {
  int prioridad;
  char nombre[31];
  List *T_precedente;
  bool visitada;
} Tarea;

typedef struct {
  int accion; 
  Tarea *tareaAfectada;
}pilaAcc;

// nombre tarea y su prioridad
// tareas por hacer
void agregarTarea(HashMap *mapaTareas, List *lista) {
  char nombre[31];
  int prioridad;
  pilaAcc * ultaccion = (pilaAcc *) malloc(sizeof(pilaAcc));
  ultaccion->accion = 1; 
  printf("Ingrese el nombre de la tarea a agregar\n");
  getchar();
  scanf("%30[^\n]", nombre);
  printf("Ingrese la prioridad de la tarea\n");
  getchar();
  scanf("%d", &prioridad);
  Tarea *aux = (Tarea *)  malloc(sizeof(Tarea));
  strcpy(aux->nombre, nombre);
  aux->prioridad = prioridad;
  aux->T_precedente = createList();
  printf("%s, %d\n", aux->nombre, aux->prioridad); // Comprobar
  insertMap(mapaTareas, aux->nombre, aux); // Se inserta la tarea en el mapa
  printf("La tarea se ha agregado con éxito\n\n");

  ultaccion->tareaAfectada = (Tarea *) malloc(sizeof(Tarea));
  strcpy(ultaccion->tareaAfectada->nombre, aux->nombre);
  pushFront(lista, ultaccion);
  return;
}

void *busqueda(List *listaP, char *nombre){
  char* aux = firstList(listaP);
  while(aux != NULL){
    if(strcmp(aux, nombre) == 0){
      return aux;
    }
    aux = nextList(listaP);
  }
  return NULL;
}

// La tarea precedente es la segunda que se ingresa!!
void estabPrecedencia(HashMap *mapa, List *listaacciones) {
  pilaAcc *des = (pilaAcc *) malloc(sizeof(pilaAcc));
  des->tareaAfectada = (Tarea *) malloc(sizeof(Tarea));
  char tarea1[31], tarea2[31];
  printf("Ingrese los nombres de las tareas\n");
  getchar();
  scanf("%30[^\n]", tarea1);
  getchar();
  scanf("%30[^\n]", tarea2);

  // Verificar si las tareas existen
  Pair *tarea1P = searchMap(mapa, tarea1);
  Pair *tarea2P = searchMap(mapa, tarea2);

  if(tarea1P == NULL || tarea2P == NULL) {
    // Imprimir mensaje de error
    printf("La tarea 1 y/o la tarea 2 ingresada no existe(n)\n\n");
    return;
  } else {
    // Agregar tarea 2 a la lista de adyacencia (lista de tareas precedentes) de la tarea 1
    Tarea *tarea = (Tarea *)malloc(sizeof(Tarea));
    tarea = tarea1P->value;
    if(tarea->T_precedente == NULL){
      tarea->T_precedente = createList();
    }
    
    strcpy(des->tareaAfectada->nombre, tarea->nombre);
    des->tareaAfectada->T_precedente = tarea->T_precedente;
    des->accion = 2;
    pushFront(listaacciones, des);
 
    if(firstList(tarea->T_precedente) == NULL){
      pushFront(tarea->T_precedente, tarea2P->key);
      // La tarea es insertada en la lista de tareas precedentes
    }
    else{
      if(busqueda(tarea->T_precedente, tarea2P->key) == NULL){
        pushFront(tarea->T_precedente, tarea2P->key); 
        /* Si la tarea no está en la lista de tareas
        precedentes de la otra tarea, se inserta*/
      }
      else{
        printf("\nLa tarea %s ya es precedente de la tarea %s\n", tarea2P->key, tarea->nombre);
        return;
      }
    }
    printf("\nSe ha establecido precedencia\n");
    printf("Tarea precedente: %s\n", tarea2P->key);
    return;
  } 
}

int compararTareas(void *tarea1, void *tarea2) {
  Tarea *t1 = (Tarea *)tarea1;
  Tarea *t2 = (Tarea *)tarea2;
  //Ordenar por prioridad 
  if(t1->prioridad > t2->prioridad)
    return -1;
  else if (t1->prioridad < t2->prioridad)
    return 1;
  else
    return 0;
}

void mostrarT(HashMap *mapa) {
  printf("Tareas por hacer, ordenadas por prioridad y precedencia\n");
  Pair* aux = firstMap(mapa);
  Heap *h = createHeap(compararTareas);
  
  if(aux == NULL){
    printf("No existen tareas\n"); 
    return; 
  }
  int cont = 1; 
  //char *aux2;
  List * listaEspera = createList(); 
  List * ListaRealizadas = createList();

  while(aux != NULL){
    Tarea *tarea = aux->value;
    if(firstList(tarea->T_precedente) != NULL){
      pushBack(listaEspera, tarea);
    }
    else{
      //pushBack(ListaRealizadas, tarea);
      heap_push(h, tarea, -(tarea->prioridad)); // se pasan al montículo
    }
    aux = nextMap(mapa);
  }
//Hasta aquí funciona bien con archivo 
  
  while(heapSize(h) != 0 ){
    while(heapSize(h) != 0){ 
      Tarea *tarea = heap_top(h); heap_pop(h); //se obtiene del montículo
      List *precedentes = tarea->T_precedente; 
      if((firstList(precedentes) != NULL)){
        char *aux = firstList(precedentes);
        bool todasT = true;
        while(aux != NULL){
          char *precedente2 = aux;
          if((busqueda(ListaRealizadas, precedente2) == NULL)){
            todasT = false; 
            break; 
          }
          aux = nextList(precedentes);
        }
        if(todasT == false){
          pushBack(listaEspera, tarea);
        }
        else{
          pushBack(ListaRealizadas, tarea->nombre);
        }
      }
      else{
        pushBack(ListaRealizadas, tarea->nombre);
      }
      
      if(cont < 10)
        printf("%i.  Tarea: %s (Prioridad: %d) ", cont,  tarea->nombre, tarea->prioridad);
      else{
        printf("%i. Tarea: %s (Prioridad: %d) ", cont,  tarea->nombre, tarea->prioridad);
      }
      
      if((tarea->T_precedente) != NULL) {
        printf("- Precedente:");
        char *node = firstList(tarea->T_precedente);
        while(node != NULL) {
          printf(" %s", node);
          node = nextList(tarea->T_precedente);
        }
      }
      else {
        printf(" No hay tareas precendentes\n");
      }
      printf("\n");
      aux = nextMap(mapa); 
      cont++; 
   }
    
    Tarea *tareaEnEspera = firstList(listaEspera);
    while(tareaEnEspera != NULL){
      heap_push(h, tareaEnEspera, -(tareaEnEspera->prioridad));
      popCurrent(listaEspera);
      tareaEnEspera = nextList(listaEspera);
    }
  }
  printf("\n");
  return;
}    

/*IDEA: si la tarea ya fue completada, es decir, que 
no apunta a ningún nodo o no tiene tareas precedentes, 
se elimina del mapa y se inserta en una lista */
void marcarTarea(HashMap *mapa, List *lista){ 
  char tareaM[31]; 
  printf("Ingrese el nombre de la tarea a marcar completada\n");
  getchar(); 
  scanf("%30[^\n]", tareaM); 
  printf("tarea a marcar-> %s\n", tareaM);
  
  Pair *auxPair = searchMap(mapa,tareaM);
  //CASO BASE: LA TAREA QUE SE DESEA MARCAR NO EXISTE 
  if(auxPair == NULL){
    printf("La tarea que desea marcar no se encuentra en la lista\n");
    return;
  }
  
  Tarea *aux = auxPair->value;
  pilaAcc *ultaccion = (pilaAcc *) malloc(sizeof(pilaAcc));
  ultaccion->accion = 3;
  ultaccion->tareaAfectada = (Tarea *) malloc(sizeof(Tarea));
  
  //Se copia el estado de la tarea antes de que sea marcada
  strcpy(ultaccion->tareaAfectada->nombre, aux->nombre); 
  ultaccion->tareaAfectada->prioridad = aux->prioridad;
  ultaccion->tareaAfectada->T_precedente = aux->T_precedente;
  pushFront(lista, ultaccion);
  
  /*SI LA TAREA NO TIENE PRECEDENTES SE DEBE MARCAR COMO COMPLETADA, SE ELIMINA DEL MAPA Y SE DEBE BUSCAR SI EXTÁ EN LA LISTA DE PRECEDENCIA DE OTRAS TAREAS Y ELIMINARLA*/
  if(firstList(aux->T_precedente) == NULL){
    aux->visitada = true; 
    Pair *auxLtarea = firstMap(mapa);
    while(auxLtarea != NULL){
      Tarea *auxTarea = auxLtarea->value;
      if(busqueda(auxTarea->T_precedente, tareaM) != NULL){
        popCurrent((auxTarea->T_precedente)); 
      }
      auxLtarea = nextMap(mapa);
    }
    eraseMap(mapa, tareaM);  
    printf("La tarea ha sido eliminada de la lista de tareas\n");
    return; 
  }
  else{ 
    /*EN ESTE CASO LA TAREA QUE SE DESEA ELIMINAR TIENE PRECEDENTES, POR LO QUE SE IMPRIME UN MENSAJE. LUEGO, EN EL CASO DE CONFIRMAR LA ELIMINACIÓN, SE DEBE ELIMINAR LA TAREA DEL MAPA Y JUNTO A ESO SE DEBE RECORRER ENTRE LAS OTRAS TAREAS Y VERIFICAR SI EXISTE EN LA LISTA DE PRECEDENCIA DE OTRAS TAREAS. DE SER ASÍ, SE ELIMINA DE DICHAS LISTAS.*/
    
    int respuesta; 
    printf("¿ESTÁ SEGURO QUE DESEA ELIMINAR LA TAREA?\n");
    printf("Ingrese (1) si la respuesta es SÍ\n");
    printf("Ingrese (2) si la respuesta es NO\n");
    scanf("%d", &respuesta); 

    //SI LA RESPUESTA INGRESADA ES 1, SE REALIZAN LAS ACCIONES 
    //ANTERIORMENTE DESCRITAS EN EL CASO DE QUE LA TAREA PRESENTE PRECEDENCIA
    if(respuesta == 1){
      if(firstList(aux->T_precedente) != NULL){
        aux->visitada = true; 
        Pair *auxLtarea = firstMap(mapa);
        while(auxLtarea != NULL){
          Tarea *auxTarea = auxLtarea->value;
          if(busqueda(auxTarea->T_precedente, tareaM) != NULL){
            popCurrent((auxTarea->T_precedente)); 
          }
          auxLtarea = nextMap(mapa);
        }
        eraseMap(mapa, tareaM);  
        printf("La tarea ha sido eliminada de la lista de tareas\n");
        return;
      }
    }
    //SI LA RESPUESTA ES 2, NO SE REALIZA NINGÚN CAMBIO DENTRO DE MARCAR TAREA
    else if(respuesta == 2){
      printf("La tarea no ha sido eliminada\n");
      return; 
    }
    else{
      //EN CASO DE QUE LA RESPUESTA SEA DISTINTA DE 1 Y 2, 
      //SE IMPRIME UN MENSAJE DE ERROR
      printf("Opción ingresada no es correcta");
      return; 
    }
  }
  return;
} 

//función 7
void mostrarTareas(HashMap* mapa){
  printf("Tareas por hacer sin orden\n\n");
  Pair* aux = firstMap(mapa); 
  if(aux == NULL){
    printf("No existen tareas\n"); 
    return; 
  }
  int cont = 1;
  while(aux != NULL){
    Tarea *tarea = tarea = aux->value;
    char *aux2;
    aux2 = firstList(tarea->T_precedente);
    if(cont < 10)
      printf("%i.  Tarea: %s (Prioridad: %d)  ", cont,  tarea->nombre, tarea->prioridad);
    else{
       printf("%i. Tarea: %s (Prioridad: %d)  ", cont,  tarea->nombre, tarea->prioridad);
    }
    
    if(aux2 == NULL){
      printf(" \n");
    }
    else{
      printf(" Precedente:");
      while(aux2 != NULL){
        printf(" %s", aux2);
        aux2 = nextList(tarea->T_precedente);
      }
      printf("\n");
    }
    aux = nextMap(mapa);
    cont++;
  } 
  printf("\n");
  return;
}

/* Deshacer acciones (funciones 1, 2, 4 y 8),
si no se ha realizado ninguna acción, se muestra un mensaje*/
void deshacerAcc(HashMap *mapa, List* lista) {
  pilaAcc *ultaccion = topStack(lista);
  List *aux = (List *) malloc(sizeof(List));
  
  if(ultaccion == NULL){
    printf("No se ha realizado ninguna acción\n");
    return;
  }
  else{     
    switch(ultaccion->accion){
      case 1:
        //Elimina la última tarea agregada
        eraseMap(mapa, ultaccion->tareaAfectada->nombre);
        printf("(1) Acción desecha\n");
        break;
      case 2:
        //Eliminar acción de establecer precedencia entre tareas
        aux = firstList(ultaccion->tareaAfectada->T_precedente);
        if(busqueda(lista, ultaccion->tareaAfectada->nombre) == NULL){
          while(aux){
            lista = ultaccion->tareaAfectada->T_precedente;
            popCurrent(lista);
            aux = nextList(ultaccion->tareaAfectada->T_precedente);
          }
        }
        printf("(2) Acción desecha\n");
        break;
      case 3:
        //Deshacer la opción de marcar una tarea
        if(searchMap(mapa, ultaccion->tareaAfectada->nombre) == NULL){  
          insertMap(mapa, ultaccion->tareaAfectada->nombre, ultaccion->tareaAfectada);
        }
        printf("(3) Acción desecha\n");
        break;
      case 4:
       printf("Eliminar una tarea\n");
        Pair *aux = firstMap(mapa);
        if(aux == NULL){
          insertMap(mapa, ultaccion->tareaAfectada->nombre, ultaccion->tareaAfectada);
          return;
        }
        else{
          while(aux != NULL){
            insertMap(mapa, ultaccion->tareaAfectada->nombre, ultaccion->tareaAfectada);
            aux = nextMap(mapa);
          }
        }
        printf("(4) Acción desecha\n");
        break;
      case 0:
        return;
    }
  }
  return;
}

//función hecha por el profe
const char *get_csv_field (char * tmp, int k) {
  int open_mark = 0;
  char* ret=(char*) malloc (100*sizeof(char));
  int ini_i=0, i=0;
  int j=0;
  while(tmp[i+1]!='\0'){
    if(tmp[i]== '\"'){
      open_mark = 1-open_mark;
      if(open_mark) ini_i = i+1;
        i++;
        continue;
      }
      if(open_mark || tmp[i]!= ','){
        if(k==j) ret[i-ini_i] = tmp[i];
          i++;
          continue;
      }
      if(tmp[i]== ','){
        if(k==j) {
          ret[i-ini_i] = 0;
          return ret;
        }
        j++; ini_i = i+1;
      }
      i++;
    }
    if(k==j) {
      ret[i-ini_i] = 0;
      return ret;
    }
  return NULL;
}

//función 6: cargar datos
HashMap * cargarDatos(){
  HashMap *mapa;
  char archivocsv[100];
  printf("¿Qué archivo desea importar?\n");
  getchar();
  scanf("%99[^\n]", archivocsv);
  getchar();
  FILE *fp = fopen(archivocsv, "r");
  if(fp == NULL){
    printf("No existe tal archivo\n");
    return NULL; 
  } 
  char linea[1024];
  int indice;
  fgets(linea, 1023, fp);
  mapa = createMap(MAX);
  while(fgets (linea, 1023, fp) != NULL) { // Se lee la linea
    Tarea *datosA = (Tarea *) malloc(sizeof(Tarea));
    indice = 0;
    char *aux = (char *)get_csv_field(linea, indice);
    while(aux != NULL){
      aux = (char*) get_csv_field (linea, indice); // Se obtiene el nombre
      if(datosA->T_precedente == NULL) 
        datosA->T_precedente = createList();
      if(indice == 0){
        strcpy(datosA->nombre, aux); // Se copia el nombre
      }  
      else if(indice == 1){
        datosA->prioridad = atoi(aux); // Se copia la prioridad
      }else{
          pushBack(datosA->T_precedente, aux); 
      }
      indice++;
    }
    insertMap(mapa, datosA->nombre, datosA); // Se inserta en el mapa
  }
  printf("Archivo importado :)\n");
  return mapa; 
  fclose(fp);
  free(mapa);
}

void eliminarTarea(HashMap *mapa, List *lista){
  char tareaElim[31];
  Tarea *tarea = (Tarea *) malloc(sizeof(Tarea));
  pilaAcc * ultaccion = (pilaAcc *) malloc(sizeof(pilaAcc));
 
  printf("¿Qué tarea quiere eliminar?\n");
  getchar();
  scanf("%30[^\n]", tareaElim);
  
  ultaccion->tareaAfectada = (Tarea *) malloc (sizeof(Tarea));
  ultaccion->accion = 4;
  Pair *aux = searchMap(mapa, tareaElim);
  tarea = aux->value;
  
  strcpy(ultaccion->tareaAfectada->nombre, tarea->nombre);
  ultaccion->tareaAfectada->prioridad = tarea->prioridad;
  ultaccion->tareaAfectada->T_precedente = tarea->T_precedente;
  pushFront(lista, ultaccion);

  if(aux){
    eraseMap(mapa, tareaElim);
  }
  printf("La tarea %s ha sido eliminada\n\n", tareaElim);
  return;
}

int main() {
  HashMap *mapa = createMap(MAX);
  char nombreI[31];
  int prioridad;
  List *pila = createStack();
  printf("Ingresa tu nombre\n");
  scanf("%30[^\n]", nombreI);
  printf("Hola %s!\n", nombreI);
  while (true) {
    BARRAS;
    printf("|     ¿QUÉ ACCIÓN DESEAS REALIZAR?       |\n");
    BARRAS;
    printf("|1. AGREGAR TAREA                        |\n");
    printf("|2. ESTABLECER PRECEDENCIA ENTRE TAREAS  |\n");
    printf("|3. MOSTRAR TAREAS POR HACER             |\n");
    printf("|4. MARCAR TAREA COMO COMPLETADA         |\n");
    printf("|5. DESHACER ÚLTIMA ACCIÓN               |\n");
    printf("|6. CARGAR DATOS DESDE UN ARCHIVO        |\n");
    printf("|7. MOSTRAR SIN ORDEN                    |\n");
    printf("|8. ELIMINAR TAREA                       |\n");
    printf("|0. TERMINAR                             |\n");
    BARRAS;
    int comando;
    scanf("%d", &comando);

    switch (comando) {
    case 1:
      agregarTarea(mapa, pila);
      break;
    case 2:
      if(mapa == NULL) {
        printf("mapa vacío\n");
        break;
      }
      estabPrecedencia(mapa, pila);
      break;
    case 3: 
      if(mapa == NULL) {
        printf("mapa vacío\n");
        break;
      }
      mostrarT(mapa);
      break;
    case 4:
      if(mapa == NULL) {
        printf("mapa vacío\n");
        break;
      }
      marcarTarea(mapa, pila);
      break;
    case 5:
      printf("Deshacer ultima acción entre tareas\n");
      deshacerAcc(mapa, pila);
      break;

    case 6:
      printf("Cargar un archivo de tareas\n");
      mapa = cargarDatos();
      break;
    case 7:
      if(mapa == NULL){
        printf("mapa vacío\n");
        break;
      }
      mostrarTareas(mapa);
      break;
    case 8:
      if(mapa == NULL){
        printf("mapa vacío\n");
      }
      eliminarTarea(mapa, pila);
      break;
    case 0:
      printf("TERMINANDO...\n");
      return 0;

    default:
      if(mapa != NULL)
        continue;
      return 0;
    }
  }
  return 0;
}