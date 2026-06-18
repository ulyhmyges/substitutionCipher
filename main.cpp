#include "chiffrer.h"
#include <string>
#include <iostream>


int main(void){
    //
    // toUppercase("test/d.txt", "test/dd.txt");

    // deux modes de création d'objet
    // decipher = false => chiffre : détermine le texte crypté 
    // decipher = true => déchiffre : détermine la clé de cryptage
    Chiffrer obj = Chiffrer(false); // cipher = true par défaut

    // by default not saving
    obj.Analyse(true);  
    


}
