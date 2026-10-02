#include <iostream>
#include <cstring>
using namespace std;

struct Comanda {
    int idMozo;
    int codigoProducto;
    int cantidad;
    float comision;
};

void apareoDosDias (const char* diaA, const char* diaB, const char* aux);

void moverComanda (const char* aux, const char* comanda);

void comandaSemanal (char idSemana[], char comandaSemana[]);

void diasSemana (char idSemana[], char matDias[][100], int &cont);

bool existeArchivo (const char* archivo);

void armarFecha (int dia, int mes, int anio, char fecha[]); 

int main() {
    
    char matDias[7][100];
    char idSemana[10];
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
            moverComanda ("aux.dat", "comandas_semana.dat");
        }
    };

    char comandaFinal[40];
    comandaSemanal(idSemana, comandaFinal);
    moverComanda ("comandas_semana.dat", comandaFinal);
    cout << "Archivo semanal generado: " << comandaFinal;

    return 0;
}

void apareoDosDias (const char* diaA, const char* diaB, const char* diaAux){
    FILE* a = fopen(diaA, "rb");
    FILE* b = fopen(diaB, "rb");
    FILE* aux = fopen(diaAux, "wb");
    Comanda ca, cb; // ca=ComandaA
    int la = fread (&ca, sizeof(Comanda), 1, a); 
    int lb = fread (&cb, sizeof(Comanda), 1, b);
    while (la==1 && lb==1){  
        if (ca.idMozo < cb.idMozo){
            fwrite(&ca, sizeof(Comanda), 1, aux);
            la = fread(&ca, sizeof(Comanda), 1, a); 
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

void moverComanda (const char* aux, const char* comanda){
    FILE* a = fopen(aux, "rb");
    FILE* c = fopen(comanda, "wb"); 
    Comanda x;
    while ((fread(&x, sizeof(Comanda), 1, a)==1)){ //Recorrido seguro
        fwrite(&x, sizeof(Comanda), 1, c);
    } 
    fclose(a);
    fclose(c);
}

void comandaSemanal (char idSemana[], char comandaSemana[]){
    strcpy (comandaSemana, "comandas_semana_");
    strcat (comandaSemana, idSemana);
    strcat (comandaSemana, ".dat");
}

bool existeArchivo (const char* archivo){
    FILE* x = fopen (archivo, "rb");
    if (x==0){
        return false;
    }
    fclose(x);
    return true;
}

void armarFecha (int dia, int mes, int anio, char fecha[]){

    strcpy (fecha, "comandas_");
   
    char decDia = '0' + (dia/10);
    char unDia = '0' + (dia%10);
    char diaCompl[3] = {decDia, unDia, '\0'};
    strcat (fecha, diaCompl); 

    char decMes = '0' + (mes/10);
    char unMes = '0' + (mes%10);
    char mesCompl[4] = {'-', decMes, unMes, '\0'};
    strcat (fecha, mesCompl);

    char milAnio = '0' + (anio/1000);
    char cenAnio = '0' + ((anio/100)%10);
    char decAnio = '0' + ((anio/10)%10);
    char unAnio = '0' + (anio%10);
    char anioCompl[10] = {'-', milAnio, cenAnio, decAnio, unAnio, '.', 'd', 'a', 't', '\0'};
    strcat (fecha, anioCompl);

    return;
}

void diasSemana (char idSemana[], char matDias[][100], int &cont){
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

    int anio;
    cout << "Ingrese el año: " << endl;
    cin >> anio;

    if (semana == 5){ 
        for (int i = 29; i<=31; i++){
            char fecha[100];
            armarFecha(i, mes, anio, fecha);
            if (existeArchivo(fecha)==true){
                cout << fecha << endl;
                strcpy (matDias[cont], fecha);
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
                strcpy (matDias[cont], fecha); 
                cont++;
            } else {
                cout << "No hay un archivo del dia " << i << "/" << mes << endl;
            }
        }
        cout << "La cantidad de días cargados son: " << cont << endl;
    }
}
