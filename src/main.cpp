// main.cpp
#include "Manager.h"
#include "string/ManagerString.h"

int main(int argc, char *argv[]) {
    char variante = 'i';
    if (argc >= 5) {
        variante = argv[4][0];
    }
    if (variante == 's') {
        ManagerString manager(argc, argv);
    } else {
        Manager manager(argc, argv);
    }
    return 0;
}
