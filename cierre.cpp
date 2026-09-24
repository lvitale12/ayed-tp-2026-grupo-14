#include <iostream>
using namespace std;

struct Comanda {
    int idMozo;
    int codigoProducto;
    int cantidad;
    float comision;
};

void apareoDosDias (const char* diaA, const char* diaB, const char* aux);

int main() {
    
    return 0;
}

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