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
#include <unordered_set>

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

    Osobnik(vector<int> trasa, int dlugosc_trasy) {
        this->droga = trasa;
        this->dlugosc_drogi = dlugosc_trasy;
    }

    Osobnik() {}


    void reset() {
        this->droga.clear();
        this->dlugosc_drogi = INFINITY;
    }
};

vector<Osobnik> populacja;

Osobnik najlepszy_osobnik = Osobnik(vector<int>(), INFINITY);


void wypisz_najlepsze() {
    cout << endl;
    cout << endl;
    cout << "Finalna Droga: ";
    for (int elem: najlepszy_osobnik.droga) {
        cout << elem << " ";
    }
    cout << najlepszy_osobnik.droga.front() << endl;
    cout << endl;
    cout << "Finalny Koszt: " << najlepszy_osobnik.dlugosc_drogi;
    cout << endl;
    cout << endl;
}

void odliczanie(int sekundy) {
    auto start = chrono::high_resolution_clock::now();
    auto koniec = start + chrono::seconds(sekundy);


    while (chrono::high_resolution_clock::now() < koniec) {
        this_thread::sleep_for(chrono::milliseconds(10));
    }
        /*
           cout << "\rPozostały czas: "
                << chrono::duration_cast<chrono::seconds>(koniec - chrono::high_resolution_clock::now()).count()
                << " sekund";
           cout.clear();



       cout << "\rPozostały czas: 0 sekund  " << endl;

       }
        */
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
        Osobnik osobnik = Osobnik(pula_miast, INFINITY);
        populacja.push_back(osobnik);
    }

    for (Osobnik &osobnik: populacja) {
        osobnik.dlugosc_drogi = oblicz_koszt_drogi(pula_miast, global_macierz);
    }

}

int oblicz_koszt_drogi(const vector<int> &rozwionzanie, vector<vector<int>> macierz2) {
    int suma = 0;
    for (int i = 0; i < rozwionzanie.size() - 1; i++) {
        suma += macierz2[rozwionzanie[i]][rozwionzanie[i + 1]];
    }

    // plus powrot do miasta poczatkowego
    suma += macierz2[rozwionzanie.back()][rozwionzanie.front()];
    return suma;
}

void ocena_populacji() {

    for (Osobnik &elem: populacja) {


        // obliczenie kosztu drogi
        elem.dlugosc_drogi = oblicz_koszt_drogi(elem.droga, global_macierz);

        // ocena rozwiązania
        if (elem.dlugosc_drogi < najlepszy_osobnik.dlugosc_drogi) {

            // aktualizacja najlepszego osobnika / rozwiązania dla przeżywalności
            najlepszy_osobnik = elem;
            for (int a: najlepszy_osobnik.droga) {
                cout << a << " ";
            }
            cout << najlepszy_osobnik.droga[0] << endl;

            cout << endl;
            cout << "Nowy najlepszy: " << elem.dlugosc_drogi << endl;
        }
    }
}

vector<Osobnik> wybranie_rodzicow() {

    // Obliczenie sumy wartości funkcji celu (czyli drogi) dla wszystkich osobników
    int suma_drog = 0;
    for (Osobnik &osobnik: populacja) {
        suma_drog = suma_drog + osobnik.dlugosc_drogi;
    }

    // Inicjalizacja pustej listy wybranych rodziców
    vector<Osobnik> wybrani;


    uniform_int_distribution<> distribution(0, suma_drog);


    //sprawdzenie czy populacja nie jest pusta
    if (populacja.empty()) {
        return wybrani;
    }


    // todo czy wybranie osobnika kilka razy jest poprawne ?
    //todo wybranie populacji pobiera wszystkich a nie najlpeszych, poprawka potrzebna

    // Wybieranie rodziców
    for (int i = 0; i < int(populacja.size()); i++) {
        int suma = 0;
        int los = distribution(gen);

        for (Osobnik &osobnik: populacja) {
            suma += osobnik.dlugosc_drogi;
            if (suma >= los) {
                wybrani.push_back(osobnik);
                break;
            }
        }
    }

    return wybrani;
}



bool czy_zawiera(vector<int> wektor, int liczba) {
    for (int pole: wektor) {
        if (pole == liczba) {
            return true;
        }
    }
    return false;
}

Osobnik krzyzowanie_OX(Osobnik rodzic1, Osobnik rodzic2) {
    Osobnik potomek;
    int rozmiar_drogi = int(populacja[0].droga.size());

    std::uniform_int_distribution<> distribution(0, rozmiar_drogi - 2);
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

    // wstawienie miast do potomka od rodzica pomiędzy punktami cięcia,  takie zielone na slajdzie 17
    for (int i = punkt1; i <= punkt2; i++) {
        potomek.droga[i] = rodzic1.droga[i];
    }

    //wybranie do osobnego vectora miast ktore mogę wziąść z rodzica2 (te miasta co nie zostały pobrane z rodzica 1) z prawej storny
    vector<int> dostepne_miasta;
    for (int i = punkt2 + 1; i < rozmiar_drogi; i++) {
        if (not czy_zawiera(potomek.droga, rodzic2.droga[i])) {
            if (not czy_zawiera(dostepne_miasta, rodzic2.droga[i])) {

                // jeśli miasta nie ma w cześci zielonej (slajd 17), dodajemy miasto do dostepnych miast
                dostepne_miasta.push_back(rodzic2.droga[i]);
            }
        }
    }

    //wybranie do osobnego vectora miast ktore mogę wziąść z rodzica2 z lewej storny
    for (int i = 0; i <= punkt2; i++) {
        if (not czy_zawiera(potomek.droga, rodzic2.droga[i])) {
            if (not czy_zawiera(dostepne_miasta, rodzic2.droga[i])) {

                // jeśli miasta nie ma w cześci zielonej (slajd 17), dodajemy miasto do dostepnych miast
                dostepne_miasta.push_back(rodzic2.droga[i]);
            }
        }
    }


    // Wypełnienie potomka miastami z rodzica 2 częsci prawej
    for (int i = punkt2 + 1; i < rozmiar_drogi; i++) {
        if (potomek.droga[i] == -1) {

            // jeśli pole jest puste (-1) oraz miasto się nie powtarza, wpsiujemy miasto
            potomek.droga[i] = dostepne_miasta.front();
            dostepne_miasta.erase(dostepne_miasta.begin());
        }
    }

    // Wypełnienie potomka miastami z rodzica 2 częsci lewej
    for (int i = 0; i < punkt1; i++) {


        if (potomek.droga[i] == -1) {

            // jeśli pole jest puste (-1) oraz miasto się nie powtarza, wpsiujemy miasto
            potomek.droga[i] = dostepne_miasta.front();
            dostepne_miasta.erase(dostepne_miasta.begin());
        }
    }

    return potomek;
}


void krzyzowanie() {
    Osobnik potomek;

    uniform_int_distribution<> distribution(0, int(populacja.size()) - 1);
    uniform_real_distribution<float> szansa_krzyzowania(0.0f, 1.0f);

    for (Osobnik &osobnik: populacja) {
        float szansa = szansa_krzyzowania(gen);
        if (szansa < wsp_krzyzowania) {
            int rodzic1 = distribution(gen);
            int rodzic2 = distribution(gen);
            osobnik = krzyzowanie_OX(populacja[rodzic1], populacja[rodzic2]);
        }
    }
}

void genetyczny(int czas) {
    vector<int> a;

    // generacja populacji startowej
    generuj_startowa_populacja();


    //reset najlepszego osobnika
    najlepszy_osobnik.reset();

    // obliczanie czasu dla wątku który odpowiada za warunek stopu
    auto start = chrono::high_resolution_clock::now();
    auto stop = start + chrono::seconds(czas);

    // start wątku odliczajacego czas do zatrzymania alogrytmu ( warunek stopu )
    thread thread_warunek_stopu(odliczanie, czas);

    // ocena populacji startowej
    ocena_populacji();                // etap 2 tylko raz na początku

    while (chrono::high_resolution_clock::now() < stop) {  // etap 3

        ocena_populacji();       // etap 2 każdy kolejny

        populacja = wybranie_rodzicow();     // etap 4

        krzyzowanie();  // etap 5


    }

    // wypisanie najlepszego
    wypisz_najlepsze();

    // czeka aż wątek się skończy
    // thread_warunek_stopu.join();
}