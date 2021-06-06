//
// Created by mikolaj on 03.06.2021.
//
#include <sstream>
#include <utility>
#include "kernel.h"


bool Kernel::SerializeAll() {
    return false;
}

Kernel::~Kernel() {

    cout<<"Koniec programu\n";
}

bool Kernel::openFile() {
    return false;
}

string readFileToString(string filename){
    ifstream t("file.txt");
    stringstream buffer;
    buffer << t.rdbuf();
    t.close();
    return buffer.str();
}

string encryptOrDecrypt(string msg, string key)
{
#if DEBUG
    cout<<"Przekonwertowano \n"<<msg<<endl<<"do\n";
#endif
    string tmp(key);
    while (key.size() < msg.size())
        key += tmp;

    for (size_t i = 0; i < msg.size(); ++i)
        msg[i] ^= key[i];

#if DEBUG
    cout<<msg<<endl;
#endif

    return msg;
}

void Kernel::Create(Obiekt &ob) {
    ob.id = databaseUniqueIdAvbl[ob.filename];
    databaseUniqueIdAvbl[ob.filename]++;
    obj.emplace_back(ob);
    allObjects.emplace_back(&obj.back());
    makeWholeFile();
}

void Kernel::readWholeFile() {
    vector<ISerializable*> a;
    string content = encryptOrDecrypt(readFileToString(currentFilename),KEY);
    bool wKlamrowych = false;
    int ile = 0;
    int pocz = -1, typ;
    for (size_t i = 0; i < content.size(); ++i) {
        if(wKlamrowych){
            if(pocz == -1) {
                pocz = i+1;
                typ = content[i];
            }
            if(content[i] == '}'){
                ile++;
                wKlamrowych = false;
                switch(typ){
                    case OBIEKT:
                        Obiekt c(content.substr(pocz, i-1-pocz), *this);
                        c.filename = currentFilename;
                        c.type = OBIEKT;
                        (*this).obj.push_back(c);
                        (*this).allObjects.push_back((ISerializable *) &(*this).obj.back());
                        if(c.id >= databaseUniqueIdAvbl[currentFilename])
                            databaseUniqueIdAvbl[currentFilename] = c.id+1;
                        break;
                }
                pocz = -1;
            }
        }else if(content[i] == '{'){
            wKlamrowych = true;
        }
    }
    cout<<"Wczytano "<<ile<<" struktur danych\n";
}

void Kernel::switchDatabase(string &databasename) {
    clear();
    currentFilename = databasename;
    databaseUniqueIdAvbl.erase(databasename);
    databaseUniqueIdAvbl.insert({databasename,0});
    readWholeFile();
}

void Kernel::makeWholeFile() {
    ofstream file(currentFilename);
    string data = "";
#if DEBUG
    cout<<"W allobjects jest "<<allObjects.size()<<" elementow"<<endl;
#endif
    data.reserve(allObjects.size() * 100);
    for (auto & i : allObjects) {
        //if((allObjects[i])->filename == currentFilename){
#if DEBUG
        cout<<"obiekt name = "<<(*((Obiekt*)i)).name<<endl;
#endif
            data.append((*((Obiekt*)i)).serialize());
#if DEBUG
            cout<<"Dodano zserializowany obiekt do bazy danych"<<endl;
#endif
       // }
    }
    file << encryptOrDecrypt(data,KEY);
    file.close();
}

void Kernel::clear() {
    allObjects.clear();
    obj.clear();
    databaseUniqueIdAvbl.clear();
    currentFilename ="";
}

string Obiekt::serialize() {
    string s = "";
#if DEBUG
    cout<<"Proba serializacji"<<endl;
#endif
    s.append("{1" + name + "," + to_string(id) + "," + to_string(parentId) + "," + to_string(childrenId.size()) + ","
     + to_string(atrybuty.size()));
    for(auto i : childrenId){
        s.append(","+to_string(i));
    }
    for(auto i : atrybuty){
        s.append("," + i.first + "," + i.second);
    }
    s.append("}");
#if DEBUG
    cout<<"Zserializowano "<<s<<endl;
#endif
    return s;
}

ISerializable* Obiekt::deSerialize(string content, Kernel &kernel) {
    bool wPrzec = false;
    string _id, _pid, ileD, ileA;
    _id ="";
    _pid="";
    ileD = "";
    ileA="";
    int valC = 0, ileDzieci, ileAtr, x = 0, y = 0;
    for (int i = 0; i < content.size(); ++i) {
        if(content[i] == ','){
            if(wPrzec){
                valC++;
                wPrzec = false;
            }else{
                wPrzec = true;
            }
        }else{
            if(wPrzec){
                switch (valC) {
                    case 0:
                        name += content[i];
                        break;
                    case 1:
                        _id += content[i];
                        break;
                    case 2:
                        _pid += content[i];
                        break;
                    case 3:
                        ileD += content[i];
                        break;
                    case 4:
                        ileA += content[i];
                        break;
                    case 5:
                        valC = i;
                        i = content.size();
                        break;
                }
            }
        }
    }
    id = stoi(_id);
    parentId = stoi(_pid);
    ileDzieci = stoi(ileD);
    ileAtr = stoi(ileA);
    string buf, buf2;
    for (int i = 0; i < ileDzieci; ++i) {
        while(content[valC] != ','){
            buf += content[valC];
            valC++;
        }
        valC++;
        childrenId.push_back(stoi(buf));
    }
    for (int i = 0; i < ileAtr; ++i) {
        while(content[valC] != ','){
            buf += content[valC];
            valC++;
        }
        while(content[valC] != ','){
            buf2 += content[valC];
            valC++;
        }
        valC++;
        atrybuty.insert({buf,buf2});
    }
    for(auto i = kernel.obj.begin(); i!=kernel.obj.end(); i++ ){
        if((*i).id == id){
            kernel.obj.erase(i);
            break;
        }
    }
    kernel.obj.push_back(*this);
    kernel.allObjects.push_back(&kernel.obj.back());
    return &kernel.obj.back();
}
