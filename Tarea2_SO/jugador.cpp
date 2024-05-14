#include <cstring>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <string>
#include <ctype.h>
#include <random>


using namespace std;
//****//
//Clase Jugador, recibe un numero id para facilitar su uso en el programa principal
//****//
class Jugador{
    //****//
    //Variables necesarias para el funcionamiento de los jugadores en el juego
    //****//
    private:
        int vida;
        int ataque;
        int defensa;
        int evasion;
        int id;

    
    public:
        //****//
        //Constructor de la clase, inicializa las variables que esta contiene 
        //****//
        Jugador(int ide){
            random_device rd1,rd2;
            mt19937 gen1(rd1());
            mt19937 gen2(rd2());
            int min_ataque, max_ataque, min_defensa, max_defensa;
            min_ataque = 30;
            max_ataque = 40;
            min_defensa = 10;
            max_defensa = 25;
            uniform_int_distribution<> dis_ataque(min_ataque, max_ataque);
            uniform_int_distribution<> dis_defensa(min_defensa, max_defensa);
            int randomAtaque = dis_ataque(gen1);
            int randomDefensa = dis_defensa(gen2);

            vida = 100;
            ataque = randomAtaque;
            defensa = randomDefensa;
            evasion = 60 - defensa;

            this->id = ide;
        }

        //****//
        //Destructor de la clase
        //****//
        ~Jugador(){
        }

        //****//
        //Getters de la clase
        //****//
        int getVida() const {
            return vida;
        }

        int getAtaque() const {
            return ataque;
        }

        int getDefensa() const {
            return defensa;
        }

        int getEvasion() const {
            return evasion;
        }
        
        int getId() const{
            return id;
        } 

        //****//
        //Metodo recibirDanio, recibe el danio del atacante y revisa si el jugador esquiva el ataque,
        //si lo esquiva, retorna 1 y si no lo hace se le descuenta vida al jugador y se retorna 0
        //****//                        
        int recibirDanio(int ataque_e){
            int danio;
            random_device rd1;
            mt19937 gen1(rd1());
            int logro_evadir;
            int min_evasion = 0;
            int max_evasion = 100;
            uniform_int_distribution<> dis_evasion(min_evasion, max_evasion);
            int randomEvasion = dis_evasion(gen1);
            if(randomEvasion > evasion){
                danio = ataque_e - defensa;
                vida -= danio;
                logro_evadir = 0;
            } else {
                logro_evadir = 1;
                return logro_evadir;
            }
            return logro_evadir;
        }
};
