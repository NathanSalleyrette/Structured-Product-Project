#include "utils/Utils.hpp"


int main(){
    PnlVect *X = pnl_vect_create_from_scalar(20,100);
    PnlVect *Y = pnl_vect_create_from_scalar(10,200);
    Utils *utils = new Utils();
    if(utils->esperance(X) == 100){
        std::cout<<"eserance valide"<<endl;
    }else{
        std::cout<<"esperance non valide"<<endl;
    }

    if(abs(utils->covariance(X,Y)) < 1E-12){
        std::cout<<"covariance valide"<<endl;
    }else{
        std:cout<<"covariance non valide"<<endl;
    }
}