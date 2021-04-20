#include "abb.h"
#include "pa2m.h"
#include <time.h>

#define MAYOR 1
#define MENOR -1
#define IGUALES 0

#define MAX_VECTOR_CHICO 10
#define MAX_VECTOR_GRANDE 100
#define MAX_VECTOR_GIGANTE 2500
#define MAX_TELEFONOS 20

#define EXITO 0
#define ERROR -1

typedef struct telefono{
	char* marca;
	int id;
} telefono_t;

typedef struct vector{
	void* elementos[MAX_VECTOR_GRANDE];
	int tope;
} vector_t;

void destruir_telefono(telefono_t* telefono){
	free(telefono);
}

void destructor_de_telefonos(void* elemento){
	if (elemento) destruir_telefono((telefono_t*)elemento);
}

int comparar_telefonos(void* telefono1, void* telefono2){

	if (!telefono1 || !telefono2) return IGUALES;

	if ( (*(telefono_t*)telefono1).id > (*(telefono_t*)telefono2).id )
		return MAYOR;

	if ( (*(telefono_t*)telefono1).id < (*(telefono_t*)telefono2).id )
		return MENOR;

	return IGUALES;
}

telefono_t* crear_telefono(int id){

	telefono_t* telefono = malloc(sizeof(telefono_t));

	if (!telefono) return NULL;

	(*telefono).id = id;

	return telefono;
}

int comparar_numeros(void* numero1, void* numero2){

	if (!numero1 || !numero2) return IGUALES;

	if (*(int*)numero1 > *(int*)numero2) return MAYOR;

	if (*(int*)numero1 < *(int*)numero2) return MENOR;

	return IGUALES;
}

bool recorrer_hasta_nro_60(void* elemento, void* extra){

	if (*(int*)elemento == 60){
		*(int*)extra = *(int*)elemento;
		return true;
	}

	return false;
}

bool almacenar_elementos(void* elemento, void* extra){

	int posicion = (*(vector_t*)extra).tope;
	(*(vector_t*)extra).elementos[posicion] = elemento;

	((*(vector_t*)extra).tope)+=1;

	return false;
}

void crear_arbol_vacio(){

	abb_t* arbol = arbol_crear(comparar_numeros, destructor_de_telefonos);

	pa2m_afirmar(arbol != NULL, "Puedo crear un arbol vacio");

	arbol_destruir(arbol);
}

void crear_arbol_sin_comparador(){

	abb_t* arbol = arbol_crear(NULL, destructor_de_telefonos);

	pa2m_afirmar(arbol == NULL, "No puedo crear un arbol vacio sin comparador");

	arbol_destruir(arbol);
}

void crear_arbol_sin_destructor(){

	abb_t* arbol = arbol_crear(comparar_numeros, NULL);

	pa2m_afirmar(arbol != NULL, "Puedo crear un arbol vacio sin destructor");

	arbol_destruir(arbol);
}

void crear_arbol_sin_comparador_ni_destructor(){

	abb_t* arbol = arbol_crear(NULL, NULL);

	pa2m_afirmar(arbol == NULL, "No puedo crear un arbol vacio sin comparador ni destructor");

	arbol_destruir(arbol);
}

void pruebas_creacion_arbol(){

	crear_arbol_vacio();
	crear_arbol_sin_comparador();
	crear_arbol_sin_destructor();
	crear_arbol_sin_comparador_ni_destructor();
}

void pruebas_insercion_simple_sin_destructor(){

	pa2m_nuevo_grupo("INSERCION SIMPLE");

	abb_t* arbol = arbol_crear(comparar_numeros, NULL);

	int numero0 = 0;
	int numero1 = 1;
	int numero2 = 2;

	pa2m_afirmar(arbol_insertar(arbol, &numero1) == EXITO, "Se pudo insertar 1 elemento");

	pa2m_afirmar(arbol_vacio(arbol) == false, "El arbol no está vacio");

	pa2m_afirmar(*(int*)arbol_raiz(arbol) == numero1, "El primer elemento insertado esta en la raiz");

	nodo_abb_t* hoja_derecha = (*(*arbol).nodo_raiz).derecha;
	nodo_abb_t* hoja_izquierda = (*(*arbol).nodo_raiz).izquierda;

	pa2m_afirmar(hoja_derecha == NULL && hoja_izquierda == NULL, "Las hijos de la raiz son nulos");

	pa2m_afirmar(arbol_borrar(arbol, &numero1) == EXITO, "Puedo borrar la raiz");

	pa2m_afirmar(arbol_vacio(arbol) == true, "El arbol está vacio");

	pa2m_afirmar(arbol_insertar(arbol, &numero1) == EXITO, "Inserto denuevo el elemento anterior");
	pa2m_afirmar(*(int*)arbol_buscar(arbol, &numero1) == numero1, "El elemento está en el arbol");
	pa2m_afirmar(*(int*)arbol_raiz(arbol) == numero1, "EL elemento está en la raiz");

	pa2m_afirmar(arbol_insertar(arbol, &numero0) == EXITO, "Inserto un hijo izquierdo a la raiz");
	int nro = *(int*)(*(*(*arbol).nodo_raiz).izquierda).elemento;
	pa2m_afirmar(nro == numero0, "El elemento está a la izquierda de la raiz");

	pa2m_afirmar(arbol_insertar(arbol, &numero2) == EXITO, "Inserto un hijo derecho a la raiz");
	nro = *(int*)(*(*(*arbol).nodo_raiz).derecha).elemento;
	pa2m_afirmar(nro == numero2, "El elemento está a la derecha de la raiz");

	pa2m_afirmar(arbol_vacio(arbol) == false, "El arbol no está vacio");

	arbol_destruir(arbol);
}

void pruebas_insercion_moderada_sin_destructor(){

	pa2m_nuevo_grupo("INSERCION MODERADA");

	abb_t* arbol = arbol_crear(comparar_numeros, NULL);

	int vector[MAX_VECTOR_CHICO] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

	pa2m_afirmar(arbol_insertar(arbol, &vector[5]) == EXITO, "Se pudo insertar el elemento nro 5");
	pa2m_afirmar(*(int*)arbol_buscar(arbol, &vector[5]) == vector[5], "El elemento nro 5 está en el arbol");

	pa2m_afirmar(arbol_insertar(arbol, &vector[3]) == EXITO, "Se pudo insertar el elemento nro 3");
	pa2m_afirmar(*(int*)arbol_buscar(arbol, &vector[3]) == vector[3], "El elemento nro 3 está en el arbol");

	pa2m_afirmar(arbol_insertar(arbol, &vector[8]) == EXITO, "Se pudo insertar el elemento nro 8");
	pa2m_afirmar(*(int*)arbol_buscar(arbol, &vector[8]) == vector[8], "El elemento nro 8 está en el arbol");

	pa2m_afirmar(arbol_insertar(arbol, &vector[4]) == EXITO, "Se pudo insertar el elemento nro 4");
	pa2m_afirmar(*(int*)arbol_buscar(arbol, &vector[4]) == vector[4], "El elemento nro 4 está en el arbol");

	pa2m_afirmar(arbol_insertar(arbol, &vector[0]) == EXITO, "Se pudo insertar el elemento nro 0");
	pa2m_afirmar(*(int*)arbol_buscar(arbol, &vector[0]) == vector[0], "El elemento nro 0 está en el arbol");

	pa2m_afirmar(arbol_insertar(arbol, &vector[1]) == EXITO, "Se pudo insertar el elemento nro 1");
	pa2m_afirmar(*(int*)arbol_buscar(arbol, &vector[1]) == vector[1], "El elemento nro 1 está en el arbol");

	pa2m_afirmar(arbol_insertar(arbol, &vector[2]) == EXITO, "Se pudo insertar el elemento nro 2");
	pa2m_afirmar(*(int*)arbol_buscar(arbol, &vector[2]) == vector[2], "El elemento nro 2 está en el arbol");

	pa2m_afirmar(arbol_insertar(arbol, &vector[9]) == EXITO, "Se pudo insertar el elemento nro 9");
	pa2m_afirmar(*(int*)arbol_buscar(arbol, &vector[9]) == vector[9], "El elemento nro 9 está en el arbol");

	pa2m_afirmar(arbol_insertar(arbol, &vector[6]) == EXITO, "Se pudo insertar el elemento nro 6");
	pa2m_afirmar(*(int*)arbol_buscar(arbol, &vector[6]) == vector[6], "El elemento nro 6 está en el arbol");

	pa2m_afirmar(arbol_insertar(arbol, &vector[7]) == EXITO, "Se pudo insertar el elemento nro 7");
	pa2m_afirmar(*(int*)arbol_buscar(arbol, &vector[7]) == vector[7], "El elemento nro 7 está en el arbol");

	pa2m_afirmar(arbol_vacio(arbol) == false, "El arbol no está vacio");

	int contador = 0;

	for (int i = 0; i < 10; i++){

		if (*(int*)arbol_buscar(arbol, &vector[i]) == vector[i]){
			contador++;
		}
	}

	pa2m_afirmar(contador == 10, "La cantidad de elementos insertada es la esperada");

	arbol_destruir(arbol);
}

bool se_utilizo(int vector_posiciones[MAX_VECTOR_GIGANTE], int tope_vector, int posicion){
	
	int i = 0;
	bool hay_repetido = false;
	
	while (i < tope_vector && !hay_repetido){
		if (vector_posiciones[i] == posicion)
			hay_repetido = true;
		i++;
	}

	return hay_repetido;
}

void pruebas_insercion_extrema_sin_destructor(){

	pa2m_nuevo_grupo("INSERCION EXTREMA");

	srand((unsigned)time(NULL));

	abb_t* arbol = arbol_crear(comparar_numeros, NULL);

	int vector[MAX_VECTOR_GIGANTE];
	int contador = 0;
	int posicion;
	int posiciones_utilizadas[MAX_VECTOR_GIGANTE];
	int tope_pos_utilizadas = 0;
	bool hubo_error1 = false;
	bool hubo_error2 = false;

	for (int i = 0; i < MAX_VECTOR_GIGANTE; i++){
		vector[i] = contador;
		contador++;
	}

	contador = 0;

	while (contador < MAX_VECTOR_GIGANTE){
		posicion = rand() % MAX_VECTOR_GIGANTE;

		if (!se_utilizo(posiciones_utilizadas, tope_pos_utilizadas, posicion)){

			if (arbol_insertar(arbol, &vector[posicion]) == ERROR)
				hubo_error1 = true;

			posiciones_utilizadas[tope_pos_utilizadas] = posicion;
			tope_pos_utilizadas++;
			contador++;

			if (arbol_buscar(arbol, &vector[posicion]) == NULL)
				hubo_error2 = true;
		}
	}

	pa2m_afirmar(contador == MAX_VECTOR_GIGANTE, "Se intentaron insertar 2.500 elementos");
	pa2m_afirmar(hubo_error1 == false, "No hubo error al intentar insertar 2.500 elementos");
	pa2m_afirmar(hubo_error2 == false, "Se buscaron los 2.500 elementos y todos están en el arbol");
	pa2m_afirmar(arbol_vacio(arbol) == false, "El arbol no está vacio");

	arbol_destruir(arbol);
}

void insertar_numeros(abb_t* arbol, int vector[MAX_VECTOR_GRANDE]){
	//raiz
	arbol_insertar(arbol, &vector[50]);
	
	//rama izquierda
	arbol_insertar(arbol, &vector[25]);
	arbol_insertar(arbol, &vector[20]);
	arbol_insertar(arbol, &vector[15]);
	arbol_insertar(arbol, &vector[10]);
	arbol_insertar(arbol, &vector[35]);
	arbol_insertar(arbol, &vector[30]);
	arbol_insertar(arbol, &vector[40]);
	arbol_insertar(arbol, &vector[45]);
	
	//rama derecha
	arbol_insertar(arbol, &vector[75]);
	arbol_insertar(arbol, &vector[60]);
	arbol_insertar(arbol, &vector[55]);
	arbol_insertar(arbol, &vector[65]);
	arbol_insertar(arbol, &vector[66]);
	arbol_insertar(arbol, &vector[90]);
	arbol_insertar(arbol, &vector[80]);
	arbol_insertar(arbol, &vector[85]);
	arbol_insertar(arbol, &vector[95]);
	arbol_insertar(arbol, &vector[94]);
	arbol_insertar(arbol, &vector[96]);
}

void vaciar_arbol_desde_la_raiz(abb_t* arbol, int elementos_restantes){

	bool hubo_error = false;
	int contador = 0;

	while (!arbol_vacio(arbol)){
		if (arbol_borrar(arbol, arbol_raiz(arbol)) == EXITO)
			contador++;
		else
			hubo_error = true;
	}

	pa2m_afirmar(contador == elementos_restantes, "Se borraron todos los elementos desde la raiz");
	pa2m_afirmar(hubo_error == false, "No hubo error al borrar los elementos desde la raiz");
	pa2m_afirmar(arbol_vacio(arbol) == true, "El arbol está vacio");
}

void pruebas_borrado_sin_destructor(){

	abb_t* arbol = arbol_crear(comparar_numeros, NULL);
	void* elemento;
	int vector[MAX_VECTOR_GRANDE];
	int contador = 0;

	for (int i = 0; i < 100; i++){
		vector[i] = contador;
		contador++;
	}

	insertar_numeros(arbol, vector);

	pa2m_afirmar(arbol_borrar(arbol, &vector[50]) == EXITO, "Borro el nro 50 (la raiz)");
	pa2m_afirmar(arbol_buscar(arbol, &vector[50]) == NULL, "El nro 50 no está en el arbol");
	pa2m_afirmar(arbol_borrar(arbol, &vector[50]) == ERROR, "Intento borrar el nro 50 y devuelve error");

	pa2m_afirmar(*(int*)arbol_raiz(arbol) == 45, "El reemplazo de la raiz es el esperado");

	int hijo_derecho = *(int*)(*(*(*arbol).nodo_raiz).derecha).elemento;
	int hijo_izquierdo = *(int*)(*(*(*arbol).nodo_raiz).izquierda).elemento;
	pa2m_afirmar(hijo_derecho == 75 && hijo_izquierdo == 25, "Los hijos de la raiz no cambiaron, son el 25 y 75");

	pa2m_afirmar(arbol_borrar(arbol, &vector[25]) == EXITO, "Borro el nro 25 (nodo con dos hijos)");
	pa2m_afirmar(arbol_buscar(arbol, &vector[25]) == NULL, "El nro 25 no está en el arbol");
	pa2m_afirmar(arbol_borrar(arbol, &vector[25]) == ERROR, "Intento borrar el nro 25 y devuelve error");

	pa2m_afirmar(arbol_borrar(arbol, &vector[65]) == EXITO, "Borro el nro 65 (nodo con 1 hijo a la izquierda)");
	pa2m_afirmar(arbol_buscar(arbol, &vector[65]) == NULL, "El nro 65 no está en el arbol");
	pa2m_afirmar(arbol_borrar(arbol, &vector[65]) == ERROR, "Intento borrar el nro 65 y devuelve error");
	
	pa2m_afirmar(arbol_borrar(arbol, &vector[66]) == EXITO, "Borro el nro 66 (nodo sin hijos)");
	pa2m_afirmar(arbol_buscar(arbol, &vector[66]) == NULL, "El nro 66 no está en el arbol");
	pa2m_afirmar(arbol_borrar(arbol, &vector[66]) == ERROR, "Intento borrar el nro 66 y devuelve error");

	pa2m_afirmar(arbol_borrar(arbol, &vector[60]) == EXITO, "Borro el nro 60 (nodo con 1 hijo a la derecha)");
	pa2m_afirmar(arbol_buscar(arbol, &vector[60]) == NULL, "El nro 60 no está en el arbol");
	pa2m_afirmar(arbol_borrar(arbol, &vector[60]) == ERROR, "Intento borrar el nro 60 y devuelve error");

	pa2m_afirmar(arbol_borrar(arbol, &vector[75]) == EXITO, "Borro el nro 75");
	pa2m_afirmar(arbol_buscar(arbol, &vector[75]) == NULL, "El nro 75 no está en el arbol");
	pa2m_afirmar(arbol_borrar(arbol, &vector[75]) == ERROR, "Intento borrar el nro 75 y devuelve error");

	pa2m_afirmar(arbol_borrar(arbol, &vector[45]) == EXITO, "Borro el nro 45 (la raiz)");
	pa2m_afirmar(arbol_buscar(arbol, &vector[45]) == NULL, "El nro 45 no está en el arbol");
	pa2m_afirmar(arbol_borrar(arbol, &vector[45]) == ERROR, "Intento borrar el nro 45 y devuelve error");

	pa2m_afirmar(*(int*)arbol_raiz(arbol) == 40, "El reemplazo de la raiz es el esperado");

	pa2m_afirmar(arbol_borrar(arbol, &vector[40]) == EXITO, "Borro el nro 40 (la raiz)");
	pa2m_afirmar(arbol_buscar(arbol, &vector[40]) == NULL, "El nro 40 no está en el arbol");
	pa2m_afirmar(arbol_borrar(arbol, &vector[40]) == ERROR, "Intento borrar el nro 40 y devuelve error");

	pa2m_afirmar(*(int*)arbol_raiz(arbol) == 35, "El reemplazo de la raiz es el esperado");

	pa2m_afirmar(arbol_borrar(arbol, &vector[35]) == EXITO, "Borro el nro 35 (la raiz)");
	pa2m_afirmar(arbol_buscar(arbol, &vector[35]) == NULL, "El nro 35 no está en el arbol");
	pa2m_afirmar(arbol_borrar(arbol, &vector[35]) == ERROR, "Intento borrar el nro 35 y devuelve error");

	pa2m_afirmar(*(int*)arbol_raiz(arbol) == 30, "El reemplazo de la raiz es el esperado");

	pa2m_afirmar(arbol_borrar(arbol, &vector[30]) == EXITO, "Borro el nro 30 (la raiz)");
	pa2m_afirmar(arbol_buscar(arbol, &vector[30]) == NULL, "El nro 30 no está en el arbol");
	pa2m_afirmar(arbol_borrar(arbol, &vector[30]) == ERROR, "Intento borrar el nro 30 y devuelve error");

	pa2m_afirmar(*(int*)arbol_raiz(arbol) == 20, "El reemplazo de la raiz es el esperado");

	vaciar_arbol_desde_la_raiz(arbol, 10);

	arbol_destruir(arbol);
}

void pruebas_insercion_simple_con_destructor(){
	
	pa2m_nuevo_grupo("INSERCION SIMPLE");

	abb_t* arbol = arbol_crear(comparar_telefonos, destructor_de_telefonos);

	telefono_t* t0 = crear_telefono(100);
	telefono_t* t1 = crear_telefono(99);
	telefono_t* t2 = crear_telefono(101);

	pa2m_afirmar(arbol_insertar(arbol, t0) == EXITO, "Se pudo insertar el telefono");
	pa2m_afirmar((*(telefono_t*)arbol_buscar(arbol, t0)).id == (*t0).id, "El telefono está en el arbol");
	pa2m_afirmar((*(telefono_t*)arbol_raiz(arbol)).id == (*t0).id, "EL telefono está en la raiz");
	
	pa2m_afirmar(arbol_vacio(arbol) == false, "El arbol no está vacio");

	nodo_abb_t* derecha = (*(*arbol).nodo_raiz).derecha;
	nodo_abb_t* izquierda = (*(*arbol).nodo_raiz).izquierda;

	pa2m_afirmar(derecha == NULL && izquierda == NULL, "Los hijos del telefono son nulos");

	pa2m_afirmar(arbol_borrar(arbol, t0) == EXITO, "Puedo borrar la raiz");

	pa2m_afirmar(arbol_vacio(arbol) == true, "El arbol está vacio");

	t0 = crear_telefono(100);

	pa2m_afirmar(arbol_insertar(arbol, t0) == EXITO, "Inserto denuevo el telefono anterior");
	pa2m_afirmar((*(telefono_t*)arbol_buscar(arbol, t0)).id == (*t0).id, "El telefono está en el arbol");
	pa2m_afirmar((*(telefono_t*)arbol_raiz(arbol)).id == (*t0).id, "EL telefono está en la raiz");

	pa2m_afirmar(arbol_insertar(arbol, t1) == EXITO, "Inserto un hijo izquierdo a la raiz");
	int id = (*(telefono_t*)(*(*(*arbol).nodo_raiz).izquierda).elemento).id;
	pa2m_afirmar(id == (*t1).id, "El telefono está a la izquierda de la raiz");

	pa2m_afirmar(arbol_insertar(arbol, t2) == EXITO, "Inserto un hijo derecho a la raiz");
	id = (*(telefono_t*)(*(*(*arbol).nodo_raiz).derecha).elemento).id;
	pa2m_afirmar(id == (*t2).id, "El telefono está a la derecha de la raiz");

	pa2m_afirmar(arbol_vacio(arbol) == false, "El arbol no está vacio");

	arbol_destruir(arbol);
}

void pruebas_insercion_moderada_con_destructor(){

	pa2m_nuevo_grupo("INSERCION MODERADA");

	abb_t* arbol = arbol_crear(comparar_telefonos, destructor_de_telefonos);

	telefono_t* telefonos[MAX_TELEFONOS];

	int id = 0;
	for (int i = 0; i < 10; i++){
		telefonos[i] = crear_telefono(id);
		id++;
	}

	pa2m_afirmar(arbol_insertar(arbol, telefonos[5]) == EXITO, "Se pudo insertar el telefono de id 5");
	pa2m_afirmar((*(telefono_t*)arbol_buscar(arbol, telefonos[5])).id == (*telefonos[5]).id,
		"El telefono de id 5 está en el arbol");
	
	pa2m_afirmar(arbol_insertar(arbol, telefonos[2]) == EXITO, "Se pudo insertar el telefono de id 2");
	pa2m_afirmar((*(telefono_t*)arbol_buscar(arbol, telefonos[2])).id == (*telefonos[2]).id,
		"El telefono de id 2 está en el arbol");

	pa2m_afirmar(arbol_insertar(arbol, telefonos[0]) == EXITO, "Se pudo insertar el telefono de id 0");
	pa2m_afirmar((*(telefono_t*)arbol_buscar(arbol, telefonos[0])).id == (*telefonos[0]).id,
		"El telefono de id 0 está en el arbol");

	pa2m_afirmar(arbol_insertar(arbol, telefonos[1]) == EXITO, "Se pudo insertar el telefono de id 1");
	pa2m_afirmar((*(telefono_t*)arbol_buscar(arbol, telefonos[1])).id == (*telefonos[1]).id,
		"El telefono de id 1 está en el arbol");

	pa2m_afirmar(arbol_insertar(arbol, telefonos[4]) == EXITO, "Se pudo insertar el telefono de id 4");
	pa2m_afirmar((*(telefono_t*)arbol_buscar(arbol, telefonos[4])).id == (*telefonos[4]).id,
		"El telefono de id 4 está en el arbol");

	pa2m_afirmar(arbol_insertar(arbol, telefonos[3]) == EXITO, "Se pudo insertar el telefono de id 3");
	pa2m_afirmar((*(telefono_t*)arbol_buscar(arbol, telefonos[3])).id == (*telefonos[3]).id,
		"El telefono de id 3 está en el arbol");

	pa2m_afirmar(arbol_insertar(arbol, telefonos[7]) == EXITO, "Se pudo insertar el telefono de id 7");
	pa2m_afirmar((*(telefono_t*)arbol_buscar(arbol, telefonos[7])).id == (*telefonos[7]).id,
		"El telefono de id 7 está en el arbol");

	pa2m_afirmar(arbol_insertar(arbol, telefonos[6]) == EXITO, "Se pudo insertar el telefono de id 6");
	pa2m_afirmar((*(telefono_t*)arbol_buscar(arbol, telefonos[6])).id == (*telefonos[6]).id,
		"El telefono de id 6 está en el arbol");

	pa2m_afirmar(arbol_insertar(arbol, telefonos[9]) == EXITO, "Se pudo insertar el telefono de id 9");
	pa2m_afirmar((*(telefono_t*)arbol_buscar(arbol, telefonos[9])).id == (*telefonos[9]).id,
		"El telefono de id 9 está en el arbol");

	pa2m_afirmar(arbol_insertar(arbol, telefonos[8]) == EXITO, "Se pudo insertar el telefono de id 8");
	pa2m_afirmar((*(telefono_t*)arbol_buscar(arbol, telefonos[8])).id == (*telefonos[8]).id,
		"El telefono de id 8 está en el arbol");

	pa2m_afirmar(arbol_vacio(arbol) == false, "El arbol no está vacio");

	int contador = 0;

	for (int i = 0; i < 10; i++){
		if ( (*(telefono_t*)arbol_buscar(arbol, telefonos[i])).id == (*telefonos[i]).id )
			contador++;
	}

	pa2m_afirmar(contador == 10, "La cantidad insertada es la esperada");

	arbol_destruir(arbol);
}

void pruebas_insercion_extrema_con_destructor(){

	pa2m_nuevo_grupo("INSERCION EXTREMA");

	srand((unsigned)time(NULL));

	abb_t* arbol = arbol_crear(comparar_telefonos, destructor_de_telefonos);

	telefono_t* telefonos[MAX_VECTOR_GIGANTE];
	int contador = 0;
	int posicion;
	int posiciones_utilizadas[MAX_VECTOR_GIGANTE];
	int tope_pos_utilizadas = 0;
	bool hubo_error1 = false;
	bool hubo_error2 = false;

	int id = 0;
	for (int i = 0; i < MAX_VECTOR_GIGANTE; i++){
		telefonos[i] = crear_telefono(id);
		id++;
	}

	while (contador < MAX_VECTOR_GIGANTE){
		posicion = rand() % MAX_VECTOR_GIGANTE;

		if (!se_utilizo(posiciones_utilizadas, tope_pos_utilizadas, posicion)){

			if (arbol_insertar(arbol, telefonos[posicion]) == ERROR)
				hubo_error1 = true;

			posiciones_utilizadas[tope_pos_utilizadas] = posicion;
			tope_pos_utilizadas++;
			contador++;

			if (arbol_buscar(arbol, telefonos[posicion]) == NULL)
				hubo_error2 = true;
		}
	}

	pa2m_afirmar(contador == MAX_VECTOR_GIGANTE, "Se intentaron insertar 2.500 telefonos");
	pa2m_afirmar(hubo_error1 == false, "No hubo error al intentar insertar 2.500 telefonos");
	pa2m_afirmar(hubo_error2 == false, "Se buscaron los 2.500 telefonos y todos están en el arbol");
	pa2m_afirmar(arbol_vacio(arbol) == false, "El arbol no está vacio");

	arbol_destruir(arbol);
}

void insertar_telefonos(abb_t* arbol, telefono_t* telefonos[MAX_TELEFONOS]){
	//raiz
	arbol_insertar(arbol, telefonos[10]);

	//rama izquierda
	arbol_insertar(arbol, telefonos[5]);
	arbol_insertar(arbol, telefonos[3]);
	arbol_insertar(arbol, telefonos[4]);
	arbol_insertar(arbol, telefonos[1]);
	arbol_insertar(arbol, telefonos[2]);
	arbol_insertar(arbol, telefonos[0]);
	arbol_insertar(arbol, telefonos[7]);
	arbol_insertar(arbol, telefonos[6]);
	arbol_insertar(arbol, telefonos[9]);
	arbol_insertar(arbol, telefonos[8]);

	//rama derecha
	arbol_insertar(arbol, telefonos[15]);
	arbol_insertar(arbol, telefonos[13]);
	arbol_insertar(arbol, telefonos[14]);
	arbol_insertar(arbol, telefonos[12]);
	arbol_insertar(arbol, telefonos[11]);
	arbol_insertar(arbol, telefonos[17]);
	arbol_insertar(arbol, telefonos[16]);
	arbol_insertar(arbol, telefonos[18]);
	arbol_insertar(arbol, telefonos[19]);

}

void pruebas_borrado_con_destructor(){

	abb_t* arbol = arbol_crear(comparar_telefonos, destructor_de_telefonos);

	telefono_t* telefonos[MAX_TELEFONOS];

	telefono_t* aux = crear_telefono(0);

	int id = 0;
	for (int i = 0; i < 20; i++){
		telefonos[i] = crear_telefono(id);
		id++;
	}

	insertar_telefonos(arbol, telefonos);

	(*aux).id = (*telefonos[10]).id;
	pa2m_afirmar(arbol_borrar(arbol, telefonos[10]) == EXITO, "Borro el telefono con id 10 (la raiz)");
	pa2m_afirmar(arbol_buscar(arbol, aux) == NULL, "El telefono con id 10 no está en el arbol");
	pa2m_afirmar(arbol_borrar(arbol, aux) == ERROR, "intento borrar el telefono con id 10 y me devuelve error");

	pa2m_afirmar((*(telefono_t*)arbol_raiz(arbol)).id == 9, "El reemplazo de la raiz es el esperado");

	int id_izquierdo = (*(telefono_t*)(*(*(*arbol).nodo_raiz).izquierda).elemento).id;
	int id_derecho = (*(telefono_t*)(*(*(*arbol).nodo_raiz).derecha).elemento).id;
	pa2m_afirmar(id_izquierdo == 5 && id_derecho == 15, "Los hijos de la raiz no cambiaron, son el 5 y el 15");

	(*aux).id = (*telefonos[12]).id;
	pa2m_afirmar(arbol_borrar(arbol, telefonos[12]) == EXITO, "Borro el telefono con id 12 (nodo con 1 hijo a la izquierda)");
	pa2m_afirmar(arbol_buscar(arbol, aux) == NULL, "El telefono con id 12 no está en el arbol");
	pa2m_afirmar(arbol_borrar(arbol, aux) == ERROR, "intento borrar el telefono con id 12 y me devuelve error");

	(*aux).id = (*telefonos[18]).id;
	pa2m_afirmar(arbol_borrar(arbol, telefonos[18]) == EXITO, "Borro el telefono con id 18 (nodo con 1 hijo a la derecha)");
	pa2m_afirmar(arbol_buscar(arbol, aux) == NULL, "El telefono con id 18 no está en el arbol");
	pa2m_afirmar(arbol_borrar(arbol, aux) == ERROR, "intento borrar el telefono con id 18 y me devuelve error");

	(*aux).id = (*telefonos[8]).id;
	pa2m_afirmar(arbol_borrar(arbol, telefonos[8]) == EXITO, "Borro el telefono con id 8 (nodo sin hijos)");
	pa2m_afirmar(arbol_buscar(arbol, aux) == NULL, "El telefono con id 8 no está en el arbol");
	pa2m_afirmar(arbol_borrar(arbol, aux) == ERROR, "intento borrar el telefono con id 8 y me devuelve error");	

	(*aux).id = (*telefonos[5]).id;
	pa2m_afirmar(arbol_borrar(arbol, telefonos[5]) == EXITO, "Borro el telefono con id 5 (nodo con 2 hijos)");
	pa2m_afirmar(arbol_buscar(arbol, aux) == NULL, "El telefono con id 5 no está en el arbol");
	pa2m_afirmar(arbol_borrar(arbol, aux) == ERROR, "intento borrar el telefono con id 5 y me devuelve error");

	(*aux).id = (*telefonos[15]).id;
	pa2m_afirmar(arbol_borrar(arbol, telefonos[15]) == EXITO, "Borro el telefono con id 15");
	pa2m_afirmar(arbol_buscar(arbol, aux) == NULL, "El telefono con id 15 no está en el arbol");
	pa2m_afirmar(arbol_borrar(arbol, aux) == ERROR, "intento borrar el telefono con id 15 y me devuelve error");	

	(*aux).id = (*telefonos[9]).id;
	pa2m_afirmar(arbol_borrar(arbol, telefonos[9]) == EXITO, "Borro el telefono con id 9 (la raiz)");
	pa2m_afirmar(arbol_buscar(arbol, aux) == NULL, "El telefono con id 9 no está en el arbol");
	pa2m_afirmar(arbol_borrar(arbol, aux) == ERROR, "intento borrar el telefono con id 9 y me devuelve error");

	pa2m_afirmar((*(telefono_t*)arbol_raiz(arbol)).id == 7, "El reemplazo de la raiz es el esperado");

	(*aux).id = (*telefonos[7]).id;
	pa2m_afirmar(arbol_borrar(arbol, telefonos[7]) == EXITO, "Borro el telefono con id 7 (la raiz)");
	pa2m_afirmar(arbol_buscar(arbol, aux) == NULL, "El telefono con id 7 no está en el arbol");
	pa2m_afirmar(arbol_borrar(arbol, aux) == ERROR, "intento borrar el telefono con id 7 y me devuelve error");

	pa2m_afirmar((*(telefono_t*)arbol_raiz(arbol)).id == 6, "El reemplazo de la raiz es el esperado");

	(*aux).id = (*telefonos[6]).id;
	pa2m_afirmar(arbol_borrar(arbol, telefonos[6]) == EXITO, "Borro el telefono con id 6 (la raiz)");
	pa2m_afirmar(arbol_buscar(arbol, aux) == NULL, "El telefono con id 6 no está en el arbol");
	pa2m_afirmar(arbol_borrar(arbol, aux) == ERROR, "intento borrar el telefono con id 6 y me devuelve error");

	pa2m_afirmar((*(telefono_t*)arbol_raiz(arbol)).id == 4, "El reemplazo de la raiz es el esperado");

	(*aux).id = (*telefonos[4]).id;
	pa2m_afirmar(arbol_borrar(arbol, telefonos[4]) == EXITO, "Borro el telefono con id 4 (la raiz)");
	pa2m_afirmar(arbol_buscar(arbol, aux) == NULL, "El telefono con id 4 no está en el arbol");
	pa2m_afirmar(arbol_borrar(arbol, aux) == ERROR, "intento borrar el telefono con id 4 y me devuelve error");

	pa2m_afirmar((*(telefono_t*)arbol_raiz(arbol)).id == 3, "El reemplazo de la raiz es el esperado");

	vaciar_arbol_desde_la_raiz(arbol, 10);

	free(aux);
	arbol_destruir(arbol);
}

/*
ARBOL RECORRIDO INORDEN
10 - 15 - 20 - 25 - 30 - 35 - 40 - 45 - 50 - 55 - 60 - 65 - 66 - 75 - 80 - 85 - 90 - 94 - 95 - 96
*/
void ordenar_vector_inorden(int vector_aux[MAX_VECTOR_GRANDE], int vector[MAX_VECTOR_GRANDE]){
	vector_aux[0] = vector[10];
	vector_aux[1] = vector[15];
	vector_aux[2] = vector[20];
	vector_aux[3] = vector[25];
	vector_aux[4] = vector[30];
	vector_aux[5] = vector[35];
	vector_aux[6] = vector[40];
	vector_aux[7] = vector[45];
	vector_aux[8] = vector[50];
	vector_aux[9] = vector[55];
	vector_aux[10] = vector[60];
	vector_aux[11] = vector[65];
	vector_aux[12] = vector[66];
	vector_aux[13] = vector[75];
	vector_aux[14] = vector[80];
	vector_aux[15] = vector[85];
	vector_aux[16] = vector[90];
	vector_aux[17] = vector[94];
	vector_aux[18] = vector[95];
	vector_aux[19] = vector[96];
}

/*
ARBOL RECORRIDO PREORDEN
50 - 25 - 20 - 15 - 10 - 35 - 30 - 40 - 45 - 75 - 60 - 55 - 65 - 66 - 90 - 80 - 85 - 95 - 94 - 96
*/
void ordenar_vector_preorden(int vector_aux[MAX_VECTOR_GRANDE], int vector[MAX_VECTOR_GRANDE]){
	vector_aux[0] = vector[50];
	vector_aux[1] = vector[25];
	vector_aux[2] = vector[20];
	vector_aux[3] = vector[15];
	vector_aux[4] = vector[10];
	vector_aux[5] = vector[35];
	vector_aux[6] = vector[30];
	vector_aux[7] = vector[40];
	vector_aux[8] = vector[45];
	vector_aux[9] = vector[75];
	vector_aux[10] = vector[60];
	vector_aux[11] = vector[55];
	vector_aux[12] = vector[65];
	vector_aux[13] = vector[66];
	vector_aux[14] = vector[90];
	vector_aux[15] = vector[80];
	vector_aux[16] = vector[85];
	vector_aux[17] = vector[95];
	vector_aux[18] = vector[94];
	vector_aux[19] = vector[96];	
}

/*
ARBOL RECORRIDO POSTORDEN
10 - 15 - 20 - 30 - 45 - 40 - 35 - 25 - 55 - 66 - 65 - 60 - 85 - 80 - 94 - 96 - 95 - 90 - 75 - 50
*/
void ordenar_vector_postorden(int vector_aux[MAX_VECTOR_GRANDE], int vector[MAX_VECTOR_GRANDE]){
	vector_aux[0] = vector[10];
	vector_aux[1] = vector[15];
	vector_aux[2] = vector[20];
	vector_aux[3] = vector[30];
	vector_aux[4] = vector[45];
	vector_aux[5] = vector[40];
	vector_aux[6] = vector[35];
	vector_aux[7] = vector[25];
	vector_aux[8] = vector[55];
	vector_aux[9] = vector[66];
	vector_aux[10] = vector[65];
	vector_aux[11] = vector[60];
	vector_aux[12] = vector[85];
	vector_aux[13] = vector[80];
	vector_aux[14] = vector[94];
	vector_aux[15] = vector[96];
	vector_aux[16] = vector[95];
	vector_aux[17] = vector[90];
	vector_aux[18] = vector[75];
	vector_aux[19] = vector[50];
}

void afirmar_segun_tope(size_t tope, size_t elementos_recorridos){

	if (tope == 10){
		pa2m_afirmar(elementos_recorridos == 10, "El tope del array vale 10 y se recorren 10 elementos");
	}
	else if (tope == 20){
		pa2m_afirmar(elementos_recorridos == 20, "Se recorrieron todos los elementos del arbol");
	}
	else {
		pa2m_afirmar(elementos_recorridos == 20, "El tope del array vale 30 pero se recorren 20 elementos");
	}
}

void probar_recorrer_cierta_cantidad(abb_t* arbol, int vector[MAX_VECTOR_GRANDE], size_t tope, int recorrido){
	
	void* vector_aux[MAX_VECTOR_GRANDE];
	size_t elementos_recorridos;
	bool hay_distinto = false;

	if (recorrido == ABB_RECORRER_INORDEN){
		elementos_recorridos = arbol_recorrido_inorden(arbol, vector_aux, tope);
	}
	else if (recorrido == ABB_RECORRER_PREORDEN){
		elementos_recorridos = arbol_recorrido_preorden(arbol, vector_aux, tope);
	}
	else if (recorrido == ABB_RECORRER_POSTORDEN){
		elementos_recorridos = arbol_recorrido_postorden(arbol, vector_aux, tope);
	}

	afirmar_segun_tope(tope, elementos_recorridos);

	for (int i = 0; i < (int)elementos_recorridos; i++){
		if (vector[i] != *(int*)vector_aux[i])
			hay_distinto = true;
	}

	pa2m_afirmar(hay_distinto == false, "Se recorrieron los elementos en el orden correcto");	
}

void pruebas_recorrido_inorden(abb_t* arbol, int vector[MAX_VECTOR_GRANDE]){

	int vector_aux[MAX_VECTOR_GRANDE];

	ordenar_vector_inorden(vector_aux, vector);

	probar_recorrer_cierta_cantidad(arbol, vector_aux, 20, ABB_RECORRER_INORDEN);
	probar_recorrer_cierta_cantidad(arbol, vector_aux, 10, ABB_RECORRER_INORDEN);
	probar_recorrer_cierta_cantidad(arbol, vector_aux, 30, ABB_RECORRER_INORDEN);
}

void pruebas_recorrido_preorden(abb_t* arbol, int vector[MAX_VECTOR_GRANDE]){

	int vector_aux[MAX_VECTOR_GRANDE];

	ordenar_vector_preorden(vector_aux, vector);

	probar_recorrer_cierta_cantidad(arbol, vector_aux, 20, ABB_RECORRER_PREORDEN);
	probar_recorrer_cierta_cantidad(arbol, vector_aux, 10, ABB_RECORRER_PREORDEN);
	probar_recorrer_cierta_cantidad(arbol, vector_aux, 30, ABB_RECORRER_PREORDEN);
}

void pruebas_recorrido_postorden(abb_t* arbol, int vector[MAX_VECTOR_GRANDE]){

	int vector_aux[MAX_VECTOR_GRANDE];

	ordenar_vector_postorden(vector_aux, vector);

	probar_recorrer_cierta_cantidad(arbol, vector_aux, 20, ABB_RECORRER_POSTORDEN);
	probar_recorrer_cierta_cantidad(arbol, vector_aux, 10, ABB_RECORRER_POSTORDEN);
	probar_recorrer_cierta_cantidad(arbol, vector_aux, 30, ABB_RECORRER_POSTORDEN);
}

void pruebas_recorridos(){

	abb_t* arbol = arbol_crear(comparar_numeros, NULL);

	int vector[MAX_VECTOR_GRANDE];
	int contador = 0;

	for (int i = 0; i < 100; i++){
		vector[i] = contador;
		contador++;
	}

	insertar_numeros(arbol, vector);

	pa2m_nuevo_grupo("RECORRIDO INORDEN");
	pruebas_recorrido_inorden(arbol, vector);
	
	pa2m_nuevo_grupo("RECORRIDO PREORDEN");
	pruebas_recorrido_preorden(arbol, vector);

	pa2m_nuevo_grupo("RECORRIDO POSTORDEN");
	pruebas_recorrido_postorden(arbol, vector);
	
	arbol_destruir(arbol);
}

void probar_iterar(abb_t* arbol, int recorrido, int vector[MAX_VECTOR_GRANDE], size_t tope_hasta_60){

	vector_t vector_extra;
	vector_extra.tope = 0;
	size_t elementos_recorridos;
	bool hay_distinto = false;
	int numero;

	elementos_recorridos = abb_con_cada_elemento(arbol, recorrido, almacenar_elementos, &vector_extra);

	pa2m_afirmar(elementos_recorridos == 20, "Se recorrieron todos los elementos el arbol");

	for (int i = 0; i < (int)elementos_recorridos; i++){
		if (*(int*)vector_extra.elementos[i] != vector[i])
			hay_distinto = true;
	}

	pa2m_afirmar(hay_distinto == false, "Se recorrieron los elementos en el orden correcto");

	elementos_recorridos = abb_con_cada_elemento(arbol, recorrido, recorrer_hasta_nro_60, &numero);

	pa2m_afirmar(elementos_recorridos == tope_hasta_60, "Se recorrieron la cantidad de elementos esperados hasta el nro 60");

	pa2m_afirmar(numero == 60, "El ultimo elemento iterado es el nro 60");
}

/*
ARBOL RECORRIDO INORDEN
10 - 15 - 20 - 25 - 30 - 35 - 40 - 45 - 50 - 55 - 60 - 65 - 66 - 75 - 80 - 85 - 90 - 94 - 95 - 96
*/
void pruebas_iterador_inorden(abb_t* arbol, int vector[MAX_VECTOR_GRANDE]){
	
	int vector_aux[MAX_VECTOR_GRANDE];

	ordenar_vector_inorden(vector_aux, vector);

	probar_iterar(arbol, ABB_RECORRER_INORDEN, vector_aux, 11);
}

/*
ARBOL RECORRIDO PREORDEN
50 - 25 - 20 - 15 - 10 - 35 - 30 - 40 - 45 - 75 - 60 - 55 - 65 - 66 - 90 - 80 - 85 - 95 - 94 - 96
*/
void pruebas_iterador_preorden(abb_t* arbol, int vector[MAX_VECTOR_GRANDE]){

	int vector_aux[MAX_VECTOR_GRANDE];

	ordenar_vector_preorden(vector_aux, vector);

	probar_iterar(arbol, ABB_RECORRER_PREORDEN, vector_aux, 11);
}

/*
ARBOL RECORRIDO POSTORDEN
10 - 15 - 20 - 30 - 45 - 40 - 35 - 25 - 55 - 66 - 65 - 60 - 85 - 80 - 94 - 96 - 95 - 90 - 75 - 50
*/
void pruebas_iterador_postorden(abb_t* arbol, int vector[MAX_VECTOR_GRANDE]){

	int vector_aux[MAX_VECTOR_GRANDE];

	ordenar_vector_postorden(vector_aux, vector);

	probar_iterar(arbol, ABB_RECORRER_POSTORDEN, vector_aux, 12);
}

void pruebas_iterador_interno(){
	
	abb_t* arbol = arbol_crear(comparar_numeros, NULL);

	int vector[MAX_VECTOR_GRANDE];
	int contador = 0;

	for (int i = 0; i < 100; i++){
		vector[i] = contador;
		contador++;
	}

	insertar_numeros(arbol, vector);

	pa2m_nuevo_grupo("ITERADOR INORDEN");
	pruebas_iterador_inorden(arbol, vector);

	pa2m_nuevo_grupo("ITERADOR PREORDEN");
	pruebas_iterador_preorden(arbol, vector);

	pa2m_nuevo_grupo("ITERADOR POSTORDEN");
	pruebas_iterador_postorden(arbol, vector);

	arbol_destruir(arbol);
}

void pruebas_de_error_y_null(){
	abb_t* arbol = NULL;
	int vector[MAX_VECTOR_CHICO];
	int tope = 10;
	int numero = 1;

	pa2m_afirmar(arbol_insertar(arbol, &numero) == ERROR, "No puedo insertar en un arbol nulo");
	pa2m_afirmar(arbol_borrar(arbol, &numero) == ERROR, "No puedo borrar en un arbol nulo");
	pa2m_afirmar(arbol_buscar(arbol, &numero) == NULL, "No puedo buscar en un arbol nulo");
	pa2m_afirmar(arbol_raiz(arbol) == NULL, "Un arbol nulo tiene raiz nula");
	pa2m_afirmar(arbol_vacio(arbol) == true, "Un arbol nulo está vacio");
	pa2m_afirmar(arbol_recorrido_inorden(arbol, (void**)vector, tope) == 0, "No puedo recorrer inorden un arbol nulo");
	pa2m_afirmar(arbol_recorrido_preorden(arbol, (void**)vector, tope) == 0, "No puedo recorrer preorden un arbol nulo");
	pa2m_afirmar(arbol_recorrido_postorden(arbol, (void**)vector, tope) == 0, "No puedo recorrer postorden un arbol nulo");
	pa2m_afirmar(abb_con_cada_elemento(arbol, ABB_RECORRER_INORDEN, almacenar_elementos, NULL) == 0,
		"No puedo iterar inorden un arbol nulo");
	pa2m_afirmar(abb_con_cada_elemento(arbol, ABB_RECORRER_PREORDEN, almacenar_elementos, NULL) == 0,
		"No puedo iterar preorden un arbol nulo");
	pa2m_afirmar(abb_con_cada_elemento(arbol, ABB_RECORRER_POSTORDEN, almacenar_elementos, NULL) == 0,
		"No puedo iterar postorden un arbol nulo");

	arbol = arbol_crear(comparar_numeros, NULL);

	pa2m_afirmar(arbol_vacio(arbol) == true, "Creo un arbol y esta vacio");
	pa2m_afirmar(arbol_borrar(arbol, &numero) == ERROR, "No puedo borrar en un arbol vacio");
	pa2m_afirmar(arbol_buscar(arbol, &numero) == NULL, "No puedo buscar en un arbol vacio");
	pa2m_afirmar(arbol_raiz(arbol) == NULL, "Un arbol vacio tiene raiz nula");
	pa2m_afirmar(arbol_recorrido_inorden(arbol, (void**)vector, tope) == 0, "No puedo recorrer inorden un arbol vacio");
	pa2m_afirmar(arbol_recorrido_preorden(arbol, (void**)vector, tope) == 0, "No puedo recorrer preorden un arbol vacio");
	pa2m_afirmar(arbol_recorrido_postorden(arbol, (void**)vector, tope) == 0, "No puedo recorrer postorden un arbol vacio");
	pa2m_afirmar(abb_con_cada_elemento(arbol, ABB_RECORRER_INORDEN, almacenar_elementos, NULL) == 0,
		"No puedo iterar inorden un arbol vacio");
	pa2m_afirmar(abb_con_cada_elemento(arbol, ABB_RECORRER_PREORDEN, almacenar_elementos, NULL) == 0,
		"No puedo iterar preorden un arbol vacio");
	pa2m_afirmar(abb_con_cada_elemento(arbol, ABB_RECORRER_POSTORDEN, almacenar_elementos, NULL) == 0,
		"No puedo iterar postorden un arbol vacio");	

	arbol_insertar(arbol, &numero);

	pa2m_afirmar(arbol_recorrido_inorden(arbol, NULL, tope) == 0, "No puedo recorrer inorden sin array");
	pa2m_afirmar(arbol_recorrido_inorden(arbol, (void**)vector, 0) == 0, "No puedo recorrer inorden con tope igual a cero");
	pa2m_afirmar(arbol_recorrido_preorden(arbol, NULL, tope) == 0, "No puedo recorrer preorden sin array");
	pa2m_afirmar(arbol_recorrido_preorden(arbol, (void**)vector, 0) == 0, "No puedo recorrer preorden con tope igual a cero");
	pa2m_afirmar(arbol_recorrido_postorden(arbol, NULL, tope) == 0, "No puedo recorrer postorden sin array");
	pa2m_afirmar(arbol_recorrido_postorden(arbol, (void**)vector, 0) == 0, "No puedo recorrer postorden con tope igual a cero");
	pa2m_afirmar(abb_con_cada_elemento(arbol, ABB_RECORRER_INORDEN, NULL, NULL) == 0,
		"No puedo iterar inorden sin funcion");
	pa2m_afirmar(abb_con_cada_elemento(arbol, ABB_RECORRER_PREORDEN, NULL, NULL) == 0,
		"No puedo iterar preorden sin funcion");
	pa2m_afirmar(abb_con_cada_elemento(arbol, ABB_RECORRER_POSTORDEN, NULL, NULL) == 0,
		"No puedo iterar postorden sin funcion");
	pa2m_afirmar(abb_con_cada_elemento(arbol, -1, almacenar_elementos, NULL) == 0, "No puedo iterar si mando un recorrido invalido");
	
	arbol_destruir(arbol);
}

int main(){

	pa2m_nuevo_grupo("PRUEBAS DE CREACION");
	pruebas_creacion_arbol();

	pa2m_nuevo_grupo("PRUEBAS DE INSERCION SIN DESTRUCTOR");
	pruebas_insercion_simple_sin_destructor();
	pruebas_insercion_moderada_sin_destructor();
	pruebas_insercion_extrema_sin_destructor();
	
	pa2m_nuevo_grupo("PRUEBAS DE BORRADO SIN DESTRUCTOR");
	pruebas_borrado_sin_destructor();

	pa2m_nuevo_grupo("PRUEBAS DE INSERCION CON DESTRUCTOR");
	pruebas_insercion_simple_con_destructor();
	pruebas_insercion_moderada_con_destructor();
	pruebas_insercion_extrema_con_destructor();

	pa2m_nuevo_grupo("PRUEBAS DE BORRADO CON DESTRUCTOR");
	pruebas_borrado_con_destructor();

	pa2m_nuevo_grupo("PRUEBAS RECORRIDOS");
	pruebas_recorridos();
	
	pa2m_nuevo_grupo("PRUEBAS DE ITERADOR INTERNO");
	pruebas_iterador_interno();
	
	pa2m_nuevo_grupo("PRUEBAS DE ERROR Y NULL");
	pruebas_de_error_y_null();

	pa2m_mostrar_reporte();
	return 0;
}