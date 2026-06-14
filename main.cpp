#include "chiffrer.h"
#include <string>
#include <iostream>


int main(void){
    //
    toUppercase("dream.txt", "out.txt");

    // deux modes de création d'objet
    // decipher = false => chiffre : détermine le texte crypté 
    // decipher = true => déchiffre : détermine la clé de cryptage
    Chiffrer obj = Chiffrer(true); // decipher ?

    // by default not saving
    obj.Analyze(true);  

}
