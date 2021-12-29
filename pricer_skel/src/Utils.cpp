#include "Utils.hpp"

Utils::Utils(){}

double Utils::covariance(const PnlVect* X, const PnlVect* Y){
    std::cout<<X->size<<endl;
    assert(("Vecteurs de tailles différentes pour le calcul de la covariance",X->size == Y->size));
    double E_x = esperance(X);
    double E_y = esperance(Y);

    double E_xy = 0;
    for(int i = 0; i < X->size; i++){
        E_xy += GET(X,i) * GET(Y,i);
    }

    E_xy /= X->size;

    return E_xy - E_y * E_x;
}

double Utils::esperance(const PnlVect* X){
    
    assert(("Vecteur nul en argument pour un calcul d'esperance",X->size > 0));
    double result = 0.;

    for(int i = 0; i < X->size; i++){
        result += GET(X, i);
    }

    return result/X->size;
}

double Utils::correlation( const PnlVect* X,  const PnlVect* Y){
    double covXY = covariance(X, Y);
    double sigmaX = sqrt(covariance(X,X));
    double sigmaY = sqrt(covariance(Y,Y));

    return covXY/(sigmaX * sigmaY);
}