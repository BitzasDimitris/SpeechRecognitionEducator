#ifndef PHONEMEPARSER_H
#define PHONEMEPARSER_H
#include <iostream>
#include <fstream>
#include <string.h>
#include <QString>
#include <vector>
#include "phoneme.h"
#include "phonemeslibrary.h"

class PhonemeParser
{
public:
    PhonemeParser(PhonemesLibrary*,int);
    bool StartProcess(QString fn,QString wrd);
    void GetPhonemes(std::vector<int *> *Phns);
    int GetId();
    void SetId(int);
private:
    int id;
    std::string filename,wrdfilename;
    std::vector<int *> Phonemes;
    int SpF;
    PhonemesLibrary* phnlib;

};

#endif // PHONEMEPARSER_H
