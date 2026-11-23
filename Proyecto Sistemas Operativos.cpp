// Proyecto Sistemas Operativos//

#include <iostream>
#include <string>
#include <vector>   

using namespace std;

struct Bloque{
    int id;
    int tam;
    int cuanto;
    Bloque* anterior;
    Bloque* sig;
};


void imprimirMem(Bloque* inicio);
Bloque* BusquedaFirst(Bloque* inicio, int tam);
Bloque* BusquedaWorst(Bloque* inicio, int tam);
Bloque* crearbloq(int id, int tam, int cuanto);
int menu();

Bloque* crearbloq(int id, int tam, int cuanto) {
    Bloque* b = new Bloque;
    b->id = id;
    b->tam = tam;
    b->cuanto = cuanto;
    b->anterior = nullptr;
    b->sig = nullptr;
    return b;
}



int main(){
    int algoritmo =menu ();
    Bloque* memoria =crearbloq(0,4096,0);
    imprimirMem(memoria); 
    return 0;
}


int menu(){
    int opc;
    cout << "Menu de opciones" << endl;
    cout << "Ingrese que tipo de algoritmo desea usar: " << endl;
    cout << "1. First Fit" << endl;
    cout << "2. Worst Fit" << endl;
    cin >> opc;
    if(opc!=1 && opc !=2){
        cout << "Opción invalida :p"<<endl;
    } while (opc != 1 && opc != 2);
    return opc;
}

void imprimirMem(Bloque* inicio) {
    for (Bloque* b = inicio; b != nullptr; b = b->sig)
        cout << "[" << b->id << "," << b->tam << "," << b->cuanto << "]";
    cout << endl;
}

