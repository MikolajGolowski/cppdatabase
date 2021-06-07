//
// Created by mikolaj on 07.06.2021.
//

#ifndef CPP_OBJECTS_H
#define CPP_OBJECTS_H
#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <iostream>
#include <list>
#include <string>
#include "kernel.h"

using namespace std;
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

    Obiekt(){
        type = OBIEKT;
    }

    Obiekt(vector<pair<string,string>> &atr, const string &nazwa, const string &_filename){         //konstruktor
        for (auto & i : atr) {                                                                      //zakresowa petla for
            atrybuty.insert({i.first,i.second});
        }
        name = nazwa;
        filename = _filename;
        parentId = 0;
        type = OBIEKT;
    }
    Obiekt(string nam, int idd, int parentidd, string filenam){
        name = nam;
        id = idd;
        parentId = parentidd;
        filename = filenam;
        type = OBIEKT;
    }
};

class Liczba : public ISerializable{
public:
    Liczba() {
        type = LICZBA;
    }

    Liczba(int l, string &nazwa, string &_filename){         //konstruktor
        liczba = l;
        name = nazwa;
        filename = _filename;
        parentId = 0;
        type = LICZBA;
    }

    string serialize() override;
    ISerializable* deSerialize(string content, Kernel& kernel) override;

    int liczba;
};

#endif //CPP_OBJECTS_H
