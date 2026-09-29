#include <iostream>
#include <cstdio>
#include <cstring>

using namespace std;

const int K = 5;

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

int contarDigitos(int numero);
void convertirIdACadena(int idMozo, char cadena[]);
void aplicarCorrimiento(char clave[]);
void generarClaveInicial(int idMozo, char clave[]);

int buscarMozo(Mozo mozos[], int cantidadMozos, const char nombre[]);
int cargarMozos(const char* nombreArchivo, Mozo mozos[]);
bool guardarMozos(const char* nombreArchivo,
                  Mozo mozos[],
                  int cantidadMozos);
int mostrarComandasHistoricas(const char* nombreArchivo);
void mostrarInventario(const char* nombreArchivo);

int main() {
    int cantidadComandas =
        mostrarComandasHistoricas("comandas_historicas.dat");

    if (cantidadComandas == 0) {
        return 1;
    }

    Mozo* mozos = new Mozo[cantidadComandas];
    int cantidadMozos =
        cargarMozos("comandas_historicas.dat", mozos);

    if (!guardarMozos("mozos.dat", mozos, cantidadMozos)) {
        delete[] mozos;
        return 1;
    }

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
            generarClaveInicial(
                mozos[cantidadMozos].idMozo,
                mozos[cantidadMozos].password
            );
            mozos[cantidadMozos].totalComision = comanda.comision;
            cantidadMozos++;
        } else {
            mozos[posicion].totalComision += comanda.comision;
        }
    }

    fclose(archivo);
    return cantidadMozos;
}

bool guardarMozos(const char* nombreArchivo,
                  Mozo mozos[],
                  int cantidadMozos) {
    FILE* archivo = fopen(nombreArchivo, "wb");

    if (archivo == NULL) {
        cout << "No se pudo crear " << nombreArchivo << endl;
        return false;
    }

    for (int i = 0; i < cantidadMozos; i++) {
        int escribio =
            fwrite(&mozos[i], sizeof(Mozo), 1, archivo);

        if (escribio != 1) {
            cout << "No se pudo escribir un mozo" << endl;
            fclose(archivo);
            return false;
        }
    }

    fclose(archivo);
    cout << "Se genero " << nombreArchivo
         << " con " << cantidadMozos << " mozos" << endl;
    return true;
}

int contarDigitos(int numero) {
    int cantidad = 0;

    while (numero > 0) {
        numero = numero / 10;
        cantidad++;
    }

    return cantidad;
}

void convertirIdACadena(int idMozo, char cadena[]) {
    int cantidad = contarDigitos(idMozo);
    int posicion = cantidad - 1;

    cadena[cantidad] = '\0';

    while (posicion >= 0) {
        int digito = idMozo % 10;
        cadena[posicion] = '0' + digito;

        idMozo = idMozo / 10;
        posicion--;
    }
}

void aplicarCorrimiento(char clave[]) {
    int i = 0;

    while (clave[i] != '\0') {
        clave[i] = clave[i] + K;
        i++;
    }
}

void generarClaveInicial(int idMozo, char clave[]) {
    convertirIdACadena(idMozo, clave);
    aplicarCorrimiento(clave);
}
