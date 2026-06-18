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
        std::cout << "\nErreur à l'ouverture du ficher\n";
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

        if (c == 32 || c == '.' || c == ',' || c == '\n' || c == '"' || c == '?' || c == '!' || c == 39)
            continue;

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

Chiffrer::Chiffrer(bool _dechiffre) : déchiffre(_dechiffre) {
    // construction des lettres et rangs de l'alphabet
    for (int i = 0; i < 26; i++){
        lettres[i] = (char)i + 65;
    }
    tampon = static_cast<char*>(malloc(sizeof(char) * 5));
}

Chiffrer::~Chiffrer() {
    free(tampon);
}

void Chiffrer::Analyse(bool isSaved) {
    analyseSauvegardée = isSaved;
    if (!saisie())
        return;

    Calcule_clé(); 
    
    sortie();
}

void Chiffrer::Calcule_clé() {
    if (déchiffre == false){
        // to do
        // nettoyer la clé de cryptage entrée par l'utilisateur ??
    }
    // Détermine La_cle en mode déchiffrement
    if (déchiffre == true){
        bool check = false;
        int index = 1;
        while (!check){
            check = true;
            int x = Cryptage(Texte_depart[0], Texte_crypté[0], TypeCrypt::TEXTE); // return cipher key letter
            int j;
            longueurClé = 0;
            for (j = index; j < longueurTexteCrypté; j++){
                int y = Cryptage(Texte_depart[j], Texte_crypté[j], TypeCrypt::TEXTE); // lettre de la clé de cryptage
                if (x == y){
                    break;
                }
            }
            longueurClé = j;
            for (int i = 0; i < longueurClé; i++){
                La_cle[i] = Cryptage(Texte_depart[i], Texte_crypté[i], TypeCrypt::TEXTE);
            }

            // VERIFICATION
            for (int i = 0; i < longueurTexteCrypté; i++){
                x = Cryptage(Texte_depart[i], La_cle[i % longueurClé]);
                if (x != Texte_crypté[i]){
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
char Chiffrer::Décalage(char lettre, char ref){
    // la lettre à décaler et la lettre de référence 
    if (lettre < 65 || lettre > 90 || ref < 65 || ref > 90) {
        std::cout << "\nErreur : argument invalide - ";
        std::cout << "letter: " << lettre << ", ref: " <<ref;
        std::cout << " - Méthode : Décalage()\n";
        return -1;
    }
    char x = lettre - ref;
    x = x < 0 ? x + 26 : x == 26 ? 26 : x % 26;
    // rang
    if (ref == 90 && x == 0)
        x = 26;
    return x;
}

// Pour chaque lettre de Texte_depart, 
// on somme son rang de départ avec le décalage fourni par la clé de cryptage La_cle
// on retourne sa valeur ASCII
char Chiffrer::Cryptage(char lettre, char crypt, TypeCrypt type){
    if (lettre < 65 || lettre > 90 || crypt < 65 || crypt > 90) {
        std::cout << "\nErreur : argument invalide\n";
        std::cout << "Méthode : Cryptage()\n";
        return -1;
    }
    char lett = 0;
    if (type == TypeCrypt::CLE){
        lett = Décalage(lettre, 90) + Décalage(crypt);
        lett = lett % 26 == 0 ? 26 : lett % 26;
        lett = lett + 64;   // code ASCII à partir du rang de la lettre
    } else {
        // difference des rangs (début et fin) = décalage de la lettre de la clé de cryptage
        lett = Décalage(crypt, 90) - Décalage(lettre, 90);
        lett = lett < 0 ? lett + 26 : lett; // si le décalage est non positif, on ajoute 26;
        lett = lett + 65;
    }
 
    return lett;
}

char* Chiffrer::format(char valeur, TypeFormat type){
    switch (type){
    case TypeFormat::CHAR:
        snprintf(tampon, 4, " %c ", valeur); 
        break;
    case TypeFormat::DECIMAL:
        if (valeur < 10)
            snprintf(tampon, 4, " %d ", valeur);
        else
            snprintf(tampon, 4, " %d", valeur);  
    }
    return tampon;
}

// On suppose la saisie correcte au clavier, 
// càd sans espace, ni accent et en lettres majuscules uniquement.
// On suppose la taille du texte de départ supérieure ou égale à la taille de la clé de cryptage
// - Texte_depart
// - La_cle
// - Texte_cipher
bool Chiffrer::saisie() {
    // SAISIR LE TEXTE DE DEPART
    std::cout << "Saisir le texte en clair (Tapez ESC + ENTER pour continuer):\n";
    char c;
    scanf("%c", &c);
    longueurTexteDepart = 0;
    while (c!= EOF && c != 27){
        Texte_depart[longueurTexteDepart] = c;
        scanf("%c", &c);
        longueurTexteDepart++;
    }
    // flush stdin
    scanf("%c", &c);    // '\n' 
    if (!déchiffre) { // SAISIR LA CLE DE CRYPTAGE
        std::cout << "Saisir la clé de cryptage (Tapez ENTER pour continuer):\n";
        scanf("%c", &c);
        longueurClé = 0;
        while (c != EOF && c != '\n'){
            La_cle[longueurClé] = c;
            scanf("%c", &c);
            longueurClé++;
        }
        
    } else { // SAISIR LE TEXTE CRYPTE
        std::cout << "Saisir le texte chiffré (Tapez ESC + ENTER pour continuer):\n";
        scanf("%c", &c);
        longueurTexteCrypté = 0;
        while (c!= EOF && c != 27){
            Texte_crypté[longueurTexteCrypté] = c;
            scanf("%c", &c);
            longueurTexteCrypté++;
        }
    }

    if ((!déchiffre && (longueurTexteDepart == 0 || longueurClé == 0)) 
        || (déchiffre && (longueurTexteCrypté == 0 || longueurTexteDepart != longueurTexteCrypté))){
        std::cout << "Erreur : Saisie invalide\n";
        return false;
    }
    if (déchiffre || !analyseSauvegardée )
        return true;
    // EN MODE CHIFFREMENT : on peut sauvegarder l'analyse des résultats
    std::cout << "\nPour la sauvegarde des résultats,\n";
    std::cout << "veuillez saisir le chemin d'un fichier existant (en local):" << std::endl;
    int r = scanf("%s", chemin);
    if (r != 1){
        std::cout << "Erreur : entrée invalide chemin de fichier\n";
        return false;
    }
    return true;
}

void Chiffrer::sortie(){
    if (déchiffre){
        // AFFICHAGE DANS LA CONSOLE POUR LE MODE NON CIPHER
        std::cout << "\n===== ANALYSE (MODE DECHIFFREMENT) =====\n";
        std::cout << "Texte initial : ";
        for (int i = 0; i < longueurTexteDepart; i++){
            std::cout << Texte_depart[i];
        }
        std::cout << "\nTexte crypté : ";
        for (int i = 0; i < longueurTexteDepart; i++){   
            std::cout << Cryptage(Texte_depart[i], La_cle[i % longueurClé]);
        }
        std::cout << "\nClé de cryptage : ";
        for (int i = 0; i < longueurClé; i++){
            std::cout << La_cle[i];
        }
        std::cout << "\n===== FIN D'ANALYSE =====\n";
        return;
    }
   
    // AFFICHAGE DANS LA CONSOLE POUR LE MODE CIPHER
    std::cout << "\n===== ANALYSE (MODE CHIFFREMENT) =====\n";
    std::cout << "Lettre\t\t\t\t\t\t";      // LETTRE
    for (int i = 0; i < 26; i++ ){
        std::cout << format(lettres[i]);
    }
    std::cout << std::endl;

    std::cout << "Rang dans l'alphabet\t\t\t\t";    // RANG
    for (int i = 0; i < 26; i++ ){
        std::cout << format(Décalage(lettres[i], 90), TypeFormat::DECIMAL);
    }
    std::cout << "\n\n\n\n" ;

    std::cout << "Clé de cryptage\t\t\t\t\t";   // CLE DE CRYPTAGE
    for (int i = 0; i < longueurClé; i++){
        std::cout << format(La_cle[i]);
    }
    std::cout << std::endl;

    std::cout << "Décalage (par rapport à la lettre A)\t\t\033[46m";    // Décalage
    for (int i = 0; i < longueurClé; i++){
        std::cout << format(Décalage(La_cle[i]), TypeFormat::DECIMAL);
    }
    std::cout << "\033[0m\n\n\n";

    std::cout << "Texte à crypter\t\t\t\t\t";   // TEXTE
    for (int i = 0; i < longueurTexteDepart; i++){
        std::cout << format(Texte_depart[i]);
    }
    std::cout << std::endl;

    std::cout << "Rang de départ\t\t\t\t\t";    // RANG AVANT
    for (int i = 0; i < longueurTexteDepart; i++ ){
        std::cout << format(Décalage(Texte_depart[i], 90), TypeFormat::DECIMAL);
    }
    std::cout << std::endl;

    std::cout << "Décalage\t\t\t\t\t";  // Décalage
    bool bascule = false;
    for (int i = 0; i < longueurTexteDepart; i++){
        if (i % longueurClé == 0)
            bascule = !bascule;  
        std::cout << (bascule ? "\033[46m" : "\033[41m");
        std::cout << format(Décalage(La_cle[i % longueurClé]), TypeFormat::DECIMAL);
    }
    std::cout << "\033[0m" << std::endl;

    std::cout << "Rang après décalage (somme)\t\t\t";   // RANG APRES
    // int sum = 0;
    for (int i = 0; i < longueurTexteDepart; i++){
        std::cout << format(Décalage(Texte_depart[i], 90) + Décalage(La_cle[i % longueurClé]), TypeFormat::DECIMAL);
    }
    std::cout << std::endl;

    std::cout << "Rang Final (Total ou Total-26)\t\t\t";    // RANG FINAL
    for (int i = 0; i < longueurTexteDepart; i++){
        std::cout << format(Décalage(Cryptage(Texte_depart[i], La_cle[i % longueurClé]), 90), TypeFormat::DECIMAL);
    }
    std::cout << std::endl;

    std::cout << "Texte crypté (lettre associée au rang final)\t";  // TEXTE CRYPTE
    for (int i = 0; i < longueurTexteDepart; i++){
        std::cout << format(Cryptage(Texte_depart[i], La_cle[i % longueurClé]));
    }
    std::cout << "\n\n";

    std::cout << "\033[1mTexte de départ\033[0m" << std::endl;  // TEXTE DE DEPART
    for (int i = 0; i < longueurTexteDepart; i++){
        std::cout << Texte_depart[i];
    }

    std::cout << "\n\n\033[1m" << "Texte crypté" << "\033[0m\n";    // TEXTE CRYPTE
    for (int i = 0; i < longueurTexteDepart; i++){   
        std::cout << Cryptage(Texte_depart[i], La_cle[i % longueurClé]);
    }
    std::cout << std::endl;

    if (!analyseSauvegardée)
        return;

    FILE* f = NULL;
    f = fopen(chemin, "r+");  // le fichier doit exister
    if (f == NULL){
        std::cout << "\nErreur: ouverture impossible du fichier ";
        std::cout << chemin << std::endl;
        std::cout << "\n===== ÉCRITURE DES RÉSULTATS NON EFFECTUÉE =====\n";
        return;
    }
    fseek(f, 0, SEEK_END);  // position curseur à la fin du fichier
    fputs("===== ANALYSE (MODE CHIFFREMENT) =====\n", f);
    fputs("Lettre\t\t\t\t\t\t\t\t\t\t\t", f);   // LETTRE
    for (int i = 0; i < 26; i++ ){
        fputs(format(lettres[i]), f);
    }
    fputs("\nRang dans l'alphabet\t\t\t\t\t\t\t", f);  // RANG
    for (int i = 0; i < 26; i++ ){
        fputs(format(Décalage(lettres[i], 90), TypeFormat::DECIMAL), f);
    }
    fputs("\n\n\n\nClé de cryptage\t\t\t\t\t\t\t\t\t", f);   // CLE DE CRYPTAGE
    for (int i = 0; i < longueurClé; i++){
        fputs(format(La_cle[i]), f);
    }
    fputs("\nDécalage (par rapport à la lettre A)\t\t\t", f);    // Décalage
    for (int i = 0; i < longueurClé; i++){
        fputs(format(Décalage(La_cle[i]), TypeFormat::DECIMAL), f);
    }
    fputs("\n\n\nTexte à crypter\t\t\t\t\t\t\t\t\t", f);   // TEXTE
    for (int i = 0; i < longueurTexteDepart; i++){
        fputs(format(Texte_depart[i]), f);
    }
    fputs("\nRang de départ\t\t\t\t\t\t\t\t\t", f);    // RANG AVANT
    for (int i = 0; i < longueurTexteDepart; i++ ){
        fputs(format(Décalage(Texte_depart[i], 90), TypeFormat::DECIMAL), f);
    }
    fputs("\nDécalage\t\t\t\t\t\t\t\t\t\t", f);  // Décalage
    for (int i = 0; i < longueurTexteDepart; i++){
        fputs(format(Décalage(La_cle[i % longueurClé]), TypeFormat::DECIMAL), f);
    }
    fputs("\nRang après décalage (somme)\t\t\t\t\t\t", f);   // RANG APRES
    for (int i = 0; i < longueurTexteDepart; i++){
        fputs(format(Décalage(Texte_depart[i], 90) + Décalage(La_cle[i % longueurClé]), TypeFormat::DECIMAL), f);
    }
    fputs("\nRang Final (Total ou Total-26)\t\t\t\t\t", f);  // RANG FINAL
    for (int i = 0; i < longueurTexteDepart; i++){
        fputs(format(Décalage(Cryptage(Texte_depart[i], La_cle[i % longueurClé]), 90), TypeFormat::DECIMAL), f);
    }
    fputs("\nTexte crypté (lettre associée au rang final)\t", f); // TEXTE CRYPTE
    for (int i = 0; i < longueurTexteDepart; i++){
        fputs(format(Cryptage(Texte_depart[i], La_cle[i % longueurClé])), f);
    }
    fputs("\n\nTexte de départ\n", f);  // TEXTE DE DEPART
    for (int i = 0; i < longueurTexteDepart; i++){
        fputc(Texte_depart[i], f);
    }
    fputs("\n\nTexte crypté\n", f); // TEXTE CRYPTE
    for (int i = 0; i < longueurTexteDepart; i++){   
        fputc(Cryptage(Texte_depart[i], La_cle[i % longueurClé]), f);
    }
    fputs("\n===== FIN D'ANALYSE =====\n", f);
    fclose(f);
    std::cout << "\n===== ÉCRITURE DES RÉSULTATS EFFECTUÉE =====\n";
    std::cout << "fichier de sortie : " << chemin << std::endl;
}