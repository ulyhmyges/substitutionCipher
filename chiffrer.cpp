#include "chiffrer.h"
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

Chiffrer::Chiffrer(bool _decipher) : decipher(_decipher), error(false) {
    // construction des lettres et rangs de l'alphabet
    for (int i = 0; i < 26; i++){
        lettres[i] = i + 65;
        rangs[i] = i + 1;
    }
}

void Chiffrer::Analyze(bool isSaved){
    input(isSaved);
    if (error)
        return;

    if (decipher == false){
        // to do
        // nettoyer la clé de cryptage entrée par l'utilisateur ??
    }
    // Détermine La_cle en mode DECIPHER = true
    // différence
    if (decipher == true){
        bool check = false;
        int index = 1;
        while (!check){
            check = true;
            int x = Cryptage(0, true); // lettre cle
            int j;
            cleLength = 0;
            for (j = index; j < textCipherLength; j++){
                int y = Cryptage(j, true); // lettre de la clé de cryptage
                if (x == y){
                    break;
                }
            }
            cleLength = j;
            for (int i = 0; i < cleLength; i++){
                La_cle[i] = Cryptage(i, true);
            }

            // VERIFICATION
            // vérification 
            for (int i = 0; i < textCipherLength; i++){
                x = Cryptage(i);
                int y = Texte_cipher[i];
                if (x != y){
                    // std::cout << "Erreur : Les textes (en clair et crypté) ne correspondent pas.\n\n\n";
                    check = false;
                    break;
                }
            }
            index = j + 1;
        }    
    }
   
    output(isSaved);
}

// Calcule le décalage de la ième lettre du tableau Texte_depart pour i = index  
// letter (int) : lettre à décaler exprimée en code ASCII
// ref (int) : lettre de référence exprimée en code ASCII
int Chiffrer::Decalage(int letter, int ref){
    // la lettre à décaler et la lettre de référence 
    if (letter < 65 || letter > 90 || ref < 65 || ref > 90) {
        std::cout << "\nErreur : argument invalide - ";
        std::cout << "letter: " << letter << ", ref: " <<ref;
        std::cout << " - Méthode : Decalage()\n";
        return -1;
    }
    // return  La_cle[index % cleLength] - 65 ;
    int x = letter - ref;
    x = x < 0 ? x + 26 : x % 26 == 0 ? 26 : x % 26;
    return x;
}

// Pour chaque lettre de Texte_depart, 
// on somme son rang de départ avec le décalage fourni par la clé de cryptage La_cle
// on retourne sa valeur ASCII
int Chiffrer::Cryptage(int i, bool key){
    if (i < 0 || i >= textLength) {
        std::cout << "\nErreur : argument invalide\n";
        std::cout << "Méthode : Cryptage()\n";
        return -1;
    }
    int letter = 0;
    if (key){
        // difference des rangs (début et fin) = décalage de la lettre de la clé de cryptage
        letter = Decalage(Texte_cipher[i], 90) - Decalage(Texte_depart[i], 90);
        letter = letter < 0 ? letter + 26 : letter; // si le décalage est non positif, on ajoute 26;
        letter = letter + 65;
    } else {
        letter = Decalage(Texte_depart[i], 90) + Decalage(La_cle[i % cleLength]);
        letter = letter % 26 == 0 ? 26 : letter % 26;
        letter = letter + 64;   // code ASCII à partir du rang de la lettre
    }
 
    return letter;
}

// Détermine le rang d'une lettre à partir de son code ASCII
// int Chiffrer::rang(int letter){
//     return letter - 64;
// }

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

// On suppose la saisie correcte au clavier, 
// càd sans espace, ni accent et en lettres majuscules uniquement.
// On suppose la taille du texte de départ supérieure ou égale à la taille de la clé de cryptage
// - Texte_depart
// - La_cle
// - Texte_cipher
void Chiffrer::input(bool isSaved) {
    // SAISIR LE TEXTE DE DEPART
    std::cout << "Saisir le texte en clair (Taper ';;' pour signaler la fin du texte):\n";
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
    if (decipher == false) {
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
        textCipherLength = 0;
        last = 0;
        while (c!= EOF && !(c == ';' && last == ';')){
            last = c;
            Texte_cipher[textCipherLength] = c;
            scanf("%c", &c);
            textCipherLength++;
        }
        if (textCipherLength == 0) {
            std::cout << "Erreur : Text chiffré invalide\n";
            return;
        }
        // La suite de caractères ";;\n" marque la séparation entre le texte et la clé
        textCipherLength--;   // ';'
        if (textCipherLength > 0 && Texte_cipher[textCipherLength - 1] == '\n'){ // '\n'
            textCipherLength--;
        }
        if (textCipherLength != textLength) {
            std::cout << "Erreur : Text chiffré invalide\n";
            error = true;
            return;
        }
        // flush
        scanf("%c", &c);    // '\n'
    }
        
    

    if (decipher || !isSaved )
        return;
    
    std::cout << "Pour la sauvegarde des résultats,\n";
    std::cout << "veuillez saisir le chemin complet d'un fichier existant (en local):" << std::endl;
    int r = scanf("%s", path);
    if (r != 1){
        std::cout << "Erreur : entrée invalide chemin de fichier\n";
        error = true;
        return;
    }
}

void Chiffrer::output(bool isSaved){
    // MODE DECIPHER = true
    if (decipher){
        // AFFICHE DANS LA CONSOLE
        std::cout << "\n===== ANALYSE (MODE DECIPHER) =====\n";
        std::cout << "Texte initial : ";
        for (int i = 0; i < textLength; i++){
            std::cout << static_cast<char>(Texte_depart[i]);
        }
        std::cout << "\nTexte crypté : ";
        for (int i = 0; i < textLength; i++){   
            std::cout << static_cast<char>(Cryptage(i));
        }
        std::cout << "\nClé de cryptage : ";
        for (int i = 0; i < cleLength; i++){
            std::cout << static_cast<char>(La_cle[i]);
        }
        std::cout << std::endl;
        return;
    }
   
    // AFFICHE DANS LA CONSOLE
    std::cout << "\n===== ANALYSE (MODE CIPHER) =====\n";
    std::cout << "Lettre\t\t\t\t\t\t";      // LETTRE
    for (int i = 0; i < 26; i++ ){
        std::cout << format(static_cast<char>(lettres[i]));
    }
    std::cout << std::endl;

    std::cout << "Rang dans l'alphabet\t\t\t\t";    // RANG
    for (int i = 0; i < 26; i++ ){
        std::cout << format(Decalage(lettres[i], 90));
    }
    std::cout << "\n\n\n\n" ;

    std::cout << "Clé de cryptage\t\t\t\t\t";   // CLE DE CRYPTAGE
    for (int i = 0; i < cleLength; i++){
        std::cout << format<char>(static_cast<char>(La_cle[i]));
    }
    std::cout << std::endl;

    std::cout << "Décalage (par rapport à la lettre A)\t\t\033[46m";    // DECALAGE
    for (int i = 0; i < cleLength; i++){
        std::cout << format(Decalage(La_cle[i]));
    }
    std::cout << "\033[0m\n\n\n";

    std::cout << "Texte à crypter\t\t\t\t\t";   // TEXTE
    for (int i = 0; i < textLength; i++){
        std::cout << format(static_cast<char>(Texte_depart[i]));
    }
    std::cout << std::endl;

    std::cout << "Rang de départ\t\t\t\t\t";    // RANG AVANT
    for (int i = 0; i < textLength; i++ ){
        std::cout << format(Decalage(Texte_depart[i], 90));
    }
    std::cout << std::endl;

    std::cout << "Décalage\t\t\t\t\t";  // DECALAGE
    bool topple = false;
    for (int i = 0; i < textLength; i++){
        if (i % cleLength == 0)
            topple = !topple;  
        std::cout << (topple ? "\033[46m" : "\033[41m");
        std::cout << format(Decalage(La_cle[i % cleLength]));
    }
    std::cout << "\033[0m" << std::endl;

    std::cout << "Rang après décalage (somme)\t\t\t";   // RANG APRES
    // int sum = 0;
    for (int i = 0; i < textLength; i++){
        std::cout << format(Decalage(Texte_depart[i], 90) + Decalage(La_cle[i % cleLength]));
    }
    std::cout << std::endl;

    std::cout << "Rang Final (Total ou Total-26)\t\t\t";    // RANG FINAL
    for (int i = 0; i < textLength; i++){
        std::cout << format(Decalage(Cryptage(i), 90));
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

    if (!isSaved)
        return;

    FILE* f = NULL;
    f = fopen(path, "r+");  // le fichier doit exister
    if (f == NULL){
        std::cout << "\nErreur: ouverture impossible du fichier ";
        std::cout << path << std::endl;
        return;
    }
    fseek(f, 0, SEEK_END);  // position curseur à la fin du fichier
    fputs("ANALYSE (MODE CIPHER) =====\n", f);
    fputs("Lettre\t\t\t\t\t\t\t\t\t\t\t", f);   // LETTRE
    for (int i = 0; i < 26; i++ ){
        fputs(format(static_cast<char>(lettres[i])).c_str(), f);
    }
    fputs("\nRang dans l'alphabet\t\t\t\t\t\t\t", f);  // RANG
    for (int i = 0; i < 26; i++ ){
        fputs(format(Decalage(lettres[i], 90)).c_str(), f);
    }
    fputs("\n\n\n\nClé de cryptage\t\t\t\t\t\t\t\t\t", f);   // CLE DE CRYPTAGE
    for (int i = 0; i < cleLength; i++){
        fputs(format<char>(static_cast<char>(La_cle[i])).c_str(), f);
    }
    fputs("\nDécalage (par rapport à la lettre A)\t\t\t", f);    // DECALAGE
    for (int i = 0; i < cleLength; i++){
        fputs(format(Decalage(La_cle[i])).c_str(), f);
    }
    fputs("\n\n\nTexte à crypter\t\t\t\t\t\t\t\t\t", f);   // TEXTE
    for (int i = 0; i < textLength; i++){
        fputs(format(static_cast<char>(Texte_depart[i])).c_str(), f);
    }
    fputs("\nRang de départ\t\t\t\t\t\t\t\t\t", f);    // RANG AVANT
    for (int i = 0; i < textLength; i++ ){
        fputs(format(Decalage(Texte_depart[i], 90)).c_str(), f);
    }
    fputs("\nDécalage\t\t\t\t\t\t\t\t\t\t", f);  // DECALAGE
    for (int i = 0; i < textLength; i++){
        fputs(format(Decalage(La_cle[i % cleLength])).c_str(), f);
    }
    fputs("\nRang après décalage (somme)\t\t\t\t\t\t", f);   // RANG APRES
    for (int i = 0; i < textLength; i++){
        fputs(format(Decalage(Texte_depart[i], 90) + Decalage(La_cle[i % cleLength])).c_str(), f);
    }
    fputs("\nRang Final (Total ou Total-26)\t\t\t\t\t", f);  // RANG FINAL
    for (int i = 0; i < textLength; i++){
        fputs(format(Decalage(Cryptage(i), 90)).c_str(), f);
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
    std::cout << "\n===== Résultats sauvegardées =====\n";
    std::cout << "fichier : " << path << std::endl;
}