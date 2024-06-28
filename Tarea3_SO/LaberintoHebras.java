import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;

public class LaberintoHebras {
    /*
     *** Función: main
     ** Parámetros:
     * Proporcionados por el sistema.
     ** Descripción:
     * Se comienza leyendo el archivo, luego se crea el laberinto en una matriz,
     * luego se llama a la función de Hebras, para luego dar inicio a la resolución.
     ***
     */
    public static void main(String[] args) {
        String rutaArchivo = "laberinto.txt";
        try (Scanner scanner = new Scanner(new File(rutaArchivo))) {
            String tamanio_s = scanner.nextLine();
            String pos_ini_s = scanner.nextLine();
            String[] partes = tamanio_s.split("x");
            int tamanio = Integer.parseInt(partes[0]);
            pos_ini_s = pos_ini_s.substring(1, pos_ini_s.length() -1);
            partes = pos_ini_s.split(",");
            int inicioX = Integer.parseInt(partes[0].trim());
            int inicioY = Integer.parseInt(partes[1].trim());
            String[][] laberinto = null;
            String[] actual;
            laberinto = new String[tamanio][tamanio];
            int i = 0;
            while (scanner.hasNextLine()) {
                String linea = scanner.nextLine();
                actual = linea.split(" ");
                for (int j = 0; j < tamanio;j++){
                    laberinto[i][j] = actual[j];
                }
                i++;   
            }
            Hebras hebra = new Hebras(laberinto,inicioX,inicioY,tamanio,1);
            hebra.start();     
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }

        
    }
}