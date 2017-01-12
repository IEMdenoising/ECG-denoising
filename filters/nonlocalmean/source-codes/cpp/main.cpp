#include <iostream>
#include <vector>
#include <fstream>
#include <math.h>
#include <chrono>

//using namespace std;

int main() {

    std::vector<int> Delta; //działa
    int iStart;
    int N; //signal length
    int iEnd;
    int P = 500;
    float h;
    float PatchHW = 10;
    float lambda = 0.6 * 0.2;
    float eps = 2.2204e-16;
    std::vector<float> signal;

    struct IncGenerator {
        int current_;
        IncGenerator (int start) : current_(start) {}
        int operator() () { return current_++; }
    };


    //Wczytanie sygnału
    std::ifstream inputFile;
    inputFile.open("/Users/martyn/Development/cpp/nowe/109.txt");

    if (inputFile) {
        float value;

        while (inputFile >> value) {
            signal.push_back(value);
        }
        inputFile.close();
    }



    //Długość sygnału
    N = signal.size()-1;


    std::vector<float> filteredSignal(N, 0);

    //Stworzenie obszaru poszukiwań
    for (int m = -P; m <= P; m++) {
        Delta.push_back(m);
    }


    //Przygotowanie brzegów sygnału
    iStart = 1;
    iEnd = N;


    //Inicjalizacja wag normalizacji
    std::vector<float> Z(N, 0);

    //Inicjalizacja ka
    std::vector<int>ka(N);

    for (int l = 1; l <= N; ++l) {
        ka.push_back(l);
    }


    //Zamiana lambdy na parametr h
    int nPatch;
    nPatch = 2 * PatchHW + 1;
    h = 2 * nPatch * lambda * lambda;

    //Rozpoczęcie mierzenia czasu
    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

    //Właściwa część
    for (int idx = -P; idx <=P; idx++) {

        std::vector<int> igood;




        std::vector<float> SSD(ka.size(), 0);
        std::vector<int> kplus;

        //Inicjalizacja kplus
        for (int i = 0; i < ka.size(); ++i) {
            kplus.push_back(ka.at(i) + idx);
        }

        //Inicjalizacja igood
        for (int i = 0; i < kplus.size(); ++i) {
            if (kplus.at(i) > 0 && kplus.at(i) <= N) {
                igood.push_back(i);
            }
        }

        //Obliczenie SSD działa
        for (int k = 0; k < igood.size()-1; ++k) {
            SSD.at(igood.at(k)) = (signal.at(ka.at(igood.at(k))) - signal.at(kplus.at(igood.at(k)))) * (signal.at(ka.at(igood.at(k))) - signal.at(kplus.at(igood.at(k))));
        }

        //Obliczenie cumsum(SSD) działa
        std::vector<float>Sdx;
        Sdx.push_back(SSD.at(0));
        for (int i = 1; i < SSD.size(); ++i) {
            Sdx.push_back(SSD.at(i) + Sdx.at(i-1));
        }

        //Jakieś zabawy jeszcze
        for (int ii = iStart; ii <= iEnd; ++ii) {
            float distance;
            if ((ii - PatchHW - 1) > 0 && (ii + PatchHW) < N) {
                distance = Sdx.at(ii + PatchHW) - Sdx.at(ii- PatchHW - 1);
                float w = exp(-distance/h);
                float t = ii + idx;

                if (t > 1 && t <= N) {
                    filteredSignal.at(ii) = (filteredSignal.at(ii) + w * signal.at(t));
                    Z.at(ii) = Z.at(ii) + w;
                }
            }
        }
    }
    std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();


    //Normalizacja
    for (int i1 = 0; i1 < filteredSignal.size(); ++i1) {
        filteredSignal.at(i1) = filteredSignal.at(i1) / (Z.at(i1) + eps);
    }

    std::ofstream outputFile("/Users/martyn/Development/cpp/nowe/nnn.txt");
    for (int n = 0; n < filteredSignal.size(); ++n) {
        outputFile << filteredSignal.at(n);
        outputFile << "\n";
    }

    float duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
    duration = duration / 1000000;
    std::cout << duration << "s";


    return 0;
}