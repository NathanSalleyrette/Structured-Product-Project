# Installation des dépendances

>npm install 

## dépendances normales
>npm install node-addon-api

>npm install --save bindings

## dépendances dev


>npm install cmake-js --save-dev

Si ils disent que cmake-js est pas installé, alors il faut faire :
>sudo npm install cmake-js -g


## Package.json

Dans ce fichier il faut indiquer le path vers la librairie Pnl

`"scripts": {
    "install": "cmake-js compile --CDCMAKE_PREFIX_PATH=/home/peyrichou/PNL/pnl/build/"`


### Problème

>INTEL MKL ERROR: /opt/intel/mkl/lib/intel64_lin/libmkl_avx512.so: undefined symbol: mkl_sparse_optimize_bsr_trsm_i8.
Intel MKL FATAL ERROR: Cannot load libmkl_avx512.so or libmkl_def.so.

### Fix

>export LD_PRELOAD=/opt/intel/mkl/lib/intel64/libmkl_def.so:/opt/intel/mkl/lib/intel64/libmkl_avx2.so:/opt/intel/mkl/lib/intel64/libmkl_core.so:/opt/intel/mkl/lib/intel64/libmkl_intel_lp64.so:/opt/intel/mkl/lib/intel64/libmkl_intel_thread.so:/opt/intel/mkl/lib/intel64_lin/libiomp5.so
