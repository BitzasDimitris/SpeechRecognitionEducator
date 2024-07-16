#include "phonemesLibrary.h"

PhonemesLibrary::PhonemesLibrary()
{

}


 int PhonemesLibrary::AddEntry(QString PHN){
    for(int i=0;i<Phonemes.size();i++){
        if(Phonemes[i]->check(PHN)){
            Phonemes[i]->FoundIncr();
            return i;
        }
    }
    Phonemes.push_back(new Phoneme(PHN,Phonemes.size()));
    AddRowCol();
    return Phonemes.size()-1;
 }

 void PhonemesLibrary::FoundFirst(int index){
     Phonemes[index]->FirstIncr();
 }

 void PhonemesLibrary::AddTransition(int p1,int p2){
     if(p1<Transitions.size()){
         if(p2<Transitions[p1].size()){
             Transitions[p1][p2]+=3;
         }
     }
 }

 void PhonemesLibrary::AddTransition(int p,float dur){
     Transitions[p][p]+=dur;
 }


 void PhonemesLibrary::AddRowCol(){
     //add col
    for(int i=0;i<Transitions.size();i++){
        Transitions[i].push_back(0);
    }
    // add row
    Transitions.push_back(std::vector<int>(Phonemes.size()));
 }


 int PhonemesLibrary::PhonemesCount(){
    return Phonemes.size();
 }

 Phoneme* PhonemesLibrary::GetPhoneme(int index){
    return Phonemes[index];
 }


int PhonemesLibrary::GetTransition(int i,int j){
    return Transitions[i][j];
}

void PhonemesLibrary::clear(){
    Phonemes.clear();
    Transitions.clear();
    Phonemes.shrink_to_fit();
    Transitions.shrink_to_fit();

}
