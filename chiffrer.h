#ifndef CHIFFRER_H
#define CHIFFRER_H
#include <string>

void toUppercase(const char* input, const char* output);

enum TypeCrypt { // MODE CIPHER / NON CIPHER
    CLE,    // la clé de cryptage est utilisé
    TEXTE    // le texte crypté est utilisé
};

enum TypeFormat { // formatage à l'affichage
    CHAR,
    DECIMAL
};

class Chiffrer {
public:
    Chiffrer(bool déchiffre = true);
    ~Chiffrer();

    char Décalage(char lettre, char ref = 65);
    char Cryptage(char lettre, char crypt, TypeCrypt type = TypeCrypt::CLE);
    void Analyse(bool sauvée = false);

private:
    bool déchiffre;
    bool analyseSauvegardée;
    char chemin[200];
    char lettres[26];
    char Texte_depart[50];
    char Texte_crypté[50];
    char La_cle[50];
    int longueurTexteDepart;
    int longueurTexteCrypté;
    int longueurClé;
    char* tampon;

    void Calcule_clé();
    char* format(char value, TypeFormat type = TypeFormat::CHAR);
    bool saisie();
    void sortie();
};

#endif  // CHIFFRER__H