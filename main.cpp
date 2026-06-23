#include "substitution.h"
#include <iostream>

int main(void){
    int cas = 2;
    switch (cas){
        case 1:
        {
            std::cout << "PARTIE 1 : CHIFFRE ET SAUVEGARDE\n";
            Substitution sub = Substitution(MODE::CHIFFRE, true); 
            bool réussie = sub.Saisie();
            if (!réussie) return -1;
            sub.Sortie();
            break;
        }
        case 2:
        {
            std::cout << "PARTIE 2 : DECHIFFRE ET NON SAUVEGARDE\n";
            Substitution sub = Substitution(MODE::DECHIFFRE); // pas de sauvegarde possible dans cette partie du projet
            bool réussie = sub.Saisie();
            if (!réussie) return -1;
            sub.Calcule_clé(); 
            sub.Sortie();
            break;
        }
        default:
        {
            std::cout << "PARTIE 1 : CHIFFRE ET NON SAUVEGARDE\n";
            Substitution sub = Substitution(); // mode CHIFFRE et analyse non sauvegardée par défaut
            bool réussie = sub.Saisie();
            if (!réussie) return -1;
            sub.Sortie();
        }
    }
}
