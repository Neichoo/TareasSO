#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <ctype.h>
#include <string.h>

//****//
// La función alfabetico recibe el nombre del pokemon que se encuentra en el png y luego
// retorna la primera letra de este en mayúsculas, para facilitar la creación de las carpetas.
//****//
char alfabetico(const char* nombre_pokemon){
    return toupper(nombre_pokemon[0]);
}

//****//
// La función generacion recibe el nombre del pokemon que se encuentra en el png y luego
// retorna la generacion de este, basandose en el numero de pokedex que tiene, para facilitar la
// creación de las carpetas.
//****//
int generacion(const char* nombre_pokemon) {
    const char* guion_bajo = strrchr(nombre_pokemon, '_');
    const char* punto = strrchr(nombre_pokemon, '.');
    if (guion_bajo == NULL || punto == NULL)
        return -1;
    char num_generacion_str[3];
    strncpy(num_generacion_str, guion_bajo + 1, punto - guion_bajo - 1);
    num_generacion_str[punto - guion_bajo - 1] = '\0';
    int num_generacion = atoi(num_generacion_str);
    if (num_generacion >= 1 && num_generacion <= 151)
        return 1;
    else if (num_generacion >= 152 && num_generacion <= 251)
        return 2;
    else if (num_generacion >= 252 && num_generacion <= 386)
        return 3;
    else if (num_generacion >= 387 && num_generacion <= 493)
        return 4;
    else
        return -1;
}

//****//
// La función nueva_carpeta recibe como input el nombre de la carpeta a crear, luego se crea
// la nueva carpeta en el directorio especificado, siempre y cuando la carpeta no exista previamente.
//****//
void nueva_carpeta(const char* nombre_carpeta, const char* agregado){
    char* todo = malloc(strlen(agregado) + 2);
    strcpy(todo, agregado);
    strcat(todo, nombre_carpeta);
    DIR* directorio = opendir(todo);
    if (directorio){
        closedir(directorio);
    } else {
        if (mkdir(todo, 0777) == -1 ){
        perror("mkdir");
        exit(EXIT_FAILURE);
        }
    }
    free(todo);
}

//****//
// La función mover_imagen está encargada de trasladar cada imagen a una carpeta especifica,
// se recibe la carpeta origen, la carpeta destino y el nombre del pokemon, luego se construyen
// las respectivas direcciones para la imagen, posteriormente haciendo uso de rename se mueven las
// imagenes.
//****//
void mover_imagen(const char* srcFolder, const char* destFolder, const char* nombre_pokemon) {
    char srcPath[FILENAME_MAX];
    char destPath[FILENAME_MAX];
    snprintf(srcPath, FILENAME_MAX, "%s/%s", srcFolder, nombre_pokemon);
    snprintf(destPath, FILENAME_MAX, "%s/%s", destFolder, nombre_pokemon);
    const char* extension = strrchr(nombre_pokemon, '.');
    if (extension != NULL && strcmp(extension, ".png") == 0) {
        if (rename(srcPath, destPath) != 0) {
            perror("Error al mover el archivo");
        }
    }
}

//****//
// La función copiar_imagen está encargada de copiar cada imagen a una carpeta especifica,
// en este caso se recibe la carpeta origen, la carpeta destino y el nombre del pokemon, se
// construyen las rutas, luego se copia y se pega la imagen en el destino deseado.
//****//
void copiar_y_pegar_imagen(const char* srcFolder, const char* destFolder, const char* nombre_pokemon) {
    char ruta_origen[FILENAME_MAX];
    char ruta_destino[FILENAME_MAX];
    snprintf(ruta_origen, FILENAME_MAX, "%s/%s", srcFolder, nombre_pokemon);
    snprintf(ruta_destino, FILENAME_MAX, "%s/%s", destFolder, nombre_pokemon);
    FILE* archivo_origen = fopen(ruta_origen, "rb");
    if (archivo_origen == NULL) {
        printf("Error: No se pudo abrir el archivo de origen.\n");
        return;
    }
    FILE* archivo_destino = fopen(ruta_destino, "wb");
    if (archivo_destino == NULL) {
        fclose(archivo_origen);
        return;
    }
    char buffer[1024];
    size_t bytes_leidos;
    while ((bytes_leidos = fread(buffer, 1, sizeof(buffer), archivo_origen)) > 0) {
        fwrite(buffer, 1, bytes_leidos, archivo_destino);
    }
    fclose(archivo_origen);
    fclose(archivo_destino);
}

//****//
// La función contar_archivos cuenta los archivos que hay presentes en la carpeta
// entregada en los parámetros. Retorna el número de elementos.
//****//
int contar_archivos(const char* carpeta){
    int contador = 0;
    DIR* directorio = opendir(carpeta);
    if(directorio){
        struct dirent* entry;
        while((entry = readdir(directorio))!= NULL){
            contador++;
        }
        closedir(directorio);
    } else{
        perror("Error al abrir la carpeta");
    }
    return (contador-2);
}



//****//
// La función main es el corazón del programa, acá se enlaza todo para que funcione, se crean
// las carpetas generacion, alfabetico y luego se realiza un ciclo while en donde se va analizando
// la carpeta de "Sprites", se saca cada nombre de archivo png, luego se crean las variables
// correspondientes tanto para las carpetas alfabeticas como para las generacionales, extrayendo
// información útil como la primera letra del pokemon, su generacion, etc. 
// Luego primero se crea la carpeta correspondiente a Alfabetico, luego se mueve la imagen a donde
// corresponda dentro de Alfabetico, después se crea la carpeta correspondiente a la Generacion, luego
// se copia y la imagen desde la carpeta en Alfabeto y se pega en su generacion correspondiente.
// Posteriormente al finalizar el ciclo se abre el archivo RegistroPokemon y se escribe en el los
// contadores de cada generacion y cada letra del alfabeto usada.
//****//
int main() {
    //Declaración y creación inicial de variables y carpetas.
    const char* srcFolder = "Sprites";
    DIR* directorio;
    struct dirent* entry;
    if ((directorio = opendir(srcFolder)) == NULL) {
        perror("Error al abrir el directorio");
        return EXIT_FAILURE;
    }
    int contador_gen1, contador_gen2, contador_gen3, contador_gen4;
    char f_alfabetico[] = "Alfabético";
    char f_generacion[] = "Generación";
    char f_agregado_i[] = "";
    nueva_carpeta(f_alfabetico, f_agregado_i);
    nueva_carpeta(f_generacion, f_agregado_i);
    //Leer cada archivo en el directorio
    while ((entry = readdir(directorio)) != NULL) {
        //Variable para ambos
        const char* filename = entry->d_name;

        //Variables Alfabetico
        char agregado_alfabetico[] = "Alfabético/";
        char primera_letra = alfabetico(filename);
        char primera_letra_str[] = {primera_letra, '\0'};
        char destFolder_alfabetico[strlen(f_alfabetico) + strlen(primera_letra_str) + 2]; // Tamaño suficiente para "Alfabético/" + 1 carácter
        strcpy(destFolder_alfabetico, agregado_alfabetico);
        strcat(destFolder_alfabetico, primera_letra_str);

        //Variables Generacion
        char agregado_generacion[] = "Generación/";
        int gen = generacion(filename);
        char f_gen[4];
        if (gen == 1){
            strcpy(f_gen, "I");
            contador_gen1 += 1;
        } else if (gen == 2){
            strcpy(f_gen, "II");
            contador_gen2 += 1;
        } else if (gen == 3){
            strcpy(f_gen, "III");
            contador_gen3 += 1;
        } else if (gen == 4){
            strcpy(f_gen, "IV");
            contador_gen4 += 1;
        }
        char destFolder_generacion[strlen(f_generacion) + strlen(f_gen) + 4];
        strcpy(destFolder_generacion, agregado_generacion);
        strcat(destFolder_generacion, f_gen);

        //Secuencia Programa
        nueva_carpeta(primera_letra_str, agregado_alfabetico);
        mover_imagen(srcFolder ,destFolder_alfabetico, filename);
        nueva_carpeta(f_gen, agregado_generacion);
        copiar_y_pegar_imagen(destFolder_alfabetico, destFolder_generacion, filename);
    }
    //Escribir la cantidad de pokemones por generacion en RegistroPokemon.txt
    FILE* RegistroPokemon;
    RegistroPokemon = fopen("RegistroPokemon.txt", "w");
    fprintf(RegistroPokemon, "%s", "Generación:");
    fprintf(RegistroPokemon, "%s", "\nI - ");
    fprintf(RegistroPokemon, "%d", contador_gen1);
    fprintf(RegistroPokemon, "%s", "\nII - ");
    fprintf(RegistroPokemon, "%d", contador_gen2);
    fprintf(RegistroPokemon, "%s", "\nIII - ");
    fprintf(RegistroPokemon, "%d", contador_gen3);
    fprintf(RegistroPokemon, "%s", "\nIIV - ");
    fprintf(RegistroPokemon, "%d", contador_gen4);

    //Escribir la cantidad de pokemones por letra del alfabeto en RegistroPokemon.txt
    fprintf(RegistroPokemon, "%s", "\nAlfabético:");
    int contador_letras;
    DIR* letras;
    if ((letras = opendir(f_alfabetico)) == NULL){
        perror("Error al abrir el directorio");
        return EXIT_FAILURE;
    }
    char abecedario[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    for (int i = 0; abecedario[i] != '\0'; i++) {
        char carpeta_letra[] = "Alfabético/";
        char abecedario_str[2] = {abecedario[i], '\0'};
        strcat(carpeta_letra, abecedario_str);
        contador_letras = contar_archivos(carpeta_letra);
        fprintf(RegistroPokemon, "\n%s - %d", abecedario_str, contador_letras);
    }
    //Cerrar archivos y directorios que fueron abiertos.
    fclose(RegistroPokemon);
    closedir(letras);
    closedir(directorio);
    return 0;
}
