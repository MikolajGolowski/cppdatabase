//
// Created by mikolaj on 03.06.2021.
//

#include "menu.h"
using namespace std;

bool Menu::parse(string &input, Kernel &kernel) {
    if(input == "help"){
        cout<<"Program obsluguje komendy:\n1. Create\n2. Read\n3. Update\n4. Delete\n Aby ich uzyc nalezy wpisac pierwsza"
              " litere komendy i postepowac zgodnie z wyswietlanymi instrukcjami.\n Zeby zmienic baze danych nalezy uzyc komendy "
              "baza LOKALIZACJA_BAZY np. aby uzyc bazy plik.bd \"baza plik.db\"\n Komenda exit konczy dzialanie programu";
    }else if(input=="exit"){
        return false;
    }else if(input == "C" || input == "c"){
        C(kernel);
    }else{
        cout<<"Wybrano baze danych "<<input<<endl;
        kernel.switchDatabase(input);
    }

    cout<<endl<<">>> ";
    return true;
}

void Menu::init() {
    cout<<"Program bazy danych CRUD z obsluga wielu baz i szyfrowaniem"<<endl<<"Aby uzyskac pomoc wpisz help"<<endl<<">>> ";
}


bool Menu::C(Kernel &kernel) {
    string buf1, buf2, nazwa;
    cout<<"Podaj nazwe obiektu: ";
    cin>>nazwa;
    cout<<"Dodawanie atrybutow do obiektu:\n";
    vector<pair<string,string>> atr;
    do{
        cout<<"Podaj nazwe atrybutu(lub zakoncz wpisujac stop): ";
        cin>>buf1;
        if(buf1=="stop")
            break;
        cout<<"Wpisz wartosc "<<buf1<<": ";
        cin>>buf2;
        atr.push_back({buf1,buf2});
    } while (true);
#if DEBUG
    cout<<"Tworzenie obiektu "<<nazwa<<" "<<kernel.currentFilename<<endl;
#endif
    Obiekt ob(atr, nazwa, kernel.currentFilename);
    kernel.Create(ob);
}

bool Menu::R(Kernel &kernel) {
    return false;
}

bool Menu::U(Kernel &kernel) {
    return false;
}

bool Menu::D(Kernel &kernel) {
    return false;
}
