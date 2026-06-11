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
    // TEXTE DE DEPART
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
    // on ignore la suite de caractères "\n ; ; \n" qui signalent la séparation entre le texte et la clé
    textLength--;   // ';'
    if (Texte_depart[textLength - 1] == '\n'){ // '\n'
        textLength--;
    }
    c = getchar();  // '\n'
    for (int i = 0; i < textLength; i++){
        printf("%c", Texte_depart[i]);
    }
    
    // CLE DE CRYPTAGE
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

int Decalage(int c){
    if (c < 65 || c > 122) {
        std::cout << "Erreur : argument invalide\n";
        std::cout << "Fonction : Decalage\n";
        return -1;
    }
    return c - 65;
}