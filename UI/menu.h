//
// Created by mikolaj on 03.06.2021.
//

#ifndef CPP_MENU_H
#define CPP_MENU_H
#include <iostream>
#include "../kernel/kernel.h"

class Menu{
public:
    int podOpcjaMenu;
    bool parse(string &input, Kernel &kernel);
    void C(Kernel &kernel);
    bool R(Kernel &kernel);
    bool U(Kernel &kernel);
    bool D(Kernel &kernel);
    void init();
    bool czyBazaOk(Kernel& kernel);
};
#endif //CPP_MENU_H
