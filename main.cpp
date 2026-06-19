#include "substitution.h"
#include <string>
#include <iostream>


int main(void){
    // En mode CHIFFRE => Détermine le texte crypté à partir du texte en clair et de la clé de cryptage
    // En mode DECHIFFRE => Détermine la clé de cryptage à partir des textes en clair et crypté
    Mode dechiffre = Mode::DECHIFFRE;
    Substitution obj = Substitution(dechiffre); // mode = CHIFFRE par défaut

    // by default not saving
    obj.Analyse(true);  
    


}
