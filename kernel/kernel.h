//
// Created by mikolaj on 03.06.2021.
//

#ifndef CPP_KERNEL_H
#define CPP_KERNEL_H
#define OBIEKT '1'
#define LICZBA '2'

#include "Objects.h"
#define KEY "lins88vys98efleviuvhkrgie8ingliv5gny//"

#define DEBUG 1

using namespace std;

class Menu;
class Obiekt;
class Kernel;
class ISerializable;
class Liczba;


class Kernel{
public:
    ~Kernel();              //destruktor
    Kernel(){
        currentFilename = "";
    }

list <Liczba> lic;
list <Obiekt> obj;  //to musi byc lista bo vector moze zmienic
                    //miejsce w pamieci gdy doda sie kolejne dane
private:
    void clear();
    string currentFilename;
    void switchDatabase(string& databasename);
    void readWholeFile();
    void makeWholeFile();
    vector <ISerializable*> allObjects;     //polimorfizm, stl vector
    map<string, int> databaseUniqueIdAvbl;
    void Create(ISerializable* ob);
    void Read(int a);
    friend Menu;                            //deklaracja przyjaciela
};

#endif //CPP_KERNEL_H
