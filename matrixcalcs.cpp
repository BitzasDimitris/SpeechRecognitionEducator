#include "matrixcalcs.h"


double MatrixCalcs::Dot(std::vector<double>A,std::vector<double>B){
    if(A.size()!=B.size()){
        return 0;
    }
    double sum=0;
    for(int i=0;i<A.size();i++){
        sum+=A[i]*B[i];
    }
    return sum;
}

std::vector<double> MatrixCalcs::MatrixMult(std::vector<double> A,std::vector<double > B){
    if(A.size()!=B.size()){
        return std::vector<double>{};
    }
    std::vector<double > C(B.size(),0);
    for(int j=0;j<A.size();j++){
        C[j]+=A[j]*B[j];
    }
    return C;
}


std::vector<std::vector<double >> MatrixCalcs::MatrixMultiplication(std::vector<std::vector<double >> A,std::vector<std::vector<double >> B){
    if(A[0].size()!=B.size()){
        return std::vector<std::vector<double >>{};
    }
    std::vector<std::vector<double >> C(A.size(),std::vector<double>(B[0].size(),0));
    for(int i=0;i<B[0].size();i++){
        for(int j=0;j<A.size();i++){
            for(int k=0;k<B.size();k++){
                C[j][i]+=A[j][k]*B[i][k];
            }
        }
    }
    return C;
}


std::vector<double > MatrixCalcs::MatrixMultiplication(std::vector<double> A,std::vector<std::vector<double >> B){
    if(A.size()!=B.size()){
        return std::vector<double>{};
    }
    std::vector<double > C(B[0].size(),0);
    for(int i=0;i<B[0].size();i++){
        for(int j=0;j<A.size();j++){
            C[i]+=A[j]*B[j][i];
        }
    }
    return C;

}



std::vector<double> MatrixCalcs::MatrixMultiplication(int a,std::vector<double> A){
    std::vector<double > C(A.size(),0);
    for(int i=0;i<A.size();i++){
        C[i]+=A[i]*a;
    }
    return C;
}

double MatrixCalcs::MatrixMultiplication(std::vector<double> A,std::vector<double > B){
    if(A.size()!=B.size()){
        return NULL;
    }
    double C=0;
    for(int i=0;i<A.size();i++){
        C+=A[i]*B[i];
    }
    return C;
}

std::vector<double> MatrixCalcs::Matrixpow(std::vector<double> A,int a){
    std::vector<double > C(A.size(),0);
    for(int i=0;i<A.size();i++){
        C[i]+=pow(A[i],a);
    }
    return C;
}

std::vector<std::vector<double >> MatrixCalcs::MatrixTranspose(std::vector<std::vector<double >> Mat){
    std::vector<std::vector<double >> Res(Mat[0].size(),std::vector<double>(Mat.size(),0));
    for(int i=0;i<Mat.size();i++){
        for(int j=0;j<Mat[0].size();j++){
            Res[j][i]=Mat[i][j];
        }
    }
    return Res;
}

std::vector<double> MatrixCalcs::MatrixDiff(std::vector<double > A,std::vector<double > B){
    if(A.size()!=B.size()){
        return std::vector<double>{};
    }
    std::vector<double >Res(A.size());
    for(int i=0;i<A.size();i++){
        Res[i]=A[i]-B[i];
    }
    return Res;
}

std::vector<double> MatrixCalcs::MatrixAdd(std::vector<double > A,std::vector<double > B){
    if(A.size()!=B.size()){
        return std::vector<double>{};
    }
    std::vector<double >Res(A.size());
    for(int i=0;i<A.size();i++){
        Res[i]=A[i]+B[i];
    }
    return Res;
}

double MatrixCalcs::MatrixDist(std::vector<double > A,std::vector<double > B){
    if(A.size()!=B.size()){
        return -1;
    }
    double dist=0;
    for(int i=0;i<A.size();i++){
       dist+=pow(A[i]-B[i],2);
    }
    return sqrt(dist);
}

std::vector<double> MatrixCalcs::MatrixDiv(std::vector<double> vec,double divider){
    for(int i=0;i<vec.size();i++){
        vec[i]/=divider;
    }
    return vec;
}

std::vector<std::vector<double >> MatrixCalcs::Inverse(std::vector<std::vector<double >> Mat){
    std::vector<std::vector<double >>Res;
    double Det = Determinant(Mat, std::vector<int>{});
    for (int i = 0; i<Mat.size(); i++){
        std::vector<double >row(Mat[0].size());
        if (i>0){
            std::copy(Mat[0].begin(), Mat[0].end(), row.begin());
            std::copy(Mat[i].begin(), Mat[i].end(), Mat[0].begin());
            std::copy(row.begin(), row.end(), Mat[i].begin());
        }
        row.clear();
        for (int j = 0; j<Mat[0].size(); j++){
            std::vector<int> blacklist(1, j);
            row.push_back(pow(-1,j)*pow(-1,i)*Determinant(Mat, blacklist) / Det);
        }
        Res.push_back(row);
    }
    Mat.push_back(Mat[0]);
    Mat.erase(Mat.begin());
    return Res;
}


std::vector<double > MatrixCalcs::DiagInverse(std::vector<double > Mat){
     std::vector<double >Res(Mat.size(),0);
     for(int i=0;i<Mat.size();i++){
         Res[i]=1.0/Mat[i];
     }
     return Res;
}

double MatrixCalcs::Determinant(std::vector<std::vector<double >> A,std::vector<int> blacklist){
    if (A.size() - blacklist.size() <= 2){
        int a, b, offset = 0;
        for (int i = 0; i<blacklist.size()+offset; i++){
            if (i != blacklist[i - offset]){
                if (offset){
                    b = i;
                    offset = 2;
                    break;
                }
                else{
                    a = i;
                    offset = 1;
                }
            }
        }
        if (offset == 1){
            b = blacklist.size()+offset;
        }
        else if(offset==0){
            a = blacklist.size();
            b = a + 1;
        }
        return A[A.size() - 2][a] * A[A.size() - 1][b] - A[A.size() - 2][b] * A[A.size() - 1][a];
    }
    else{
        double det = 0;
        int j = 0;
        int offset = 0;
        if (blacklist.size() != 0){
            for (int i = 0; i < A.size(); i++){

                if (i != blacklist[j]){
                    std::vector<int> BL;
                    BL=blacklist;
                    for (int l = 0; l < BL.size(); l++){
                        if (i < BL[l]){
                            BL.insert(BL.begin()+l,i);
                            break;
                        }
                    }
                    if (BL.size() == blacklist.size()){
                        BL.push_back(i);
                    }
                    det += pow(-1, i - j+offset)* A[blacklist.size()][i] * Determinant(A, BL);
                }
                else{
                    j++;
                    if (j >= blacklist.size()){
                        j = blacklist.size() - 1;
                        offset = 1;
                    }
                }
            }
        }
        else{
            for (int i = 0; i < A.size(); i++){
                std::vector<int> BL(1,i);
                det +=pow(-1,i)* A[0][i] * Determinant(A, BL);
            }
        }
        return det;
    }
}


double MatrixCalcs::DiagDeterminant(std::vector<double > A){
    double det=1;
    for(int i=0;i<A.size();i++){
        det*=A[i];
    }
    return det;
}
