#include <iostream>
#include <vector>



using namespace std;

vector<int> genetyczny(int);

void ocena_populacji();

void generuj_startowa_populacja();

int oblicz_koszt_drogi(const vector<int>& rozwionzanie, vector<vector<int>> macierz2);