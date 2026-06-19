#include "substitution.h"
#include <iostream>

int main(void){
    // std::cout << "PARTIE 1 : CHIFFRE ET NON SAUVEGARDE\n";
    // Substitution sub = Substitution(); // mode CHIFFRE et analyse non sauvegardée par défaut
    // sub.Saisie();
    // sub.Sortie();

    std::cout << "PARTIE 1 : CHIFFRE ET SAUVEGARDE\n";
    Substitution sub1 = Substitution(MODE::CHIFFRE, true); 
    sub1.Saisie();
    sub1.Sortie();

    // std::cout << "PARTIE 2 : DECHIFFRE ET NON SAUVEGARDE\n";
    // Substitution sub0 = Substitution(MODE::DECHIFFRE); // pas de sauvegarde possible dans cette partie du projet
    // sub0.Saisie();
    // sub0.Calcule_clé(); 
    // sub0.Sortie();
}
