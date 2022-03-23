#Installer les modules du front

cd Frontend
npm install
cd ..

#Installer les modules du back

cd backend-web
npm install --build-from-source --python=/usr/bin/python3


#Installer pnl lib
unzip pnl-1.10.4
cd pnl-1.10.4
mkdir build
cd build
cmake ..
make
make install
cd ../..

#Construire l'addon
npm install
cd ..

#Lancer le back et le front

node launch.js

