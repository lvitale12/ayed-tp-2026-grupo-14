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
bool cargarVenta(FILE* archivoMozos, FILE* archivoInventario, const char* nombreArchivo);

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

// Por ahora valido todo y calculo la comision, pero todavia no grabo nada.
bool cargarVenta(FILE* archivoMozos, FILE* archivoInventario, const char*) {
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

    float comision = producto.precio * cantidad * TASA_COMISION;

    cout << "Venta valida: " << cantidad << " x " << producto.descripcion
         << " | comision " << comision << " (todavia no se guarda)" << endl;
    return false;
}
