// Proyecto Sistemas Operativos

#include <iostream>
#include <deque>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>
#include <conio.h>   // _kbhit, _getch (Windows)

using namespace std;

const int MIN_SPLIT = 32;   // tamano minimo de split en KB

struct Bloque {
    int id;
    int tam;
    int cuanto;
    Bloque* anterior;
    Bloque* sig;
};

struct Config {
    int tamMem;      // 1024, 4096, 8192
    int quantum;     // cuanto del sistema
    int maxTam;      // tamano maximo de proceso
    int maxCuanto;   // cuanto maximo por proceso
    int algoritmo;   // 1 First Fit, 2 Worst Fit
};

struct Pendiente { int id, tam, cuanto; };

// ---------- Memoria ----------

Bloque* crearbloq(int id, int tam, int cuanto) {
    Bloque* b = new Bloque;
    b->id = id;
    b->tam = tam;
    b->cuanto = cuanto;
    b->anterior = nullptr;
    b->sig = nullptr;
    return b;
}

void imprimirMem(Bloque* inicio) {
    for (Bloque* b = inicio; b != nullptr; b = b->sig)
        cout << "[" << b->id << "," << b->tam << "," << b->cuanto << "]";
    cout << endl;
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

Bloque* buscar(const Config& c, Bloque* mem, int tam) {
    if (c.algoritmo == 1) return BusquedaFirst(mem, tam);
    return BusquedaWorst(mem, tam);
}

// Coloca el proceso en el hueco; respeta el split minimo
int colocar(Bloque* hueco, int id, int tam, int cuanto) {
    if (!hueco || hueco->id != 0 || hueco->tam < tam) return 0;

    if (hueco->tam - tam >= MIN_SPLIT) {
        Bloque* resto = crearbloq(0, hueco->tam - tam, 0);
        resto->anterior = hueco;
        resto->sig = hueco->sig;
        if (hueco->sig) hueco->sig->anterior = resto;
        hueco->sig = resto;
        hueco->tam = tam;
    }
    // si sobra menos de 32, el proceso se queda con el hueco completo
    hueco->id = id;
    hueco->cuanto = cuanto;
    return 1;
}

// Libera el bloque y condensa huecos vecinos
Bloque* liberar(Bloque* b) {
    b->id = 0;
    b->cuanto = 0;
    if (b->sig && b->sig->id == 0) {
        Bloque* s = b->sig;
        b->tam += s->tam;
        b->sig = s->sig;
        if (s->sig) s->sig->anterior = b;
        delete s;
    }
    if (b->anterior && b->anterior->id == 0) {
        Bloque* a = b->anterior;
        a->tam += b->tam;
        a->sig = b->sig;
        if (b->sig) b->sig->anterior = a;
        delete b;
        b = a;
    }
    return b;
}


Pendiente generarProceso(const Config& c, int& sigId) {
    Pendiente p;
    p.id = sigId++;
    p.tam = 1 + rand() % c.maxTam;
    p.cuanto = 1 + rand() % c.maxCuanto;
    return p;
}

// Un ciclo de simulacion
void ciclo(const Config& c, Bloque* mem, deque<Bloque*>& rr,
            Pendiente& pend, int& sigId, int& atendidos) {
    // 1) Intentar meter el proceso pendiente (siempre hay uno listo)
    Bloque* h = buscar(c, mem, pend.tam);
    if (h && colocar(h, pend.id, pend.tam, pend.cuanto)) {
        cout << "Entra Proceso [" << h->id << "," << h->tam << "," << h->cuanto << "]\n";
        rr.push_back(h);
        pend = generarProceso(c, sigId);
        imprimirMem(mem);
    }

    // 2) Round Robin
    if (rr.empty()) return;
    Bloque* p = rr.front();
    rr.pop_front();
    cout << "Atendiendo proceso [" << p->id << "," << p->tam << "," << p->cuanto << "]\n";
    p->cuanto -= min(c.quantum, p->cuanto);

    if (p->cuanto == 0) {
        cout << "Descargando proceso [" << p->id << "," << p->tam << ",0]\n";
        liberar(p);
        atendidos++;
        cout << "Condensando huecos\n";
    } else {
        rr.push_back(p);
    }
    cout << "Lista RR ";
    for (Bloque* q : rr)
        cout << "[" << q->id << "," << q->tam << "," << q->cuanto << "] ";
    cout << "\n";
    imprimirMem(mem);
}


int menuMemoria() {
    int opc;

    do {
        cout << "Memory size:" << endl;
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
            cout << "Invalid option" << endl;
    } while (opc < 1 || opc > 3);

    if (opc == 1) return 1024;
    if (opc == 2) return 4096;

    return 8192;
}

int menu() {
    int opc;
    do {
        cout << "Algoritmo:\n1. First Fit\n2. Worst Fit\n";
        cin >> opc;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            opc = 0;
        }
        if (opc != 1 && opc != 2) cout << "Invalid option\n";
    } while (opc != 1 && opc != 2);
    return opc;
}

int leerEntero(const string& msg, int min, int max) {
    int v;
    while (true) {
        cout << msg << " (" << min << "-" << max << "): ";
        cin >> v;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            v = min - 1;
        }
        if (v >= min && v <= max) return v;
        cout << "Valor invalido\n";
    }
}


int main() {
    srand((unsigned)time(nullptr));
    Config c;
    c.tamMem    = menuMemoria();
    c.algoritmo = menu();
    c.quantum   = leerEntero("Cuanto del sistema", 1, 100);
    c.maxTam    = leerEntero("Tamano maximo de proceso en KB", MIN_SPLIT, c.tamMem);
    c.maxCuanto = leerEntero("Cuanto maximo por proceso", 1, 100);
    int delayMs = leerEntero("Velocidad: ms entre ciclos", 50, 5000);
    cout << "Presiona una tecla para detener la simulacion\n";

    Bloque* mem = crearbloq(0, c.tamMem, 0);   // un solo hueco inicial
    deque<Bloque*> rr;
    int sigId = 1, atendidos = 0, nCiclos = 0;
    Pendiente pend = generarProceso(c, sigId);


    imprimirMem(mem);

    while (!_kbhit()) {
        nCiclos++;
        cout << "--- CICLO " << nCiclos << " ---\n";
        ciclo(c, mem, rr, pend, sigId, atendidos);
        this_thread::sleep_for(chrono::milliseconds(delayMs));
    }
    _getch();

    // Estadisticas
    int ocupado = 0;
    for (Bloque* b = mem; b; b = b->sig)
        if (b->id != 0) ocupado += b->tam;
    cout << "\n=== ESTADISTICAS ===\n";
    cout << "Ciclos ejecutados: " << nCiclos << endl;
    cout << "Procesos atendidos: " << atendidos << endl;
    cout << "Ocupacion de memoria: " << (100.0 * ocupado / c.tamMem) << "%\n";
    return 0;
}
