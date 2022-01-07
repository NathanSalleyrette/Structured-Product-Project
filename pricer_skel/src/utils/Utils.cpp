#include "../../headers/utils/Utils.hpp"

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

void Utils::correlationMatrix(const PnlMat* path, PnlMat* corrMat){
    PnlVect* X = pnl_vect_create(path->m);
    PnlVect* Y = pnl_vect_create(path->m);

    int m = path->n;
    
    for(int i = 0; i < m; i++){
        for(int j = i; j < m; j++){
            pnl_mat_get_col(X, path, i);
            pnl_mat_get_col(Y, path, j);
            MLET(corrMat, i, j) = correlation(X,Y);
            MLET(corrMat, j, i) = correlation(X,Y);
        }
    }
}

void Utils::volsOnMat(PnlVect* volatilities, const PnlMat* path){
    PnlVect* temp = pnl_vect_create(1);
    for (int i = 0; i < volatilities->size; i++){
        pnl_mat_get_col(temp, path, i);
        LET(volatilities, i) = sqrt(covariance(temp,temp));
    }
}       