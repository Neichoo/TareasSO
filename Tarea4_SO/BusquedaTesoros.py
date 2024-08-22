import threading
import random
import datetime
import time

piratas_terminados = 0
#Reiniciar los archivos .txt con cada ejecucion, diccionario con las islas y su informacion
bar_del_oraculo = open("BarOraculo.txt", "w")
bar_del_oraculo.truncate(0)
bar_del_oraculo.close()
salida = open("Salida.txt", "w")
salida.truncate(0)
salida.close()
islas = {
    "Isla de las Sombras": {"capacidad_puente": 20, "duracion_busqueda": 9, "capacidad_piratas": 10}, #0
    "Isla del Dragón Rojo": {"capacidad_puente": 8, "duracion_busqueda": 5, "capacidad_piratas": 2}, #1
    "Isla de las Estrellas": {"capacidad_puente": 15, "duracion_busqueda": 7, "capacidad_piratas": 5}, #2
    "Isla del Bosque Encantado": {"capacidad_puente": 6, "duracion_busqueda": 4, "capacidad_piratas": 3}, #3
    "Isla de los Susurros": {"capacidad_puente": 6, "duracion_busqueda": 1, "capacidad_piratas": 5}, #4
    "Isla del Trueno": {"capacidad_puente": 9, "duracion_busqueda": 4, "capacidad_piratas": 4}, #5
    "Isla del Tesoro Oculto": {"capacidad_puente": 7, "duracion_busqueda": 5, "capacidad_piratas": 2}, #6 
}
for isla in islas:
    txt_isla = open(f"{isla}.txt", "w")
    txt_isla.close()



def piratas_en_bar(id_pirata):
    global piratas_terminados
    cadenas = []
    #Tiempo de llegada del pirata al bar
    tiempo_llegada = datetime.datetime.now()
    #Se abren los 2 archivos de texto que piden en "a" que es de append
    bar_del_oraculo = open("BarOraculo.txt", "a")
    salida = open("Salida.txt", "a")
    cadena_llegada_bar = f"Pirata {id_pirata}, {tiempo_llegada}, "
    cadenas.append(cadena_llegada_bar)
    #Se hace una lista con las keys para poder hacer un random que tire 2 islas al azar no repetidas
    lista_islas = list(islas.keys())
    islas_randoms = random.sample(lista_islas, 2)
    num_isla = 1
    #Mecanismo con semaforos y locks
    for isla in islas_randoms:
        # Siempre que haya espacio, esperar en el puente
        with puentes_lock[isla]:
            while semaforos_capacidad_puentes[isla]._value == 0:
                puentes_cond[isla].wait()
            semaforos_capacidad_puentes[isla].acquire()
            tiempo_llegada_fila = datetime.datetime.now()
            cadena_fila_puente = f"{isla}, {tiempo_llegada_fila}, "
            cadenas.append(cadena_fila_puente)

        # Entrar a la isla cuando haya capacidad suficiente para el grupo completo
        with puentes_lock[isla]:
            while (semaforos_capacidad_piratas[isla]._value < islas[isla]["capacidad_piratas"]) or ( ((datetime.datetime.now() - tiempo_llegada_fila).total_seconds() >= 30) and piratas_terminados >= 400 ):
                puentes_cond[isla].wait()
            for _ in range(islas[isla]["capacidad_piratas"]):
                semaforos_capacidad_piratas[isla].acquire()
            tiempo_acceso_isla = datetime.datetime.now()
            with locks[isla]:
                grupo_isla_contadores[isla] += 1

        with locks[isla]:
            txt_isla = open(f"{isla}.txt", "a")
            txt_isla.write(f"Pirata{id_pirata}, {tiempo_llegada_fila}, {tiempo_acceso_isla}, {num_isla}\n")
            txt_isla.close()

        # Simular la búsqueda del tesoro
        time.sleep(islas[isla]["duracion_busqueda"])

        # Liberar los semáforos cuando todos los piratas del grupo hayan terminado
        with locks[isla]:
            grupo_isla_contadores[isla] -= 1
            if grupo_isla_contadores[isla] == 0:
                for _ in range(islas[isla]["capacidad_piratas"]):
                    semaforos_capacidad_piratas[isla].release()
                if semaforos_capacidad_piratas[isla]._value == islas[isla]["capacidad_piratas"]:
                    with puentes_cond[isla]:
                        puentes_cond[isla].notify_all()

        with puentes_lock[isla]:
            semaforos_capacidad_puentes[isla].release()
            if semaforos_capacidad_puentes[isla]._value == islas[isla]["capacidad_puente"]:
                with puentes_cond[isla]:
                    puentes_cond[isla].notify_all()
        num_isla += 1

    #Fin, cierre de archivos y escritura en salida
    cadenas_juntas = cadenas[0] + cadenas[1] + cadenas[2] + "\n"
    bar_del_oraculo.write(cadenas_juntas)
    tiempo_salida = datetime.datetime.now()
    salida.write(f"Pirata {id_pirata}, {tiempo_salida}\n")
    bar_del_oraculo.close()
    salida.close()
    piratas_terminados += 1

semaforos_capacidad_puentes = {}
semaforos_capacidad_piratas = {}
locks = {}
puentes_lock = {}
puentes_cond = {}
grupo_isla_contadores = {}
for isla, valores in islas.items():
    #Valores para los semaforos
    capacidad_puente = valores["capacidad_puente"]
    capacidad_pirata = valores["capacidad_piratas"]
    #Semaforo con cuanta capacidad disponible hay, parte en la capacidad maxima
    semaforo_capacidad_puente = threading.Semaphore(capacidad_puente)
    semaforo_capacidad_pirata = threading.Semaphore(capacidad_pirata)
    lock = threading.Lock()
    puente_lock = threading.Lock()
    #2 diccionarios con la clave isla y sus valores de semaforo y 1 diccionario con la clave isla y su lock correspondiente
    semaforos_capacidad_puentes[isla] = semaforo_capacidad_puente
    semaforos_capacidad_piratas[isla] = semaforo_capacidad_pirata
    locks[isla] = lock
    puentes_lock[isla] = puente_lock
    puentes_cond[isla] = threading.Condition(puente_lock)
    grupo_isla_contadores[isla] = 0

#Se crean los 500 piratas que iran a la funcion piratas_en_bar desde el pirata 1 hasta el pirata 500.
hebras = []
i = 1
while(i < 501):
    hebra = threading.Thread(target=piratas_en_bar, args=(i,))
    hebra.start()
    hebras.append(hebra)
    i += 1

for hebra in hebras:
    hebra.join()
