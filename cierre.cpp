#include <iostream>
#include <cstring>
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

void diasSemana (string idSemana, char matDias[][100], int &cont);

bool existeArchivo (char archivo[]);

void armarFecha (int dia, int mes, int anio, char fecha[]); // Cambio de string a void por que no puedo devolver un array

int main() {
    
    char matDias[7][100]; // 7 filas de dia con 100 char cada una
    string idSemana;
    int cantidadDias = 0;
    cout << "Ingrese el ID semanal: " << endl;
    cin >> idSemana;

    diasSemana(idSemana, matDias, cantidadDias);
    
    if (cantidadDias == 0){
        cout << "No hay dias cargados en la semana " << idSemana << "." << endl;
    }
    else {
        apareoDosDias(matDias[0], matDias[1], "comandas_semana.dat");
        for (int i = 2; i < cantidadDias; i++) {
            apareoDosDias("comandas_semana.dat", matDias[i], "aux.dat");
            //swap --> aux tiene que ser comandas
}
    };

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
bool existeArchivo (const char* archivo){
    FILE* x = fopen (archivo, "rb");
    if (x==0){
        return false;
    }
    fclose(x);
    return true;
}

// Pasa los int de la fecha al formato comandas_DD-MM-AAA.dat
void armarFecha (int dia, int mes, int anio, char fecha[]){
    // calculo decenas y unidades

    strcpy (fecha, "comandas_"); //Copia comandas_ al char fecha desde la pos 0
   
    char decDia = '0' + (dia/10);
    char unDia = '0' + (dia%10);
    char diaCompl[3] = {decDia, unDia, '\0'};
    strcat (fecha, diaCompl); //Concatena 2 char

    char decMes = '0' + (mes/10);
    char unMes = '0' + (mes%10);
    char mesCompl[4] = {'-', decMes, unMes, '\0'};
    strcat (fecha, mesCompl);


    char milAnio = '0' + (anio/1000);
    char cenAnio = '0' + ((anio/100)%10);
    char decAnio = '0' + ((anio/10)%10);
    char unAnio = '0' + (anio%10);
    char anioCompl[7] = {'-', milAnio, cenAnio, decAnio, unAnio, '.dat', '\0'};
    strcat (fecha, anioCompl);

    return;
}

//Desglosa el STRING idSemana (formato s(num de semana)_(mes)) para poder obtener los archivos. Empieza en el 1 de cada mes y no se suporponen los meses.
void diasSemana (string idSemana, char matDias[][100], int &cont){
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
            char fecha[100];
            armarFecha(i, mes, anio, fecha);
            if (existeArchivo(fecha)==true){
                cout << fecha << endl;
                strcpy (matDias[cont], fecha); // Copio en la fila del cont (Una fila/Suma al contador es un dia), el archivo de fecha obtenido
                cont++;
            } else {
                cout << "No hay un archivo del dia " << i << "/" << mes << endl;
            }
        }
        cout << "La cantidad de días cargados son: " << cont << endl;
    } else {
        int ultDia = (semana*7);
        int primerDia = (ultDia-6);
        for (int i = primerDia; i<=ultDia; i++){
           char fecha[100];
        armarFecha(i, mes, anio, fecha);
            if (existeArchivo(fecha)==true){
                cout << fecha << endl;
                strcpy (matDias[cont], fecha); // Copio en la fila del cont (Una fila/Suma al contador es un dia), el archivo de fecha obtenido
                cont++;
            } else {
                cout << "No hay un archivo del dia " << i << "/" << mes << endl;
            }
        }
        cout << "La cantidad de días cargados son: " << cont << endl;
    }
    // Aca separo por semanas completas de 7 dias, recorro, analizo existencia y devuelvo nombre de archivos y los ordeno en un array.
}
