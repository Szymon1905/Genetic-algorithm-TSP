#include "genetyczny.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <limits>
#include <fstream>
#include <chrono>

#include <thread>
#include <random>

extern int global_liczba_miast;
extern int startowa_wielkosc_populacji;
extern vector<vector<int>> global_macierz;


// inicjajca generatora liczb losowych
random_device rd;
mt19937 gen(rd());

class Osobnik {
public:
    vector<int> droga;
    int dlugosc_drogi;

    Osobnik(vector<int> trasa, int dlugosc_trasy){
        this->droga = trasa;
        this->dlugosc_drogi = dlugosc_trasy;
    }
};

vector<Osobnik> populacja;

Osobnik najlepszy_osobnik = Osobnik(vector<int>(),INFINITY);
int najlepsza_dlugosc = INFINITY;




void wypisz_najlepsze(){
    cout<<endl;
    cout<<endl;
    cout << "Finalna Droga: ";
    for (int elem: najlepszy_osobnik.droga) {
        cout << elem << " ";
    }
    cout << endl;
    cout << "Finalny Koszt: " << najlepszy_osobnik.dlugosc_drogi;
    cout<<endl;
    cout<<endl;
}

void odliczanie(int sekundy) {
    auto start = chrono::high_resolution_clock::now();
    auto koniec = start + chrono::seconds(sekundy);

    while (chrono::high_resolution_clock::now() < koniec) {
        cout << "\rPozostały czas: " << chrono::duration_cast<chrono::seconds>(koniec - chrono::high_resolution_clock::now()).count() << " sekund";
        //cout.clear();
        this_thread::sleep_for(chrono::milliseconds(10));
    }

    cout << "\rPozostały czas: 0 sekund  " << endl;
    cout << "Koniec czasu" << endl;
}

void generuj_startowa_populacja() {

    // generowanie puli miast do losowania
    vector<int> pula_miast;
    for (int i = 0; i < global_liczba_miast; i++) {
        pula_miast.push_back(i);
    }

    // zerowanie populacji przy restarcie
    populacja.clear();

    // generowanie metodą shuffle losowych permutacji miast oraz wkładanie ich do populacji
    for (int i = 0; i < startowa_wielkosc_populacji; i++) {
        shuffle(pula_miast.begin(), pula_miast.end(), gen);
        Osobnik osobnik = Osobnik(pula_miast, oblicz_koszt_drogi(pula_miast,global_macierz));
        populacja.push_back(osobnik);
    }
}

int oblicz_koszt_drogi(const vector<int>& rozwionzanie, vector<vector<int>> macierz2) {
    int suma = 0;
    for (int i = 0; i < global_liczba_miast - 1; ++i) {
        suma += macierz2[rozwionzanie[i]][rozwionzanie[i + 1]];
    }
    return suma;
}

void ocena_populacji(){
    int dlugosc_drogi;
    for (const Osobnik& elem : populacja){

        // obliczenie kosztu drogi
        dlugosc_drogi = oblicz_koszt_drogi(elem.droga, global_macierz);

        // ocena rozwiązania
        if ( dlugosc_drogi < najlepszy_osobnik.dlugosc_drogi ){

            // aktualizacja najlepszego osobnika / rozwiązania dla przeżywalności
            najlepszy_osobnik = elem;
        }
    }
}

vector<Osobnik> wybranie_rodzicow(){

    // Obliczenie sumy wartości funkcji celu (czyli drogi) dla wszystkich osobników
    int suma_drog = 0;
    for (Osobnik& osobnik : populacja) {
        suma_drog = suma_drog + osobnik.dlugosc_drogi;
    }

    // Inicjalizacja pustej listy wybranych rodziców
    vector<Osobnik> wybrani;


    uniform_int_distribution<> distribution(0, suma_drog);


    //sprawdzenie czy populacja nie jest pusta
    if (populacja.empty()){
        return wybrani;
    }


    // todo czy wybranie osobnika kilka razy jest poprawne ?
    // Wybieranie rodziców
    for (int i = 0; i < int(populacja.size()/5); i++) {
        int suma = 0;
        int los = distribution(gen);

        for (Osobnik& osobnik : populacja) {
            suma += osobnik.dlugosc_drogi;
            if (suma >= los){
                wybrani.push_back(osobnik);
                break;
            }
        }
    }

    return wybrani;
}

vector<int> genetyczny(int czas){
    vector<int> a;

    // generacja populacji startowej
    generuj_startowa_populacja();


    // obliczanie czasu dla wątku który odpowiada za warunek stopu
    auto start = chrono::high_resolution_clock ::now();
    auto stop = start + chrono::seconds (czas);

    // start wątku odliczajacego czas do zatrzymania alogrytmu ( warunek stopu )
    thread thread_warunek_stopu(odliczanie,czas);

    // ocena populacji startowej
    ocena_populacji();

    while(chrono::high_resolution_clock::now() < stop){

        ocena_populacji();

        populacja = wybranie_rodzicow();
    }

    // wypisanie najlepszego
    wypisz_najlepsze();

    // czeka aż wątek się skończy
    thread_warunek_stopu.join();
    return a;
}