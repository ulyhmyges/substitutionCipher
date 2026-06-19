#include "substitution.h"
#include <string>
#include <iostream>


int main(void){
    // En mode CHIFFRE => Détermine le texte crypté à partir du texte en clair et de la clé de cryptage
    // En mode DECHIFFRE => Détermine la clé de cryptage à partir des textes en clair et crypté

    // CHIFFRE et NON SAUVEGARDE
    Substitution sub = Substitution(); // mode CHIFFRE et analyse non sauvegardée par défaut
    sub.Saisie();
    sub.Calcule_clé(); 
    sub.Sortie();

    // // CHIFFRE et SAUVEGARDE
    // Substitution sub = Substitution(MODE::CHIFFRE, true); // mode CHIFFRE et analyse non sauvegardée par défaut
    // sub.Saisie();
    // sub.Calcule_clé(); 
    // sub.Sortie();

    // // DECHIFFRE et NON SAUVEGARDE
    // Substitution sub = Substitution(MODE::DECHIFFRE); // mode CHIFFRE et analyse non sauvegardée par défaut
    // sub.Saisie();
    // sub.Calcule_clé(); 
    // sub.Sortie();

}
