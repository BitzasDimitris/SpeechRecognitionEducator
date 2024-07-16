#include "phoneme.h"

Phoneme::Phoneme(){

}

Phoneme::Phoneme(QString str,int LI)
{
    StringRepresantation=str;
    LibIndex=LI;
    Found=1;
    FoundFirst=0;
}


bool Phoneme::check(QString str){
    if(!QString::compare(StringRepresantation,str,Qt::CaseSensitive))
        return true;
    return false;
}


 QString Phoneme::toString(){
    return StringRepresantation;
 }

 int Phoneme::LibraryIndex(){
     return LibIndex;
 }

 void Phoneme::FoundIncr(){
     Found++;
 }

 void Phoneme::FirstIncr(){
     FoundFirst++;
 }

 int Phoneme::TimesFound(){
     return Found;
 }

 int Phoneme::TimesFoundFirst(){
     return FoundFirst;
 }
