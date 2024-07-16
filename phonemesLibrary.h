#ifndef PHONEMESLIBRARY_H
#define PHONEMESLIBRARY_H
#include <vector>
#include "phoneme.h"

class PhonemesLibrary
{
public:
    PhonemesLibrary();
    int AddEntry(QString PHN);
    void AddTransition(int p1,int p2);
    void AddTransition(int p1,float dur);
    void FoundFirst(int index);
    int PhonemesCount();
    Phoneme* GetPhoneme(int index);
    int GetTransition(int i,int j);
    void clear();
private:
    std::vector<Phoneme* > Phonemes;
    std::vector<std::vector<int> > Transitions;

    void AddRowCol();
};

#endif // PHONEMES_H
