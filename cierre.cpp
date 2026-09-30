#include <iostream>
using namespace std;

//Devuelvo un archivo ordenado por IdMozo con todos los dias de la semana, osea comandas_semana_s1-06.dat
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
    
    string idSemana;
    cout << "Ingrese el ID semanal: " << endl;
    cin >> idSemana;

    diasSemana(idSemana);
    //solo estoy retornando el nombre de los archivos no los estoy guardando

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
string armarFecha (int dia, int mes, int anio){
    string archivoFecha = "comandas_";
    // calculo decenas y unidades
    char decDia = '0' + (dia/10);
    archivoFecha+=decDia;
    char unDia = '0' + (dia%10);
    archivoFecha+=unDia;
    archivoFecha+="-";

    char decMes = '0' + (mes/10);
    archivoFecha+=decMes;
    char unMes = '0' + (mes%10);
    archivoFecha+=unMes;
    archivoFecha+="-";

    char milAnio = '0' + (anio/1000);
    archivoFecha+=milAnio;
    char cenAnio = '0' + ((anio/100)%10);
    archivoFecha+=cenAnio;
    char decAnio = '0' + ((anio/10)%10);
    archivoFecha+=decAnio;
    char unAnio = '0' + (anio%10);
    archivoFecha+=unAnio;

    archivoFecha+=".dat";
    return archivoFecha;
}

//Desglosa el STRING idSemana (formato s(num de semana)_(mes)) para poder obtener los archivos. Empieza en el 1 de cada mes y no se suporponen los meses.
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

    if (semana == 5){  // La semana 5 serian los dias 29-31
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
        int primerDia = (ultDia-6);
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
