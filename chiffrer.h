#ifndef CHIFFRER_H
#define CHIFFRER_H
#include <string>

void toUppercase(const char* input, const char* output);

class Chiffrer {
public:
    Chiffrer(bool decipher = false);
    char Decalage(char letter, char ref = 65);
    char Cryptage(int index, bool key = false);
    void Analyse(bool isSaved = false);


private:
    char lettres[26];
    char Texte_depart[500];

    int textLength;
    char Texte_cipher[500];
    int textCipherLength;
    char La_cle[100];
    int cleLength;
  
    char path[200];
    bool decipher;
    bool error;

    std::string format(char value, bool decimal = false);
    void input(bool isSaved = false);
    void output(bool isSaved = false);
};

#endif  // CHIFFRER__H