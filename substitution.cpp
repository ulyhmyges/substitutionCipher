#include "substitution.h"
// #include <cstdio>
#include <iostream>

Substitution::Substitution(Mode _mode) : mode(_mode) {
    // construction des lettres et rangs de l'alphabet
    for (int i = 0; i < 26; i++){
        lettres[i] = (char)i + 65;
    }
    tampon = static_cast<char*>(malloc(sizeof(char) * 5));
}

Substitution::~Substitution() {
    free(tampon);
}

void Substitution::Analyse(bool isSaved) {
    analyseSauvegardee = isSaved;
    if (!saisie())
        return;

    Calcule_clé(); 
    
    sortie();
}

void Substitution::Calcule_clé() {
    if (mode == Mode::CHIFFRE){
        // to do
        // nettoyer la clé de cryptage entrée par l'utilisateur ??
    }
    // Détermine La_cle en mode déchiffre
    if (mode == Mode::DECHIFFRE){
        bool check = false;
        int index = 1;
        while (!check){
            check = true;
            int x = Cryptage(Texte_depart[0], Texte_crypté[0], mode); // return cipher key letter
            int j;
            longueurCle = 0;
            for (j = index; j < longueurTexteCrypté; j++){
                int y = Cryptage(Texte_depart[j], Texte_crypté[j], mode); // lettre de la clé de cryptage
                if (x == y){
                    break;
                }
            }
            longueurCle = j;
            for (int i = 0; i < longueurCle; i++){
                La_cle[i] = Cryptage(Texte_depart[i], Texte_crypté[i], mode);
            }

            // VERIFICATION
            for (int i = 0; i < longueurTexteCrypté; i++){
                x = Cryptage(Texte_depart[i], La_cle[i % longueurCle]);
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
char Substitution::Décalage(char lettre, char ref){
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
char Substitution::Cryptage(char lettre, char crypt, Mode m){
    if (lettre < 65 || lettre > 90 || crypt < 65 || crypt > 90) {
        std::cout << "\nErreur : argument invalide\n";
        std::cout << "Méthode : Cryptage()\n";
        return -1;
    }
    char c = 0;
    switch (m){
    case Mode::DECHIFFRE:
        // difference des rangs (début et fin) = décalage de la lettre de la clé de cryptage
        c = Décalage(crypt, 90) - Décalage(lettre, 90);
        c = c < 0 ? c + 26 : c; // si le décalage est non positif, on ajoute 26;
        c = c + 65;
        break;
    default: // CHIFFRE
        c = Décalage(lettre, 90) + Décalage(crypt);
        c = c % 26 == 0 ? 26 : c % 26;
        c = c + 64;   // code ASCII à partir du rang de la lettre
    }
    return c;
}

char* Substitution::format(char valeur, TypeFormat type){
    switch (type){
    case TypeFormat::DECIMAL:
        if (valeur < 10)
            snprintf(tampon, 4, " %d ", valeur);
        else
            snprintf(tampon, 4, " %d", valeur);  
        break;
    default: // CHAR
        snprintf(tampon, 4, " %c ", valeur); 
    }
    return tampon;
}

// On suppose la saisie correcte au clavier, 
// càd sans espace, ni accent et en lettres majuscules uniquement.
// On suppose la taille du texte de départ supérieure ou égale à la taille de la clé de cryptage
// - Texte_depart
// - La_cle
// - Texte_cipher
bool Substitution::saisie() {
    bool réussi = true;
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
    switch (mode){
    case Mode::DECHIFFRE:
        std::cout << "Saisir le texte chiffré (Tapez ESC + ENTER pour continuer):\n";
        scanf("%c", &c);
        longueurTexteCrypté = 0;
        while (c!= EOF && c != 27){
            Texte_crypté[longueurTexteCrypté] = c;
            scanf("%c", &c);
            longueurTexteCrypté++;
        }
        if (longueurTexteCrypté == 0 || longueurTexteDepart != longueurTexteCrypté){
            std::cout << "Erreur : Saisie invalide\n";
            réussi = false;
        }
        break;

    default: // CHIFFRE
        std::cout << "Saisir la clé de cryptage (Tapez ENTER pour continuer):\n";
        scanf("%c", &c);
        longueurCle = 0;
        while (c != EOF && c != '\n'){
            La_cle[longueurCle] = c;
            scanf("%c", &c);
            longueurCle++;
        }
        if (longueurTexteDepart == 0 || longueurCle == 0){
            std::cout << "Erreur : Saisie invalide\n";
            réussi = false;
        }
        if (!analyseSauvegardee) return réussi;
        std::cout << "\nPour la sauvegarde des résultats,\n";
        std::cout << "veuillez saisir le chemin d'un fichier existant (en local):" << std::endl;
        int r = scanf("%s", chemin);
        if (r != 1){
            std::cout << "Erreur : entrée invalide chemin de fichier\n";
            réussi = false;
        }
    }
    return réussi;
}

void Substitution::sortie(){
    if (mode == Mode::DECHIFFRE){ // AFFICHAGE DANS LA CONSOLE
        std::cout << "\n===== ANALYSE (MODE DECHIFFRE) =====\n";
        std::cout << "Texte initial : ";
        for (int i = 0; i < longueurTexteDepart; i++){
            std::cout << Texte_depart[i];
        }
        std::cout << "\nTexte crypté : ";
        for (int i = 0; i < longueurTexteDepart; i++){   
            std::cout << Cryptage(Texte_depart[i], La_cle[i % longueurCle]);
        }
        std::cout << "\nClé de cryptage : ";
        for (int i = 0; i < longueurCle; i++){
            std::cout << La_cle[i];
        }
        std::cout << "\n===== FIN D'ANALYSE =====\n";
        return;
    }
   
    // AFFICHAGE MODE CHIFFRE
    std::cout << "\n===== ANALYSE (MODE CHIFFRE) =====\n";
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
    for (int i = 0; i < longueurCle; i++){
        std::cout << format(La_cle[i]);
    }
    std::cout << std::endl;

    std::cout << "Décalage (par rapport à la lettre A)\t\t\033[46m";    // Décalage
    for (int i = 0; i < longueurCle; i++){
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
        if (i % longueurCle == 0)
            bascule = !bascule;  
        std::cout << (bascule ? "\033[46m" : "\033[41m");
        std::cout << format(Décalage(La_cle[i % longueurCle]), TypeFormat::DECIMAL);
    }
    std::cout << "\033[0m" << std::endl;

    std::cout << "Rang après décalage (somme)\t\t\t";   // RANG APRES
    // int sum = 0;
    for (int i = 0; i < longueurTexteDepart; i++){
        std::cout << format(Décalage(Texte_depart[i], 90) + Décalage(La_cle[i % longueurCle]), TypeFormat::DECIMAL);
    }
    std::cout << std::endl;

    std::cout << "Rang Final (Total ou Total-26)\t\t\t";    // RANG FINAL
    for (int i = 0; i < longueurTexteDepart; i++){
        std::cout << format(Décalage(Cryptage(Texte_depart[i], La_cle[i % longueurCle]), 90), TypeFormat::DECIMAL);
    }
    std::cout << std::endl;

    std::cout << "Texte crypté (lettre associée au rang final)\t";  // TEXTE CRYPTE
    for (int i = 0; i < longueurTexteDepart; i++){
        std::cout << format(Cryptage(Texte_depart[i], La_cle[i % longueurCle]));
    }
    std::cout << "\n\n";

    std::cout << "\033[1mTexte de départ\033[0m" << std::endl;  // TEXTE DE DEPART
    for (int i = 0; i < longueurTexteDepart; i++){
        std::cout << Texte_depart[i];
    }

    std::cout << "\n\n\033[1m" << "Texte crypté" << "\033[0m\n";    // TEXTE CRYPTE
    for (int i = 0; i < longueurTexteDepart; i++){   
        std::cout << Cryptage(Texte_depart[i], La_cle[i % longueurCle]);
    }
    std::cout << std::endl;

    if (!analyseSauvegardee)
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
    fputs("===== ANALYSE (CHIFFREMENT) =====\n", f);
    fputs("Lettre\t\t\t\t\t\t\t\t\t\t\t", f);   // LETTRE
    for (int i = 0; i < 26; i++ ){
        fputs(format(lettres[i]), f);
    }
    fputs("\nRang dans l'alphabet\t\t\t\t\t\t\t", f);  // RANG
    for (int i = 0; i < 26; i++ ){
        fputs(format(Décalage(lettres[i], 90), TypeFormat::DECIMAL), f);
    }
    fputs("\n\n\n\nClé de cryptage\t\t\t\t\t\t\t\t\t", f);   // CLE DE CRYPTAGE
    for (int i = 0; i < longueurCle; i++){
        fputs(format(La_cle[i]), f);
    }
    fputs("\nDécalage (par rapport à la lettre A)\t\t\t", f);    // Décalage
    for (int i = 0; i < longueurCle; i++){
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
        fputs(format(Décalage(La_cle[i % longueurCle]), TypeFormat::DECIMAL), f);
    }
    fputs("\nRang après décalage (somme)\t\t\t\t\t\t", f);   // RANG APRES
    for (int i = 0; i < longueurTexteDepart; i++){
        fputs(format(Décalage(Texte_depart[i], 90) + Décalage(La_cle[i % longueurCle]), TypeFormat::DECIMAL), f);
    }
    fputs("\nRang Final (Total ou Total-26)\t\t\t\t\t", f);  // RANG FINAL
    for (int i = 0; i < longueurTexteDepart; i++){
        fputs(format(Décalage(Cryptage(Texte_depart[i], La_cle[i % longueurCle]), 90), TypeFormat::DECIMAL), f);
    }
    fputs("\nTexte crypté (lettre associée au rang final)\t", f); // TEXTE CRYPTE
    for (int i = 0; i < longueurTexteDepart; i++){
        fputs(format(Cryptage(Texte_depart[i], La_cle[i % longueurCle])), f);
    }
    fputs("\n\nTexte de départ\n", f);  // TEXTE DE DEPART
    for (int i = 0; i < longueurTexteDepart; i++){
        fputc(Texte_depart[i], f);
    }
    fputs("\n\nTexte crypté\n", f); // TEXTE CRYPTE
    for (int i = 0; i < longueurTexteDepart; i++){   
        fputc(Cryptage(Texte_depart[i], La_cle[i % longueurCle]), f);
    }
    fputs("\n===== FIN D'ANALYSE =====\n", f);
    fclose(f);
    std::cout << "\n===== ÉCRITURE DES RÉSULTATS EFFECTUÉE =====\n";
    std::cout << "fichier de sortie : " << chemin << std::endl;
}