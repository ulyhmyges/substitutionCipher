#include "substitution.h"
#include <string>
#include <iostream>


int main(void){
    // PARTIE 1
    // En mode CHIFFRE => Détermine le texte crypté à partir du texte en clair et de la clé de cryptage
    // CHIFFRE et NON SAUVEGARDE
    // Substitution sub = Substitution(); // mode CHIFFRE et analyse non sauvegardée par défaut
    // sub.Saisie();
    // sub.Sortie();

    // CHIFFRE et SAUVEGARDE
    Substitution sub1 = Substitution(MODE::CHIFFRE, true); // mode CHIFFRE et analyse non sauvegardée par défaut
    sub1.Saisie();
    sub1.Sortie();

    // PARTIE 2
    // // En mode DECHIFFRE => Détermine la clé de cryptage à partir des textes en clair et crypté
    // // DECHIFFRE et NON SAUVEGARDE
    // Substitution sub0 = Substitution(MODE::DECHIFFRE); // mode CHIFFRE et analyse non sauvegardée par défaut
    // sub0.Saisie();
    // sub0.Calcule_clé(); 
    // sub0.Sortie();

}
