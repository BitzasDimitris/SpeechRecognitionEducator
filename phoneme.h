#ifndef PHONEME_H
#define PHONEME_H
#include <QString>
#include <vector>


class Phoneme
{
public:
    Phoneme();
    Phoneme(QString,int);
    bool check(QString);
    QString toString();
    int LibraryIndex();
    void FirstIncr();
    void FoundIncr();
    int TimesFound();
    int TimesFoundFirst();
private:
    QString StringRepresantation;
    std::vector<QString> PossibleLetters;
    int LibIndex;
    int Found;
    int FoundFirst;
};

#endif // PHONEME_H
