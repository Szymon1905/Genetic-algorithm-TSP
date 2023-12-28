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

class Osobnik {
public:
    vector<int> trasa;
    int dlugosc_trasy;

    Osobnik(vector<int> trasa, int dlugosc_trasy){
        this->trasa = trasa;
        this->dlugosc_trasy = dlugosc_trasy;
    }
};

vector<Osobnik> populacja;

Osobnik najlepszy_osobnik = Osobnik(vector<int>(),INFINITY);
int najlepsza_dlugosc = INFINITY;




void wypisz_najlepsze(){
    cout<<endl;
    cout<<endl;
    cout << "Finalna Droga: ";
    for (int elem: najlepsze_rozwionzanie) {
        cout << elem << " ";
    }
    cout << endl;
    cout << "Finalny Koszt: " << najlepsza_dlugosc;
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

    // inicjajca generatora liczb losowych
    random_device rd;
    mt19937 gen(rd());

    // zerowanie populacji przy restarcie
    populacja.clear();

    // generowanie metodą shuffle losowych permutacji miast oraz wkładanie ich do populacji
    for (int i = 0; i < startowa_wielkosc_populacji; i++) {
        shuffle(pula_miast.begin(), pula_miast.end(), gen);
        populacja.push_back(pula_miast);
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
    int droga;
    for (const vector<int>& elem : populacja){

        // obliczenie kosztu drogi
        droga = oblicz_koszt_drogi(elem, global_macierz);

        // ocena rozwiązania
        if ( droga < najlepsza_dlugosc ){

            // aktualizacja najlepszego rozwiązania dla przeżywalności
            najlepsza_dlugosc = droga;
            najlepsze_rozwionzanie = elem;
        }
    }
}

void ocena_populacji(){
    int droga;
    for (const vector<int>& elem : populacja){

        // obliczenie kosztu drogi
        droga = oblicz_koszt_drogi(elem, global_macierz);

        // ocena rozwiązania
        if ( droga < najlepsza_dlugosc ){

            // aktualizacja najlepszego rozwiązania dla przeżywalności
            najlepsza_dlugosc = droga;
            najlepsze_rozwionzanie = elem;
        }
    }
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
    }

    // wypisanie najlepszego
    wypisz_najlepsze();

    // czeka aż wątek się skończy
    thread_warunek_stopu.join();
    return a;
}