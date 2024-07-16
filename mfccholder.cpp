#include "mfccholder.h"

MFCCHolder::MFCCHolder()
{
    srand (time(NULL));
}


void MFCCHolder::AddItem(int id,std::vector<std::vector<double>> MFCC,std::vector<int* > PHN){
    MFCCs.push_back(MFCC);
    PHNs.push_back(PHN);
    ids.push_back(id);
}

void MFCCHolder::GetItem(int index,std::vector<std::vector<double>> *MFCC,std::vector<int* > *PHN){
    *MFCC=MFCCs[index];
    *PHN=PHNs[index];
}

int MFCCHolder::GetRandomItem(std::vector<std::vector<double>> *MFCC,std::vector<int* > *PHN){
    int randomint=rand()%MFCCs.size();
    *MFCC=MFCCs[randomint];
    *PHN=PHNs[randomint];
    return randomint;
}


int MFCCHolder::GetSizeofObservations(){
    return MFCCs[0][0].size();
}

int MFCCHolder::GetSizeOfMFCCs(){
    return MFCCs.size();
}

void MFCCHolder::clear(){
    MFCCs.clear();
    PHNs.clear();
    ids.clear();
    MFCCs.shrink_to_fit();
    PHNs.shrink_to_fit();
    ids.shrink_to_fit();
}
