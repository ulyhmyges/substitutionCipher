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

Chiffrer::Chiffrer(bool cipher) : cipher(cipher) {
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
    std::cout << "Saisir le texte en clair (Taper ';;' pour signaler la fin du texte):\n";
    // int c = getchar();
    char c;
    scanf("%c", &c);
    textLength = 0;
    char last = 0;
    while (c!= EOF && !(c == ';' && last == ';')){
        last = c;
        Texte_depart[textLength] = c;
        scanf("%c", &c);
        textLength++;
    }
    // La suite de caractères ";;\n" marque la séparation entre le texte et la clé
    textLength--;   // ';'
    if (textLength > 0 && Texte_depart[textLength - 1] == '\n'){ // '\n'
        textLength--;
    }
    // flush stdin
    scanf("%c", &c);    // '\n' 
    
    // SAISIR LA CLE DE CRYPTAGE
    if (cipher == true) {
        std::cout << "Saisir la clé de cryptage (Tapez Ctrl + d pour continuer):\n";
        scanf("%c", &c);
        cleLength = 0;
        while (c != EOF && c != '\n'){
            La_cle[cleLength] = c;
            scanf("%c", &c);
            cleLength++;
        }
        // n'inclue pas le caractère '\n' dans la clé de cryptage La_cle
        if (cleLength > 0 && La_cle[cleLength - 1] == '\n'){
        cleLength--;
        }
        std::cout << std::endl;
    } else {
        // SAISIR LE TEXTE CRYPTE
        std::cout << "Saisir le texte chiffré (Taper ';;' pour signaler la fin du texte):\n";
        scanf("%c", &c);
        cTextLength = 0;
        last = 0;
        while (c!= EOF && !(c == ';' && last == ';')){
            last = c;
            cipherText[cTextLength] = c;
            scanf("%c", &c);
            cTextLength++;
        }
        if (cTextLength == 0) {
            std::cout << "Erreur : Text chiffré invalide\n";
            return;
        }
        // La suite de caractères ";;\n" marque la séparation entre le texte et la clé
        cTextLength--;   // ';'
        if (cTextLength > 0 && cipherText[cTextLength - 1] == '\n'){ // '\n'
            cTextLength--;
        }
        if (cTextLength != textLength) {
            std::cout << "Erreur : Text chiffré invalide\n";
            return;
        }
        // flush
        scanf("%c", &c);    // '\n'

     
        int x = rang(cipherText[0]) - rang(Texte_depart[0]);
        x = x > 0 ? x : x + 26; // si le décalage est négatif, on ajoute 26;
        int j;
        cleLength = 0;
        for (j = 1; j < cTextLength; j++){
            int y = rang(cipherText[j]) - rang(Texte_depart[j]);
            y = y > 0 ? y : y + 26; // si le décalage est négatif, on ajoute 26;
            if (x == y){
                break;
            }
        }
        cleLength = j;
        for (int i = 0; i < cleLength; i++){
            x = rang(cipherText[i]) - rang(Texte_depart[i]);
            x = x > 0 ? x : x + 26; // si le décalage est négatif, on ajoute 26;
            La_cle[i] = x + 65;
        }

        for (int i = 0; i < cTextLength; i++){
            x = rang(Texte_depart[i]) + La_cle[i % cleLength] - 65;
            x = x % 26 == 0 ? 26 : x % 26;
            int y = rang(cipherText[i]);
            y = y % 26 == 0 ? 26 : y % 26;
            if (x != y){
                std::cout << "Erreur : Les textes (en clair et crypté) ne correspondent pas.\n";
                return;
            }
        }
    }
    
}

// Calcule le décalage de la ième lettre du tableau Texte_depart pour i = index  
int Chiffrer::Decalage(int index){
    if (index < 0 || index >= textLength) {
        std::cout << "Erreur : argument invalide\n";
        std::cout << "Méthode : Decalage()\n";
        return -1;
    }
    return  La_cle[index % cleLength] - 65 ;
}

// Pour chaque lettre de Texte_depart, 
// on somme son rang de départ avec le décalage fourni par la clé de cryptage La_cle
// on retourne sa valeur ASCII
int Chiffrer::Cryptage(int index){
    if (index < 0 || index >= textLength) {
        std::cout << "Erreur : argument invalide\n";
        std::cout << "Méthode : rang()\n";
        return -1;
    }
    int r = rang(Texte_depart[index]) + Decalage(index);
    r = r % 26 == 0 ? 26 : r % 26;
    return r + 64;
}

// Détermine le rang d'une lettre à partir de son code ASCII
int Chiffrer::rang(int letter){
    return letter - 64;
}

template<typename T>
std::string Chiffrer::format(T value){
    char str[5];
    // type T = char ou type T = int
    if (sizeof(T) == 1){
        snprintf(str, 4, " %c ", value); 
    } else if (value < 10) {
        snprintf(str, 4, " %d ", value);
    } else {
        snprintf(str, 4, " %d", value);
    }
    return str;
}

void Chiffrer::print() {
    std::cout << "Lettre\t\t\t\t\t\t";      // LETTRE
    for (int i = 0; i < 26; i++ ){
        std::cout << format(static_cast<char>(lettres[i]));
    }
    std::cout << std::endl;

    std::cout << "Rang dans l'alphabet\t\t\t\t";    // RANG
    for (int i = 0; i < 26; i++ ){
        std::cout << format(rang(lettres[i]));
    }
    std::cout << "\n\n\n\n" ;

    std::cout << "Clé de cryptage\t\t\t\t\t";   // CLE DE CRYPTAGE
    for (int i = 0; i < cleLength; i++){
        std::cout << format<char>(static_cast<char>(La_cle[i]));
    }
    std::cout << std::endl;

    std::cout << "Décalage (par rapport à la lettre A)\t\t\033[46m";    // DECALAGE
    for (int i = 0; i < cleLength; i++){
        std::cout << format(Decalage(i));
    }
    std::cout << "\033[0m\n\n\n";

    std::cout << "Texte à crypter\t\t\t\t\t";   // TEXTE
    for (int i = 0; i < textLength; i++){
        std::cout << format(static_cast<char>(Texte_depart[i]));
    }
    std::cout << std::endl;

    std::cout << "Rang de départ\t\t\t\t\t";    // RANG AVANT
    for (int i = 0; i < textLength; i++ ){
        std::cout << format(rang(Texte_depart[i]));
    }
    std::cout << std::endl;

    std::cout << "Décalage\t\t\t\t\t";  // DECALAGE
    bool topple = false;
    for (int i = 0; i < textLength; i++){
        if (i % cleLength == 0)
            topple = !topple;  
        std::cout << (topple ? "\033[46m" : "\033[41m");
        std::cout << format(Decalage(i));
    }
    std::cout << "\033[0m" << std::endl;

    std::cout << "Rang après décalage (somme)\t\t\t";   // RANG APRES
    // int sum = 0;
    for (int i = 0; i < textLength; i++){
        std::cout << format(rang(Texte_depart[i]) + Decalage(i));
    }
    std::cout << std::endl;

    std::cout << "Rang Final (Total ou Total-26)\t\t\t";    // RANG FINAL
    for (int i = 0; i < textLength; i++){
        std::cout << format(rang(Cryptage(i)));
    }
    std::cout << std::endl;

    std::cout << "Texte crypté (lettre associée au rang final)\t";  // TEXTE CRYPTE
    for (int i = 0; i < textLength; i++){
        std::cout << format(static_cast<char>(Cryptage(i)));
    }
    std::cout << "\n\n";

    std::cout << "\033[1mTexte de départ\033[0m" << std::endl;  // TEXTE DE DEPART
    for (int i = 0; i < textLength; i++){
        std::cout << static_cast<char>(Texte_depart[i]);
    }

    std::cout << "\n\n\033[1m" << "Texte crypté" << "\033[0m\n";    // TEXTE CRYPTE
    for (int i = 0; i < textLength; i++){   
        std::cout << static_cast<char>(Cryptage(i));
    }
    std::cout << std::endl;
}

void Chiffrer::toFile(){
    std::cout << "\n===== Sauvegarde des résultats =====\n";
    std::cout << "Veuillez saisir le chemin complet d'un fichier existant (en local):" << std::endl;
    char path[200];
    scanf("%s", path);
 
    FILE* f = NULL;
    f = fopen(path, "r+");
    if (f == NULL){
        std::cout << "Erreur: ouverture impossible du fichier\n";
        return;
    }
    fseek(f, 0, SEEK_END);
    fputs(" ========================================================= RESULTATS", f);
    fputs(" =========================================================\n", f);
    fputs("Lettre\t\t\t\t\t\t\t\t\t\t\t", f);   // LETTRE
    for (int i = 0; i < 26; i++ ){
        fputs(format(static_cast<char>(lettres[i])).c_str(), f);
    }
    
    fputs("\nRang dans l'alphabet\t\t\t\t\t\t\t", f);  // RANG
    for (int i = 0; i < 26; i++ ){
        fputs(format(rang(lettres[i])).c_str(), f);
    }

    fputs("\n\n\n\nClé de cryptage\t\t\t\t\t\t\t\t\t", f);   // CLE DE CRYPTAGE
    for (int i = 0; i < cleLength; i++){
        fputs(format<char>(static_cast<char>(La_cle[i])).c_str(), f);
    }

    fputs("\nDécalage (par rapport à la lettre A)\t\t\t", f);    // DECALAGE
    for (int i = 0; i < cleLength; i++){
        fputs(format(Decalage(i)).c_str(), f);
    }
  
    fputs("\n\n\nTexte à crypter\t\t\t\t\t\t\t\t\t", f);   // TEXTE
    for (int i = 0; i < textLength; i++){
        fputs(format(static_cast<char>(Texte_depart[i])).c_str(), f);
    }

    fputs("\nRang de départ\t\t\t\t\t\t\t\t\t", f);    // RANG AVANT
    for (int i = 0; i < textLength; i++ ){
        fputs(format(rang(Texte_depart[i])).c_str(), f);
    }

    fputs("\nDécalage\t\t\t\t\t\t\t\t\t\t", f);  // DECALAGE
    for (int i = 0; i < textLength; i++){
        fputs(format(Decalage(i)).c_str(), f);
    }

    fputs("\nRang après décalage (somme)\t\t\t\t\t\t", f);   // RANG APRES
    for (int i = 0; i < textLength; i++){
        fputs(format(rang(Texte_depart[i]) + Decalage(i)).c_str(), f);
    }

    fputs("\nRang Final (Total ou Total-26)\t\t\t\t\t", f);  // RANG FINAL
    for (int i = 0; i < textLength; i++){
        fputs(format(rang(Cryptage(i))).c_str(), f);
    }

    fputs("\nTexte crypté (lettre associée au rang final)\t", f); // TEXTE CRYPTE
    for (int i = 0; i < textLength; i++){
        fputs(format(static_cast<char>(Cryptage(i))).c_str(), f);
    }

    fputs("\n\nTexte de départ\n", f);  // TEXTE DE DEPART
    for (int i = 0; i < textLength; i++){
        fputc(static_cast<char>(Texte_depart[i]), f);
    }

    fputs("\n\nTexte crypté\n", f); // TEXTE CRYPTE
    for (int i = 0; i < textLength; i++){   
        fputc(static_cast<char>(Cryptage(i)), f);
    }
    fputc('\n', f);
    fclose(f);
}