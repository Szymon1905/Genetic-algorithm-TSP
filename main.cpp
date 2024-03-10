#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <limits>
#include <fstream>
#include <chrono>
#include <Windows.h>
#include <thread>
#include "genetyczny.h"

using namespace std;


// globalne ustawienia

int global_liczba_miast;
vector<vector<int>> global_macierz;

int czas=10;
int startowa_wielkosc_populacji = 500;
float wsp_mutacji = 0.01;
float crossover_factor = 0.8;
int mutation_method = 0;
int roulette_ver = 0;
string nazwa_wczytanej_macierzy = "";


vector<vector<int>> wczytaj_macierz(const string& daneWejsciowe) {

    ifstream plikWejsciowy;  // na odczyt pliku

    plikWejsciowy.open(daneWejsciowe);    // otwieram plik
    if (plikWejsciowy.is_open()) {          // sprawdzam czy plik poprawnie otwarty
        cout << "Otwarto plik " << daneWejsciowe << endl;
        nazwa_wczytanej_macierzy = daneWejsciowe;
    } else {
        cout << "Nie udało się otworzyć pliku wejściowego" << endl;
        return {};
    }
    int liczba_miast;
    plikWejsciowy >> liczba_miast; // wczytanie liczby miast

    vector<vector<int> > macierz(liczba_miast, vector<int>(liczba_miast));   // macierz na przechowanie danych z pliku

    for (int i = 0; i < liczba_miast; ++i) {    // wpisywanie do macierzy
        for (int j = 0; j < liczba_miast; ++j) {
            plikWejsciowy >> macierz[i][j];
        }
    }
    plikWejsciowy.close();   // zamykam plik

    global_liczba_miast = liczba_miast;
    return macierz;
}




int main() {
    SetConsoleOutputCP(CP_UTF8); // Konsola ustawiona na utf-8 aby były Polskie litery
    cout<<"Author: Szymon Borzdyński"<<endl;
    int option;

    string name;


    while(true){
        cout << "Opcje:  [] <- obecna wartość parametru" << endl;
        cout << "0 - wczytaj macierz["<<nazwa_wczytanej_macierzy<<"]" << endl;
        cout << "1 - kryterium stopu[" << czas <<"]"<<endl;
        cout << "2 - population startowa[" << startowa_wielkosc_populacji <<"]"<< endl;
        cout << "3 - współczynnik mutacji[" << wsp_mutacji <<"]"<< endl;
        if(mutation_method == 0){
            cout << "4 - typ mutacji[invert]"<< endl;
        } else{
            cout << "4 - typ mutacji[swapowanie]"<< endl;
        }
        cout << "5 - współczynnik krzyżowania[" << crossover_factor << "]" << endl;
        cout << "6 - algorytm genetyczny" << endl;
        cout << "7 - Roulette method[";
        if(roulette_ver == 0){
            cout << "Custom (Recommended)]"<< endl;
        } else{
            cout << "Book]"<< endl;
        }
        cin >> option;

        switch (option) {
            default:
                system("CLS");
                cout << "Invalid option" << endl << endl;
                cin >> option;
                break;
            case 0:
                cout<<"Podaj nazwę pliku: "<<endl;
                cin >> name;
                global_macierz = wczytaj_macierz(name);
                system("CLS");
                break;
            case 1:
                cout<<"Podaj kryterium stopu w sekundach: "<<endl;
                cin>>czas;
                break;
            case 2:
                cout<<"Podaj wielkość populacji startowej: "<<endl;
                cin>>startowa_wielkosc_populacji;
                system("CLS");
                break;
            case 3:
                cout<<"Podaj wartość współczynnika mutacji: "<<endl;
                cin >> wsp_mutacji;
                system("CLS");
                break;
            case 4:
                cout<<"Podaj typ mutacji: "<<endl;
                cout<<"0 - invert"<<endl;  //Mutacja przez inwersję (Inversion Mutation):
                cout<<"1 - swapowanie"<<endl;  // Mutacja przez zamianę (Swap Mutation):
                cin >> mutation_method;
                while(mutation_method != 0 and mutation_method != 1){
                    cout<<"błędny parametr, podaj ponownie "<<endl;
                    cin >> mutation_method;
                }
                system("CLS");
                break;
            case 5:
                cout<<"Podaj wartość współczynnika krzyżowania: "<<endl;
                cin >> crossover_factor;
                break;
            case 6:
                if(global_macierz.empty()){
                    cout<<"pusta macierz"<<endl;
                    break;
                }
                cout<<"Start algorytmu"<<endl;
                genetic(czas);
                break;
            case 7:
                cout<<"Podaj wersje ruletki: "<<endl;
                cout<<" 0 - Własna"<<endl;
                cout<<" 1 - Literaturowa"<<endl;
                cin >> roulette_ver;
                break;
            case 8:
                return 0;
        }
    }
}
