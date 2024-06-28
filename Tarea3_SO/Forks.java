import java.util.concurrent.RecursiveTask;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.ForkJoinPool;

public class Forks extends RecursiveTask<Boolean> {
    private static final AtomicBoolean salidaEncontrada = new AtomicBoolean(false);
    private static String[][] laberinto;
    private int n;
    private int f, c;
    private static int tamanio;
    private static final long serialVersionUID = 1L;

    /*
    *** Función: Forks
    ** Parámetros:
    * String[][] laberinto: Matriz de representación del laberinto.
    * int c: Columna donde se inicia el hilo.
    * int f: Fila donde se inicia el hilo.
    * int tamanio: Tamaño del laberinto (NxN)
    * int n: ID del hilo
    ** Descripción:
    * Constructor de forks, para iniciar un nuevo fork en las coordenadas indicadas.
    ***
    */
    public Forks(String[][] laberinto, int c, int f, int tamanio, int n) {
        this.n = n;
        Forks.laberinto = laberinto;
        this.f = f;
        this.c = c;
        Forks.tamanio = tamanio;
    }

    /*
    *** Función: compute
    ** Parámetros:
    * Ninguno.
    ** Descripción:
    * Se comienza a resolver el laberinto a partir de las coordenadas (f,c).
    ***
     */
    @Override
    protected Boolean compute() {
        return resolver(f, c);
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
    private boolean resolver(int f, int c) {
        if (valido(f, c)) {
            if ("S".equals(laberinto[f][c])) {
                // Laberinto Resuelto
                salidaEncontrada.set(true);
                System.out.println("P" + n + "- [" + c + "," + f + "] - Salida\n");
                System.out.println("Laberinto resuelto con forks\n");
                return true;
            } else {
                int contador = 0, cArriba = 0, cIzq = 0, cDer = 0, cAbajo = 0;
                laberinto[f][c] = "0"; // Marcar camino como visitado
                if (valido(f + 1, c)) {
                    contador++;
                    cAbajo++;
                }
                if (valido(f - 1, c)) {
                    contador++;
                    cArriba++;
                }
                if (valido(f, c + 1)) {
                    contador++;
                    cDer++;
                }
                if (valido(f, c - 1)) {
                    contador++;
                    cIzq++;
                }
                if (contador == 0) {
                    System.out.println("P" + n + "- [" + c + "," + f + "]\n");
                    return false;
                } else if (contador == 1) {
                    // arriba
                    boolean resultado = resolver(f - 1, c);

                    // abajo
                    if (!resultado) {
                        resultado = resolver(f + 1, c);
                    }

                    // derecha
                    if (!resultado) {
                        resultado = resolver(f, c + 1);
                    }

                    // izquierda
                    if (!resultado) {
                        resultado = resolver(f, c - 1);
                    }
                    return resultado;
                } else {
                    if (!salidaEncontrada.get()) {
                        int n_nuevo = n;
                        if (cArriba == 1) {
                            n_nuevo += 1;
                            Forks forkArr = new Forks(laberinto, c, f - 1, tamanio, n_nuevo);
                            ForkJoinPool poolArr = new ForkJoinPool();
                            Boolean resultado = poolArr.invoke(forkArr);
                        }
                        if (cAbajo == 1) {
                            n_nuevo += 1;
                            Forks forkAb = new Forks(laberinto, c, f + 1, tamanio, n_nuevo);
                            ForkJoinPool poolAb = new ForkJoinPool();
                            Boolean resultado = poolAb.invoke(forkAb);
                        }
                        if (cDer == 1) {
                            n_nuevo += 1;
                            Forks forkDer = new Forks(laberinto, c + 1, f, tamanio, n_nuevo);
                            ForkJoinPool poolDer = new ForkJoinPool();
                            Boolean resultado = poolDer.invoke(forkDer);
                        }
                        if (cIzq == 1) {
                            n_nuevo += 1;
                            Forks forkIzq = new Forks(laberinto, c - 1, f, tamanio, n_nuevo);
                            ForkJoinPool poolIzq = new ForkJoinPool();
                            Boolean resultado = poolIzq.invoke(forkIzq);
                        }
                    } else {
                        System.out.println("P" + n + "- [" + c + "," + f + "]\n");
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
    private static boolean valido(int f, int c) {
        if (f >= 0 && c >= 0 && f < tamanio && c < tamanio) {
            String value = laberinto[f][c];
            return "1".equals(value) || "E".equals(value) || "S".equals(value);
        }
        return false;
    }
}
