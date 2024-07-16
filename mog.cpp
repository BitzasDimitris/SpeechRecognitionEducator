#include "mog.h"
MoG::MoG(){

}

MoG::MoG(int s,double percent)
{
    size=s;
    numofG=0;
    critical= 1.94947;
    critical*=1+2*(1-percent);
    quality=percent;
}


double MoG::evaluate(std::vector<double> vec,double avgenergy){
    double val=0;
    if(numofG==0){
        if(vec[0]>avgenergy){
            val=0.6;
        }
    }
    for(int i=0;i<numofG;i++){
        val+= priors[i]*((pow(2*M_PI,-size/2.0)*1/sqrt(Dets[i]))*exp(-0.5*M::MatrixMultiplication(M::MatrixMult(M::MatrixDiff(vec,means[i]),M::DiagInverse(sigmas[i])),M::MatrixDiff(vec,means[i]))));
    }

    return log(val);
}




void MoG::Gmeans(std::vector<std::vector<double>> vals){
    bool finished=false;
    std::vector<double> m(vals[0].size(),0.01);
    std::vector<std::vector<double>> Centers(1,std::vector<double>(vals[0].size(),0));
    std::vector<int> Centerfinal(1,0);
    for(int i=0;i<vals.size();i++){
        Centers[0]=M::MatrixAdd(Centers[0],vals[i]);
    }
    Centers[0]=M::MatrixDiv(Centers[0],vals.size());
    std::vector<int> Assignments(vals.size(),0);
    while(!finished){
        finished=true;
        for(int i=0;i<Centers.size();i++){
            if(Centerfinal[i]==0){
                finished=false;
                std::array<std::vector<double>,2>c12;
                c12[0]=M::MatrixAdd(Centers[i],m);
                c12[1]=M::MatrixDiff(Centers[i],m);
                std::vector<std::vector<double>>vals2pass;
                for(int j=0;j<vals.size();j++){
                    if(Assignments[j]==i){
                        vals2pass.push_back(vals[j]);
                    }
                }
                std::vector<int> ass2pass(vals2pass.size(),0);
                int minimumsize=5;
                if(vals2pass.size()>=minimumsize){
                    c12=Kmeans(vals2pass,c12[0],c12[1],&ass2pass);
                    if(CheckNewCenters(c12,vals2pass)){
                        //vazw ta kainourgia
                        Centers[i]=c12[0];
                        Centers.push_back(c12[1]);
                        Centerfinal.push_back(0);
                        int index2=Centers.size()-1;
                        int k=0;
                        for(int j=0;j<vals.size();j++){
                            if(Assignments[j]==i){
                                if(ass2pass.at(k)==1){
                                    Assignments[j]=index2;
                                }
                                k++;
                            }
                        }
                    }
                    else{
                        //oristikopoiw to palio
                        Centerfinal[i]=1;
                    }
                }
                else{
                    Centerfinal[i]=1;
                }
            }

        }
    }
    // calculate covarriance
    std::vector<int> sizes(Centers.size(),0);
    for(int i=0;i<Assignments.size();i++){
        sizes[Assignments[i]]++;
    }
    std::vector<std::vector<double>> covariances(Centers.size(),std::vector<double>(size,0.01));
    for(int i=0;i<vals.size();i++){
        for(int j=0;j<size;j++){
            covariances[Assignments[i]][j]+=pow(vals[i][j]-Centers[Assignments[i]][j],2)/sizes[Assignments[i]];
        }
    }
    // enwnw osa gaussians prepei
    if(numofG>=1){
        for(int i=0;i<Centers.size();i++){
            double mindist=M::MatrixDist(means[0],Centers[i]);
            int minindex=0;
            for(int j=1;j<means.size();j++){
                double temp=M::MatrixDist(means[j],Centers[i]);
                if(mindist>temp){
                    mindist=temp;
                    minindex=j;
                }
            }
            if(mindist<M::MatrixDist(sigmas[minindex],std::vector<double>(sigmas[minindex].size(),0))/(6.833-6*quality)){
                Centerfinal[i]=0;
                std::vector<double> newmean;
                newmean=M::MatrixDiv(M::MatrixAdd(M::MatrixMultiplication(Density[minindex],means[minindex]),M::MatrixMultiplication(sizes[i],Centers[i])),Density[minindex]+sizes[i]);
                std::vector<double> newsigmas;
                newsigmas=M::MatrixDiv(M::MatrixAdd(M::MatrixMultiplication(Density[minindex],sigmas[minindex]),M::MatrixMultiplication(sizes[i],covariances[i])),Density[minindex]+sizes[i]);
                means[minindex].clear();
                means[minindex].insert(means[minindex].begin(),newmean.begin(),newmean.end());
                sigmas[minindex].clear();
                sigmas[minindex].insert(sigmas[minindex].begin(),newsigmas.begin(),newsigmas.end());
                Dets[minindex]=M::DiagDeterminant(sigmas[minindex]);
                Density[minindex]+=sizes[i];
            }
        }
    }
    // prosthetw ta kentra sth domh
    for(int i=0;i<Centers.size();i++){
        if(Centerfinal[i]){
            means.push_back(Centers[i]);
            sigmas.push_back(covariances[i]);
            Dets.push_back(M::DiagDeterminant(sigmas[sigmas.size()-1]));
            Density.push_back(sizes[i]);
            ofDensity.push_back(0);
            numofG++;
        }
    }
    //*
    double sum=0;
    for(int i=0;i<Density.size();i++){
        sum+=Density[i];
    }
    //*/
    priors.clear();
    for(int i=0;i<Density.size();i++){
        priors.push_back(Density[i]/sum);
    }

}

bool MoG::CheckNewCenters(std::array<std::vector<double>,2>centers,std::vector<std::vector<double>>vals){
    // tha vgei apo tis diafaneies CDF provolh to c1-c2 olw twn vals kai xrhsh ths katanomh N(0,1) twn provolwn gia ta zi sthn A^2*z
    // projection opws to 3erw
    std::vector<double>V=M::MatrixDiff(centers[0],centers[1]);
    std::vector<double> Proj(vals.size(),0);
    double m=0;
    // project and calc mean
    for(int i=0;i<vals.size();i++){
        Proj[i]=M::Dot(vals[i],V)/sqrt(M::Dot(vals[i],vals[i]));
        m+=Proj[i];
    }
    m/=Proj.size();
    // transform so tha mean=0 and calculate variance
    double s=0;
    for(int i=0;i<Proj.size();i++){
        Proj[i]-=m;
        s+=pow(Proj[i]-m,2);
    }
    s/=Proj.size();
    // transform so that variance=1
    for(int i=0;i<Proj.size();i++){
        Proj[i]/=s;
    }
    // sort results;
    std::sort(Proj.begin(),Proj.end());
    // calc cdf
    for(int i=0;i<Proj.size();i++){
        Proj[i]=0.5+0.5*erf((Proj[i]-m)/(s*1.41421356237309504880));
    }


    double dt,n,ff,fn,fo=0.0,d=0.0;
    n=Proj.size();
    for(int i=0;i<n;i++){
        fn=(i+1.0)/n;
        ff=Proj[i];
        dt=fmax(fabs(fo-ff),fabs(fn-ff));
        if(dt>d)
            d=dt;
        fo=fn;
    }
    n=sqrt(n);
    if(d<critical/n){
        return false;
    }
    else{
        return true;
    }
}

double MoG::probks(double alam){
    int j;
    double a2, fac=2.0,sum=0.0,term,termbf=0.0;
    a2=-2.0*alam*alam;
    for(j=1;j<=100;j++){
        term=fac*exp(a2*j*j);
        sum+=term;
        if(fabs(term)<0.001*termbf||fabs(term)<=1.0e-8*sum)return sum;
        fac=-fac;
        termbf=fabs(term);
    }
    return 1.0;
}

std::array<std::vector<double>,2> MoG::Kmeans(std::vector<std::vector<double>> vals,std::vector<double> c1,std::vector<double> c2,std::vector<int> *Assignments){
    std::vector<double> Oldc1,Oldc2;
    bool finished;
    do{
        Oldc1=c1;
        Oldc2=c2;
        //clustering and suming
        std::vector<double> sum1(vals[0].size(),0);
        std::vector<double> sum2(vals[0].size(),0);
        int num1=0;
        int num2=0;
        for(int i=0;i<vals.size();i++){
            if(M::MatrixDist(c1,vals[i])<=M::MatrixDist(c2,vals[i])){
                sum1=M::MatrixAdd(sum1,vals[i]);
                num1++;
                Assignments->at(i)=0;
            }
            else{
                sum2=M::MatrixAdd(sum2,vals[i]);
                num2++;
                Assignments->at(i)=1;
            }
        }
        // new centers
        if(num1!=0){
            c1=M::MatrixDiv(sum1,num1);
        }
        else{
            c1=std::vector<double>(c1.size(),0);
        }
        if(num2!=0){
            c2=M::MatrixDiv(sum2,num2);
        }
        else{
            c2=std::vector<double>(c2.size(),0);
        }
        // cehck new centers with old
        finished=true;
        if(M::MatrixDist(c1,Oldc1)!=0){
            finished=false;
        }
        if(M::MatrixDist(c2,Oldc2)!=0){
            finished=false;
        }
    }while(finished);
    std::array<std::vector<double>,2> centers;
    centers[0]=c1;
    centers[1]=c2;
    return centers;
}




int MoG::GetData(std::vector<std::vector<double>> * m,std::vector<std::vector<double>>*s,std::vector<double> * d,std::vector<double> * p){
    for(int i=0;i<means.size();i++){
        m->push_back(means[i]);
        s->push_back(sigmas[i]);
        d->push_back(Dets[i]);
        p->push_back(priors[i]);
    }
    return numofG;
}


