---------------------------------------------------------------------------------
---------------------------------PRIMERA SECCION---------------------------------
---------------------------------------------------------------------------------


¿QUÉ ES LO ENTREGADO?
El trabajo se trata de una implementacion de un Tipo de Dato Abstracto (TDA)
denominado Arbol Binario de Busqueda (ABB). Uno de los objetivos del trabajo se
basa en diseñar funciones primtivas lo menos complejas posible, a pesar de que
ésto esté arraigado a la entrada de datos al arbol.

Sus funciones primitivas son:

-arbol_crear(): reserva la memoria necesaria para un struct que tiene un puntero
al nodo_raiz, un comparador y un destructor cuyas estructuras quedan a cargo del
usuario. Tener en cuenta que para la creacion del arbol es imprescindible un
comparador valido, en cambio el destructor no es necesario. Depende exclusivamente
de lo que el usuario desee almacenar en el arbol.

-arbol_insertar(): inserta un elemento dentro del arbol (reservando la memoria
necesaria para cada nodo) utilizando el comparador que fue asignado en la creacion,
de forma tal que se vaya comparando con los elementos dentro del arbol hasta
encontrar su lugar más adecuado. De esta forma se logra que el arbol siga siento
un arbol binario de busqueda.
La politica mas general de insercion se basa en: los elementos menores a la izquierda,
los elementos mayores a la derecha. Aunque esto depende de como el usuario hace su
funcion de comparacion.

-arbol_borrar(): elimina un elemento especifico dentro del arbol (liberando la
memoria del nodo reservada, y en caso de que el elemento tambien este en el heap),
ademas de mantener la estructura del arbol para que siga siendo de busqueda dependiendo
de cuantos hijos tiene el nodo que almancena el elemento a borrar.

-arbol_buscar(): busca un elemento especifico dentro del arbol, utilizando el
comparador para recorrerlo.

-arbol_raiz(): devuelve la raiz del arbol en caso de que haya.

-arbol_vacio(): informa si el arbol está vacio devolviendo 'true' o 'false'.

-arbol_recorrido_inorden(): recorre el arbol en secuencia inorden almacenando los
elementos en un vector hasta el tope del vector, o si el tope es mayor a la cantidad
de elementos, recorre hasta el ultimo elemento del arbol.

-arbol_recorrido_preorden(): recorre el arbol en secuencia preorden almacenando los
elementos en un vector hasta el tope del vector, o si el tope es mayor a la cantidad
de elementos, recorre hasta el ultimo elemento del arbol.

-arbol_recorrido_postorden(): recorre el arbol en secuencia postorden almacenando los
elementos en un vector hasta el tope del vector, o si el tope es mayor a la cantidad
de elementos, recorre hasta el ultimo elemento del arbol. Devuelve la cantidad de
elementos que pudo recorrer.

-abb_con_cada_elemento(): itera el arbol e interacta con los elementos segun el tipo
de recorrido que recibe, y segun lo que devuelva la funcion que recibe, devolviendo
la cantidad de elementos que puro iterar.

-arbol_destruir(): libera toda la memoria reservada tanto de los nodos que almacenan
los elementos como el struct de arbol que se crea al principio. Además, en caso de
haber recibido un destructor valido, lo llama con cada elemento del arbol, sin excepcion.

---------------------------------------------------------------------------------

COMPILACION

[SOLO CORRE DE MANERA CORRECTA EN UNA TERMINAL DE LINUX QUE TENGA GCC, MAKEFILE Y VALGRIND INSTALADOS]

Para la compilacion de las pruebas del arbol se brinda un archivo Makefile cuyos
comandos por terminal son:

-Para las minipruebas:
	'make ejecutar_minipruebas'
-Para comprobar el correcto uso de memoria dinamica en las minipruebas:
	'make valgrind_minipruebas'

-Para las pruebas mas complejas:
	'make ejecutar_testing'
-Para comprobar el correcto uso de memoria dinamica en las pruebas mas complejas:
	'make valgrind_testing'

---------------------------------------------------------------------------------

ACLARACION
Para la realizacion de las pruebas mas complejas se utiliza el 'pa2m.h', el cual
es una implementacion ofrecida por la cátedra.


---------------------------------------------------------------------------------
---------------------------------SEGUNDA SECCION---------------------------------
---------------------------------------------------------------------------------


¿Qué es un ABB?
Un Arbol Binario de Busqueda es un tipo de dato abstracto que se basa en un nodo
raiz que almacena un elemento de cualquier tipo, y que tiene un hijo derecho y un
hijo izquierdo; a la vez estos hijos tambien son nodos que almacenan elementos
de cualquier tipo y tiene un hijo derecho y un hijo izquierdo. La particularidad
del ABB es que para insertar un elemento se compara con los elementos ya insertados
de forma tal que se cree un orden donde los elementos menores se almacenan a la
izquierda del arbol y los elementos mayores se almacenen a la derecha del arbol.

¿Cómo se diferencia un ABB de un Arbol Binario?
La principal diferencia entre estos dos TDAs es que el Arbol Binario no se ve
obligado a seguir una regla para insertar elementos. La insercion no depende del
valor del elemento ni de los ya insertados.
La otra diferencia está en que en un arbol binario, su funcion de busqueda nunca
va a poder tener complejidad O(Log(n)). Esto debido a que en un ABB la funcion de
busqueda se intenta asemejar a la busqueda binaria, en cambio en un arbol binario
para buscar se debe definir un inicio y un fin (o un recorrido) para comenzar la
busqueda, ya que no se posee un comparador, lo que evitaria recorrer elementos
innecesariamente. Sucede lo mismo en la funcion de borrado.

---------------------------------------------------------------------------------

¿Cuál es el objetivo de tener una funcion de destrucción en el TDA?
La principal razon por la cual se le pide al usuario implementar un destructor
es porque no se puede asumir de entrada que tipo de dato quiere almancenar dentro
del arbol. Puede ser un entero, un caracter, o bien puede ser una direccion de
memoria que se reservo en el heap. Por esta razon, al momento de destruir el arbol
hay que tener precaucion de que elementos se almacenaron.

¿Qué implicaría para el usuario no tener una funcion de destruccion?
Una de las consecuencias mas evidentes es que, por ejemplo, si almacena direcciones
de memoria al heap se pierda memoria, lo cual si se genera a gran escala dificulta
la correcta ejecucion del programa.

---------------------------------------------------------------------------------

¿Cuál es la complejidad de las diferentes operaciones del ABB?

-arbol_crear(): su cumplejidad es O(1) debido a que solo realiza operaciones simples.
No realiza operaciones iterativas ni recursivas.

-arbol_insertar(): el mejor de los casos su complejidad es O(1) donde no haya arbol.
En el caso promedio su complejidad es O(Log(n)) ya que el arbol se encontraria
balanceado, de tal forma que le permita recorrer el arbol como una busqueda
binaria.
En el peor de los casos su complejidad es O(n) en caso de que la forma en que se
ingresaron los elementos hicieron degenerar al arbol en una lista simplemente enlazada.

-arbol_borrar(): el mejor de los casos su complejidad es O(1) donde no haya arbol o
no haya nodo raiz.
En el caso promedio su complejidad es O(Log(n)) ya que el arbol se encontraria
balanceado, de tal forma que le permita recorrer el arbol como una busqueda
binaria.
En el peor de los casos su complejidad es O(n) en caso de que la forma en que se
ingresaron los elementos hicieron degenerar al arbol en una lista simplemente enlazada.

-arbol_buscar(): el mejor de los casos su complejidad es O(1) donde no haya arbol o
no haya nodo raiz.
En el caso promedio su complejidad es O(Log(n)) ya que el arbol se encontraria
balanceado, de tal forma que le permita recorrer el arbol como una busqueda
binaria.
En el peor de los casos su complejidad es O(n) en caso de que la forma en que se
ingresaron los elementos hicieron degenerar al arbol en una lista simplemente enlazada.

-arbol_raiz(): su cumplejidad es O(1) debido a que solo realiza operaciones simples.
No realiza operaciones iterativas ni recursivas.

-arbol_vacio(): su cumplejidad es O(1) debido a que solo realiza operaciones simples.
No realiza operaciones iterativas ni recursivas.

-arbol_recorrido_inorden(): en el mejor de los casos su complejidad es O(1) donde no haya
arbol o no haya nodo raiz.
Tanto el caso promedio como el peor caso es O(n) ya que se intentaran recorrer todos los
elementos del arbol.

-arbol_recorrido_preorden(): en el mejor de los casos su complejidad es O(1) donde no haya
arbol o no haya nodo raiz.
Tanto el caso promedio como el peor caso es O(n) ya que se intentaran recorrer todos los
elementos del arbol.

-arbol_recorrido_postorden(): en el mejor de los casos su complejidad es O(1) donde no haya
arbol o no haya nodo raiz.
Tanto el caso promedio como el peor caso es O(n) ya que se intentaran recorrer todos los
elementos del arbol.

-abb_con_cada_elemento(): en el mejor de los casos su complejidad es O(1) donde no haya
arbol o no haya nodo raiz.
Tanto el caso promedio como el peor caso es O(n) ya que se intentaran recorrer todos los
elementos del arbol.

-arbol_destruir(): en el mejor de los casos su complejidad es O(1) donde no haya
arbol o no haya nodo raiz.
Tanto el caso promedio como el peor caso es O(n) ya que se intentaran recorrer todos los
elementos del arbol.
