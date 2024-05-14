#include <cstring>
#include <unistd.h>
#include <iostream>
#include <string>
#include <random>
#include <vector>
#include "jugador.cpp"
using namespace std;

//****//
//Fucion main, primero crea los pipes necesarios para la comunicacion entre los distintos procesos,
//luego crea los 4 jugadores necesarios para la realizacion de la partida y 
//da comienzo al juego, manteniendo todo sincronizado para su correcto funcionamiento
//Al finalizar la partida cierra todos los pipes creados y el programa termina
//****//
int main(){
    //Direcciones de los pipes, cada jugador tiene un pipe con el padre para escribir y otro para leer
    //no hay pipes entre hijos
    //H0 -> H1: 00 / H1 -> H0: 01
    //H0 -> H2: 02 / H2 -> H0: 03
    //H0 -> H3: 04 / H3 -> H0: 05
    //H0 -> H4: 06 / H4 -> H0: 07

    //Creacion de variables para elegir a que jugador atacar
    random_device rd;
    mt19937 gen(rd());
    int min_jugador, max_jugador, randomJugador;
    //Variable del numero de ronda actual 
    int ronda = 1;
    //creacion de pipes
    int pipes[8][2];
    for (int i = 0; i < 8; i++){
        pipe(pipes[i]);
    }
    //Se crea al 1er hijo, que sera controlado por consola
    pid_t jugador1 = fork();
    if (jugador1 == 0){
        //Cierre de pipes
        close(pipes[0][1]);
        close(pipes[1][0]);
        close(pipes[2][1]);
        close(pipes[2][0]);
        close(pipes[3][1]);
        close(pipes[3][0]);
        close(pipes[4][1]);
        close(pipes[4][0]);
        close(pipes[5][1]);
        close(pipes[5][0]);
        close(pipes[6][1]);
        close(pipes[6][0]);
        close(pipes[7][1]);
        close(pipes[7][0]);
        
        //Se crea el 1er jugador y se mandan sus estadisticas al padre
        Jugador j1(1);
        int n_ataquesr = 0, danio_r, num_jugador, logro_evadir, permiso;
        string statsj1 = "Jugador " + to_string(j1.getId()) + "\nVida: " + to_string(j1.getVida()) + "\nAtaque: " + to_string(j1.getAtaque()) + "\nDefensa: " + to_string(j1.getDefensa()) + "\nEvasion: " + to_string(j1.getEvasion()) + "\n";
        char statsj1CharArray[512];
        strcpy(statsj1CharArray, statsj1.c_str());
        write(pipes[1][1], &statsj1CharArray, sizeof(statsj1CharArray));
        int aj1 = j1.getAtaque();
        write(pipes[1][1], &aj1, sizeof(aj1));
        int vj1 = j1.getVida();
        write(pipes[1][1], &vj1, sizeof(vj1));

        //Ciclo principal del jugador, se ejecuta mientras el jugador este vivo
        while(j1.getVida() > 0){
            //Recibe la ronda actual, esto es para la sincronizacion con el padre o para terminar el juego si la ronda es -1
            read(pipes[0][0],&ronda, sizeof(ronda));
            if(ronda == -1){
                close(pipes[1][1]);
                close(pipes[0][0]);
                exit(0);
            } else{
                //Se envia la vida actual de este jugador y luego recibe "permiso" para pedir informacion por consola
                vj1 = j1.getVida();
                write(pipes[1][1], &vj1, sizeof(vj1));
                read(pipes[0][0],&permiso, sizeof(permiso));
                //Se pregunta a que jugador atacara
                if(permiso == 1){
                    do {
                        cout << "Jugador 1, a quien quieres atacar ahora? (Solo ingresar un numero)\n1)Jugador 2\n2)Jugador 3\n3)Jugador 4\n";
                        cin >> num_jugador;
                        cout << "\n";
                        // Si el cin falla o el número ingresado no está dentro del rango esperado
                        if (cin.fail() || num_jugador < 1 || num_jugador > 3) {
                            cout << "Por favor, ingresa un número válido.\n";
                            cin.clear();
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        }
                        // Repetir hasta que se ingrese un número válido
                    } while (cin.fail() || num_jugador < 1 || num_jugador > 3);
                    if(num_jugador == 1){
                        num_jugador = 2;
                    } else if(num_jugador == 2){
                        num_jugador = 3;
                    } else if(num_jugador == 3){
                        num_jugador = 4;
                    }
                    //Se envia al padre a que jugador atacar y luego recibe cuantos jugadores lo atacan a el
                    write(pipes[1][1], &num_jugador, sizeof(num_jugador));
                    read(pipes[0][0],&n_ataquesr, sizeof(n_ataquesr));
                    if(n_ataquesr > 0){
                        for(int i = 0; i < n_ataquesr; i++){
                            //lee el danio del jugador que lo ataca
                            read(pipes[0][0],&danio_r, sizeof(danio_r));
                            logro_evadir = j1.recibirDanio(danio_r);
                            //Le envia al padre si logro esquivar o no el ataque 
                            write(pipes[1][1], &logro_evadir, sizeof(logro_evadir));
                        }
                    }
                    n_ataquesr = 0;  
                } else{
                    //Permiso = 0, gana el jugador 1
                    close(pipes[1][1]);
                    close(pipes[0][0]);
                    exit(0);
                }
            }
        }
        //Cierre de pipes y anuncio de la muerte del jugador 1
        //Su vida se actualiza a 0
        cout << "\n**Ha muerto el jugador 1**\n";
        vj1 = 0;
        write(pipes[1][1], &vj1, sizeof(vj1));
        close(pipes[1][1]);
        close(pipes[0][0]);
        exit(0);
    }else{
        //Se crea al 2do jugador
        pid_t jugador2 = fork();
        if (jugador2 == 0){
            //Cierre de pipes
            close(pipes[2][1]);
            close(pipes[3][0]);
            close(pipes[0][1]);
            close(pipes[0][0]);
            close(pipes[1][1]);
            close(pipes[1][0]);
            close(pipes[4][1]);
            close(pipes[4][0]);
            close(pipes[5][1]);
            close(pipes[5][0]);
            close(pipes[6][1]);
            close(pipes[6][0]);
            close(pipes[7][1]);
            close(pipes[7][0]);
            //Se crea el 2do jugador y se mandan sus estadisticas al padre
            Jugador j2(2);
            int n_ataquesr = 0, danio_r, logro_evadir;
            min_jugador = 1;
            max_jugador = 3;
            uniform_int_distribution<> dis_ataque(min_jugador, max_jugador);
            string statsj2 = "Jugador " + to_string(j2.getId()) + "\nVida: " + to_string(j2.getVida()) + "\nAtaque: " + to_string(j2.getAtaque()) + "\nDefensa: " + to_string(j2.getDefensa()) + "\nEvasion: " + to_string(j2.getEvasion()) + "\n";
            char statsj2CharArray[512];
            strcpy(statsj2CharArray, statsj2.c_str());
            write(pipes[3][1], &statsj2CharArray, sizeof(statsj2CharArray));
            int aj2 = j2.getAtaque();
            write(pipes[3][1], &aj2, sizeof(aj2));
            int vj2 = j2.getVida();
            write(pipes[3][1], &vj2, sizeof(vj2));

            //Ciclo principal del jugador, se ejecuta mientras el jugador este vivo
            while(j2.getVida() > 0){
                //Se recibe el numero de ronda para sincronizar o para terminar el juego
                read(pipes[2][0],&ronda, sizeof(ronda));
                if(ronda == -1){
                    close(pipes[2][0]);
                    close(pipes[3][1]);
                    exit(0);
                } else{
                    //Se envia  la vida actual al padre
                    //se elije a que jugador atacar 
                    vj2 = j2.getVida();
                    write(pipes[3][1], &vj2, sizeof(vj2));
                    randomJugador = dis_ataque(gen);
                    if(randomJugador == 1){
                        randomJugador = 1;
                    } else if(randomJugador == 2){
                        randomJugador = 3;
                    } else if(randomJugador == 3){
                        randomJugador = 4;
                    }
                    //Se envia a que jugador atacar y tambien cuantos jugadores atacaran a 2
                    write(pipes[3][1], &randomJugador, sizeof(randomJugador));
                    read(pipes[2][0],&n_ataquesr, sizeof(n_ataquesr));
                    if(n_ataquesr > 0){
                        for(int i = 0; i < n_ataquesr; i++){
                            //lee el danio del jugador que lo ataca
                            read(pipes[2][0],&danio_r, sizeof(danio_r));
                            logro_evadir = j2.recibirDanio(danio_r);
                            //Le envia al padre si logro esquivar o no el ataque 
                            write(pipes[3][1], &logro_evadir, sizeof(logro_evadir));
                        }
                    }
                    n_ataquesr = 0;  
                }
            }
            //Cierre de pipes y anuncio de la muerte del jugador 2
            //Su vida se actualiza a 0
            cout << "\n**Ha muerto el jugador 2**\n";
            vj2 = 0;
            write(pipes[3][1], &vj2, sizeof(vj2));
            close(pipes[3][1]);
            close(pipes[2][0]);
            exit(0);
        }else{
            //Se crea al 3er jugador
            pid_t jugador3 = fork();
            if (jugador3 == 0){
                //cierre de pipes
                close(pipes[4][1]);
                close(pipes[5][0]);
                close(pipes[0][1]);
                close(pipes[0][0]);
                close(pipes[1][1]);
                close(pipes[1][0]);
                close(pipes[2][1]);
                close(pipes[2][0]);
                close(pipes[3][1]);
                close(pipes[3][0]);
                close(pipes[6][1]);
                close(pipes[6][0]);
                close(pipes[7][1]);
                close(pipes[7][0]);
                //Se crea el 3er jugador y se mandan sus estadisticas al padre
                Jugador j3(3);
                int n_ataquesr = 0, danio_r, logro_evadir;
                min_jugador = 1;
                max_jugador = 3;
                uniform_int_distribution<> dis_ataque(min_jugador, max_jugador);
                string statsj3 = "Jugador " + to_string(j3.getId()) + "\nVida: " + to_string(j3.getVida()) + "\nAtaque: " + to_string(j3.getAtaque()) + "\nDefensa: " + to_string(j3.getDefensa()) + "\nEvasion: " + to_string(j3.getEvasion()) + "\n";
                char statsj3CharArray[512];
                strcpy(statsj3CharArray, statsj3.c_str());
                write(pipes[5][1], &statsj3CharArray, sizeof(statsj3CharArray));
                int aj3 = j3.getAtaque();
                write(pipes[5][1], &aj3, sizeof(aj3));
                int vj3 = j3.getVida();
                write(pipes[5][1], &vj3, sizeof(vj3));

                //Ciclo principal del jugador, se ejecuta mientras el jugador este vivo
                while(j3.getVida() > 0){
                    //Se recibe el numero de ronda para sincronizar o para terminar el juego
                    read(pipes[4][0],&ronda, sizeof(ronda));
                    if(ronda == -1){
                        close(pipes[4][0]);
                        close(pipes[5][1]);
                        exit(0);
                    } else{
                        //Se envia  la vida actual al padre
                        //se elije a que jugador atacar 
                        vj3 = j3.getVida();
                        write(pipes[5][1], &vj3, sizeof(vj3));
                        randomJugador = dis_ataque(gen);
                        if(randomJugador == 1){
                            randomJugador = 1;
                        } else if(randomJugador == 2){
                            randomJugador = 2;
                        } else if(randomJugador == 3){
                            randomJugador = 4;
                        }
                        //Se envia a que jugador atacar y tambien cuantos jugadores atacaran a 3
                        write(pipes[5][1], &randomJugador, sizeof(randomJugador));
                        read(pipes[4][0],&n_ataquesr, sizeof(n_ataquesr));
                        if(n_ataquesr > 0){
                            for(int i = 0; i < n_ataquesr; i++){
                                //lee el danio del jugador que lo ataca
                                read(pipes[4][0],&danio_r, sizeof(danio_r));
                                logro_evadir = j3.recibirDanio(danio_r);
                                //Le envia al padre si logro esquivar o no el ataque 
                                write(pipes[5][1], &logro_evadir, sizeof(logro_evadir));
                            }
                        }
                        n_ataquesr = 0;  
                    }
                   
                }
                //Cierre de pipes y anuncio de la muerte del jugador 3
                //Su vida se actualiza a 0
                cout << "\n**Ha muerto el jugador 3**\n";
                vj3 = 0;
                write(pipes[5][1], &vj3, sizeof(vj3)); 
                close(pipes[5][1]);
                close(pipes[4][0]);
                exit(0);
            }
            else{
                //Se crea al 3er jugador
                pid_t jugador4 = fork();
                if (jugador4 == 0){
                    //Cierre de pipes
                    close(pipes[6][1]);
                    close(pipes[7][0]);
                    close(pipes[0][1]);
                    close(pipes[0][0]);
                    close(pipes[1][1]);
                    close(pipes[1][0]);
                    close(pipes[2][1]);
                    close(pipes[2][0]);
                    close(pipes[3][1]);
                    close(pipes[3][0]);
                    close(pipes[4][1]);
                    close(pipes[4][0]);
                    close(pipes[5][1]);
                    close(pipes[5][0]);
                    //Se crea el 4to jugador y se mandan sus estadisticas al padre
                    Jugador j4(4);
                    int n_ataquesr = 0, danio_r, logro_evadir;
                    min_jugador = 1;
                    max_jugador = 3;
                    uniform_int_distribution<> dis_ataque(min_jugador, max_jugador);
                    string statsj4 = "Jugador " + to_string(j4.getId()) + "\nVida: " + to_string(j4.getVida()) + "\nAtaque: " + to_string(j4.getAtaque()) + "\nDefensa: " + to_string(j4.getDefensa()) + "\nEvasion: " + to_string(j4.getEvasion()) + "\n";
                    char statsj4CharArray[512];
                    strcpy(statsj4CharArray, statsj4.c_str());
                    write(pipes[7][1], &statsj4CharArray, sizeof(statsj4CharArray));
                    int aj4 = j4.getAtaque();
                    write(pipes[7][1], &aj4, sizeof(aj4));
                    int vj4 = j4.getVida();
                    write(pipes[7][1], &vj4, sizeof(vj4));

                    //Ciclo principal del jugador, se ejecuta mientras el jugador este vivo
                    while(j4.getVida() > 0){
                        //Se recibe el numero de ronda para sincronizar o para terminar el juego
                        read(pipes[6][0],&ronda, sizeof(ronda));
                        if(ronda == -1){
                            close(pipes[6][0]);
                            close(pipes[7][1]);
                            exit(0);
                        } else{
                            //Se envia  la vida actual al padre
                            //se elije a que jugador atacar
                            vj4 = j4.getVida();
                            write(pipes[7][1], &vj4, sizeof(vj4));
                            randomJugador = dis_ataque(gen);
                            if(randomJugador == 1){
                                randomJugador = 1;
                            } else if(randomJugador == 2){
                                randomJugador = 2;
                            } else if(randomJugador == 3){
                                randomJugador = 3;
                            }
                            //Se envia a que jugador atacar y tambien cuantos jugadores atacaran a 4
                            write(pipes[7][1], &randomJugador, sizeof(randomJugador));
                            read(pipes[6][0],&n_ataquesr, sizeof(n_ataquesr));
                            if(n_ataquesr > 0){
                                for(int i = 0; i < n_ataquesr; i++){
                                    //lee el danio del jugador que lo ataca
                                    read(pipes[6][0],&danio_r, sizeof(danio_r));
                                    logro_evadir = j4.recibirDanio(danio_r);
                                    //Le envia al padre si logro esquivar o no el ataque 
                                    write(pipes[7][1], &logro_evadir, sizeof(logro_evadir));
                                }
                            }
                            n_ataquesr = 0;  
                        }
                    }
                    //Cierre de pipes y anuncio de la muerte del jugador 3
                    //Su vida se actualiza a 0
                    cout << "\n**Ha muerto el jugador 4**\n";
                    vj4 = 0;
                    write(pipes[7][1], &vj4, sizeof(vj4));
                    close(pipes[7][1]);
                    close(pipes[6][0]);
                    exit(0);
                }
                else{
                    //Codigo del proceso padre ("ring")
                    min_jugador = 1;
                    max_jugador = 2;
                    uniform_int_distribution<> dis_desempate(min_jugador, max_jugador);
                    char statsj1[512], statsj2[512], statsj3[512], statsj4[512];
                    //Lee los stats de cada jugador y los imprime por pantalla
                    read(pipes[1][0], &statsj1, sizeof(statsj1));
                    read(pipes[3][0], &statsj2, sizeof(statsj2));
                    read(pipes[5][0], &statsj3, sizeof(statsj3));
                    read(pipes[7][0], &statsj4, sizeof(statsj4));
                    cout << "Bienvenido a la partida, a continuación se mostrarán tus stats seguidos de los de tus oponente.\n";
                    cout << "*****\n" << statsj1 << "*****\n\n";
                    cout << "*****\n" << statsj2 << "*****\n\n";
                    cout << "*****\n" << statsj3 << "*****\n\n";
                    cout << "*****\n" << statsj4 << "*****\n\n";
                    int atq1, atq2, atq3, atq4;
                    //Lee el ataque de cada jugador
                    read(pipes[1][0], &atq1, sizeof(atq1));
                    read(pipes[3][0], &atq2, sizeof(atq2));
                    read(pipes[5][0], &atq3, sizeof(atq3));
                    read(pipes[7][0], &atq4, sizeof(atq4));
                    int j1_a_otro, j2_a_otro, j3_a_otro, j4_a_otro, num_actual, danio_actual, esquivado_actual, permiso = 1;
                    int cj1r = 0, cj2r = 0, cj3r = 0, cj4r = 0;
                    vector<int> danios(4,0);
                    vector<int> esquivado(4,2);
                    danios[0] = atq1;
                    danios[1] = atq2;
                    danios[2] = atq3;
                    danios[3] = atq4;
                    vector<int> ataques(4,0);
                    int vj1, vj2, vj3, vj4;
                    //Lee la vida actual de cada jugador
                    read(pipes[1][0], &vj1, sizeof(vj1)); 
                    read(pipes[3][0], &vj2, sizeof(vj2)); 
                    read(pipes[5][0], &vj3, sizeof(vj3));
                    read(pipes[7][0], &vj4, sizeof(vj4));

                    while(ronda != -1){
                        //Escribir ronda a hijos
                        for (int i = 0; i < 4; i++) {
                            write(pipes[i * 2][1], &ronda, sizeof(ronda));
                        }
                        //Siempre que sigan vivos, que reporten la vida y a quien atacaran
                        if(vj1 > 0){
                            read(pipes[1][0], &vj1, sizeof(vj1));
                        }
                        if(vj2 > 0){
                            read(pipes[3][0], &vj2, sizeof(vj2));
                        }
                        if(vj3 > 0){
                            read(pipes[5][0], &vj3, sizeof(vj3));
                        }
                        if(vj4> 0){
                            read(pipes[7][0], &vj4, sizeof(vj4));
                        }
                        //Prints rondas
                        cout << "***** Ronda " << ronda << " *****" << endl;
                        cout << "Vida jugador 1: " << vj1 << endl;
                        cout << "Vida jugador 2: " << vj2 << endl;
                        cout << "Vida jugador 3: " << vj3 << endl;
                        cout << "Vida jugador 4: " << vj4 << "\n" << endl;
                        ronda ++;
                        if((vj2 <= 0) && (vj3 <= 0) && (vj4 <= 0)){
                            permiso = 0;
                            vj1 = -1;
                            break;
                        }
                        write(pipes[0][1], &permiso, sizeof(permiso));
                        //Lee a que oponente atacara cada jugador
                        if((vj1 > 0)){
                            read(pipes[1][0], &j1_a_otro, sizeof(j1_a_otro));
                            ataques[0] = j1_a_otro;
                        }
                        if(vj2 > 0){
                            read(pipes[3][0], &j2_a_otro, sizeof(j2_a_otro));
                            ataques[1] = j2_a_otro;
                        }
                        if(vj3 > 0){
                            read(pipes[5][0], &j3_a_otro, sizeof(j3_a_otro));
                            ataques[2] = j3_a_otro;
                        }
                        if(vj4 > 0){
                            read(pipes[7][0], &j4_a_otro, sizeof(j4_a_otro));
                            ataques[3] = j4_a_otro;
                        }
                        //Calcular cuantos ataques recibe cada jugador.
                        for(int i = 0; i < 4; i++){
                            num_actual = ataques[i];
                            if(num_actual == 1){
                                cj1r ++;
                            } else if(num_actual == 2){
                                cj2r ++;
                            } else if(num_actual == 3){
                                cj3r ++;
                            } else if( num_actual == 4){
                                cj4r ++;
                            }
                        }
                        //Enviar a los hijos cuantos ataques recibe cada uno
                        write(pipes[0][1], &cj1r, sizeof(cj1r));
                        write(pipes[2][1], &cj2r, sizeof(cj2r));
                        write(pipes[4][1], &cj3r, sizeof(cj3r));
                        write(pipes[6][1], &cj4r, sizeof(cj4r));
                        cj1r = 0;
                        cj2r = 0;
                        cj3r = 0;
                        cj4r = 0;
                        //Enviar a los hijos el daño con el que los atacan
                        for (int i = 0; i < 4; i++) {
                            switch (ataques[i]) {
                                case 1:
                                    if(vj1 > 0){
                                        danio_actual = danios[i];
                                        write(pipes[0][1], &danio_actual, sizeof(danio_actual));
                                        read(pipes[1][0], &esquivado_actual, sizeof(esquivado_actual));
                                        esquivado[i] = esquivado_actual;
                                        break;
                                    } else{
                                        break;
                                    }
                                case 2:
                                    if(vj2 > 0){
                                        danio_actual = danios[i];
                                        write(pipes[2][1], &danio_actual, sizeof(danio_actual));
                                        read(pipes[3][0], &esquivado_actual, sizeof(esquivado_actual));
                                        esquivado[i] = esquivado_actual;
                                        break;
                                    } else{
                                        break;
                                    }
                                case 3:
                                    if(vj3 > 0){
                                        danio_actual = danios[i];
                                        write(pipes[4][1], &danio_actual, sizeof(danio_actual));
                                        read(pipes[5][0], &esquivado_actual, sizeof(esquivado_actual));
                                        esquivado[i] = esquivado_actual;
                                        break;
                                    } else{
                                        break;
                                    }
                                case 4:
                                    if (vj4 > 0){
                                        danio_actual = danios[i];
                                        write(pipes[6][1], &danio_actual, sizeof(danio_actual));
                                        read(pipes[7][0], &esquivado_actual, sizeof(esquivado_actual));
                                        esquivado[i] = esquivado_actual;
                                        break;
                                    } else{
                                        break;
                                    }
                                default:
                                    break;
                            }
                        }
                        //Ataques y esquivar
                        if(ataques[0] != 0){
                            cout << "Jugador 1 ha atacado a " << ataques[0] << endl;
                            if(esquivado[0] == 1){
                                cout << "Jugador " << ataques[0] << " ha esquivado el ataque de Jugador 1\n";
                            } else if(esquivado[0] == 0){
                                cout << "Jugador " << ataques[0] << " no ha esquivado el ataque de Jugador 1\n";
                            } else{
                                cout << "El jugador " << ataques[0] << " ya está muerto, pero Jugador 1 le sigue pegando en el piso\n";
                            }
                        }
                        if(ataques[1] != 0){
                            cout << "Jugador 2 ha atacado a " << ataques[1] << endl;
                            if(esquivado[1] == 1){
                                cout << "Jugador " << ataques[1] << " ha esquivado el ataque de Jugador 2\n";
                            } else if (esquivado[1] == 0){
                                cout << "Jugador " << ataques[1] << " no ha esquivado el ataque de Jugador 2\n";
                            } else{
                                cout << "El jugador " << ataques[1] << " ya está muerto, pero Jugador 2 le sigue pegando en el piso\n";
                            }
                        }
                        if(ataques[2] != 0){
                            cout << "Jugador 3 ha atacado a " << ataques[2] << endl;
                            if(esquivado[2] == 1){
                                cout << "Jugador " << ataques[2] << " ha esquivado el ataque de Jugador 3\n";
                            } else if(esquivado[2] == 0){
                                cout << "Jugador " << ataques[2] << " no ha esquivado el ataque de Jugador 3\n";
                            } else{
                                cout << "El jugador " << ataques[2] << " ya está muerto, pero Jugador 3 le sigue pegando en el piso\n";
                            }
                        }
                        if(ataques[3] != 0){
                            cout << "Jugador 4 ha atacado a " << ataques[3] << endl;
                            if(esquivado[3] == 1){
                                cout << "Jugador " << ataques[3] << " ha esquivado el ataque de Jugador 4\n";
                            } else if(esquivado[3] == 0){
                                cout << "Jugador " << ataques[3] << " no ha esquivado el ataque de Jugador 4\n";
                            } else{
                                cout << "El jugador " << ataques[3] << " ya está muerto, pero Jugador 4 le sigue pegando en el piso\n";
                            }
                        }
                        //CONDICIONES DE VICTORIA SIMPLES
                        if((vj1 > 0) && (vj2 <= 0) && (vj3 <= 0) && (vj4 <= 0)){
                            cout << "Ganaste jugador 1!" << endl;
                            ronda = -1;

                        } else if ((vj2 > 0) && (vj1 <= 0) && (vj3 <= 0) && (vj4 <= 0)){
                            cout << "Ganaste jugador 2!" << endl;
                            ronda = -1;

                        } else if ((vj3 > 0) && (vj2 <= 0) && (vj1 <= 0) && (vj4 <= 0)){
                            cout << "Ganaste jugador 3!" << endl;
                            ronda = -1;

                        } else if ((vj4 > 0) && (vj2 <= 0) && (vj3 <= 0) && (vj1 <= 0)){
                            cout << "Ganaste jugador 4!" << endl;
                            ronda = -1;
                        //CONDICIONES DE VICTORIA POR EMPATES  
                        } else if ((vj3 <= 0) && (vj4 <= 0)){
                            if((vj1 <= 0) && (vj2 <= 0)){
                                randomJugador = dis_desempate(gen);
                                if(randomJugador == 1){
                                    cout << "Por un fallo milimétrico ganaste jugador 1!" << endl;
                                    ronda = -1;
                                } else{
                                    cout << "Por un fallo milimétrico ganaste jugador 2!" << endl;
                                    ronda = -1;
                                }
                            }
                        } else if ((vj2 <= 0) && (vj4 <= 0)){
                            if((vj1 <= 0) && (vj3 <= 0)){
                                randomJugador = dis_desempate(gen);
                                if(randomJugador == 1){
                                    cout << "Por un fallo milimétrico ganaste jugador 1!" << endl;
                                    ronda = -1;
                                } else{
                                    cout << "Por un fallo milimétrico ganaste jugador 3!" << endl;
                                    ronda = -1;
                                }
                            }
                        } else if ((vj3 <= 0) && (vj2 <= 0)){
                            if((vj1 <= 0) && (vj4 <= 0)){
                                randomJugador = dis_desempate(gen);
                                if(randomJugador == 1){
                                    cout << "Por un fallo milimétrico ganaste jugador 1!" << endl;
                                    ronda = -1;
                                } else{
                                    cout << "Por un fallo milimétrico ganaste jugador 4!" << endl;
                                    ronda = -1;
                                }
                            }
                        } else if ((vj1 <= 0) && (vj4 <= 0)){
                            if((vj2 <= 0) && (vj3 <= 0)){
                                randomJugador = dis_desempate(gen);
                                if(randomJugador){
                                    cout << "Por un fallo milimétrico ganaste jugador 2!" << endl;
                                    ronda = -1;
                                } else{
                                    cout << "Por un fallo milimétrico ganaste jugador 3!" << endl;
                                    ronda = -1;
                                }
                            }
                        } else if ((vj1 <= 0) && (vj3 <= 0)){
                            if((vj2 <= 0) && (vj4 <= 0)){
                                randomJugador = dis_desempate(gen);
                                if(randomJugador == 1){
                                    cout << "Por un fallo milimétrico ganaste jugador 2!" << endl;
                                    ronda = -1;
                                } else{
                                    cout << "Por un fallo milimétrico ganaste jugador 4!" << endl;
                                    ronda = -1;
                                }
                            }
                        } else if ((vj1 <= 0) && (vj2 <= 0)){
                            if((vj3 <= 0) && (vj4 <= 0)){
                                randomJugador = dis_desempate(gen);
                                if(randomJugador == 1){
                                    cout << "Por un fallo milimétrico ganaste jugador 3!" << endl;
                                    ronda = -1;
                                } else{
                                    cout << "Por un fallo milimétrico ganaste jugador 4!" << endl;
                                    ronda = -1;
                                }
                            }
                        }
                        cout << "\n***Fin de la ronda ***\n\n";
                        //Reseteo de valores a 0
                        for(int i = 0; i < 4; i++){
                            ataques[i] = 0;
                            esquivado[i] = -1;
                        }
                        //Manejo final, luego de que algun jugador gana la partida.
                        if(ronda == -1){
                            write(pipes[0][1], &ronda, sizeof(ronda));
                            write(pipes[2][1], &ronda, sizeof(ronda));
                            write(pipes[4][1], &ronda, sizeof(ronda));
                            write(pipes[6][1], &ronda, sizeof(ronda));
                        }
                    }
                    if(vj1 == -1){
                        write(pipes[0][1], &ronda, sizeof(ronda));
                        write(pipes[2][1], &ronda, sizeof(ronda));
                        write(pipes[4][1], &ronda, sizeof(ronda));
                        write(pipes[6][1], &ronda, sizeof(ronda));
                        cout << "Ganaste jugador 1!" << endl;
                    }
                }

            }
        }
    }
    //Cierre de los pipes y finalizacion del juego
    close(pipes[0][1]);
    close(pipes[0][0]);
    close(pipes[1][1]);
    close(pipes[1][0]);
    close(pipes[2][1]);
    close(pipes[2][0]);
    close(pipes[3][1]);
    close(pipes[3][0]);
    close(pipes[4][1]);
    close(pipes[4][0]);
    close(pipes[5][1]);
    close(pipes[5][0]);
    close(pipes[6][1]);
    close(pipes[6][0]);
    close(pipes[7][1]);
    close(pipes[7][0]);
    cout << "FIN DEL JUEGO" << endl;
    return 0;
}