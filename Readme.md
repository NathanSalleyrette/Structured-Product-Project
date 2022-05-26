# Valuation of structured products project
#Installer les modules du front

cd Frontend

npm install

cd ..

#Installer les modules du back

cd backend-web

npm install

cd ..

(si erreur sur install sqlite3 : npm install --build-from-source --python=/usr/bin/python3 )
(si l'erreur persiste : télécharger le paquet python-is-python3 et re essayer)

#Installer pnl lib

cd AddonJS

(si la librairie est déjà installée sur votre machine, il faut modifier le install dans le AddonJS/package.json avec le bon path)

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

##En même temps :

node launch.js

##Sur deux terminaux :

terminal 1:

cd Frontend

npm start

terminal 2:

cd backend-web

node srcBack/server.js
