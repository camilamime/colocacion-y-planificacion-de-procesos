// Proyecto Sistemas Operativos//

#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct Bloque {
    int id;
    int tam;
    int cuanto;
    Bloque* anterior;
    Bloque* sig;
};

struct Proceso {
    int id;
    Proceso* sig;
};

void imprimirMem(Bloque* inicio);
void imprimirCola(Proceso* frente);
void condensarMemoria(Bloque* inicio);
Bloque* BusquedaFirst(Bloque* inicio, int tam);
Bloque* BusquedaWorst(Bloque* inicio, int tam);
Bloque* buscarHueco(Bloque* inicio, int tam, int algoritmo);
Bloque* crearbloq(int id, int tam, int cuanto);
Bloque* buscarProceso(Bloque* inicio, int id);
int menu();
int menuMemoria();
int colocar(Bloque* hueco, int id, int tam, int cuanto);
int liberarMemoria(Bloque* inicio, int id);
void encolar(Proceso*& frente, Proceso*& final, int id);
int desencolar(Proceso*& frente, Proceso*& final);
void RoundRobin(Bloque* memoria, Proceso*& frente,
                Proceso*& final, int quantum);

Bloque* crearbloq(int id, int tam, int cuanto) {
    Bloque* b = new Bloque;
    b->id = id;
    b->tam = tam;
    b->cuanto = cuanto;
    b->anterior = nullptr;
    b->sig = nullptr;
    return b;
}

int main() {
    int algoritmo = menu();
    int tamMem = menuMemoria();

    int quantumSistema;

    cout << "Quantum del sistema: ";
    cin >> quantumSistema;

    while (cin.fail() || quantumSistema <= 0) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Quantum invalido. Introduce otro: ";
        cin >> quantumSistema;
    }

    Bloque* memoria = crearbloq(0, tamMem, 0);

    Proceso* frente = nullptr;
    Proceso* final = nullptr;

    int cantidad;
    cout << "Cantidad de procesos: ";
    cin >> cantidad;

    for (int i = 0; i < cantidad; i++) {
        int id, tam, cuanto;

        cout << "\nProceso " << i + 1 << endl;

        cout << "ID: ";
        cin >> id;

        cout << "Tamano: ";
        cin >> tam;

        cout << "Cuanto: ";
        cin >> cuanto;

        if (id <= 0 || tam <= 0 || cuanto <= 0) {
            cout << "Datos invalidos" << endl;
            i--;
            continue;
        }

        if (buscarProceso(memoria, id) != nullptr) {
            cout << "El ID ya existe" << endl;
            i--;
            continue;
        }

        Bloque* hueco = buscarHueco(memoria, tam, algoritmo);

        if (hueco != nullptr) {
            if (colocar(hueco, id, tam, cuanto)) {
                encolar(frente, final, id);
                cout << "Proceso agregado correctamente" << endl;
            }
        }
        else {
            cout << "No hay suficiente memoria para el proceso" << endl;
        }

        imprimirMem(memoria);
        imprimirCola(frente);
    }

    cout << "\n--- INICIO ROUND ROBIN ---\n";

    RoundRobin(memoria, frente, final, quantumSistema);

    cout << "\n--- SIMULACION TERMINADA ---" << endl;

    imprimirMem(memoria);

    delete memoria;

    return 0;
}

int menuMemoria() {
    int opc;

    do {
        cout << "\nTamano de la memoria:" << endl;
        cout << "1. 1 MB" << endl;
        cout << "2. 4 MB" << endl;
        cout << "3. 8 MB" << endl;
        cin >> opc;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            opc = 0;
        }

        if (opc < 1 || opc > 3)
            cout << "Opcion invalida" << endl;

    } while (opc < 1 || opc > 3);

    if (opc == 1) return 1024;
    if (opc == 2) return 4096;

    return 8192;
}

int menu() {
    int opc;

    do {
        cout << "\nWhat algorithm do you want to use?" << endl;
        cout << "1. First Fit" << endl;
        cout << "2. Worst Fit" << endl;
        cin >> opc;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            opc = 0;
        }

        if (opc != 1 && opc != 2)
            cout << "Invalid option" << endl;

    } while (opc != 1 && opc != 2);

    return opc;
}

void imprimirMem(Bloque* inicio) {
    for (Bloque* b = inicio; b != nullptr; b = b->sig)
        cout << "[" << b->id << "," << b->tam << ","
             << b->cuanto << "]";

    cout << endl;
}

void imprimirCola(Proceso* frente) {
    cout << "Lista RR: ";

    for (Proceso* p = frente; p != nullptr; p = p->sig)
        cout << "[" << p->id << "] ";

    cout << endl;
}

int colocar(Bloque* hueco, int id, int tam, int cuanto) {
    if (hueco == nullptr || hueco->id != 0 || hueco->tam < tam)
        return 0;

    int tamOriginal = hueco->tam;
    int sobrante = tamOriginal - tam;

    if (sobrante >= 32) {
        Bloque* resto = crearbloq(0, sobrante, 0);

        resto->anterior = hueco;
        resto->sig = hueco->sig;

        if (hueco->sig != nullptr)
            hueco->sig->anterior = resto;

        hueco->sig = resto;

        hueco->tam = tam;
    }
    else {
        hueco->tam = tamOriginal;
    }

    hueco->id = id;
    hueco->cuanto = cuanto;

    return 1;
}

Bloque* BusquedaFirst(Bloque* inicio, int tam) {
    for (Bloque* b = inicio; b != nullptr; b = b->sig)
        if (b->id == 0 && b->tam >= tam)
            return b;

    return nullptr;
}

Bloque* BusquedaWorst(Bloque* inicio, int tam) {
    Bloque* peor = nullptr;

    for (Bloque* b = inicio; b != nullptr; b = b->sig) {
        if (b->id == 0 && b->tam >= tam) {
            if (peor == nullptr || b->tam > peor->tam)
                peor = b;
        }
    }

    return peor;
}

Bloque* buscarHueco(Bloque* inicio, int tam, int algoritmo) {
    if (algoritmo == 1)
        return BusquedaFirst(inicio, tam);

    if (algoritmo == 2)
        return BusquedaWorst(inicio, tam);

    return nullptr;
}

Bloque* buscarProceso(Bloque* inicio, int id) {
    for (Bloque* b = inicio; b != nullptr; b = b->sig)
        if (b->id == id)
            return b;

    return nullptr;
}

void condensarMemoria(Bloque* inicio) {
    Bloque* actual = inicio;

    while (actual != nullptr && actual->sig != nullptr) {
        if (actual->id == 0 && actual->sig->id == 0) {
            Bloque* eliminar = actual->sig;

            actual->tam += eliminar->tam;
            actual->sig = eliminar->sig;

            if (eliminar->sig != nullptr)
                eliminar->sig->anterior = actual;

            delete eliminar;
        }
        else {
            actual = actual->sig;
        }
    }
}

int liberarMemoria(Bloque* inicio, int id) {
    Bloque* actual = inicio;

    while (actual != nullptr) {
        if (actual->id == id) {
            actual->id = 0;
            actual->cuanto = 0;

            condensarMemoria(inicio);

            return 1;
        }

        actual = actual->sig;
    }

    return 0;
}

void encolar(Proceso*& frente, Proceso*& final, int id) {
    Proceso* nuevo = new Proceso;

    nuevo->id = id;
    nuevo->sig = nullptr;

    if (frente == nullptr) {
        frente = nuevo;
        final = nuevo;
    }
    else {
        final->sig = nuevo;
        final = nuevo;
    }
}

int desencolar(Proceso*& frente, Proceso*& final) {
    if (frente == nullptr)
        return -1;

    Proceso* eliminar = frente;
    int id = eliminar->id;

    frente = frente->sig;

    if (frente == nullptr)
        final = nullptr;

    delete eliminar;

    return id;
}

void RoundRobin(Bloque* memoria, Proceso*& frente,
                Proceso*& final, int quantum) {

    int ciclo = 1;

    while (frente != nullptr) {
        cout << "\n========== CICLO " << ciclo << " ==========" << endl;

        int id = desencolar(frente, final);

        Bloque* actual = buscarProceso(memoria, id);

        if (actual == nullptr)
            continue;

        cout << "Atendiendo proceso [" << actual->id << ","
             << actual->tam << "," << actual->cuanto << "]" << endl;

        if (actual->cuanto <= quantum) {
            actual->cuanto = 0;

            cout << "Descargando proceso [" << id << "]" << endl;

            liberarMemoria(memoria, id);

            cout << "Condensando huecos" << endl;
        }
        else {
            actual->cuanto -= quantum;

            encolar(frente, final, id);
        }

        imprimirCola(frente);
        imprimirMem(memoria);

        ciclo++;
    }
}
