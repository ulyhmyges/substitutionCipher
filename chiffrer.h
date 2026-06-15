#ifndef CHIFFRER_H
#define CHIFFRER_H
#include <string>

void toUppercase(const char* input, const char* output);

enum TypeCrypt {
    KEY,
    TEXT
};

enum TypeFormat {
    CHAR,
    DECIMAL
};

class Chiffrer {
public:
    Chiffrer(bool cipher = true);
    char Decalage(char letter, char ref = 65);
    // TO DO: args char letter, char cipher, bool
    char Cryptage(int i, TypeCrypt type = TypeCrypt::KEY);
    void Analyse(bool analyseSaved = false);


private:

    char lettres[26];
    char Texte_depart[500];
    int textLength;
    char Texte_cipher[500];
    int textCipherLength;
    char La_cle[500];
    int cleLength;
  
    char path[200];
    bool cipher;
    bool analyseSaved;
    bool error;

    std::string format(char value, TypeFormat type = TypeFormat::CHAR);
    void input();
    void output();
};

#endif  // CHIFFRER__H