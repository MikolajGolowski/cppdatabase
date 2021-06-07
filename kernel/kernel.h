//
// Created by mikolaj on 03.06.2021.
//

#ifndef CPP_KERNEL_H
#define CPP_KERNEL_H
#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <iostream>
#include <list>

#define KEY "lins88vys98efleviuvhkrgie8ingliv5gny//"
#define OBIEKT '1'
#define LICZBA '2'

#define DEBUG 1

using namespace std;

class Menu;
class Kernel;

class ISerializable{                                        //klasy, klasa abstrakcyjna
public:
    virtual string serialize() = 0;
    virtual ISerializable* deSerialize(string content, Kernel& kernel) = 0;
    string filename;
    int type;
    string name;
protected:
    vector<int> childrenId;
    int parentId, id;
    friend Kernel;

};

class Obiekt: public ISerializable{                         //dziedziczenie
public:

    map<string,string> atrybuty;                            //stl map
    string serialize() override;
    ISerializable* deSerialize(string content, Kernel& kernel) override;

    Obiekt(){}

    Obiekt(vector<pair<string,string>> &atr, const string &nazwa, const string &_filename){         //konstruktor
        for (auto & i : atr) {                                                                      //zakresowa petla for
            atrybuty.insert({i.first,i.second});
        }
        name = nazwa;
        filename = _filename;
        parentId = 0;
    }
    Obiekt(string nam, int idd, int parentidd, string filenam){
        name = nam;
        id = idd;
        parentId = parentidd;
        filename = filenam;
    }
};

class Kernel{
public:
    ~Kernel();              //destruktor
    Kernel(){
        currentFilename = "";
    }
private:
    void clear();
    string currentFilename;
    void switchDatabase(string& databasename);
    vector <int> toBeSerializedIds;
    void readWholeFile();
    void makeWholeFile();
    vector <ISerializable*> allObjects;     //polimorfizm, stl vector
    vector <int> allObjectsTypes;
    list <Obiekt> obj;                      //to musi byc lista bo vector dynamicznie zmienia
                                            //miejsce w pamieci jak sie dodaje kolejne dane i musi zaalokowac jej wiecej
    map<string, int> databaseUniqueIdAvbl;
    bool SerializeAll();
    bool openFile();
    void Create(Obiekt &ob);
    friend Menu;                            //deklaracja przyjaciela
    friend Obiekt;

};

#endif //CPP_KERNEL_H
