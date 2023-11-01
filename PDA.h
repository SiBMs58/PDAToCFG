//
// Created by Siebe Mees on 01/11/2023.
//

#ifndef PDATOCFG_PDA_H
#define PDATOCFG_PDA_H

#include "CFG.h"
#include <string>
#include <vector>
#include <map>

using namespace std;

struct PDATransition {
    string from_state;
    string input_symbol;
    string stack_symbol;
    string to_state;
    vector<string> stack_replacement_actions;
};

class PDA {
private:
    vector<string> Q; // Set of states
    vector<string> Σ; // Set of input symbols
    vector<string> Γ; // Set of stack symbols
    vector<PDATransition> δ; // Set of transitions
    string q0; // Start state
    string Z0; // Start stack symbol
    vector<string> F; // Set of final states

public:
    PDA(const string &filename);

    CFG toCFG();
};


#endif //PDATOCFG_PDA_H
