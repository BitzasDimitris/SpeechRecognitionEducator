#include "phonemeparser.h"


PhonemeParser::PhonemeParser(PhonemesLibrary* pl,int s)
{
    phnlib=pl;
    SpF=s;
}


bool PhonemeParser::StartProcess(QString fn,QString wrd){
    filename=fn.toStdString();
    wrdfilename=wrd.toStdString();
    std::ifstream file(filename.c_str());

    std::string str;
    int start,end;
    if(file.is_open()){
        while(file>>start>>end>>str){
            int index;
            if(QString::fromStdString(str)!="breath"){
                index=phnlib->AddEntry(QString::fromStdString(str));
            }
            else{
                index=phnlib->AddEntry("sil");
            }
            Phonemes.push_back(new int[3]{index,start,end});
            if(Phonemes.size()>1){
                phnlib->AddTransition(Phonemes[Phonemes.size()-2][0],Phonemes[Phonemes.size()-1][0]);
            }
        }
    }
    else{
        return false;
    }
    if(file.eof()&&wrd.compare("")!=0){
        file.close();
        std::ifstream wrdfile(wrdfilename.c_str());
        int it=0;
        if(wrdfile.is_open()){
            while(wrdfile>>start>>end>>str){
                while(it<Phonemes.size()){
                    if(start==Phonemes[it][1]){
                        phnlib->FoundFirst(Phonemes[it][0]);
                    }
                    it++;
                }
            }
        }
        return true;
    }
    else{
        file.close();
        if(wrd.compare("")==0){
            return true;
        }
        return false;
    }
}

void PhonemeParser::GetPhonemes(std::vector<int * > *Phns){
    *Phns=Phonemes;
}

int PhonemeParser::GetId(){
    return id;
}

void PhonemeParser::SetId(int a){
    id=a;
}
