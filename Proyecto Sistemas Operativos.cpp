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
int menuMemoria();

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
    int algoritmo = menu();
    int tamMem    = menuMemoria();
    Bloque* memoria = crearbloq(0, tamMem, 0);
    imprimirMem(memoria);     
    return 0;
}

int menuMemoria(){
    int opc;
    do {
        cout << "Tamaño de la memoria:" << endl;
        cout << "1. 1 MB" << endl;
        cout << "2. 4 MB" << endl;
        cout << "3. 8 MB" << endl;
        cin >> opc;

        if (cin.fail()) {          // el usuario escribió una letra
            cin.clear();
            cin.ignore(10000, '\n');
            opc = 0;
        }
        if (opc < 1 || opc > 3)
            cout << "Opción inválida" << endl;
    } while (opc < 1 || opc > 3);

    if (opc == 1) return 1024;
    if (opc == 2) return 4096;
    return 8192;
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
    }  while (opc != 1 && opc != 2);
    return opc;
}

void imprimirMem(Bloque* inicio) {
    for (Bloque* b = inicio; b != nullptr; b = b->sig)
        cout << "[" << b->id << "," << b->tam << "," << b->cuanto << "]";
    cout << endl;
}

int colocar(Bloque* hueco, int id, int tam, int cuanto) {
    if (hueco == nullptr || hueco->id != 0 || hueco->tam < tam)
        return 0;                                  // error

    if (hueco->tam > tam) {
        Bloque* resto = crearbloq(0, hueco->tam - tam, 0);

        resto->anterior = hueco;                 
        resto->sig = hueco->sig;                  
        if (hueco->sig != nullptr)
            hueco->sig->anterior = resto;         
        hueco->sig = resto;                       
    }

    hueco->id = id;
    hueco->tam = tam;
    hueco->cuanto = cuanto;
    return 1;                                     //ya jalo
}

Bloque* BusquedaFirst(Bloque* inicio, int tam) {
    for (Bloque* b = inicio; b != nullptr; b = b->sig)
        if (b->id == 0 && b->tam >= tam)
            return b;
    return nullptr;
}

Bloque* BusquedaWorst(Bloque* inicio, int tam) {
    Bloque* peor = nullptr;
    for (Bloque* b = inicio; b != nullptr; b = b->sig)
        if (b->id == 0 && b->tam >= tam)
            if (peor == nullptr || b->tam > peor->tam)
                peor = b;
    return peor;
}