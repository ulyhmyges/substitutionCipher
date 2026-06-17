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
    }
    fclose(f);
    fclose(w);
}

Chiffrer::Chiffrer(bool _cipher) : cipher(_cipher) {
    // construction des lettres et rangs de l'alphabet
    for (int i = 0; i < 26; i++){
        lettres[i] = (char)i + 65;
    }
    buffer = static_cast<char*>(malloc(sizeof(char) * 5));
}

Chiffrer::~Chiffrer() {
    free(buffer);
}

void Chiffrer::Analyse(bool isSaved) {
    analyseSaved = isSaved;
    if (!input())
        return;

    Calcule_clé(); 
    
    output();
}

void Chiffrer::Calcule_clé() {
    if (cipher == true){
        // to do
        // nettoyer la clé de cryptage entrée par l'utilisateur ??
    }
    // Détermine La_cle en mode NON CIPHER
    if (cipher == false){
        bool check = false;
        int index = 1;
        while (!check){
            check = true;
            int x = Cryptage(Texte_depart[0], Texte_cipher[0], TypeCrypt::TEXT); // return cipher key letter
            int j;
            cleLength = 0;
            for (j = index; j < textCipherLength; j++){
                int y = Cryptage(Texte_depart[j], Texte_cipher[j], TypeCrypt::TEXT); // lettre de la clé de cryptage
                if (x == y){
                    break;
                }
            }
            cleLength = j;
            for (int i = 0; i < cleLength; i++){
                La_cle[i] = Cryptage(Texte_depart[i], Texte_cipher[i], TypeCrypt::TEXT);
            }

            // VERIFICATION
            for (int i = 0; i < textCipherLength; i++){
                x = Cryptage(Texte_depart[i], La_cle[i % cleLength]);
                if (x != Texte_cipher[i]){
                    check = false;
                    break;
                }
            }
            index = j + 1;
        }    
    }
}

// Calcule le décalage de la ième lettre du tableau Texte_depart pour i = index  
// letter (int) : lettre à décaler exprimée en code ASCII
// ref (int) : lettre de référence exprimée en code ASCII
char Chiffrer::Decalage(char letter, char ref){
    // la lettre à décaler et la lettre de référence 
    if (letter < 65 || letter > 90 || ref < 65 || ref > 90) {
        std::cout << "\nErreur : argument invalide - ";
        std::cout << "letter: " << letter << ", ref: " <<ref;
        std::cout << " - Méthode : Decalage()\n";
        return -1;
    }
    char x = letter - ref;
    x = x < 0 ? x + 26 : x == 26 ? 26 : x % 26;
    return x;
}

// Pour chaque lettre de Texte_depart, 
// on somme son rang de départ avec le décalage fourni par la clé de cryptage La_cle
// on retourne sa valeur ASCII
char Chiffrer::Cryptage(char letter, char crypt, TypeCrypt type){
    if (letter < 65 || letter > 90) {
        std::cout << "\nErreur : argument invalide\n";
        std::cout << "Méthode : Cryptage()\n";
        return -1;
    }
    char lett = 0;
    if (type == TypeCrypt::KEY){
        lett = Decalage(letter, 90) + Decalage(crypt);
        lett = lett % 26 == 0 ? 26 : lett % 26;
        lett = lett + 64;   // code ASCII à partir du rang de la lettre
    } else {
        // difference des rangs (début et fin) = décalage de la lettre de la clé de cryptage
        lett = Decalage(crypt, 90) - Decalage(letter, 90);
        lett = lett < 0 ? lett + 26 : lett; // si le décalage est non positif, on ajoute 26;
        lett = lett + 65;
    }
 
    return lett;
}

char* Chiffrer::format(char value, TypeFormat type){
    switch (type){
    case TypeFormat::CHAR:
        snprintf(buffer, 4, " %c ", value); 
        break;
    case TypeFormat::DECIMAL:
        if (value < 74)
            snprintf(buffer, 4, " %d ", value);
        else
            snprintf(buffer, 4, " %d", value);  
    }
    return buffer;
}

// On suppose la saisie correcte au clavier, 
// càd sans espace, ni accent et en lettres majuscules uniquement.
// On suppose la taille du texte de départ supérieure ou égale à la taille de la clé de cryptage
// - Texte_depart
// - La_cle
// - Texte_cipher
bool Chiffrer::input() {
    // SAISIR LE TEXTE DE DEPART
    std::cout << "Saisir le texte en clair (Tapez ESC + ENTER pour continuer):\n";
    char c;
    scanf("%c", &c);
    textLength = 0;
    while (c!= EOF && c != 27){
        Texte_depart[textLength] = c;
        scanf("%c", &c);
        textLength++;
    }
    // flush stdin
    scanf("%c", &c);    // '\n' 
    if (cipher) { // SAISIR LA CLE DE CRYPTAGE
        std::cout << "Saisir la clé de cryptage (Tapez ENTER pour continuer):\n";
        scanf("%c", &c);
        cleLength = 0;
        while (c != EOF && c != '\n'){
            La_cle[cleLength] = c;
            scanf("%c", &c);
            cleLength++;
        }
    } else { // SAISIR LE TEXTE CRYPTE
        std::cout << "Saisir le texte chiffré (Tapez ESC + ENTER pour continuer):\n";
        scanf("%c", &c);
        textCipherLength = 0;
        while (c!= EOF && c != 27){
            Texte_cipher[textCipherLength] = c;
            scanf("%c", &c);
            textCipherLength++;
        }
    }

    if ((cipher && (textLength == 0 || cleLength == 0)) 
        || (!cipher && (textCipherLength == 0 || textLength != textCipherLength))){
        std::cout << "Erreur : Saisie invalide\n";
        return false;
    }
    if (!cipher || !analyseSaved )
        return true;
    // MODE CIPHER : on peut sauvegarder l'analyse des résultats
    std::cout << "\nPour la sauvegarde des résultats,\n";
    std::cout << "veuillez saisir le chemin d'un fichier existant (en local):" << std::endl;
    int r = scanf("%s", path);
    if (r != 1){
        std::cout << "Erreur : entrée invalide chemin de fichier\n";
        return false;
    }
    return true;
}

void Chiffrer::output(){
    if (!cipher){
        // AFFICHAGE DANS LA CONSOLE POUR LE MODE NON CIPHER
        std::cout << "\n===== ANALYSE (MODE DECIPHER) =====\n";
        std::cout << "Texte initial : ";
        for (int i = 0; i < textLength; i++){
            std::cout << Texte_depart[i];
        }
        std::cout << "\nTexte crypté : ";
        for (int i = 0; i < textLength; i++){   
            std::cout << Cryptage(Texte_depart[i], La_cle[i % cleLength]);
        }
        std::cout << "\nClé de cryptage : ";
        for (int i = 0; i < cleLength; i++){
            std::cout << La_cle[i];
        }
        std::cout << "\n===== FIN D'ANALYSE =====\n";
        return;
    }
   
    // AFFICHAGE DANS LA CONSOLE POUR LE MODE CIPHER
    std::cout << "\n===== ANALYSE (MODE CIPHER) =====\n";
    std::cout << "Lettre\t\t\t\t\t\t";      // LETTRE
    for (int i = 0; i < 26; i++ ){
        std::cout << format(lettres[i]);
    }
    std::cout << std::endl;

    std::cout << "Rang dans l'alphabet\t\t\t\t";    // RANG
    for (int i = 0; i < 26; i++ ){
        std::cout << format(Decalage(lettres[i], 90), TypeFormat::DECIMAL);
    }
    std::cout << "\n\n\n\n" ;

    std::cout << "Clé de cryptage\t\t\t\t\t";   // CLE DE CRYPTAGE
    for (int i = 0; i < cleLength; i++){
        std::cout << format(La_cle[i]);
    }
    std::cout << std::endl;

    std::cout << "Décalage (par rapport à la lettre A)\t\t\033[46m";    // DECALAGE
    for (int i = 0; i < cleLength; i++){
        std::cout << format(Decalage(La_cle[i]), TypeFormat::DECIMAL);
    }
    std::cout << "\033[0m\n\n\n";

    std::cout << "Texte à crypter\t\t\t\t\t";   // TEXTE
    for (int i = 0; i < textLength; i++){
        std::cout << format(Texte_depart[i]);
    }
    std::cout << std::endl;

    std::cout << "Rang de départ\t\t\t\t\t";    // RANG AVANT
    for (int i = 0; i < textLength; i++ ){
        std::cout << format(Decalage(Texte_depart[i], 90), TypeFormat::DECIMAL);
    }
    std::cout << std::endl;

    std::cout << "Décalage\t\t\t\t\t";  // DECALAGE
    bool topple = false;
    for (int i = 0; i < textLength; i++){
        if (i % cleLength == 0)
            topple = !topple;  
        std::cout << (topple ? "\033[46m" : "\033[41m");
        std::cout << format(Decalage(La_cle[i % cleLength]), TypeFormat::DECIMAL);
    }
    std::cout << "\033[0m" << std::endl;

    std::cout << "Rang après décalage (somme)\t\t\t";   // RANG APRES
    // int sum = 0;
    for (int i = 0; i < textLength; i++){
        std::cout << format(Decalage(Texte_depart[i], 90) + Decalage(La_cle[i % cleLength]), TypeFormat::DECIMAL);
    }
    std::cout << std::endl;

    std::cout << "Rang Final (Total ou Total-26)\t\t\t";    // RANG FINAL
    for (int i = 0; i < textLength; i++){
        std::cout << format(Decalage(Cryptage(Texte_depart[i], La_cle[i % cleLength]), 90), TypeFormat::DECIMAL);
    }
    std::cout << std::endl;

    std::cout << "Texte crypté (lettre associée au rang final)\t";  // TEXTE CRYPTE
    for (int i = 0; i < textLength; i++){
        std::cout << format(Cryptage(Texte_depart[i], La_cle[i % cleLength]));
    }
    std::cout << "\n\n";

    std::cout << "\033[1mTexte de départ\033[0m" << std::endl;  // TEXTE DE DEPART
    for (int i = 0; i < textLength; i++){
        std::cout << Texte_depart[i];
    }

    std::cout << "\n\n\033[1m" << "Texte crypté" << "\033[0m\n";    // TEXTE CRYPTE
    for (int i = 0; i < textLength; i++){   
        std::cout << Cryptage(Texte_depart[i], La_cle[i % cleLength]);
    }
    std::cout << std::endl;

    if (!analyseSaved)
        return;

    FILE* f = NULL;
    f = fopen(path, "r+");  // le fichier doit exister
    if (f == NULL){
        std::cout << "\nErreur: ouverture impossible du fichier ";
        std::cout << path << std::endl;
        std::cout << "\n===== ÉCRITURE DES RÉSULTATS NON EFFECTUÉE =====\n";
        return;
    }
    fseek(f, 0, SEEK_END);  // position curseur à la fin du fichier
    fputs("===== ANALYSE (MODE CIPHER) =====\n", f);
    fputs("Lettre\t\t\t\t\t\t\t\t\t\t\t", f);   // LETTRE
    for (int i = 0; i < 26; i++ ){
        fputs(format(lettres[i]), f);
    }
    fputs("\nRang dans l'alphabet\t\t\t\t\t\t\t", f);  // RANG
    for (int i = 0; i < 26; i++ ){
        fputs(format(Decalage(lettres[i], 90), TypeFormat::DECIMAL), f);
    }
    fputs("\n\n\n\nClé de cryptage\t\t\t\t\t\t\t\t\t", f);   // CLE DE CRYPTAGE
    for (int i = 0; i < cleLength; i++){
        fputs(format(La_cle[i]), f);
    }
    fputs("\nDécalage (par rapport à la lettre A)\t\t\t", f);    // DECALAGE
    for (int i = 0; i < cleLength; i++){
        fputs(format(Decalage(La_cle[i]), TypeFormat::DECIMAL), f);
    }
    fputs("\n\n\nTexte à crypter\t\t\t\t\t\t\t\t\t", f);   // TEXTE
    for (int i = 0; i < textLength; i++){
        fputs(format(Texte_depart[i]), f);
    }
    fputs("\nRang de départ\t\t\t\t\t\t\t\t\t", f);    // RANG AVANT
    for (int i = 0; i < textLength; i++ ){
        fputs(format(Decalage(Texte_depart[i], 90), TypeFormat::DECIMAL), f);
    }
    fputs("\nDécalage\t\t\t\t\t\t\t\t\t\t", f);  // DECALAGE
    for (int i = 0; i < textLength; i++){
        fputs(format(Decalage(La_cle[i % cleLength]), TypeFormat::DECIMAL), f);
    }
    fputs("\nRang après décalage (somme)\t\t\t\t\t\t", f);   // RANG APRES
    for (int i = 0; i < textLength; i++){
        fputs(format(Decalage(Texte_depart[i], 90) + Decalage(La_cle[i % cleLength]), TypeFormat::DECIMAL), f);
    }
    fputs("\nRang Final (Total ou Total-26)\t\t\t\t\t", f);  // RANG FINAL
    for (int i = 0; i < textLength; i++){
        fputs(format(Decalage(Cryptage(Texte_depart[i], La_cle[i % cleLength]), 90), TypeFormat::DECIMAL), f);
    }
    fputs("\nTexte crypté (lettre associée au rang final)\t", f); // TEXTE CRYPTE
    for (int i = 0; i < textLength; i++){
        fputs(format(Cryptage(Texte_depart[i], La_cle[i % cleLength])), f);
    }
    fputs("\n\nTexte de départ\n", f);  // TEXTE DE DEPART
    for (int i = 0; i < textLength; i++){
        fputc(Texte_depart[i], f);
    }
    fputs("\n\nTexte crypté\n", f); // TEXTE CRYPTE
    for (int i = 0; i < textLength; i++){   
        fputc(Cryptage(Texte_depart[i], La_cle[i % cleLength]), f);
    }
    fputs("\n===== FIN D'ANALYSE =====\n", f);
    fclose(f);
    std::cout << "\n===== ÉCRITURE DES RÉSULTATS EFFECTUÉE =====\n";
    std::cout << "fichier de sortie : " << path << std::endl;
}