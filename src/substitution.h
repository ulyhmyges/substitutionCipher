/*
**  Filename : substitution.h
**
**  Made by : ulyh
**
**  Description : Substitution cipher
*/
#ifndef SUBSTITUTION_H
#define SUBSTITUTION_H

enum MODE {
    CHIFFRE,    // crypte le texte avec la clé de cryptage
    DECHIFFRE    // détermine la clé de cryptage
};
enum TYPE { // formatage à l'affichage
    CHAR,
    DECIMAL
};
struct Tableau {
    char tab[50];
    int dim;
};

// Pour une analyse de la cryptographie utilisée
// Appelez successivement
// en mode CHIFFRE les méthodes Saisie et Sortie
// en mode DECHIFFRE les méthodes Saisie, Calcule_clé et Sortie
class Substitution {
public:
    // Le mode CHIFFRE crypte le texte avec la clé de cryptage
    // Le mode DECHIFFRE détermine la clé de cryptage à partir des textes en clair et crypté
    Substitution(MODE mode = MODE::CHIFFRE, bool sauvegardee = false);
    ~Substitution();

    bool Saisie();
    void Calcule_clé();
    void Sortie();

private:
    MODE Mode;
    Tableau Texte_depart;
    Tableau Texte_crypté;
    Tableau La_cle;
    Tableau Chemin;
    char Lettres[26];
    bool AnalyseSauvegardee;
    char* Tampon;

    char Décalage(char lettre, char ref = 65);
    char Cryptage(char lettre, char crypt, MODE mode = MODE::CHIFFRE);
    char* Format(char value, TYPE type = TYPE::CHAR);
};

#endif  // SUBSTITUTION__H