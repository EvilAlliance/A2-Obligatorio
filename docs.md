#  Obligatorio de Estructura de Datos y Algoritmos 2

- Nombre: Pedro Benjamín Chialanza Arrieta
- Número de Estudiante: 302782

## Ejercicio 01

### Solución

La solución de este ejerció es indicado por la misma letra que indica el uso de la estructura AVL.

### Justificación de Orden de Cada Acción

#### ADD

La acción ADjD tiene que ser de orden O(log(N)) este comando crea el libro y recibe los datos por el stdin y los almacena en el dicho libro y lo inserta utilizando el método set del AVL.
Set llama a un método recursivo que insertá el libro en la raíz si no tiene y utilizando la función compareKey que en este caso es constante para saber si es menor, igual o mayor, si es igual se modifica este nodo, por ahora todo esto sería constante, si no dependiendo de si es menor o mayor va a la rama izquierda o derecha, repitiéndose constantemente hasta que suceda la situación mencionada al principio.
Para tener la cuenta de los libros habilitados y deshabilitados hay una funcion insertHandler que mantiene la cuneta que en este caso seria constante.

#### FIND

La acción FIND recibe una key por el stdin y la busca en el árbol con el método get que este busca la key en los nodos de dicho árbol de tal forma que va a la rama izquierda si la key buscada es menor a la del nodo y va a la rama derecha si este es mayor, si es igual entonces encontró la key buscada. Esto se repite hasta encontrarla, logrando un O(log(N)).
Después se imprime si se encontró el libro el nombre y si está habilitado o no, si no informa que no se encontró, todo esto es constante.

#### TOGGLE

La accion TOGGLE recibe la key por el srdin y la busca utilizando el metodo get explicado en la accion anterior. Informa si no lo encotro y si lo encuontra deshabilitado lo habilita y viceversa.
Esto tambien mantiene la constancia de la misma forma que la insercion

#### COUNT

La acción COUNT es constante porque extrae todos los datos de una estructura que mantiene la constancia de los libros deshabilitados y habilitados.
Esta estructura es la que mantiene la constancia y es la misma que se menciona en las acciones ADD y TOGGLE.

## Ejercicio 02

### Solución

La solución de este ejerció es indicado por la misma letra que indica el uso de la estructura tabla hash cerrado con solucion de colisión doble hash.

### Justificación de Orden de Cada Acción

#### ADD

La acción ADD tiene que ser de orden O(1) promedio este comando crea el libro y recibe los datos por el stdin y los almacena en el dicho libro y lo inserta utilizando el método set de la tabla hash.
Esta funcion almacena la informacion de la siguiente forma, se calcula el hash de la key y se calcula un segundo hash por la estrategia de colision doble hash esto asegura que sea de orden constatnte, pero hay un caso que se no encuetra un lugar despues de n intentos se va a rehashear y instentar de vuelta. Este rehash tambien sucede cundo el factor de carga llega a ser de un 70%.
Este tambien tiene un insertHandler de igual forma que el ejercicio anterior para mantener la constancia de libros habilitod y deshabilitados.

#### FIND

La acción FIND recibe una key por el stdin y busca el elemento utilizando el método get, este funciona de una forma similar al set, calcula el primer hash y el segundo hash, buscando la misma key, esto frena cuando la encuentra o cuando encuentra una celda vacía, logrando un orden constante.

#### TOGGLE

La acción TOGGLE recibe la key por el stdin y la busca utilizando el método get explicado en la acción anterior. Informa si no lo encontró y si lo encuentra deshabilitado lo habilita y viceversa.
Esto también mantiene la constancia de la misma forma que la inserción.

#### COUNT

La acción COUNT es constante porque extrae todos los datos de una estructura que mantiene la constancia de los libros deshabilitados y habilitados.
Esta estructura es la que mantiene la constancia y es la misma que se menciona en las acciones ADD y TOGGLE.

## Ejercicio 3

### Solucion

Al tener varios objetos repetidos y elegir el que tiene menor precio se usuaria un minheap y un hashtable para no comprar obejetos repetidos.

### Justificación de Orden de Cada Acción

La justificacion del HashTable ya ta hecha en el ejericio 2 

#### Push
La accion PUSH recibe el elemento y lo inserta en el en el primer lugar disponible y lo flota, estadisticamente en un heap ya ordenado seria O(1) caso promedo (estadisticamente) y O(log(N)) ya que puede ir a desde el ultimo al primero (es log(N) porque el heap es la representacion de un arbol en un array).

#### Pop
La accion POP devuelve el valor y sobre escribe el primer lugar por el ultimo elemento y lo hunde, por la msima razon que el flotar es promedio O(1) y O(log(N), la unica diferencia seria que uno trae el elemento hacia el indice 0 y el otro lo lleva al indice count.

#### Levitate
Seria la accion de flotar que fue explicada en el Push

#### Sink
Seria la accion de hundir que fue explicada en el Pop

## Ejercicio 4

### Solucion
Se utilizo el heap para mantener el un order y un hashmap para ver si el pedido ya fue entregado

### Justificacion de Orden de Cada Accion

#### I

La accion "I", construyelos objetos necesario con los datos entregado por stdin. Utilizando push de HashHeap que es muy similar a push Heap del ejercicio 3, la unica diferencia es que cuando inserte el el objeto guardo la key y el indice donde se encuentra y levitate guarda los indices nuevos de las key utilizado la funcion set de HashTable explicada en el ejericio 2 con un O(1) promedio mantendio el mismo orden indicado en el ejercio 2 del push (O(log(N))). Luego utiliza set de Hashtable explicado en el ejericio 2.

#### E

La accion "E" utilizando el id remueve el dato relacionado con el en la HashHeap. Esto lo logra usando el metodo de remove de HashTable, funciona igual que el set, lo unica diferencia es que en vez de buscar un lugar libre, busca a la key indicada, y le cambia un dato para que quede "borrado" (order O(1) promedio).

#### C

Con el id recicibido armo un obeto para buscar la celda del HashHeap para modificar la key menos el id, y luego rebalancea la celda. Consigue la celda con el metodo getCell de HashHeap y utiliza el metodo de set HashTable para conseguir el indice y devuelve la celda en ese lugar. Luego podes rebalancear la celda pasandole el objeto de pioridad para conseguir el indice donde se encuentra y lo hunde  y lo flota. Para conseguir el indece se utiliza el metodo get de HashTable explicado en el ejericio 2, y levitate y sink utiliza set de HashTable para mantener constancia del indice de cada uno manteneido su O(1) promedio estadisticamente.

#### Resolucion

El print utiliza get de HashTable que fue explicado en el ejericio 2 y el pop de HashHeap que es my similar a el de Heap simplemente elimina la key del HashTable dentro del HashHeap.

## Ejercicio 5

### Solucion

Se armo dos SparseGraph (Grafo Disperso) de misiones y cuidades. Se hace un dijkstra de de las ciudades con el origen siendo el origen brindado, y el dijkstra de cada cuidad para despues de hacer un order topologico de las misiones.

### Justificacion de Ordenes

#### Dijkstra

Le pasas por parametro el vertice de origen y busca el indice de donde esta ubicado en el array de verices y aristas utilizando get de HashTable (explicado en el ejercicio 2), creo un Heap (todos los ordenes explicados en el ejercicio 2) en el cual le inserto 0 ademas del indice del vertice y empieza la a ver si ya se llego a sus vertices adyacentes y si no a llegado y el costo es menor al que ya se habia guardado lo agrega al Heap, si ya lo visito lo saltea, O((V + E)log(V)). Todo esto lo hace mientra guarda los resultoado y los devuelve.

#### Orden Topologico

- Hago un array que contiene su la incidencia de cada vetice.
- Los que tienen incidencia 0 los agrego en un Heap con pioridad siendo la distancia de la cuidades origen.
- Mientras haya algun elemento en el heap lo siguente se repite
    - Consigo el index y imprimo el camino para llegar a la mision (Hago la mision).
    - Reseteo el Heap esto deberia ser orden constante (lo discuti en clase practica) porque al ser un grafo disperso ya son un 30% de aritas de los posibles (misiones) y luego tenemos que tener en cuenta que el Heap contiene los de incidencia cero, se le pone la distancia nueva del nuevo lugar de origen.
    - Resto la incidencia y si es cero lo agrego al heap

#### findVertex

Es orden constante utiliza un HashTable para conseguir el indice de un vertice en el grafo.

## Ejercicio 6

### Solucion

Uso el dijkstra mencionado anteriormente y entre medio de cada uno llamo una funcion que actualiza los caminos tomados duplicando su costo. Creo dos grafos una para desactivar primero la Entidad y el otro buscar el equipo, por ultimo los comaparo y los imprimo como las pruebas lo pide.

### Justificacion de Ordenes

El orden del dijkstra es el mismo que el ejercicio 6 siendo O((V + E)log(V)) y la la funcion de updatePath que seria el peor caso O(E) (porque puede recorrer todo los aristas). Siendo el orden del ejercicio completo O(2((V + E)log(V) + E) + 2(V + E)log(V)), siendo O((V + E)log(V)).

## Ejercicio 7

### Solucion 

La solucion consiste en un Divide and Conquer y un Greedy, si consideramos una barra de tiempo total de las canciones, se toma uno de base valido (que se escuchan todas las canciones) y se hace el divide se prueba de los dos lados y actualizo la base (con el de menor tiempo) y se repite, pero tambien con cada repeticion se achica la distacia la cual resta o se uma la base. Luego tenemos la parte Greedy que es la ejecuccion de los estudiantes escuchan canciones y este trata de hacer que el estudiante escuche todas las canciones posibles sin superar el maximo tiempo este siendo establecido por el divide.

### Justificacion de Ordenes

Si imaginamos la barra como un array ordenado y se hace un binary search, nada mas que con cada repeticion se prueba valores cada vez mas cercano de la base siendo O(log(T)) T siendo el tiempo total de todas las canciones, y con cada repeticion se ejecuta el parte Greedy que intenta eschar las N canciones siendo de O(N). Entonces el O(log(T)N).

## Ejercicio 8

### Solucion

Teniedo todas las cuidades se ordenan por la coordenada y y se hace un divide and conquer tomando la mitad del area y se evalua cada lado hasta que haya una cantidad manejable obteniedo la mejor distancia efectiva en esa zona y caso, luego se compara el de ambos lados y se toma el mas chico, esto se repite hasta conseguir un resultado. Este no conseguira el mejor resultado porque cerca de la linea divisoria puede haber dos cuidades mas cerca (esas dos cuidades son separadas por la linea divisoria).
El metodo de resolucion de la linea divisoria es aumentar el low y reducir el high achicando la zona, este cumpla la condicion que las de la zona se encuentran cierta distancia de la ciudad central. Paso 2, al principio lo que hicia es recorrer devuelta la funcion con esta nueva zona, pero hay caso borde que seria que generas una nueva linea y no logras achicar la zona de la forma mencionada anteriormente (generado un bucle infinito). Esta forma fue descartada, se siguo achicando el area de la misma forma pero en vez de pasarlo de vuelta por la funcion se pasan a otro array y se ordena por x y se prueba cada pareja para encontrar una pareja de ciudades mas cercana, para que no sea tan lento se aplican ciertas podas.


| Problema |    Resultado    |
|:--------:|:---------------:|
|     1    |     Completo    |
|     2    |     Completo    |
|     3    |     Completo    |
|     4    |     Completo    |
|     5    |     Completo    |
|     6    |     Completo    |
|     7    |     Completo    |
|     8    |     Completo    |
|     9    | No Implementado |
|    10    | No Implementado |
|          |                 |
