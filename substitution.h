#ifndef SUBSTITUTION_H
#define SUBSTITUTION_H
#include <string>

enum MODE { // MODE chiffrement / déchiffrement
    CHIFFRE,    // la clé de cryptage est utilisé
    DECHIFFRE    // le texte crypté est utilisé
};

enum TYPE { // formatage à l'affichage
    CHAR,
    DECIMAL
};

struct Tableau {
    char tab[50];
    int dim;
};

class Substitution {
public:
    Substitution(MODE mode = MODE::CHIFFRE, bool sauvegardee = false);
    ~Substitution();

    char Décalage(char lettre, char ref = 65);
    char Cryptage(char lettre, char crypt, MODE mode = MODE::CHIFFRE);
    void Calcule_clé();
    bool Saisie();
    void Sortie();
    char* Format(char value, TYPE type = TYPE::CHAR);

private:
    MODE Mode;
    Tableau Texte_depart;
    Tableau Texte_crypté;
    Tableau La_cle;
    Tableau Chemin;
    char Lettres[26];
    bool AnalyseSauvegardee;
  
    char* Tampon;
};

#endif  // SUBSTITUTION__H