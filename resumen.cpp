#include <iostream>
#include <stdio.h>  // Requerido por la catedra para fopen, fread, fclose
#include <string.h>

using namespace std;

// --- DEFINICIÓN DE ESTRUCTURA (Fija, sin strings dinámicos) ---
struct Comanda {
    int idMozo;
    int codigoProducto;
    int cantidad;
    float comision;
};

int main() {
    char nombreArchivo[50];
    
    cout << "Ingrese nombre de la planilla semanal (ej: comandas_semana_s1-06.dat): ";
    cin >> nombreArchivo;

    // Regla de la catedra: Archivos con API estandar de C
    FILE* fSemana = fopen(nombreArchivo, "rb");
    if (fSemana == NULL) {
        cout << "Error: No se pudo abrir " << nombreArchivo << "." << endl;
        return 1;
    }

    int totalBuffetProductos = 0;
    struct Comanda comandaLeida;

    // --- LECTURA ADELANTADA ---
    int leyo = fread(&comandaLeida, sizeof(struct Comanda), 1, fSemana);

    // --- CRITERIO: MANEJAR ARCHIVO VACÍO ---
    if (leyo == 0) {
        cout << "Aviso: El archivo " << nombreArchivo << " esta vacio. No hay ventas." << endl;
        fclose(fSemana);
        return 0; // Termina la ejecución limpiamente
    }

    cout << "\n===================================================\n";
    cout << "              RESUMEN SEMANAL DEL BUFFET             \n";
    cout << "===================================================\n";

    // --- CICLO EXTERNO: Itera sobre los mozos ---
    while (leyo == 1) {
        int mozoActual = comandaLeida.idMozo;
        int totalMozoProductos = 0;
        float totalMozoComision = 0.0f;
        
        // --- CICLO INTERNO: Itera sobre comandas del MISMO mozo ---
        while (leyo == 1 && comandaLeida.idMozo == mozoActual) {
            totalMozoProductos += comandaLeida.cantidad;
            totalMozoComision += comandaLeida.comision;
            totalBuffetProductos += comandaLeida.cantidad;
            
            // --- AVANCE ---
            leyo = fread(&comandaLeida, sizeof(struct Comanda), 1, fSemana);
        }
        
        // --- CORTE DE CONTROL: Imprime mozo actual ---
        cout << "Mozo ID: " << mozoActual 
             << " | Productos Vendidos: " << totalMozoProductos 
             << " | Comision a Pagar: $" << totalMozoComision << endl;
    }

    // --- REPORTE FINAL ---
    cout << "---------------------------------------------------\n";
    cout << "TOTAL BUFFET - Productos vendidos en la semana: " << totalBuffetProductos << endl;
    cout << "===================================================\n";

    fclose(fSemana);
    return 0;
}
