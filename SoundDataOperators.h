#include <vector>
#define M_PI 3.141592653589793238462643383279502884
#include <cmath>
#include <QDebug>

class SoundDataOperators
{
private:
    static void FFTLoop(std::vector<double>* data);
    static void Swap(std::vector<double>* data,unsigned long ,unsigned long);
public:
    static void FFT(std::vector<double>* frame,std::vector<double>* dftframe);
    static void FCT(std::vector<double>* frame,std::vector<double>* dftframe);
    static void Preemphasis(std::vector<double>* frame, std::vector<double>* pframe,double z);
    static void HammingFilter(std::vector<double>* frame,std::vector<double>* pframe);
    static void CreateMelFilterBank(int num,float low,float high,int samplesnum,std::vector<std::vector<double> >*FilterBank,std::vector<int>* inits,std::vector<double>* mids);
    static void ApplyMelFilterBank(std::vector<std::vector<double> >* FilterBank,std::vector<int>* inits,std::vector<double>* data,std::vector<double>* output);
    static void Loging(std::vector<double>* frame,std::vector<double>* oframe);
    static void CalculateDeltas(int window,std::vector<std::vector<double> >* data,std::vector<std::vector<double> >* output);
    static double FrameEnergy(std::vector<double>* frame);
};

