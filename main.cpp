#include <iostream>
#include "kernel/kernel.h"
#include "UI/menu.h"

using namespace std;

int main() {
    Menu menu;
    Kernel kernel;
    string input;
    menu.init();
    do{
        cin>>input;
    } while (menu.parse(input,kernel));
    return 0;
}
