#ifndef SUBSTITUTION_H
#define SUBSTITUTION_H
#include <string>

enum Mode { // MODE chiffrement / déchiffrement
    CHIFFRE,    // la clé de cryptage est utilisé
    DECHIFFRE    // le texte crypté est utilisé
};

enum TypeFormat { // formatage à l'affichage
    CHAR,
    DECIMAL
};

class Substitution {
public:
    Substitution(Mode mode = Mode::CHIFFRE);
    ~Substitution();

    char Décalage(char lettre, char ref = 65);
    char Cryptage(char lettre, char crypt, Mode mode = Mode::CHIFFRE);
    void Analyse(bool sauvée = false);

private:
    Mode mode;
    bool analyseSauvegardee;
    char chemin[200];
    char lettres[26];
    char Texte_depart[50];
    char Texte_crypté[50];
    char La_cle[50];
    int longueurTexteDepart;
    int longueurTexteCrypté;
    int longueurCle;
    char* tampon;

    void Calcule_clé();
    char* format(char value, TypeFormat type = TypeFormat::CHAR);
    bool saisie();
    void sortie();
};

#endif  // SUBSTITUTION__H