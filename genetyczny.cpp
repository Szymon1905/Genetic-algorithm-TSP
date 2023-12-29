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
extern float wsp_krzyzowania;
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

    Osobnik(){}

    void dodaj_miasto_koncowe(){
        int miasto_startowe = this->droga.front();
        this->droga.push_back(miasto_startowe);
        this->dlugosc_drogi = oblicz_koszt_drogi(this->droga,global_macierz);
    }

    void aktualizuj_miasto_koncowe(){
        int miasto_startowe = this->droga.front();
        this->droga.back() = miasto_startowe;
        this->dlugosc_drogi = oblicz_koszt_drogi(this->droga,global_macierz);
    }
};

vector<Osobnik> populacja;

Osobnik najlepszy_osobnik = Osobnik(vector<int>(),INFINITY);





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
        //cout << "\rPozostały czas: " << chrono::duration_cast<chrono::seconds>(koniec - chrono::high_resolution_clock::now()).count() << " sekund";
        //cout.clear();
        //this_thread::sleep_for(chrono::milliseconds(10));
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

    // dodaje brakujące miasto końcowe do każdego osobnika
    for (Osobnik& osobnik: populacja) {
        osobnik.dodaj_miasto_koncowe();
    }
}

int oblicz_koszt_drogi(const vector<int>& rozwionzanie, vector<vector<int>> macierz2) {
    int suma = 0;
    for (int i = 0; i < global_liczba_miast-1; ++i) {
        suma += macierz2[rozwionzanie[i]][rozwionzanie[i + 1]];
    }
    return suma;
}

void ocena_populacji(){
    int dlugosc_drogi;
    for (Osobnik& elem : populacja){

        elem.aktualizuj_miasto_koncowe();

        // obliczenie kosztu drogi
        dlugosc_drogi = oblicz_koszt_drogi(elem.droga, global_macierz);

        // ocena rozwiązania
        if ( dlugosc_drogi < najlepszy_osobnik.dlugosc_drogi ){

            // aktualizacja najlepszego osobnika / rozwiązania dla przeżywalności
            najlepszy_osobnik = elem;
            cout<<"Nowy najlepszy: "<<elem.dlugosc_drogi<<endl;
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
    for (int i = 0; i < int(populacja.size()); i++) {
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

Osobnik krzyzowanie_OX(Osobnik rodzic1, Osobnik rodzic2){
    Osobnik potomek;
    int rozmiar_drogi = int(populacja[0].droga.size());

    std::uniform_int_distribution<> distribution(0, rozmiar_drogi - 1);
    int punkt1 = distribution(gen);
    int punkt2 = distribution(gen);

    // Upewnienie się, że punkt1 < punkt2
    if (punkt1 > punkt2) {
        swap(punkt1, punkt2);
    }

    // wypełninie potomka -1 (puste pole)
    for (int i = 0; i < rozmiar_drogi; i++) {
        potomek.droga.push_back(-1);
    }

    // wstawienie miast do potomka od rodzica pomiędzy punktami cięcia
    for (int i = punkt1; i <= punkt2; i++) {
        potomek.droga[i] = rodzic1.droga[i];
    }


    // Wypełnienie potomka miastami z rodzica 2 w kolejności
    for (int i = 0; i < rozmiar_drogi; i++) {
        if(potomek.droga[i] == -1){
            potomek.droga[i] = rodzic2.droga[i];
        }
    }


    return potomek;
}

// todo krzyzowanie psuje droge
void krzyzowanie(){
    Osobnik potomek;

    for (Osobnik& osobnik: populacja) {
        osobnik.aktualizuj_miasto_koncowe();
    }

    uniform_int_distribution<> distribution(0, int(populacja.size()) - 1);
    uniform_real_distribution<float> szansa_krzyzowania(0.0f, 1.0f);

    for (Osobnik& osobnik: populacja){
        float szansa = szansa_krzyzowania(gen);
        if(szansa < wsp_krzyzowania){
            int rodzic1 = distribution(gen);
            int rodzic2 = distribution(gen);
            osobnik = krzyzowanie_OX(populacja[rodzic1], populacja[rodzic2]);
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
    ocena_populacji();                // etap 2 tylko raz na początku

    while(chrono::high_resolution_clock::now() < stop){  // etap 3

        ocena_populacji();       // etap 2 każdy kolejny

        populacja = wybranie_rodzicow();     // etap 4

        //krzyzowanie();  // etap 5
    }

    // wypisanie najlepszego
    wypisz_najlepsze();

    // czeka aż wątek się skończy
    thread_warunek_stopu.join();
    return a;
}