#include "chiffre.h"
// #include <cstdio>
#include <iostream>

void toUppercase(const char *input, const char *output)
{
    FILE *f = NULL;
    FILE *w = NULL;
    f = fopen(input, "r");
    w = fopen(output, "w"); // file created
    if (f == NULL || w == NULL)
    {
        std::cout << "erreur à l'ouverture du ficher";
        return;
    }

    int c = 0;
    while (!feof(f)){
        c = fgetc(f);
        if (c == EOF)
            break;

        // traitement des caractère spéciaux ( lettres accentuées et c cédille)
        if (c == 195) {
            c = fgetc(f);
            switch (c) 
            {
            case 169:   // é
            case 168:   // è   
            case 170:   // ê
            case 171:   // ë
            case 139:   // Ë
                fputc('E', w);
                break;
            case 160:   // à
            case 162:   // â
                fputc('A', w);
                break;
            case 174:   // î
            case 175:   // ï
                fputc('I', w);
                break;
            case 185:   // ù
            case 188:   // ü
                fputc('U', w);
                break;
            case 180:   // ô
                fputc('O', w);
                break;
          
            }
            continue;
        }

        // change lettre minuscule en lettre majuscule
        if (c >= 97 && c <= 122)
        {
            c = c - 32;
        }

        // écriture dans le fichier de sortie output
        fputc(c, w);
        //std::cout << static_cast<char>(c) << " = " << c << std::endl;
    }
    fclose(f);
    fclose(w);
}

Chiffrer::Chiffrer() {
    build();
}

void Chiffrer::build(){
    // construction des lettres et rangs de l'alphabet
    for (int i = 0; i < 26; i++){
        lettres[i] = i + 65;
        rangs[i] = i + 1;
    }
}

// On suppose saisie correcte au clavier, 
// càd sans espace, ni accent et en lettres majuscules uniquement :
// - Text_depart
// - La_cle
void Chiffrer::enter() {
    // SAISIR LE TEXTE DE DEPART
    std::cout << "Saisir un texte à chiffrer (Taper ';;' pour signaler la fin du texte):\n";
    int c = getchar();
    textLength = 0;
    int last = 0;
    while (c!= EOF && !(c == ';' && last == ';')){
        last = c;
        Texte_depart[textLength] = c;
        c = getchar();
        textLength++;
    }
    // La suite de caractères ";;\n" marque la séparation entre le texte et la clé
    textLength--;   // ';'
    if (Texte_depart[textLength - 1] == '\n'){ // '\n'
        textLength--;
    }
    c = getchar();  // '\n'
    for (int i = 0; i < textLength; i++){
        printf("%c", Texte_depart[i]);
    }
    
    // SAISIR LA CLE DE CRYPTAGE
    std::cout << std::endl;
    std::cout << "Saisir la clé de cryptage (Tapez Ctrl + d pour continuer):\n";
  
    c = getchar();
    cleLength = 0;
    while (c != EOF){
        // printf("%c %d,", c, cleLength);
        La_cle[cleLength] = c;
        c = getchar();
        cleLength++;
    }
    // n'inclue pas le caractère '\n' dans la clé de cryptage La_cle
    if (La_cle[cleLength - 1] == '\n'){
        cleLength--;
    }
    for (int i = 0; i < cleLength; i++){
        printf("%c", La_cle[i]);
    }
    std::cout << std::endl;
}

int Chiffrer::Decalage(int c){
    if (c < 65 || c > 122) {
        std::cout << "Erreur : argument invalide\n";
        std::cout << "Fonction : Decalage\n";
        return -1;
    }
    return c - 65;

    int m = textLength / cleLength;
    int r = textLength % cleLength;
    int count = 0;
    // cleLength * m + r
    while (count < m){
        if (count % 2 == 0)
            std::cout << "\033[46m";
        else
            std::cout << "\033[41m";

        for (int i = 0; i < cleLength; i++){
            std::cout << " " << La_cle[i] - 65 ;
            if (La_cle[i] < 74)
                std::cout << " ";
        }
        count++;
    }
    std::cout << "\033[46m";
    for (int i = 0; i < r; i++){
        std::cout << " " << La_cle[i] - 65 ;
        if (La_cle[i] < 74)
            std::cout << " ";
    }
    std::cout << "\033[0m" << std::endl;
}

void Chiffrer::print() {
    // LETTRE ET RANG
    std::cout << "Lettre\t\t\t\t\t\t";
    for (int i = 0; i < 26; i++ ){
        std::cout << " " << static_cast<char>(lettres[i]) << " ";
    }
    std::cout << std::endl;
    std::cout << "Rang dans l'alphabet\t\t\t\t";
    for (int i = 0; i < 26; i++ ){
        std::cout << " " << lettres[i] - 64 ;
        if (lettres[i] < 74)
            std::cout << " ";
    }
    std::cout << "\n\n\n\n" ;

    // CLE DE CRYPTAGE ET DECALAGE
    std::cout << "Clé de cryptage\t\t\t\t\t";
    for (int i = 0; i < cleLength; i++){
        std::cout << " " << static_cast<char>(La_cle[i]) << " ";
    }
    std::cout << std::endl;
    std::cout << "Décalage (par rapport à la lettre A)\t\t\033[46m";
    for (int i = 0; i < cleLength; i++){
        std::cout << " " << La_cle[i] - 65 ;
        if (La_cle[i] < 74)
            std::cout << " ";
    }
    std::cout << "\033[0m\n\n\n";

    // TEXTE ET RANG
    std::cout << "Texte à crypter\t\t\t\t\t";
    for (int i = 0; i < textLength; i++){
        std::cout << " " << static_cast<char>(Texte_depart[i]) << " ";
    }
    std::cout << std::endl;
    std::cout << "Rang de départ\t\t\t\t\t";
    for (int i = 0; i < textLength; i++ ){
        std::cout << " " << Texte_depart[i] - 64;
        if (Texte_depart[i] < 74)
            std::cout << " ";
    }
    std::cout << std::endl;

    // DECALAGE
    std::cout << "Décalage\t\t\t\t\t";
    bool topple = false;
    for (int i = 0; i < textLength; i++){
        if (i % cleLength == 0)
            topple = !topple;
            
        std::cout << (topple ? "\033[46m" : "\033[41m");
        
        std::cout << " " << La_cle[i % cleLength] - 65 ;
            if (La_cle[i % cleLength] < 74)
                std::cout << " ";
    }
    std::cout << "\033[0m" << std::endl;

    // RANG APRES (somme)
    std::cout << "Rang après décalage (somme)\t\t\t";
    int rang = 0;
    for (int i = 0; i < textLength; i++){
        rang = (Texte_depart[i] - 64) + (La_cle[i % cleLength] - 65);
        std::cout << " " << rang;
            if (rang < 10)
            std::cout << " ";
    }
    std::cout << std::endl;

    // RANG FINAL (1 <= rang <= 26 )
    std::cout << "Rang Final (Total ou Total-26)\t\t\t";
    for (int i = 0; i < textLength; i++){
        rang = (Texte_depart[i] - 64) + (La_cle[i % cleLength] - 65);
        rang = rang % 26 == 0 ? 26 : rang % 26;

        std::cout << " " << rang;
        if (rang < 10)
            std::cout << " ";
    }
    std::cout << std::endl;

    // LETTRE APRES
    std::cout << "Texte crypté (lettre associée au rang final)\t";
    for (int i = 0; i < textLength; i++){
        rang = (Texte_depart[i] - 64) + (La_cle[i % cleLength] - 65);
        rang = rang % 26 == 0 ? 26 : rang % 26;
        
        std::cout << " " << static_cast<char>(rang + 64) << " ";   // CODE ASCII DE LA LETTRE
    }
    std::cout << "\n\n";

    // TEXTE DE DEPART
    std::cout << "\033[1mTexte de départ\033[0m" << std::endl;
    for (int i = 0; i < textLength; i++){
        std::cout << static_cast<char>(Texte_depart[i]);
    }

    // TEXTE CRYPTE
    std::cout << "\n\n\033[1m" << "Texte crypté" << "\033[0m\n";
    for (int i = 0; i < textLength; i++){
        rang = (Texte_depart[i] - 64) + (La_cle[i % cleLength] - 65);
        rang = rang % 26 == 0 ? 26 : rang % 26;
        
        std::cout << static_cast<char>(rang + 64);   // CODE ASCII DE LA LETTRE
    }
    std::cout << std::endl;
}