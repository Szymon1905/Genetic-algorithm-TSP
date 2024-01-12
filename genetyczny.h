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

void genetyczny(int);

void ocena_populacji();

void generuj_startowa_populacja();

int oblicz_koszt_drogi(const vector<int> &rozwionzanie, vector<vector<int>> macierz2);



