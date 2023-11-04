//
// Created by Siebe Mees on 01/11/2023.
//
#include "PDA.h"

using namespace std;

int main() {
    PDA pda("../inputPDA3.json");
    pda.toCFG().print();
    return 0;
}