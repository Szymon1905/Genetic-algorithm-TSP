#include <iostream>
#include <vector>


using namespace std;

class Osobnik {
public:
    vector<int> droga;
    int dlugosc_drogi;

    Osobnik(vector<int> trasa, int dlugosc_trasy);

    Osobnik();


    void reset();
};