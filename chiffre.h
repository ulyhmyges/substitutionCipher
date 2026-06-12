#ifndef CHIFFRE_H
#define CHIFFRE_H
#include <string>

void toUppercase(const char* input, const char* output);

class Chiffrer {
public:
    Chiffrer();
    int Decalage(int letter);
    int Cryptage(int letter);
    int rang(int letter);
    void enter();
    template <typename T>
    std::string format(T value);
    void print();
    void fprint();

private:
    int lettres[26];
    int rangs[26];
    void build();
    int Texte_depart[500];
    int textLength;
    int cleLength;
    int La_cle[100];
    std::string output = "sortie.txt";
};

#endif  // CHIFFRE__H