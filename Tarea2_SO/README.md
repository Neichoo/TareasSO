# Grupo-27
## Integrantes (Nombre/Paralelo):
- Diego Ramírez Rojas/200
- Ignacio Gómez González/200

## Uso correcto del programa:
- Se necesita tener todos los archivos en un misma carpeta.
- Para compilar el programa se escribe por consola "make BattleRoyale" (viene con comprobación de warnings incluido),
luego para ejecutarlo se escribe "make run", también se pueden verificar leaks de memoria a través de "make runvalgrind".
También contamos con "make remove", que borra el ejecutable creado.

## Supuestos Utilizados:
- Se le puede seguir pegando a un jugador ya muerto, para fines cómicos, a algunos usuarios les gustan las interacciones.
- Una vez que un hijo muere, su proceso se termina, ya que no tiene sentido seguir manteniendolo vivo, puesto que el padre
regula todo.

## Archivos:
- main.c: Núcleo del programa, donde se encuentra toda la lógica.
- jugador.cpp: Usado para crear la clase jugador, con sus respectivos métodos.
- Makefile: Para facilitar la ejecución del programa.

Esperamos que el programa sea de su agrado c:

