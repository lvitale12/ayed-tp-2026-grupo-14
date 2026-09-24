#include <iostream>
using namespace std;

//Me pasan las comandas por semana, osea comandas_semana_s1-06.dat
//Hago una funcion que al darle el identificador (string) de la semana (s1_06) obtengo cuantos dias tiene cargada esa semana (si no estan los 7 no crashea!!) y me da cada fecha como comandas_DD-MM-AAAA.dat
//Si no existe una fecha lo aviso

struct Comanda {
    int idMozo;
    int codigoProducto;
    int cantidad;
    float comision;
};

void apareoDosDias (const char* diaA, const char* diaB, const char* aux);

void diasSemana (string idSemana);

bool existeArchivo (string archivo);

string armarFecha (int dia, int mes, int anio);

int main() {
    // Construir el main !!!
    return 0;
}

//Apareo de 2 archivos ordenados por idMozo
void apareoDosDias (const char* diaA, const char* diaB, const char* diaAux){
    FILE* a = fopen(diaA, "rb");
    FILE* b = fopen(diaB, "rb");
    FILE* aux = fopen(diaAux, "wb");
    Comanda ca, cb; // ca=ComandaA
    int la = fread (&ca, sizeof(Comanda), 1, a); //Leo el 1er registro de A, si =1 hay dato
    int lb = fread (&cb, sizeof(Comanda), 1, b);
    while (la==1 && lb==1){  //Mientras haya datos en ambos archivos
        if (ca.idMozo < cb.idMozo){
            fwrite(&ca, sizeof(Comanda), 1, aux);
            la = fread(&ca, sizeof(Comanda), 1, a); // Avanzo en archivo A, equivalente a un i++
        } else {
            fwrite(&cb, sizeof(Comanda), 1, aux);
            lb = fread(&cb, sizeof(Comanda), 1, b);
        }
    }
    while (la==1){ //Agoto los archivos
        fwrite(&ca, sizeof(Comanda), 1, aux);
        la = fread(&ca, sizeof(Comanda), 1, a);
    }
    while (lb==1){
        fwrite(&cb, sizeof(Comanda), 1, aux);
        lb = fread(&cb, sizeof(Comanda), 1, b);
    }
    fclose(a);
    fclose(b);
    fclose(aux);
}

//Comprueba si abre o no
bool existeArchivo (string archivo){
    FILE* x = fopen (archivo.c_str(), "rb");
    if (x==0){
        return false;
    }
    fclose(x);
    return true;
}

// Pasa los int de la fecha al formato comandas_DD-MM-AAA.dat
// Esta recontra mal (si tiene mas de un digito no sirve), dsp corrijo !!!
string armarFecha (int dia, int mes, int anio){
    string archivoFecha = "comandas_";
    char d = '0' + dia;
    char m = '0' + mes;
    char a = '0' + anio; 
    archivoFecha = archivoFecha + d + "-" + m + "-" + a + ".dat";
    return archivoFecha;
}

//Desglosa el STRING idSemana para poder obtener los archivos. Empieza en el 1 de cada mes y no se suporponen los meses.
void diasSemana (string idSemana){
    int semana = 0;
    int mes = 0;
    for (int i=0; i<2; i++){
        if (i==1){
            semana=idSemana[i]-'0';
        }
    }
    for (int i=0; i<5; i++){
        if (i==3){
            mes=idSemana[i]-'0';
        }
        if (i==4){
            mes = (mes*10) + (idSemana[i]-'0');
        }
    }
    //Hasta aca, a partir del idSemana obtuve que numero de semana es y el mes
    int anio;
    cout << "Ingrese el año: " << endl;
    cin >> anio;
    if (semana == 5){
        for (int i = 29; i<=31; i++){
            string fecha = armarFecha(i, mes, anio);
            if (existeArchivo(fecha)==true){
                cout << fecha << endl;
            } else {
                cout << "No hay un archivo del dia " << i << "/" << mes << endl;
            }
        }
    } else {
        int ultDia = (semana*7);
        int primerDia = ultDia-6;
        for (int i = primerDia; i<=ultDia; i++){
            string fecha = armarFecha(i, mes, anio);
            if (existeArchivo(fecha)==true){
                cout << fecha << endl;
            } else {
                cout << "No hay un archivo del dia " << i << "/" << mes << endl;
            }
        }
    }
    // Aca separo por semanas completas de 7 dias, recorro, analizo existencia y devuelvo archivos.
}