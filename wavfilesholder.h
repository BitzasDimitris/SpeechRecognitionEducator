#ifndef WAVFILESHOLDER_H
#define WAVFILESHOLDER_H

#include <QMutex>
#include <QMutexLocker>
#include "WavFile.h"
#include <vector>
#include "mfccholder.h"
#include "phonemeparser.h"


class WavFilesHolder: public QObject
{
    Q_OBJECT
public:
    WavFilesHolder():mtx(),wfiles(0){}
    void initialize(MFCCHolder* mh);
    void InsertEntry(WavFile* wf,PhonemeParser* phn);
    bool DeleteEntry(int id);
    WavFile* GetWfile(int index,int* id);
    void clear();
    int count();
    int staticid=0;

private:
    QMutex mtx;
    std::vector<WavFile*> wfiles;
    std::vector<PhonemeParser*> pfiles;
    std::vector<int> ids;
    MFCCHolder* mfcch;

signals:
    void HasElements();
};

#endif // WAVFILESHOLDER_H
