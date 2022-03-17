#include "utils/Utils.hpp"
#include "spdlog/spdlog.h"

Utils::Utils(){}

double Utils::covariance(const PnlVect* X, const PnlVect* Y){
    // std::cout<<X->size<<endl;
    //assert(("Vecteurs de tailles différentes pour le calcul de la covariance",X->size == Y->size));
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
    
    //assert(("Vecteur nul en argument pour un calcul d'esperance",X->size > 0));
    double result = 0.;

    for(int i = 0; i < X->size; i++){
        result += GET(X, i);
    }

    return result/X->size;
}

double Utils::correlation( const PnlVect* X,  const PnlVect* Y){
    double covXY = covariance(X, Y);
    double sigmaX = std::sqrt(covariance(X,X));
    double sigmaY = std::sqrt(covariance(Y,Y));

    if(sigmaX < 1E-6){
        sigmaX = 1;
    }
    if(sigmaY < 1E-6){
        sigmaY = 1;
    }

    return covXY/(sigmaX * sigmaY);
}

void Utils::correlationMatrix(const PnlMat* path, PnlMat* corrMat){
    PnlVect* X = pnl_vect_create(path->m);
    PnlVect* Y = pnl_vect_create(path->m);
    PnlVect* returnsX = pnl_vect_create(path->m - 1);
    PnlVect* returnsY = pnl_vect_create(path->m - 1);

    int m = path->n;
    
    for(int i = 0; i < m; i++){
        for(int j = i; j < m; j++){
            pnl_mat_get_col(X, path, i);
            pnl_mat_get_col(Y, path, j);

            for(int l = 0; l < X->size - 1; l++){
                LET(returnsX, l) = log(GET(X,l + 1)/GET(X, l));
                LET(returnsY, l) = log(GET(Y,l + 1)/GET(Y, l));
            }

            MLET(corrMat, i, j) = correlation(returnsX,returnsY);
            MLET(corrMat, j, i) = correlation(returnsX,returnsY);
        }
    }
    pnl_vect_free(&X);
    pnl_vect_free(&Y);
    std::shared_ptr<spdlog::logger> _logger = spdlog::get("MainLogs");
    SPDLOG_LOGGER_INFO(_logger, "Correlation Matrix computed");
}

void Utils::volsOnMat(PnlVect* volatilities, const PnlMat* path){
    PnlVect* temp = pnl_vect_create(1);
    PnlVect* returns = pnl_vect_create(volatilities->size);
    double cov;
    double sqrtcov;
    double expsqrtcov;
    for (int i = 0; i < volatilities->size; i++){
        pnl_mat_get_col(temp, path, i);
        //On calcul les log rendements

        for(int j = 1; j < temp->size; j++){
            LET(returns, j) = log( GET(temp, j) / GET(temp, j - 1) );
        }
        cov = covariance(returns, returns);
        sqrtcov = sqrt(cov);
        expsqrtcov = exp(sqrtcov);
        //LET(volatilities, i) = std::exp(std::sqrt(covariance(returns,returns)));
        LET(volatilities, i) = expsqrtcov;

    }
    pnl_vect_free(&temp);
    pnl_vect_free(&returns);
    std::shared_ptr<spdlog::logger> _logger = spdlog::get("MainLogs");
    SPDLOG_LOGGER_INFO(_logger, "Vols computed from path matrix");
}       