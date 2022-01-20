#include <iostream>
#include<cstdio>

int main(){
    FILE * f;
    f = fopen ("toto.txt", "wt");
    if (f == NULL)
    std::cout << "Impossible d'ouvrir le fichier en écriture !" << std::endl;
  else
   {
      fprintf (f, "%lf, oui", 15.);
   
    fclose (f);
   }
   return 0;
}