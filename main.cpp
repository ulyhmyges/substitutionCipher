#include "substitution.h"
#include <iostream>

int main(void){
    Substitution sub0, sub1, sub2;
    int cas = 0;
    switch (cas){
    case 1:
        std::cout << "PARTIE 1 : CHIFFRE ET SAUVEGARDE\n";
        sub1 = Substitution(MODE::CHIFFRE, true); 
        sub1.Saisie();
        sub1.Sortie();
        break;
    case 2:
        std::cout << "PARTIE 2 : DECHIFFRE ET NON SAUVEGARDE\n";
        sub2 = Substitution(MODE::DECHIFFRE); // pas de sauvegarde possible dans cette partie du projet
        sub0.Saisie();
        sub0.Calcule_clé(); 
        sub0.Sortie();
        break;
    default:
        std::cout << "PARTIE 1 : CHIFFRE ET NON SAUVEGARDE\n";
        Substitution sub = Substitution(); // mode CHIFFRE et analyse non sauvegardée par défaut
        sub.Saisie();
        sub.Sortie();
    }
}
