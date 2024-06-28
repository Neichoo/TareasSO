# Grupo-27
## Integrantes (Nombre/Paralelo):
- Diego Ramírez Rojas/200
- Ignacio Gómez González/200

## Uso correcto del programa:
- Se necesita mantener todos los archivos dentro de una misma carpeta, luego se debe insertar en la carpeta un archivo .txt con el
laberinto, llamado "laberinto.txt", este mismo se va cambiando para probar distintos escenarios.
- Para ir probando cada escenario, se debe escribir por consola las siguientes opciones:
 -Si se busca probar las hebras, se escribe "make laberintohebras" para compilarlo y posteriormente "make runhebras".
 -Si se busca probar los forks, se escribe "make laberintoforks" para compilarlo y posteriormente "make runforks".
- Finalmente se cuenta con "make remove", el cual elimina los ejectuables.

## Supuestos Utilizados:
- El programa se puede dividir en 2 separados, para facilitar la visualización del tiempo de ejecución puro del método utilizado.

## Archivos:
- Forks.java: Clase Forks, donde está la lógica del método que utiliza forks.
- Hebras.java: Clase Hebras, donde está la lógica del método que utiliza hebras.
- LaberintoHebras.java: Este es el main para ejecutar el caso de hebras.
- LaberintoForks.java: Este es el main para ejecutar el caso de forks.
- Makefile: Para facilitar la ejecución del programa.
- AnalisisForks_Hebras.xlsx: Análisis que debe ser abierto en excel o cualquier programa que acepte esa extensión.

Esperamos que el programa sea de su agrado c:
