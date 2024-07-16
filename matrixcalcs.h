#ifndef MATRIXCALCS_H
#define MATRIXCALCS_H
#include <vector>
#include <math.h>


class MatrixCalcs
{
public:
    static double Dot(std::vector<double>A,std::vector<double>B);
    static std::vector<std::vector<double >> MatrixMultiplication(std::vector<std::vector<double >> A,std::vector<std::vector<double >> B);
    static std::vector<double > MatrixMultiplication(std::vector<double> A,std::vector<std::vector<double >> B);
    static std::vector<double> MatrixMultiplication(int a,std::vector<double> A);
    static double MatrixMultiplication(std::vector<double> A,std::vector<double > B);
    static std::vector<double> MatrixMult(std::vector<double> A,std::vector<double > B);

    static std::vector<double> Matrixpow(std::vector<double> A,int a);
    static std::vector<std::vector<double >> MatrixTranspose(std::vector<std::vector<double >> Mat);
    static std::vector<double> MatrixDiff(std::vector<double > A,std::vector<double > B);
    static std::vector<double> MatrixAdd(std::vector<double > A,std::vector<double > B);
    static double MatrixDist(std::vector<double > A,std::vector<double > B);
    static std::vector<double> MatrixDiv(std::vector<double>,double divider);


    static double Determinant(std::vector<std::vector<double >> A,std::vector<int> blacklist);
    static double DiagDeterminant(std::vector<double > A);
    static std::vector<std::vector<double >> Inverse(std::vector<std::vector<double >> Mat);
    static std::vector<double > DiagInverse(std::vector<double > Mat);
};

#endif // MATRIXCALCS_H
