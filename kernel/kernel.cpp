//
// Created by mikolaj on 03.06.2021.
//
#include <sstream>
#include <utility>
#include <algorithm>
#include "kernel.h"


Kernel::~Kernel() {

    cout<<"Koniec programu\n";
}


string readFileToString(string filename){
    ifstream t(filename);
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

void Kernel::Create(ISerializable* ob, vector<int> chldIds, int parentID) {
#if DEBUG
    cout<<"Przed wpisem w allobj jest "<<allObjects.size()<<" elementow"<<endl;
#endif
    auto parent = getById(parentID);
    ob->parentId = parent->id;
    ob->childrenId = chldIds;
    ob->id = databaseUniqueIdAvbl[ob->filename];
    databaseUniqueIdAvbl[ob->filename]++;
    if(ob->type == OBIEKT){
        obj.push_back(*(Obiekt *)ob);
        allObjects.emplace_back(&obj.back());
    }else if(ob->type == LICZBA){
        lic.push_back(*(Liczba *)ob);
        allObjects.emplace_back((&lic.back()));
    }
    parent->childrenId.emplace_back(ob->id);
    makeWholeFile();
}

void Kernel::readWholeFile() {
#if DEBUG
    cout<<"Przed czytaniem w allobj jest "<<allObjects.size()<<" elementow"<<endl;
#endif
    string content = encryptOrDecrypt(readFileToString(currentFilename),KEY);
    if(content.empty()){
        content = "1{2root,0,0,0,0}";
    }
    size_t j = 0;
    while(content[j] != '{' && j < content.size()){
        j++;
    }
#if DEBUG
    cout<<"Database id max="<<content.substr(0,j)<<endl;
#endif
    if(j > 0)
        databaseUniqueIdAvbl[currentFilename] = stoi(content.substr(0,j));
    bool wKlamrowych = false;
    int ile = 0;
    int pocz = -1, typ;
    Obiekt c;
    Liczba l;
    for (size_t i = j; i < content.size(); ++i) {
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
                        c.filename = currentFilename;
                        c.type = OBIEKT;
                        allObjects.push_back(c.deSerialize(content.substr(pocz, i-pocz), *this));
                        if(c.id >= databaseUniqueIdAvbl[currentFilename])
                            databaseUniqueIdAvbl[currentFilename] = c.id+1;
                        break;
                    case LICZBA:
                        l.filename = currentFilename;
                        l.type = LICZBA;
                        allObjects.push_back(l.deSerialize(content.substr(pocz, i-pocz), *this));
                        if(l.id >= databaseUniqueIdAvbl[currentFilename])
                            databaseUniqueIdAvbl[currentFilename] = l.id+1;
                        break;
                }
                pocz = -1;

            }
        }else if(content[i] == '{'){
            wKlamrowych = true;
        }
    }
    cout<<"Wczytano "<<ile<<" struktur danych\n";
#if DEBUG
    cout<<"Po wczytaniu w allobj jest "<<allObjects.size()<<" elementow"<<endl;
#endif
}

void Kernel::switchDatabase(string &databasename) {
#if DEBUG
    cout<<"Zmieniam baze danych"<<endl;
#endif
    clear();
    currentFilename = databasename;
    databaseUniqueIdAvbl.erase(databasename);
    databaseUniqueIdAvbl.insert({databasename,0});
    readWholeFile();
}

void Kernel::makeWholeFile() {
    ofstream file(currentFilename);
    string data = "";
    data.append(to_string(databaseUniqueIdAvbl[currentFilename]));
#if DEBUG
    cout<<"W allobjects jest "<<allObjects.size()<<" elementow"<<endl;
#endif
    data.reserve(allObjects.size() * 100);
    for (auto & i : allObjects) {
#if DEBUG
        cout<<"Dodano zserializowany obiekt do bazy danych"<<endl;
        cout<<"obiekt name = "<< (*i).name<<endl;
#endif
        data.append((*i).serialize());
    }
    file << encryptOrDecrypt(data,KEY);
    file.close();
}

void Kernel::clear() {
    allObjects.clear();
    obj.clear();
    lic.clear();
    databaseUniqueIdAvbl.clear();
    currentFilename = "";
}

void Kernel::Read(int a) {
    for(auto i : allObjects){
        if(a==-1 || i->id == a){
            cout<<endl<<i->id<<". "<<i->name<<" typ: "<<i->type - '0'<<endl<<"ParentID: "<<i->parentId<<endl<<"Children IDs: ";
            for(auto j : i->childrenId)
                cout<<j<<" ";
            cout<<endl;
            switch(i->type){
                case OBIEKT:
                    for(auto z : ((Obiekt*)i)->atrybuty)
                        cout<<z.first<<": "<<z.second<<endl;
                    break;
                case LICZBA:
                    cout<<"Wartosc: "<<((Liczba*)i)->liczba<<endl;
                    break;
            }
        }
    }
}

bool Kernel::czyJestDuplikat(int id) {
    for(auto i : allObjects){
        if(i->id == id){
            cout<<"Baza danych uszkodzona - sa w niej duplikaty"<<endl;
            return true;
        }
    }
    return false;
}

void Kernel::removeById(int id) {
    for(auto i = allObjects.begin(); i!=allObjects.end() ; i++){
        if((*i)->id == id){
            for(auto j : (*i)->childrenId){
                removeById(j);
            }
            auto elem = getById((*i)->parentId);
            for(auto j = elem->childrenId.begin(); j != elem->childrenId.end(); j++){
                if(*j == (*i)->id){
                    elem->childrenId.erase(j);
                    break;
                }
            }
            allObjects.erase(i);
            break;
        }
    }
    makeWholeFile();
}

ISerializable *Kernel::getById(int id) {
    for(auto i = allObjects.begin(); i!=allObjects.end() ; i++){
        if((*i)->id == id){
            return *i;
        }
    }
    return nullptr;
}

bool Kernel::checkIfElementExists(int id) {
    for(auto i : allObjects){
        if((*i).id == id){
            return true;
        }
    }
    return false;
}
