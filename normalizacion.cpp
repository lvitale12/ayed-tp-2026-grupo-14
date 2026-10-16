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

struct Comanda {
    int idMozo;
    int codigoProducto;
    int cantidad;
    float comision;
};

// Conserva la fecha mientras las comandas se procesan en memoria.
struct ComandaConFecha {
    char fecha[11];
    Comanda comanda;
};

int contarDigitos(int numero);
void convertirIdACadena(int idMozo, char cadena[]);
void aplicarCorrimiento(char clave[]);
void generarClaveInicial(int idMozo, char clave[]);

int buscarMozo(Mozo mozos[], int cantidadMozos, const char nombre[]);
Comanda convertirComanda(ComandaHistorica comandaHistorica, int idMozo);
int cargarComandasNormalizadas(const char* nombreArchivo,
                               Mozo mozos[],
                               int cantidadMozos,
                               ComandaConFecha comandas[]);
void ordenarComandas(ComandaConFecha comandas[], int cantidadComandas);
bool guardarComandasDiarias(ComandaConFecha comandas[],
                            int cantidadComandas);
long buscarProducto(FILE* archivo, int codigo, Producto &producto);
bool actualizarInventario(const char* nombreComandas,
                          const char* nombreInventario);
int cargarMozos(const char* nombreArchivo, Mozo mozos[]);
bool guardarMozos(const char* nombreArchivo,
                  Mozo mozos[],
                  int cantidadMozos);
int contarComandasHistoricas(const char* nombreArchivo);

int main() {
    int cantidadComandas =
        contarComandasHistoricas("comandas_historicas.dat");

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

    ComandaConFecha* comandas =
        new ComandaConFecha[cantidadComandas];
    int cantidadNormalizadas = cargarComandasNormalizadas(
        "comandas_historicas.dat",
        mozos,
        cantidadMozos,
        comandas
    );

    if (cantidadNormalizadas != cantidadComandas) {
        delete[] comandas;
        delete[] mozos;
        return 1;
    }

    ordenarComandas(comandas, cantidadNormalizadas);

    if (!guardarComandasDiarias(comandas, cantidadNormalizadas)) {
        delete[] comandas;
        delete[] mozos;
          return 1;
    }

    if (!actualizarInventario("comandas_historicas.dat",
                              "inventario.dat")) {
        delete[] comandas;
        delete[] mozos;
        return 1;
    }

    delete[] comandas;
    delete[] mozos;
    return 0;
}

int contarComandasHistoricas(const char* nombreArchivo) {
    FILE* archivo = fopen(nombreArchivo, "rb");

    if (archivo == NULL) {
            cout << "No se pudo abrir " << nombreArchivo << endl;
        return 0;
    }

    ComandaHistorica comanda;
    int cantidadRegistros = 0;

    while (fread(&comanda, sizeof(ComandaHistorica), 1, archivo) == 1) {
        cantidadRegistros++;
    }

    fclose(archivo);
    return cantidadRegistros;
}

long buscarProducto(FILE* archivo, int codigo, Producto &producto) {
    fseek(archivo, 0, SEEK_END);
    long cantidadProductos = ftell(archivo) / sizeof(Producto);
    long primero = 0;
    long ultimo = cantidadProductos - 1;

    while (primero <= ultimo) {
        long medio = (primero + ultimo) / 2;
        fseek(archivo, medio * sizeof(Producto), SEEK_SET);

        if (fread(&producto, sizeof(Producto), 1, archivo) != 1) {
            return -1;
        }

        if (producto.codigo == codigo) {
            return medio;
        }

        if (codigo > producto.codigo) {
            primero = medio + 1;
        } else {
            ultimo = medio - 1;
        }
    }

    return -1;
}

bool actualizarInventario(const char* nombreComandas,
                          const char* nombreInventario) {
    FILE* comandas = fopen(nombreComandas, "rb");

    if (comandas == NULL) {
        cout << "No se pudo abrir " << nombreComandas << endl;
        return false;
    }

    FILE* inventario = fopen(nombreInventario, "rb+");

    if (inventario == NULL) {
        cout << "No se pudo abrir " << nombreInventario << endl;
        fclose(comandas);
        return false;
    }

    ComandaHistorica comanda;

    while (fread(&comanda,
                 sizeof(ComandaHistorica),
                 1,
                 comandas) == 1) {
        Producto producto;
        long posicion = buscarProducto(
            inventario,
            comanda.codigoProducto,
            producto
        );

        if (posicion == -1) {
            cout << "No se encontro el producto "
                 << comanda.codigoProducto << endl;
            fclose(comandas);
            fclose(inventario);
            return false;
        }

        producto.stockActual -= comanda.cantidad;
        fseek(inventario,
              posicion * sizeof(Producto),
              SEEK_SET);

        if (fwrite(&producto,
                   sizeof(Producto),
                   1,
                   inventario) != 1) {
            cout << "No se pudo actualizar el inventario" << endl;
            fclose(comandas);
            fclose(inventario);
            return false;
        }
    }

    fclose(comandas);
    fclose(inventario);
    return true;
}

int buscarMozo(Mozo mozos[], int cantidadMozos, const char nombre[]) {
    for (int i = 0; i < cantidadMozos; i++) {
         if (strcmp(mozos[i].nombre, nombre) == 0) {
            return i;
        }
    }

    return -1;
}

Comanda convertirComanda(ComandaHistorica comandaHistorica, int idMozo) {
    Comanda comandaNormalizada;

    comandaNormalizada.idMozo = idMozo;
    comandaNormalizada.codigoProducto = comandaHistorica.codigoProducto;
    comandaNormalizada.cantidad = comandaHistorica.cantidad;
     comandaNormalizada.comision = comandaHistorica.comision;

    return comandaNormalizada;
}

int cargarComandasNormalizadas(const char* nombreArchivo,
                               Mozo mozos[],
                               int cantidadMozos,
                               ComandaConFecha comandas[]) {
    FILE* archivo = fopen(nombreArchivo, "rb");

      if (archivo == NULL) {
           cout << "No se pudo abrir " << nombreArchivo << endl;
         return 0;
    }

    ComandaHistorica comandaHistorica;
    int cantidadComandas = 0;

    while (fread(&comandaHistorica,
                 sizeof(ComandaHistorica),
                 1,
                 archivo) == 1) {
        int posicionMozo = buscarMozo(
             mozos,
            cantidadMozos,
            comandaHistorica.nombreMozo
          );

        if (posicionMozo == -1) {
             fclose(archivo);
             return cantidadComandas;
        }

        strcpy(comandas[cantidadComandas].fecha,
               comandaHistorica.fecha);
        comandas[cantidadComandas].comanda = convertirComanda(
              comandaHistorica,
            mozos[posicionMozo].idMozo
        );
        cantidadComandas++;
    }

    fclose(archivo);
    return cantidadComandas;
}

// Ordena por fecha y, dentro de cada fecha, por id de mozo.
void ordenarComandas(ComandaConFecha comandas[], int cantidadComandas) {
    for (int i = 0; i < cantidadComandas - 1; i++) {
        int posicionMenor = i;

        for (int j = i + 1; j < cantidadComandas; j++) {
            int comparacionFecha = strcmp(
                   comandas[j].fecha,
                comandas[posicionMenor].fecha
            );

            if (comparacionFecha < 0 ||
                (comparacionFecha == 0 &&
                  comandas[j].comanda.idMozo <
                 comandas[posicionMenor].comanda.idMozo)) {
                posicionMenor = j;
            }
        }

        if (posicionMenor != i) {
            ComandaConFecha auxiliar = comandas[i];
            comandas[i] = comandas[posicionMenor];
            comandas[posicionMenor] = auxiliar;
        }
    }
}

bool guardarComandasDiarias(ComandaConFecha comandas[],
                            int cantidadComandas) {
    int posicion = 0;

    while (posicion < cantidadComandas) {
        char fechaActual[11];
        strcpy(fechaActual, comandas[posicion].fecha);

        char nombreArchivo[30] = "comandas_";
        strcat(nombreArchivo, fechaActual);
        strcat(nombreArchivo, ".dat");

        FILE* archivo = fopen(nombreArchivo, "wb");

        if (archivo == NULL) {
            cout << "No se pudo crear " << nombreArchivo << endl;
            return false;
        }

        while (posicion < cantidadComandas &&
               strcmp(comandas[posicion].fecha, fechaActual) == 0) {
            int escribio = fwrite(
                &comandas[posicion].comanda,
                sizeof(Comanda),
                1,
                archivo
            );

            if (escribio != 1) {
                cout << "No se pudo escribir en "
                     << nombreArchivo << endl;
                fclose(archivo);
                return false;
            }

            posicion++;
        }

        fclose(archivo);
    }

    return true;
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
