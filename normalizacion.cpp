#include <iostream>
#include <cstdio>
#include <cstring>

using namespace std;

struct ComandaHistorica {
    char fecha[11];
    char nombreMozo[50];
    int codigoProducto;
    int cantidad;
    float comision;
};

struct Producto {
    int codigo;
    char descripcion[50];
    float precio;
    int stockActual;
};

struct Mozo {
    int idMozo;
    char nombre[50];
    char password[20];
    float totalComision;
};

int mostrarComandasHistoricas(const char* nombreArchivo);
void mostrarInventario(const char* nombreArchivo);
int buscarMozo(Mozo mozos[], int cantidadMozos, const char nombre[]);
int cargarMozos(const char* nombreArchivo, Mozo mozos[]);
void mostrarMozos(Mozo mozos[], int cantidadMozos);

int main() {
    int cantidadComandas =
        mostrarComandasHistoricas("comandas_historicas.dat");

    if (cantidadComandas == 0) {
        return 1;
    }

    Mozo* mozos = new Mozo[cantidadComandas];
    int cantidadMozos =
        cargarMozos("comandas_historicas.dat", mozos);

    cout << endl;
    mostrarMozos(mozos, cantidadMozos);

    delete[] mozos;

    cout << endl;
    mostrarInventario("inventario.dat");
    return 0;
}

int mostrarComandasHistoricas(const char* nombreArchivo) {
    FILE* archivo = fopen(nombreArchivo, "rb");

    if (archivo == NULL) {
        cout << "No se pudo abrir " << nombreArchivo << endl;
        return 0;
    }

    ComandaHistorica comanda;
    int cantidadRegistros = 0;

    while (fread(&comanda, sizeof(ComandaHistorica), 1, archivo) == 1) {
        cout << comanda.fecha << " | "
             << comanda.nombreMozo << " | producto "
             << comanda.codigoProducto << " | cantidad "
             << comanda.cantidad << " | comision "
             << comanda.comision << endl;

        cantidadRegistros++;
    }

    fclose(archivo);

    cout << "Total de comandas historicas: "
         << cantidadRegistros << endl;

    return cantidadRegistros;
}

void mostrarInventario(const char* nombreArchivo) {
    FILE* archivo = fopen(nombreArchivo, "rb");

    if (archivo == NULL) {
        cout << "No se pudo abrir " << nombreArchivo << endl;
        return;
    }

    Producto producto;
    int cantidadProductos = 0;

    while (fread(&producto, sizeof(Producto), 1, archivo) == 1) {
        cout << producto.codigo << " | "
             << producto.descripcion << " | precio "
             << producto.precio << " | stock "
             << producto.stockActual << endl;

        cantidadProductos++;
    }

    fclose(archivo);

    cout << "Total de productos: "
         << cantidadProductos << endl;
}

int buscarMozo(Mozo mozos[], int cantidadMozos, const char nombre[]) {
    for (int i = 0; i < cantidadMozos; i++) {
        if (strcmp(mozos[i].nombre, nombre) == 0) {
            return i;
        }
    }

    return -1;
}

int cargarMozos(const char* nombreArchivo, Mozo mozos[]) {
    FILE* archivo = fopen(nombreArchivo, "rb");

    if (archivo == NULL) {
        cout << "No se pudo abrir " << nombreArchivo << endl;
        return 0;
    }

    ComandaHistorica comanda;
    int cantidadMozos = 0;

    while (fread(&comanda, sizeof(ComandaHistorica), 1, archivo) == 1) {
        int posicion =
            buscarMozo(mozos, cantidadMozos, comanda.nombreMozo);

        if (posicion == -1) {
            mozos[cantidadMozos].idMozo = cantidadMozos + 1;
            strcpy(mozos[cantidadMozos].nombre, comanda.nombreMozo);
            mozos[cantidadMozos].password[0] = '\0';
            mozos[cantidadMozos].totalComision = comanda.comision;
            cantidadMozos++;
        } else {
            mozos[posicion].totalComision += comanda.comision;
        }
    }

    fclose(archivo);
    return cantidadMozos;
}

void mostrarMozos(Mozo mozos[], int cantidadMozos) {
    float comisionTotal = 0;

    cout << "Mozos encontrados:" << endl;

    for (int i = 0; i < cantidadMozos; i++) {
        cout << mozos[i].idMozo << " | "
             << mozos[i].nombre << " | comision total "
             << mozos[i].totalComision << endl;

        comisionTotal += mozos[i].totalComision;
    }

    cout << "Total de mozos: " << cantidadMozos << endl;
    cout << "Comision historica total: "
         << comisionTotal << endl;
}
