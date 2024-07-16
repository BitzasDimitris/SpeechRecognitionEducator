#ifndef MOG_H
#define MOG_H
#define M_PI 3.141592653589793238462643383279502884
#include <vector>
#include <array>
#include <math.h>
#include "matrixcalcs.h"

typedef MatrixCalcs M;


class MoG
{
public:
    int numofG;
    int lastmerged;
    MoG();
    MoG(int s,double percent);
    double evaluate(std::vector<double> vec,double avgenergy);
    void Gmeans(std::vector<std::vector<double>> vals);
    int GetData(std::vector<std::vector<double>> * m,std::vector<std::vector<double>>*s,std::vector<double> * dets,std::vector<double> * priors);
private:
    bool CheckNewCenters(std::array<std::vector<double>,2>centers,std::vector<std::vector<double>>vals);
    std::array<std::vector<double>,2> Kmeans(std::vector<std::vector<double>> vals,std::vector<double> c1,std::vector<double> c2,std::vector<int> *Assignments);
    double probks(double alam);
    float quality=1;
    std::vector<std::vector<double >> means;
    std::vector<std::vector<double >> sigmas;
    std::vector<int> Density;
    std::vector<int> ofDensity;
    std::vector<double> Dets;
    std::vector<double> priors;
    int size;
    double critical;


};

#endif // MOG_H
