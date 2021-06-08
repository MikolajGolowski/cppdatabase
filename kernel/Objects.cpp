//
// Created by mikolaj on 07.06.2021.
//

#include "Objects.h"


string Obiekt::serialize() {
    string s = "";
#if DEBUG
    cout<<"Proba serializacji"<<endl;
#endif
    s.append("{" + to_string(OBIEKT - '0') + name + "," + to_string(id) + "," + to_string(parentId) + "," + to_string(childrenId.size()) + ","
             + to_string(atrybuty.size()));
    for(auto i : childrenId){
        s.append(","+to_string(i));
    }
    for(auto &i : atrybuty){
        s.append("," + i.first + "," + i.second);
    }
    s.append("}");
#if DEBUG
    cout<<"Zserializowano "<<s<<endl;
#endif
    return s;
}

ISerializable* Obiekt::deSerialize(string content, Kernel &kernel) {
    string _id, _pid, ileD, ileA;
    _id ="";
    _pid="";
    ileD = "";
    ileA="";
    int valC = 0, ileDzieci, ileAtr, x = 0, y = 0;
    for (int i = 0; i < content.size(); ++i) {
        if(content[i] == ','){
            valC++;
        }else{
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
#if DEBUG
    cout<<"sparsowano z "<<content<<"\nname="<<name<<" id="<<_id<<" parentId="<<_pid<<" ileD="<<ileD<<" ileA="<<ileA<<endl;
#endif
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
        valC++;
        while(valC < content.size() && content[valC] != ','){
            buf2 += content[valC];
            valC++;
        }
        valC++;
        atrybuty.insert({buf,buf2});
#if DEBUG
        cout<<" wstawiam na mape pare "<<buf<<" "<<buf2<<endl;
#endif
    }
    if(!kernel.czyJestDuplikat(this->id)){
        kernel.obj.push_back(*this);
        return &kernel.obj.back();
    }
    throw;
}

string Liczba::serialize() {
    string s = "";
#if DEBUG
    cout<<"Proba serializacji"<<endl;
#endif
    s.append("{" + to_string(LICZBA - '0') + name + "," + to_string(id) + "," + to_string(parentId) + "," + to_string(childrenId.size()));
    for(auto i : childrenId){
        s.append(","+to_string(i));
    }
    s.append("," + to_string(liczba) + "}");
#if DEBUG
    cout<<"Zserializowano "<<s<<endl;
#endif
    return s;
}

ISerializable *Liczba::deSerialize(string content, Kernel &kernel) {
    string _id, _pid, ileD, ileA;
    _id ="";
    _pid="";
    ileD = "";
    ileA="";
    int valC = 0, ileDzieci, ileAtr, x = 0, y = 0;
    for (int i = 0; i < content.size(); ++i) {
        if(content[i] == ','){
            valC++;
        }else{
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
                    valC = i;
                    i = content.size();
                    break;
            }
        }
    }
#if DEBUG
    cout<<"sparsowano z "<<content<<"\nname="<<name<<" id="<<_id<<" parentId="<<_pid<<" ileD="<<ileD<<" ileA="<<ileA<<endl;
#endif
    id = stoi(_id);
    parentId = stoi(_pid);
    ileDzieci = stoi(ileD);
    string buf;
    for (int i = 0; i < ileDzieci; ++i) {
        while(content[valC] != ','){
            buf += content[valC];
            valC++;
        }
        valC++;
        childrenId.push_back(stoi(buf));
    }
    while(valC < content.size()){
        buf += content[valC];
        valC++;
    }
#if DEBUG
        cout<<" odczytalem liczbe "<<buf<<endl;
#endif
    liczba = stoi(buf);
    if(!kernel.czyJestDuplikat(this->id)){
        kernel.lic.push_back(*this);
        return &kernel.lic.back();
    }
    throw;
}
