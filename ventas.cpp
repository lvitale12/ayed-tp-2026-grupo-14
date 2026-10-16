#include <iostream>
#include <cstdio>
#include <cstring>

using namespace std;

const int K = 5;
const float TASA_COMISION = 0.10f;
const int ID_INICIAL_MOZO = 1;   // los ids de mozos.dat arrancan en 1 y no tienen huecos

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

bool fechaValida(const char fecha[]);
void aplicarCorrimiento(char clave[]);
bool leerEntero(const char* mensaje, int& valor);
bool buscarMozoPUP(FILE* archivo, int idMozo, Mozo& mozo);
long busquedaBinaria(FILE* archivo, int codigo, Producto& producto);
bool validarMozo(FILE* archivoMozos, int& idMozo);
bool agregarComanda(const char* nombreArchivo, Comanda comanda);
bool descontarStock(FILE* archivo, long posicion, Producto producto);
bool cargarVenta(FILE* archivoMozos, FILE* archivoInventario, const char* nombreArchivo);
bool ordenarPlanilla(const char* nombreArchivo);

int main() {
    FILE* archivoMozos = fopen("mozos.dat", "rb");
    FILE* archivoInventario = fopen("inventario.dat", "rb+");

    if (archivoMozos == NULL || archivoInventario == NULL) {
        cout << "No se pudo abrir mozos.dat o inventario.dat" << endl;
        if (archivoMozos != NULL) fclose(archivoMozos);
        if (archivoInventario != NULL) fclose(archivoInventario);
        return 1;
    }

    // Leo el primer registro de cada archivo con fread: si no lo puedo leer, esta vacio.
    Mozo primerMozo;
    Producto primerProducto;
    bool mozosVacio = fread(&primerMozo, sizeof(Mozo), 1, archivoMozos) != 1;
    bool inventarioVacio = fread(&primerProducto, sizeof(Producto), 1, archivoInventario) != 1;

    if (mozosVacio || inventarioVacio) {
        if (mozosVacio) cout << "mozos.dat esta vacio" << endl;
        if (inventarioVacio) cout << "inventario.dat esta vacio" << endl;
        fclose(archivoMozos);
        fclose(archivoInventario);
        return 1;
    }

    // Pido la fecha una sola vez, asi todas las ventas de la tanda van a la misma planilla.
    // Uso char[11] como en ComandaHistorica: 10 caracteres de DD-MM-AAAA + el '\0'.
    char fecha[11] = "";
    while (!fechaValida(fecha) && cin) {
        cout << "Fecha del dia a cargar (DD-MM-AAAA): ";
        cin.getline(fecha, 11);

        if (cin.fail() && !cin.eof()) {     // escribio mas de 10 caracteres, la descarto y la vuelvo a pedir
            cin.clear();
            cin.ignore(10000, '\n');
            fecha[0] = '\0';
        }
    }

    // Armo el nombre igual que en normalizacion.cpp: comandas_<fecha>.dat
    char nombreArchivo[30] = "comandas_";
    strcat(nombreArchivo, fecha);
    strcat(nombreArchivo, ".dat");

    int opcion = -1;
    int ventasCargadas = 0;

    while (fechaValida(fecha) && opcion != 0 && cin) {
        cout << "\n=== Buffet Albert Einstein - dia " << fecha << " ===\n"
             << "1) Cargar venta\n0) Terminar" << endl;

        if (!leerEntero("Opcion: ", opcion)) {
            opcion = -1;
        } else if (opcion == 1) {
            if (cargarVenta(archivoMozos, archivoInventario, nombreArchivo)) ventasCargadas++;
        } else if (opcion != 0) {
            cout << "Opcion invalida" << endl;
        }
    }

    fclose(archivoMozos);
    fclose(archivoInventario);

    // Si no cargue ninguna venta no creo ni toco ninguna planilla.
    if (ventasCargadas == 0) {
        cout << "No se cargaron ventas" << endl;
        return 0;
    }

    if (!ordenarPlanilla(nombreArchivo)) return 1;

    cout << "Se cargaron " << ventasCargadas << " ventas. Planilla "
         << nombreArchivo << " ordenada por mozo" << endl;
    return 0;
}

bool fechaValida(const char fecha[]) {
    static const int dias[12] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int dia = 0, mes = 0, anio = 0;

    return strlen(fecha) == 10 && fecha[2] == '-' && fecha[5] == '-' &&
           sscanf(fecha, "%2d-%2d-%4d", &dia, &mes, &anio) == 3 &&
           anio >= 2000 && mes >= 1 && mes <= 12 && dia >= 1 && dia <= dias[mes - 1];
}

// Es la misma cuenta que hizo normalizacion.cpp: le sumo K a cada caracter.
void aplicarCorrimiento(char clave[]) {
    int i = 0;

    while (clave[i] != '\0') {
        clave[i] = clave[i] + K;
        i++;
    }
}

bool leerEntero(const char* mensaje, int& valor) {
    cout << mensaje;
    if (cin >> valor) return true;

    if (!cin.eof()) {            // escribio texto en vez de un numero: limpio el buffer y sigo
        cin.clear();
        cin.ignore(10000, '\n');
    }
    return false;
}

// Uso PUP (Posicion Unica Predecible): los ids son numericos, sin huecos y arrancan
// en 1, asi que la posicion la calculo (id - idInicial) y leo con un solo acceso al disco.
bool buscarMozoPUP(FILE* archivo, int idMozo, Mozo& mozo) {
    long pos = idMozo - ID_INICIAL_MOZO;

    if (pos < 0) return false;

    fseek(archivo, pos * (long)sizeof(Mozo), SEEK_SET);
    // si el archivo esta vacio o no llega a esa posicion, fread no devuelve 1
    return fread(&mozo, sizeof(Mozo), 1, archivo) == 1 && mozo.idMozo == idMozo;
}

// Busqueda binaria porque inventario.dat esta ordenado por codigo pero tiene huecos,
// asi que PUP no sirve. Devuelve la posicion del registro o -1 si no esta.
long busquedaBinaria(FILE* archivo, int codigo, Producto& producto) {
    fseek(archivo, 0, SEEK_END);
    long n = ftell(archivo) / sizeof(Producto);
    long pri = 0, ult = n - 1, pos = -1;

    while (pri <= ult && pos == -1) {
        long med = (pri + ult) / 2;

        fseek(archivo, med * (long)sizeof(Producto), SEEK_SET);
        if (fread(&producto, sizeof(Producto), 1, archivo) != 1) return -1;   // no pude leer: corto

        if (producto.codigo == codigo) pos = med;
        else if (codigo > producto.codigo) pri = med + 1;
        else ult = med - 1;
    }

    return pos;
}

// A la clave que tipea el mozo le hago la misma cuenta con la que se guardo y comparo con lo del archivo.
bool validarMozo(FILE* archivoMozos, int& idMozo) {
    Mozo mozo;

    if (!leerEntero("Numero de mozo: ", idMozo) || !buscarMozoPUP(archivoMozos, idMozo, mozo)) {
        cout << "El mozo no existe" << endl;
        return false;
    }

    char clave[20];
    cout << "Clave de " << mozo.nombre << ": ";
    cin.ignore(10000, '\n');                 // saco el Enter que me dejo leerEntero
    cin.getline(clave, 20);                  // leo hasta 19 caracteres + el '\0'

    bool demasiadoLarga = cin.fail();        // si escribio mas de lo que entra, la clave no sirve
    if (demasiadoLarga) {
        cin.clear();
        cin.ignore(10000, '\n');
    }

    aplicarCorrimiento(clave);

    if (demasiadoLarga || strcmp(clave, mozo.password) != 0) {
        cout << "Clave incorrecta" << endl;
        return false;
    }

    return true;
}

// Con "ab" agrego al final, y si la planilla del dia no existe la crea.
bool agregarComanda(const char* nombreArchivo, Comanda comanda) {
    FILE* archivo = fopen(nombreArchivo, "ab");

    if (archivo == NULL) return false;

    int escribio = fwrite(&comanda, sizeof(Comanda), 1, archivo);
    fclose(archivo);
    return escribio == 1;
}

// Edicion in situ: como la lectura (busquedaBinaria) me corrio el puntero,
// tengo que volver a APUNTAR a la misma posicion antes de GRABAR.
bool descontarStock(FILE* archivo, long posicion, Producto producto) {
    fseek(archivo, posicion * (long)sizeof(Producto), SEEK_SET);
    return fwrite(&producto, sizeof(Producto), 1, archivo) == 1;
}

// Devuelve true si la venta quedo registrada. Hago todas las validaciones ANTES de
// escribir, asi una venta rechazada no modifica ningun archivo. Despues grabo
// primero el stock y luego la comanda; si la comanda falla, restituyo el stock.
bool cargarVenta(FILE* archivoMozos, FILE* archivoInventario, const char* nombreArchivo) {
    int idMozo = 0, codigo = 0, cantidad = 0;
    Producto producto;

    if (!validarMozo(archivoMozos, idMozo)) return false;

    if (!leerEntero("Codigo de producto: ", codigo)) {
        cout << "Codigo invalido" << endl;
        return false;
    }

    long posicion = busquedaBinaria(archivoInventario, codigo, producto);

    if (posicion == -1) {
        cout << "El producto " << codigo << " no existe" << endl;
        return false;
    }

    if (!leerEntero("Cantidad: ", cantidad) || cantidad <= 0) {
        cout << "Cantidad invalida" << endl;
        return false;
    }

    if (cantidad > producto.stockActual) {      // tambien cubre el caso de stock = 0
        cout << producto.descripcion << ": stock insuficiente (quedan "
             << producto.stockActual << ")" << endl;
        return false;
    }

    Comanda comanda = {idMozo, codigo, cantidad, producto.precio * cantidad * TASA_COMISION};
    Producto original = producto;               // me guardo una copia por si tengo que deshacer
    producto.stockActual -= cantidad;

    if (!descontarStock(archivoInventario, posicion, producto)) {
        cout << "No se pudo actualizar el stock. Venta cancelada" << endl;
        return false;
    }

    if (!agregarComanda(nombreArchivo, comanda)) {
        descontarStock(archivoInventario, posicion, original);
        cout << "No se pudo guardar la venta. Stock restituido" << endl;
        return false;
    }

    cout << "Venta cargada: " << cantidad << " x " << producto.descripcion
         << " | comision " << comanda.comision
         << " | stock restante " << producto.stockActual << endl;
    return true;
}

// Llevo la planilla a memoria, la ordeno por idMozo y la reescribo. Elegi insercion
// porque es estable (las ventas de un mismo mozo quedan en el orden en que las cargue)
// y anda bien si el dia ya venia casi ordenado. Escribo primero en un temporal y recien
// despues reemplazo el original, asi si algo falla no pierdo la planilla.
bool ordenarPlanilla(const char* nombreArchivo) {
    const char* temporal = "tmp_planilla.dat";
    FILE* archivo = fopen(nombreArchivo, "rb");

    if (archivo == NULL) return false;

    fseek(archivo, 0, SEEK_END);
    long cantidad = ftell(archivo) / sizeof(Comanda);
    rewind(archivo);

    Comanda* comandas = new Comanda[cantidad];
    long len = 0;

    while (len < cantidad && fread(&comandas[len], sizeof(Comanda), 1, archivo) == 1) len++;
    fclose(archivo);

    if (len == 0) {                       // el primer fread ya no pudo leer: planilla vacia
        cout << nombreArchivo << " esta vacia, no hay nada que ordenar" << endl;
        delete[] comandas;
        return true;
    }

    if (len < cantidad) {                 // si no lei todo, no reescribo para no perder ventas
        cout << "No se pudo leer completa " << nombreArchivo << endl;
        delete[] comandas;
        return false;
    }

    for (long i = 1; i < len; i++) {
        Comanda clave = comandas[i];
        long j = i - 1;

        while (j >= 0 && comandas[j].idMozo > clave.idMozo) {
            comandas[j + 1] = comandas[j];
            j--;
        }
        comandas[j + 1] = clave;
    }

    archivo = fopen(temporal, "wb");
    bool ok = archivo != NULL && (long)fwrite(comandas, sizeof(Comanda), len, archivo) == len;
    if (archivo != NULL) fclose(archivo);
    delete[] comandas;

    if (!ok) {
        remove(temporal);
        cout << "No se pudo ordenar la planilla" << endl;
        return false;
    }

    remove(nombreArchivo);
    if (rename(temporal, nombreArchivo) != 0) {
        cout << "No se pudo reemplazar " << nombreArchivo << ". Datos ordenados en " << temporal << endl;
        return false;
    }

    return true;
}
