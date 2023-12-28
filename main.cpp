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
# include "genetyczny.h"

using namespace std;


// globalne ustawienia

int global_liczba_miast;
vector<vector<int>> global_macierz;

int czas;
int startowa_wielkosc_populacji = 500;
float wsp_mutacji = 0.01;
float wsp_krzyzowania = 0.8;
int sposob_mutacji = 1;
int sposob_krzyzowania = 1;



vector<vector<int>> wczytaj_macierz(const string& daneWejsciowe) {

    ifstream plikWejsciowy;  // na odczyt pliku

    plikWejsciowy.open(daneWejsciowe);    // otwieram plik
    if (plikWejsciowy.is_open()) {          // sprawdzam czy plik poprawnie otwarty
        cout << "Otwarto plik " << daneWejsciowe << endl;
    } else {
        cout << "Nie udało się otworzyć pliku wejściowego" << endl;
        exit(-1);
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
    cout<<"Autor: Szymon Borzdyński"<<endl;
    int opcja;

    string nazwa;


    while(true){
        cout << "Opcje:  [] - wartość domyślna" << endl;
        cout << "0 - wczytaj macierz" << endl;
        cout << "1 - kryterium stopu [60]" << endl;
        cout << "2 - populacja startowa" << endl;
        cout << "3 - współczynnik mutacji" << endl;
        cout << "4 - współczynnik krzyżowania" << endl;
        cout << "5 - algorytm genetyczny" << endl;
        //cout << "6 - algorytm genetyczny TEST" << endl;
        cin>>opcja;

        switch (opcja) {
            default:
                system("CLS");
                cout << "Błędna opcja" << endl << endl;
                cin >> opcja;
                break;
            case 0:
                cout<<"Podaj nazwę pliku: "<<endl;
                cin>>nazwa;
                global_macierz = wczytaj_macierz(nazwa);

                break;
            case 1:
                cout<<"Podaj kryterium stopu w sekundach: "<<endl;
                cin>>czas;

                break;
            case 2:
                cout<<"Podaj wielkość populacji startowej[500]: "<<endl;
                cin>>startowa_wielkosc_populacji;

                break;
            case 3:
                cout<<"Podaj wartość współczynnika mutacji[0.01]: "<<endl;
                cin >> wsp_mutacji;

                break;
            case 4:
                cout<<"Podaj wartość współczynnika krzyżowania[0.8]: "<<endl;
                cin >> wsp_krzyzowania;

                break;
            case 5:
                cout<<"Start alogrytmu"<<endl;
                genetyczny(czas);
                break;
            case 6:
                cout<<"TEST "<<endl;
                //genetyczny(czas);
                break;
            case 7:
                return 0;
        }
    }
}
