#ifndef CHIFFRER_H
#define CHIFFRER_H
#include <string>

void toUppercase(const char* input, const char* output);

class Chiffrer {
public:
    Chiffrer(bool cipher);
    int Decalage(int letter);
    int Cryptage(int letter);
    void enter();

    template <typename T>
    std::string format(T value);
    int rang(int letter);

    void print();
    void toFile();

private:
    int lettres[26];
    int rangs[26];
    void build();
    int Texte_depart[500];
    int textLength;
    int cleLength;
    int La_cle[100];
    int cipherText[500];
    int cTextLength;
    bool cipher;
    std::string output = "sortie.txt";
};

#endif  // CHIFFRER__H