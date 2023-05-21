#ifndef List_h
#define List_h

typedef struct List List;

List * createList(void);

//funcion para el primer nodo
void * firstList(List * list);

//funcion que retorna 1 si la lista tiene datos o 0 si no tiene
//int haveData(List* lista);

//funcion para el siguiente nodo
void * nextList(List * list);

//funcion para el nodo final
void * lastList(List * list);

//funcion nodo anterior
void * prevList(List * list);

//funcion que agrega Nodo en la posicion ¨inicial¨
void pushFront(List * list, const void * data);

//funcion que agrega Nodo  en la posiscion final
void pushBack(List * list, const void * data);

//funcion que agrega un nodo al lado de el current actual
void pushCurrent(List * list, const void * data);

void * popFront(List * list);

void * popBack(List * list);

void * popCurrent(List * list);

void cleanList(List * list);

#endif /* List_h */
