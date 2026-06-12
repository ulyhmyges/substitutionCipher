#include "chiffre.h"
#include <string>
#include <iostream>


int main(void){
    //
    toUppercase("dream.txt", "out.txt");

    Chiffrer obj;
    obj.enter();
    obj.print();
    obj.toFile();
    
}