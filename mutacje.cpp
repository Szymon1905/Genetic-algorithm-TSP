#include "mutacje.h"
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

using namespace std;
extern float wsp_mutacji;
extern int metoda_mutacji;
extern vector<Osobnik> populacja;
extern mt19937 gen;

void inwersja(Osobnik &osobnik) {

    // Wybieram losowy fragment drogi
    uniform_int_distribution<int> distribution(0, osobnik.droga.size() - 1);
    int punkt1 = distribution(gen);
    int punkt2 = distribution(gen);

    // Upewnienie się, że punkt1 < punkt2
    if (punkt1 > punkt2) {
        swap(punkt1, punkt2);
    }

    // Odwraca kolejność miast w wybranym fragmencie drogi
    reverse(osobnik.droga.begin() + punkt1, osobnik.droga.begin() + punkt2 + 1);

}

void mutacja_inwersja() {
    auto rozmiar_populacji = populacja.size();
    int numberOfMutations = static_cast<int>(wsp_mutacji * rozmiar_populacji);

    uniform_int_distribution<int> distribution(0, rozmiar_populacji - 1);
    for (int i = 0; i < numberOfMutations; ++i) {
        int wylosowany = distribution(gen);
        inwersja(populacja[wylosowany]);
    }
}

void mutacja_swapowanie() {
    uniform_int_distribution<int> distribution(0, populacja.size() - 1);
    int punkt1 = distribution(gen);
    int punkt2 = distribution(gen);
    swap(populacja[punkt1], populacja[punkt2]);
}

void mutacja() {
    if (metoda_mutacji == 0) {
        mutacja_inwersja();
    }
    if (metoda_mutacji == 1) {
        mutacja_swapowanie();
    }
}