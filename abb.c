#include "abb.h"

#define ERROR -1
#define EXITO 0


/* Reserva memoria para un nodo de ABB y lo inicializa.
 * Recibe un puntero a elemento valido.
 * Devuelve un puntero al nuevo nodo o NULL en caso de error.
*/
nodo_abb_t* insertar_nodo(void* elemento){

	nodo_abb_t* nodo_nuevo = malloc(sizeof(nodo_abb_t));

	if (nodo_nuevo){
		(*nodo_nuevo).elemento = elemento;
		(*nodo_nuevo).derecha = NULL;
		(*nodo_nuevo).izquierda = NULL;
	}

	return nodo_nuevo;
}

/* Crea el arbol y reserva la memoria necesaria de la estructura.
 * Comparador se utiliza para comparar dos elementos.
 * Destructor es invocado sobre cada elemento que sale del arbol,
 * puede ser NULL indicando que no se debe utilizar un destructor.
 *
 * Devuelve un puntero al arbol creado o NULL en caso de error.
 */
abb_t* arbol_crear(abb_comparador comparador, abb_liberar_elemento destructor){

	if (!comparador) return NULL;

	abb_t* arbol = malloc(sizeof(abb_t));

	if (arbol){
		(*arbol).nodo_raiz = NULL;
		(*arbol).comparador = comparador;
		(*arbol).destructor = destructor;
	}

	return arbol;
}

/* Recorre el arbol para insertar un elemento en él.
 * Recibe un nodo que puede ser valido o no, un puntero al elemento valido,
 * un comparador valido y un puntero a 'hay_error'.
 * Devuelve un nuevo nodo o NULL en caso de error.
*/
nodo_abb_t* recorrer_para_insertar(nodo_abb_t* nodo_actual, void* elemento, abb_comparador comparador, bool* hay_error){

	if (!nodo_actual){

		nodo_actual = insertar_nodo(elemento);

		if (!nodo_actual) (*hay_error) = true;
	}
	else {
		int comparacion = comparador((*nodo_actual).elemento, elemento);

		if (comparacion < 0)
			(*nodo_actual).derecha = recorrer_para_insertar((*nodo_actual).derecha, elemento, comparador, hay_error);
		else
			(*nodo_actual).izquierda = recorrer_para_insertar((*nodo_actual).izquierda, elemento, comparador, hay_error);
	}

	return nodo_actual;
}


/* Inserta un elemento en el arbol.
 * Devuelve 0 si pudo insertar o -1 si no pudo.
 * El arbol admite elementos con valores repetidos.
 */
int arbol_insertar(abb_t* arbol, void* elemento){

	if (!arbol) return ERROR;

	bool hay_error = false;
	
	(*arbol).nodo_raiz = recorrer_para_insertar((*arbol).nodo_raiz, elemento, (*arbol).comparador, &hay_error);

	if (hay_error) return ERROR;

	return EXITO;
}

/* Busca el elemento predecesor inorden.
 * Recibe un nodo valido.
 * Devuelve un puntero al elemento buscado.
*/
void* buscar_predecesor_inorden(nodo_abb_t* nodo_actual){

	if ((*nodo_actual).derecha)
		return buscar_predecesor_inorden((*nodo_actual).derecha);
	
	return (*nodo_actual).elemento;
}

/* Busca el nodo perteneciente al predecesor inorden y lo libera.
 * Recibe un nodo valido.
 * Devuelve NULL o un nodo del arbol para mantener su estructura.
*/
nodo_abb_t* borrar_predecesor_inorden(nodo_abb_t* nodo_actual){

	if ((*nodo_actual).derecha){
		(*nodo_actual).derecha = borrar_predecesor_inorden((*nodo_actual).derecha);
	}
	else {
		if (!(*nodo_actual).izquierda){
			free(nodo_actual);
			nodo_actual = NULL;
		}
		else{
			nodo_abb_t* nodo_aux = (*nodo_actual).izquierda;
			free(nodo_actual);
			nodo_actual = nodo_aux;
		}
	}

	return nodo_actual;
}

/* Recorre el arbol para borrar el elemento que recibe (y, de ser necesario,
 * el nodo donde está almacenado).
 * Recibe un arbol valido, un nodo valido, un puntero a elemento valido y un
 * puntero a 'hay_error'
 * Devuelve NULL o un nodo del arbol para mantener su estructura.
*/
nodo_abb_t* recorrer_para_borrar(abb_t* arbol, nodo_abb_t* nodo_actual, void* elemento, bool* hay_error){

	if (!nodo_actual){
		(*hay_error) = true;
		return nodo_actual;
	}

	int comparacion = (*arbol).comparador((*nodo_actual).elemento, elemento);

	if (comparacion == 0){
		
		if ((*arbol).destructor) (*arbol).destructor((*nodo_actual).elemento);

		if (!(*nodo_actual).izquierda){

			if (!(*nodo_actual).derecha){
				free(nodo_actual);
				nodo_actual = NULL;
			}
			else {
				nodo_abb_t* nodo_aux = (*nodo_actual).derecha;
				free(nodo_actual);
				nodo_actual = nodo_aux;
			}
		}
		else {
			(*nodo_actual).elemento = buscar_predecesor_inorden((*nodo_actual).izquierda);
			(*nodo_actual).izquierda = borrar_predecesor_inorden((*nodo_actual).izquierda);
		}
	}
	else if (comparacion < 0){
		(*nodo_actual).derecha = recorrer_para_borrar(arbol, (*nodo_actual).derecha, elemento, hay_error);
	}
	else {
		(*nodo_actual).izquierda = recorrer_para_borrar(arbol, (*nodo_actual).izquierda, elemento, hay_error);
	}

	return nodo_actual;
}

/* Busca en el arbol un elemento igual al provisto (utilizando la
 * funcion de comparación) y si lo encuentra lo quita del arbol.
 * Adicionalmente, si encuentra el elemento, invoca el destructor con
 * dicho elemento.
 * Devuelve 0 si pudo eliminar el elemento o -1 en caso contrario.
 */
int arbol_borrar(abb_t* arbol, void* elemento){

	if (!arbol) return ERROR;

	bool hay_error = false;

	(*arbol).nodo_raiz = recorrer_para_borrar(arbol, (*arbol).nodo_raiz, elemento, &hay_error);

	if (hay_error) return ERROR;

	return EXITO;
}

/* Recorre el arbol para buscar el elemento que recibe comparando nodo por nodo
 * si es necesario.
 * Recibe un nodo valido, un puntero a elemento valido y un comparador valido.
 * Devuelve el elemento encontrado o NULL si no lo encontró.
*/
void* recorrer_para_buscar(nodo_abb_t* nodo_actual, void* elemento, abb_comparador comparador){

	if (!nodo_actual) return nodo_actual;

	int comparacion = comparador((*nodo_actual).elemento, elemento);

	if (comparacion == 0)
		return (*nodo_actual).elemento;

	if (comparacion < 0)
		return recorrer_para_buscar((*nodo_actual).derecha, elemento, comparador);

	return recorrer_para_buscar((*nodo_actual).izquierda, elemento, comparador);
}

/* Busca en el arbol un elemento igual al provisto (utilizando la
 * funcion de comparación).
 *
 * Devuelve el elemento encontrado o NULL si no lo encuentra.
 */
void* arbol_buscar(abb_t* arbol, void* elemento){

	if (!arbol) return NULL;

	return recorrer_para_buscar((*arbol).nodo_raiz, elemento, (*arbol).comparador);
}

/* Devuelve el elemento almacenado como raiz o NULL si el árbol está
 * vacío o no existe.
 */
void* arbol_raiz(abb_t* arbol){
	
	if (!arbol || !(*arbol).nodo_raiz) return NULL;

	return (*(*arbol).nodo_raiz).elemento;
}

/* Determina si el árbol está vacío.
 * Devuelve true si está vacío o el arbol es NULL, false si el árbol tiene elementos.
 */
bool arbol_vacio(abb_t* arbol){
	return (!arbol || !(*arbol).nodo_raiz);
}

/* Itera el arbol segun el recorrido inorden hasta que 'cant_elementos' sea
 * igual a 'tope'. Y a medida que visita elementos los almacena en un array
 * que recibe.
 * Recibe un puntero nodo valido, un array de tipo void**, el tope del array y
 * un puntero a 'cant_elementos' inicializado.
 * Devuelve el puntero a 'cant_elementos' aumentado.
*/
void recorrer_inorden(nodo_abb_t* nodo_actual, void** array, size_t tope, size_t* cant_elementos){

	if ((*nodo_actual).izquierda && (*cant_elementos) < tope)
		recorrer_inorden((*nodo_actual).izquierda, array, tope, cant_elementos);

	if ((*cant_elementos) < tope){
		array[*cant_elementos] = (*nodo_actual).elemento;	
		(*cant_elementos)+= 1;
	}

	if ((*nodo_actual).derecha && (*cant_elementos) < tope)
		recorrer_inorden((*nodo_actual).derecha, array, tope, cant_elementos);
}

/* Llena el array del tamaño dado con los elementos de arbol
 * en secuencia inorden.
 * Devuelve la cantidad de elementos del array que pudo llenar (si el
 * espacio en el array no alcanza para almacenar todos los elementos,
 * llena hasta donde puede y devuelve la cantidad de elementos que
 * pudo poner).
 */
size_t arbol_recorrido_inorden(abb_t* arbol, void** array, size_t tamanio_array){
	
	size_t cant_elementos = 0;

	if (!arbol || !(*arbol).nodo_raiz || !array || tamanio_array == 0) return cant_elementos;
	
	recorrer_inorden((*arbol).nodo_raiz, array, tamanio_array, &cant_elementos);

	return cant_elementos;
}

/* Itera el arbol segun el recorrido preorden hasta que 'cant_elementos' sea
 * igual a 'tope'. Y a medida que visita elementos los almacena en un array
 * que recibe.
 * Recibe un puntero nodo valido, un array de tipo void**, el tope del array y
 * un puntero a 'cant_elementos' inicializado.
 * Devuelve el puntero a 'cant_elementos' aumentado.
*/
void recorrer_preorden(nodo_abb_t* nodo_actual, void** array, size_t tope, size_t* cant_elementos){

	if ((*cant_elementos) < tope){
		array[*cant_elementos] = (*nodo_actual).elemento;	
		(*cant_elementos)+= 1;
	}

	if ((*nodo_actual).izquierda && (*cant_elementos) < tope)
		recorrer_preorden((*nodo_actual).izquierda, array, tope, cant_elementos);

	if ((*nodo_actual).derecha && (*cant_elementos) < tope)
		recorrer_preorden((*nodo_actual).derecha, array, tope, cant_elementos);
}

/* Llena el array del tamaño dado con los elementos de arbol
 * en secuencia preorden.
 * Devuelve la cantidad de elementos del array que pudo llenar (si el
 * espacio en el array no alcanza para almacenar todos los elementos,
 * llena hasta donde puede y devuelve la cantidad de elementos que
 * pudo poner).
 */
size_t arbol_recorrido_preorden(abb_t* arbol, void** array, size_t tamanio_array){
	
	size_t cant_elementos = 0;

	if (!arbol || !(*arbol).nodo_raiz || !array || tamanio_array == 0) return cant_elementos;
	
	recorrer_preorden((*arbol).nodo_raiz, array, tamanio_array, &cant_elementos);

	return cant_elementos;
}

/* Itera el arbol segun el recorrido postorden hasta que 'cant_elementos' sea
 * igual a 'tope'. Y a medida que visita elementos los almacena en un array
 * que recibe.
 * Recibe un puntero nodo valido, un array de tipo void**, el tope del array y
 * un puntero a 'cant_elementos' inicializado.
 * Devuelve el puntero a 'cant_elementos' aumentado.
*/
void recorrer_postorden(nodo_abb_t* nodo_actual, void** array, size_t tope, size_t* cant_elementos){

	if ((*nodo_actual).izquierda && (*cant_elementos) < tope)
		recorrer_postorden((*nodo_actual).izquierda, array, tope, cant_elementos);

	if ((*nodo_actual).derecha && (*cant_elementos) < tope)
		recorrer_postorden((*nodo_actual).derecha, array, tope, cant_elementos);

	if ((*cant_elementos) < tope){
		array[*cant_elementos] = (*nodo_actual).elemento;	
		(*cant_elementos)+= 1;
	}
}

/* Llena el array del tamaño dado con los elementos de arbol
 * en secuencia postorden.
 * Devuelve la cantidad de elementos del array que pudo llenar (si el
 * espacio en el array no alcanza para almacenar todos los elementos,
 * llena hasta donde puede y devuelve la cantidad de elementos que
 * pudo poner).
 */
size_t arbol_recorrido_postorden(abb_t* arbol, void** array, size_t tamanio_array){

	size_t cant_elementos = 0;

	if (!arbol || !(*arbol).nodo_raiz || !array || tamanio_array == 0) return cant_elementos;
	
	recorrer_postorden((*arbol).nodo_raiz, array, tamanio_array, &cant_elementos);

	return cant_elementos;
}

/* Recorre todo el arbol liberando tanto los nodos que lo forman
 * como los elementos que éstos almacenan, solo en el caso que
 * reciba un destructor valido.
 * Recibe un nodo valido, y un puntero a la funcion destructor.
 * Devuelve los nodos del arbol y los elementos de los nodos liberados.
*/
void recorrer_para_destruir(nodo_abb_t* nodo_actual, abb_liberar_elemento destructor){

	if (!nodo_actual) return;

	if ((*nodo_actual).izquierda)
		recorrer_para_destruir((*nodo_actual).izquierda, destructor);

	if ((*nodo_actual).derecha)
		recorrer_para_destruir((*nodo_actual).derecha, destructor);

	if (destructor) destructor((*nodo_actual).elemento);

	free(nodo_actual);
}

/* Destruye el arbol liberando la memoria reservada por el mismo.
 * Adicionalmente invoca el destructor con cada elemento presente en
 * el arbol.
 */
void arbol_destruir(abb_t* arbol){

	if (!arbol) return;

	if ((*arbol).nodo_raiz)
		recorrer_para_destruir((*arbol).nodo_raiz, (*arbol).destructor);
	
	free(arbol);
}

/* Iterador inorden. Itera el arbol segun el recorrido inorden hasta que el
 * puntero a funcion que recibe devuelve 'true'. Y a medida que visita elementos
 * aumenta el puntero cant_elementos.
 * Recibe un puntero nodo valido, un puntero a funcion valido, un puntero a 'extra',
 * un puntero a 'cant_elementos' inicializado y un puntero a 'hay que seguir' inicializado.
 * Devuelve la cantidad de elementos que pudo visitar.
*/
void iterar_inorden(nodo_abb_t* nodo_actual, bool (*funcion)(void*, void*), void* extra, size_t* cant_elementos, bool* hay_que_seguir){

	if ((*nodo_actual).izquierda && *hay_que_seguir)
		iterar_inorden((*nodo_actual).izquierda, funcion, extra, cant_elementos, hay_que_seguir);
	
	if (*hay_que_seguir){

		if (funcion((*nodo_actual).elemento, extra))
			(*hay_que_seguir) = false;
		
		(*cant_elementos)+=1;
	}

	if ((*nodo_actual).derecha && *hay_que_seguir)
		iterar_inorden((*nodo_actual).derecha, funcion, extra, cant_elementos, hay_que_seguir);
}

/* Iterador preorden. Itera el arbol segun el recorrido preorden hasta que el
 * puntero a funcion que recibe devuelve 'true'. Y a medida que visita elementos
 * aumenta el puntero cant_elementos.
 * Recibe un puntero nodo valido, un puntero a funcion valido, un puntero a 'extra',
 * un puntero a 'cant_elementos' inicializado y un puntero a 'hay que seguir' inicializado.
 * Devuelve la cantidad de elementos que pudo visitar.
*/
void iterar_preorden(nodo_abb_t* nodo_actual, bool (*funcion)(void*, void*), void* extra, size_t* cant_elementos, bool* hay_que_seguir){

	if (*hay_que_seguir){

		if (funcion((*nodo_actual).elemento, extra))
			(*hay_que_seguir) = false;

		(*cant_elementos)+=1;
	}

	if ((*nodo_actual).izquierda && *hay_que_seguir)
		iterar_preorden((*nodo_actual).izquierda, funcion, extra, cant_elementos, hay_que_seguir);

	if ((*nodo_actual).derecha && *hay_que_seguir)
		iterar_preorden((*nodo_actual).derecha, funcion, extra, cant_elementos, hay_que_seguir);
}

/* Iterador postorden. Itera el arbol segun el recorrido postorden hasta que el
 * puntero a funcion que recibe devuelve 'true'. Y a medida que visita elementos
 * aumenta el puntero cant_elementos.
 * Recibe un puntero nodo valido, un puntero a funcion valido, un puntero a 'extra',
 * un puntero a 'cant_elementos' inicializado y un puntero a 'hay que seguir' inicializado.
 * Devuelve la cantidad de elementos que pudo visitar.
*/
void iterar_postorden(nodo_abb_t* nodo_actual, bool (*funcion)(void*, void*), void* extra, size_t* cant_elementos, bool* hay_que_seguir){

	if ((*nodo_actual).izquierda && *hay_que_seguir)
		iterar_postorden((*nodo_actual).izquierda, funcion, extra, cant_elementos, hay_que_seguir);

	if ((*nodo_actual).derecha && *hay_que_seguir)
		iterar_postorden((*nodo_actual).derecha, funcion, extra, cant_elementos, hay_que_seguir);

	if (*hay_que_seguir){
	
		if (funcion((*nodo_actual).elemento, extra))
			(*hay_que_seguir) = false;

		(*cant_elementos)+=1;
	}
}

/* Iterador interno. Recorre el arbol e invoca la funcion con cada
 * elemento del mismo. El puntero 'extra' se pasa como segundo
 * parámetro a la función. Si la función devuelve true, se finaliza el
 * recorrido aun si quedan elementos por recorrer. Si devuelve false
 * se sigue recorriendo mientras queden elementos.
 * El recorrido se realiza de acuerdo al recorrido solicitado.  Los
 * recorridos válidos son: ABB_RECORRER_INORDEN, ABB_RECORRER_PREORDEN
 * y ABB_RECORRER_POSTORDEN.
 * Devuelve la cantidad de elementos que fueron recorridos.
*/
size_t abb_con_cada_elemento(abb_t* arbol, int recorrido, bool (*funcion)(void*, void*), void* extra){
	
	size_t cant_elementos = 0;
	bool hay_que_seguir = true;

	if (!arbol || !(*arbol).nodo_raiz || !funcion) return cant_elementos;

	if (recorrido == ABB_RECORRER_INORDEN){
		iterar_inorden((*arbol).nodo_raiz, funcion, extra, &cant_elementos, &hay_que_seguir);
	}
	else if (recorrido == ABB_RECORRER_PREORDEN){
		iterar_preorden((*arbol).nodo_raiz, funcion, extra, &cant_elementos, &hay_que_seguir);
	}
	else if (recorrido == ABB_RECORRER_POSTORDEN){
		iterar_postorden((*arbol).nodo_raiz, funcion, extra, &cant_elementos, &hay_que_seguir);
	}

	return cant_elementos;
}