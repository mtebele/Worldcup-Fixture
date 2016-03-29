## Estructuras de datos implementadas
Hicimos uso de las siguientes estructuras previamente implementadas:
### Lista Simplemente Enlazada
La lista se utiliza en la carga del fixture, para almacenar los nombres de
los equipos participantes. También es empleada para almacenar listados de
jugadores que son solicitados en el enunciado.
### Pila Dinámica
Esta estructura es utilizada indirectamente en la implementación del TAD
abb.
### Hash
Usamos tablas de Hash para guardar los jugadores y los equipos. Cabe
destacar que los elementos de la primera tabla son parte de los elementos
almacenados en la segunda. De esta manera, la información se encuentra
actualizada en ambas. Vimos necesario armar dos Hashes debido a que la
especificación requiere que accedamos a jugadores y equipos en orden constante.
### ABB
El árbol binario de búsqueda es utilizado para ordenar los jugadores de
un equipo, cuando el usuario solicita un listado de los mismos organizado por
nombre.
### Heap
Usamos un heap para organizar a los jugadores bajo el criterio de goles
convertidos.
### Jugador
Es una estructura que engloba las características necesarias para identificar un jugador (equipo, nombre, dorsal) y su historial de goles.
### Equipo
Agrupa jugadores, un nombre identificador y la cantidad de jugadores del
plantel.
### Fixture
Contiene los partidos a jugar en el torneo. Es el que determina las llaves
y el desarrollo del torneo.
### Sistema
Es la estructura encargada de administrar la información obtenida en
el archivo. Contiene las estructuras de ordenamiento antes mencionadas, de
manera tal que las consultas puedan ser efectuadas en los tiempos solicitados
en la especificación.
## Flujo del programa
El programa recibe un archivo de texto, el cual sería procesado en el
programa principal.
Se creará un nuevo sistema, el cual obtendrá la información del archivo
y la organizará en estructuras que manipularán las actualizaciones recibidas
y facilitarán el acceso a la información existente. Una vez procesado todo
el archivo, es cerrado y el sistema queda dispuesto para recibir consultas o
actualizaciones.
### Interacción con el usuario
El usuario enviará instrucciones al programa respetando el estándar especificado en el enunciado del TP. Si bien hay varias instancias de validación
dentro del sistema, somos conscientes de que hay ciertos casos en los cuales
el programa puede fallar debido a formatos inválidos. Esto podría ser un
aspecto a mejorar.
### Interpretación de las instrucciones recibidas por el usuario
Cada línea de texto (secuencia de caracteres finalizada con ENTER)
será la instrucción a interpretar. Internamente, el programa almacena la línea
en una cadena, que posteriormente es dividida en secciones. La primera sección es considerada el comando a realizar, mientras que el resto de la cadena
representará el/los parámetros con los que operará el comando. De acuerdo a
lo explicado en la subsección anterior, el programa validará el comando y posteriormente
hará lo mismo con la relación comando - cantidad de parámetros.
### Ejecución de las instrucciones
Una vez que los comandos y parámetros fueron validados correctamente,
el programa procede a ejecutar la función correspondiente al comando ingresado.
Esta es determinada dentro del programa principal, el cual administra
la correspondencia entre comando y función a ejecutar y los parámetros a enviar
a la función y es ejecutada dentro del módulo correspondiente al sistema
de inscripciones. Las funciones del sistema, tras ser ejecutadas, devolverán
valores que permitirán al programa principal imprimir la salida apropiada.
### Fin de la ejecución
El programa finaliza al recibir en la entrada una se~nal de fin de archivo.
Al finalizar, destruye el sistema, junto con sus estructuras asociadas.
## Resolución de los problemas planteados
### Agregar resultado
Solución: procesa el identificador de ronda recibido y accede al partido
asociado en el fixture. Consulta si el mismo puede jugarse y en caso
afirmativo, agrega el resultado al partido y actualiza los goles de cada
uno de los jugadores correspondientes a los equipos involucrados en
el encuentro. La operación también se encarga de clasificar al equipo
ganador a la próxima ronda y de actualizar goleador.
Orden: El proceso se vale de operaciones constantes para acceder al
partido y a los equipos. Para actualizar los goles, recorre los jugadores
de cada plantel y por último, el ordenamiento de los goleadores es
logarítmico respecto de la cantidad de jugadores. Por lo tanto, el orden
de la solución es O(k), siendo k la cantidad de jugadores.
### Listar jugadores
Solución: El sistema busca el equipo consultado y, en base al orden
solicitado por el usuario, muestra la información de los jugadores de
dicho equipo recorriendo el plantel.
Orden: La obtención del equipo es constante y el listado de equipos
recorre linealmente la estructura que almacena la información. Luego,
el algoritmo resuelve el problema en O(k), siendo k los jugadores a
procesar.
### Listar goleadores
Solución: Se obtiene el máximo goleador del sistema y se imprime la
información.
Orden: O(1), pues la obtención del goleador es constante, así como el
relevamiento de los datos del mismo.
### Goles Jugador
Solución: El sistema accede al jugador solicitado e imprime información
sobre el mismo.
Orden: O(1), pues la obtención del jugador y la devolución de la informaci
ón correspondiente son constantes.
### Mostrar Resultado
Solución: Busca en el fixture el partido asociado al idr recibido y emite
el informe del mismo.
Orden: O(1). Encuentro el partido y proceso la información en orden
constante.
