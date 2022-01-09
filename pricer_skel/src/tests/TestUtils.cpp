#include "utils/Utils.hpp"


int main(){
    PnlVect *X = pnl_vect_create_from_scalar(10,100);
    PnlVect *Y = pnl_vect_create_from_scalar(10,200);
    
    if(Utils::esperance(X) == 100){
        std::cout<<"eserance valide"<<endl;
    }else{
        std::cout<<"esperance non valide"<<endl;
    }

    PnlVect *N = pnl_vect_create(15);
    PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
    pnl_rng_sseed(rng, time(NULL));

    pnl_vect_rng_normal(N,15,rng);

    std::cout<<Utils::covariance(N,N)<<std::endl;


    if(abs(Utils::covariance(X,Y)) < 1E-12){
        std::cout<<"covariance valide"<<endl;
    }else{
        std:cout<<"covariance non valide"<<endl;
    }


    // On veut maintenant tester la fonction de correlation

    PnlMat* path = pnl_mat_create(1000, 10);
    pnl_mat_rng_normal(path, 1000, 10, rng);

    PnlMat* corr = pnl_mat_create(10, 10);

    Utils::correlationMatrix(path, corr);

}