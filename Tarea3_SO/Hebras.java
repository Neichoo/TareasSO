import java.util.concurrent.atomic.AtomicBoolean;

public class Hebras extends Thread{
    private static final AtomicBoolean salidaEncontrada = new AtomicBoolean(false);
    private int n;
    private static String[][] laberinto;
    private int f,c;
    private volatile boolean ejecutandose = true;
    private static int tamanio;

    /*
    *** Función: Hebras
    ** Parámetros:
    * String[][] laberinto: Matriz de representación del laberinto.
    * int c: Columna donde se inicia el hilo.
    * int f: Fila donde se inicia el hilo.
    * int tamanio: Tamaño del laberinto (NxN)
    * int n: ID del hilo
    ** Descripción:
    * Constructor de hilos, para iniciar un nuevo hilo en las coordenadas indicadas.
    ***
    */
    public Hebras(String[][] laberinto,int c, int f,int tamanio,int n){
        this.n = n;
        Hebras.laberinto = laberinto;
        this.f = f;
        this.c = c;
        Hebras.tamanio = tamanio;
    }

    /*
    *** Función: run
    ** Parámetros:
    * Ninguno.
    ** Descripción:
    * Se comienza a resolver el laberinto a partir de las coordenadas (f,c).
    ***
     */
    @Override
    public void run(){
        boolean resultado = resolver(f, c);
    }

    /*
     *** Función: resolver
     ** Parámetros:
     * int f: Coordenada de la fila.
     * int c: Coordenada de la columna.
     ** Descripción:
     * Función recursiva que resuelve el laberinto, se marcan las
     * coordenadas visitadas y se busca la salida 'S', en caso de encontrarla,
     * se retorna 'True'.
     ***
     */
    public boolean resolver(int f, int c){
        if(ejecutandose){
            if(valido(f, c)){
                if("S".equals(laberinto[f][c])){
                    // Laberinto Resuelto
                    ejecutandose = false;
                    salidaEncontrada.set(true);
                    System.out.println("P" + n + "- [" + c + "," + f + "] - Salida\n");
                    System.out.println("Laberinto resuelto con hebras\n");
                    return true;
                }else{
                    int contador, cArriba, cIzq, cDer, cAbajo;
                    contador = 0;
                    cArriba = 0;
                    cAbajo = 0;
                    cIzq = 0;
                    cDer = 0;
                    laberinto[f][c] = "0"; //Marcar camino como visitado
                    if(valido(f+1, c)){
                        contador += 1;
                        cAbajo += 1;
                    }
                    if(valido(f-1, c)){
                        contador += 1;
                        cArriba += 1;
                    }
                    if(valido(f, c+1)){
                        contador += 1;
                        cDer += 1;
                    }
                    if(valido(f, c-1)){
                        contador += 1;
                        cIzq += 1;
                    }
                    if(contador == 0){
                        System.out.println("P" + n + "- [" + c + "," + f + "]\n");
                        ejecutandose = false;
                        return false;
                    } else if(contador == 1){
                        //arriba
                        boolean resultado = resolver(f-1, c);
        
                        //abajo
                        if(!resultado){
                            resultado = resolver(f+1, c);
                        }
        
                        //derecha
                        if(!resultado){
                            resultado = resolver(f, c+1);
                        }
                    
                        //izquierda
                        if(!resultado){
                            resultado = resolver(f, c-1);
                        }
                        return resultado; 
                    } else{
                        if(!salidaEncontrada.get()){
                            int n_nuevo = n;
                            if(cArriba == 1){
                                n_nuevo += 1;
                                Hebras hebraArr = new Hebras(laberinto,c,f-1,tamanio,n_nuevo);
                                hebraArr.start();
                                ejecutandose = false;
                            }
                            if(cAbajo == 1){
                                n_nuevo += 1;
                                Hebras hebraAb = new Hebras(laberinto,c,f+1,tamanio,n_nuevo);
                                hebraAb.start();
                                ejecutandose = false;
                            }
                            if(cDer == 1){
                                n_nuevo += 1;
                                Hebras hebraDer = new Hebras(laberinto,c+1,f,tamanio,n_nuevo);
                                hebraDer.start();
                                ejecutandose = false;
                            }
                            if(cIzq == 1){
                                n_nuevo += 1;
                                Hebras hebraIzq = new Hebras(laberinto,c-1,f,tamanio,n_nuevo);
                                hebraIzq.start();
                                ejecutandose = false;
                            }
                        } else{
                            System.out.println("P" + n + "- [" + c + "," + f + "]\n");
                        }
                    }
                }
            }
        }
        return false;
    }

    /*
     *** Función: valido
     ** Parámetros:
     * int f: Fila
     * int c: Columna
     ** Descripción:
     * Verifica que la posición entregada no sea un muro y que esté dentro del rango
     * de la matriz.
     ***
     */
    private static boolean valido(int f, int c){
        if(f >= 0 && c >= 0 && f < tamanio && c < tamanio ){
            String value = laberinto[f][c];
            if ("1".equals(value) || "E".equals(value) || "S".equals(value)){
                return true;
            }else{
                return false;
            }
            
        }
        return false;
    }
}