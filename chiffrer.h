#ifndef CHIFFRER_H
#define CHIFFRER_H
#include <string>

void toUppercase(const char* input, const char* output);

class Chiffrer {
public:
    Chiffrer(bool decipher = false);
    int Decalage(int letter, int ref = 65);
    int Cryptage(int letter, bool key = false);
    void Analyze(bool isSaved = false);


private:
    int lettres[26];
    int rangs[26];  // utile ??
    int Texte_depart[500];
    int textLength;
    int cleLength;
    int La_cle[100];
    int Texte_cipher[500];
    int textCipherLength;
    char path[200];
    bool decipher;
    std::string filename = "sortie.txt";
    bool error;

    template <typename T>
    std::string format(T value);
    void input(bool isSaved = false);
    void output(bool isSaved = false);

    // int rang(int letter);
};

#endif  // CHIFFRER__H