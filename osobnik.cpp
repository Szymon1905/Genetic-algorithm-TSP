#include "osobnik.h"
#include <vector>
#include <random>

using namespace std;


Osobnik::Osobnik(vector<int> trasa, int dlugosc_trasy) {
    this->droga = trasa;
    this->dlugosc_drogi = dlugosc_trasy;
}

Osobnik::Osobnik() {}

void Osobnik::reset() {
    this->droga.clear();
    this->dlugosc_drogi = INFINITY;
}
