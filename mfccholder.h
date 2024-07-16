#ifndef MFCCHOLDER_H
#define MFCCHOLDER_H

#include <QString>
#include <vector>
#include "phoneme.h"
#include <time.h>
#include <stdlib.h>
#include "WavFile.h"


class MFCCHolder
{
public:
    MFCCHolder();
    void AddItem(int id,std::vector<std::vector<double>> MFCC,std::vector<int* > PHN);
    void GetItem(int index,std::vector<std::vector<double>> *MFCC,std::vector<int* > *PHN);
    int GetRandomItem(std::vector<std::vector<double>> *MFCC,std::vector<int* > *PHN);
    int GetSizeofObservations();
    int GetSizeOfMFCCs();
    void clear();
private:
    std::vector<int> ids;
    std::vector<std::vector<std::vector<double>>> MFCCs;
    std::vector<std::vector<int* > > PHNs;
};

#endif // MFCCHOLDER_H
