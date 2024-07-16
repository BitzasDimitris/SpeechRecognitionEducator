#include "wavfilesholder.h"



void WavFilesHolder::initialize(MFCCHolder* mh){
    mfcch=mh;
}

void WavFilesHolder::InsertEntry(WavFile* wf,PhonemeParser* phn){
    QMutexLocker ml(&mtx);
    wfiles.push_back(wf);
    pfiles.push_back(phn);
    ids.push_back(staticid);
    wf->SetId(staticid);
    phn->SetId(staticid);
    staticid++;
    emit HasElements();
    ml.unlock();
}


bool WavFilesHolder::DeleteEntry(int id){
    std::vector<int>::iterator it;
    QMutexLocker ml(&mtx);
    it=std::find(ids.begin(),ids.end(),id);
    if(it!=ids.end()){
        int index=distance(ids.begin(),it);
        std::vector<std::vector<double>> MFCCFrames;
        std::vector<int* > PHN;
        wfiles[index]->GetMFCC(&MFCCFrames);
        pfiles[index]->GetPhonemes(&PHN);
        mfcch->AddItem(ids[index],MFCCFrames,PHN);
        wfiles[index]->freememory();
        wfiles.erase(wfiles.begin()+index);
        pfiles.erase(pfiles.begin()+index);
        ids.erase(ids.begin()+index);
        ids.shrink_to_fit();
        wfiles.shrink_to_fit();
        pfiles.shrink_to_fit();
        return true;
    }
    else{
        return false;
    }
    ml.unlock();
}


WavFile* WavFilesHolder::GetWfile(int index,int* id){
    WavFile* wf;
    QMutexLocker ml(&mtx);
    wf= wfiles[index];
    if(id!=NULL)
    *id=ids[index];
    ml.unlock();
    return wf;
}

int WavFilesHolder::count(){
    QMutexLocker ml(&mtx);
    int temp=wfiles.size();
    ml.unlock();
    return temp;
}
void WavFilesHolder::clear(){
    wfiles.clear();
    pfiles.clear();
    ids.clear();
    wfiles.shrink_to_fit();
    pfiles.shrink_to_fit();
    ids.shrink_to_fit();
}
