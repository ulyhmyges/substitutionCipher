#ifndef CHIFFRER_H
#define CHIFFRER_H
#include <string>

void toUppercase(const char* input, const char* output);

enum TypeCrypt { // MODE CIPHER / NON CIPHER
    KEY,    // la clé de cryptage est utilisé
    TEXT    // le texte crypté est utilisé
};

enum TypeFormat { // formatage à l'affichage
    CHAR,
    DECIMAL
};

class Chiffrer {
public:
    Chiffrer(bool cipher = true);
    ~Chiffrer();

    char Décalage(char letter, char ref = 65);
    char Cryptage(char letter, char crypt, TypeCrypt type = TypeCrypt::KEY);
    void Analyse(bool analyseSaved = false);

private:
    bool cipher;
    bool analyseSaved;
    char path[200];
    char lettres[26];
    char Texte_depart[50];
    char Texte_cipher[50];
    char La_cle[50];
    int textLength;
    int textCipherLength;
    int cleLength;
    char* buffer;

    void Calcule_clé();
    char* format(char value, TypeFormat type = TypeFormat::CHAR);
    bool input();
    void output();
};

#endif  // CHIFFRER__H