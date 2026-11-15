// Proyecto Sistemas Operativos//

#include <iostream>
#include <string>
#include <vector>   

using namespace std;

void menu(){
    int opc;
    cout << "Bienvenido al menu de opciones" << endl;
    cout << "Ingrese que tipo de operacion desea realizar: " << endl;
    cout << "1. First Fit" << endl;
    cout << "2. Worst Fit" << endl;
    switch (opc){
        case 1:
            FirstFit();
            break;
        case 2:
            WorstFit();
            break;
        case default:
            cout << "Opcion no valida" << endl;
            break;
            return menu();
    }
}

void FirstFit(){
    cout << "Bienvenido al algoritmo First Fit" << endl;
    
}

void WorstFit(){
    cout << "Bienvenido al algoritmo Worst Fit" << endl;

}


int main(){

    menu();


}

