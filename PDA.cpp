//
// Created by Siebe Mees on 01/11/2023.
//

#include "PDA.h"
#include <iostream>
#include <fstream>
#include "json.hpp"
using namespace std;

using json = nlohmann::json;

PDA::PDA(const std::string &filename) {
    // inlezen uit file
    ifstream input(filename);

    json j;
    input >> j;

    // Access the elements of the "States" array
    vector<string> statesPDA;
    for (const auto& state : j["States"]) {
        statesPDA.push_back(state);
    }
    Q = statesPDA;

    // Access the elements of the "Input symbols" array
    vector<string> inputSymbolsPDA;
    for (const auto& inputSymbol : j["Alphabet"]) {
        inputSymbolsPDA.push_back(inputSymbol);
    }
    A = inputSymbolsPDA;

    // Access the elements of the "Stack symbols" array
    vector<string> stackSymbolsPDA;
    for (const auto& stackSymbol : j["StackAlphabet"]) {
        stackSymbolsPDA.push_back(stackSymbol);
    }
    Stack = stackSymbolsPDA;

    // Access the elements of the "Transitions" array
    vector<PDATransition> transitionsPDA;
    for (const auto& transition : j["Transitions"]) {
        PDATransition newTransition;
        newTransition.from_state = transition["from"];
        newTransition.input_symbol = transition["input"];
        newTransition.stack_symbol = transition["stacktop"];
        newTransition.to_state = transition["to"];
       for (const auto& replacementTransition : transition["replacement"]) {
            newTransition.stack_replacement_actions.push_back(replacementTransition);
        }
        transitionsPDA.push_back(newTransition);
    }
    T = transitionsPDA;

    // Access the elements of the "Start state" array
    string startStatePDA;
    for (const auto& startState : j["StartState"]) {
        startStatePDA += startState;
    }
    q0 = startStatePDA;

   // Access the elements of the "Start stack symbol" array
    string startStackSymbolPDA;
    for (const auto& startStackSymbol : j["StartStack"]) {
        startStackSymbolPDA += startStackSymbol;
    }
    Z0 = startStackSymbolPDA;

}

CFG PDA::toCFG() {
    CFG cfg;

    // V, Set of variables
    vector<string> VCFG;
    VCFG.push_back("S");
    for (int i = 0; i < Q.size(); i++) {
        for (int j = 0; j < Stack.size(); ++j) {
            for (int k = 0; k < Q.size(); ++k) {
                VCFG.push_back("["+Q[i]+"," + Stack[j]+"," + Q[k]+"]");
            }
        }
    }
    cfg.setV(VCFG);

    // T, Set of terminals
    vector<string> TCFG;
    for (int i = 0; i < A.size(); ++i) {
        TCFG.push_back(A[i]);
    }
    cfg.setT(TCFG);

    // S, Start symbol
    cfg.setS("S");

    // P, Productions
    vector<pair<string, vector<string>>> PCFG;
    // Only productions for S
    vector<string> replacementsForS;
    for (int i = 0; i < Q.size(); ++i) {
        replacementsForS.push_back("["+q0+",Z0,"+Q[i]+"]");
        PCFG.push_back(make_pair("S", replacementsForS));
        replacementsForS.clear();
    }
    // Step two of "the productions of the grammar G are as follows" p.250
    for (int i = 0; i < T.size(); ++i) {
        string head = "[" + T[i].from_state + "," + T[i].stack_symbol + ",";
        vector<string> replacementsForHead;
        replacementsForHead.push_back(T[i].input_symbol);

        if (!T[i].stack_replacement_actions.empty()) {
            for (int j = 0; j < Q.size(); ++j) {
                string newHead = head + Q[j] + "] ";
                if (T[i].stack_replacement_actions.size() == 1) {
                    replacementsForHead.push_back("["+T[i].to_state+","+T[i].stack_replacement_actions[0]+","+Q[j]+"]");
                    PCFG.push_back(make_pair(newHead, replacementsForHead));
                    replacementsForHead.clear();
                    replacementsForHead.push_back(T[i].input_symbol);
                } else {
                    for (int k = 0; k < T[i].stack_replacement_actions.size(); ++k) {
                        replacementsForHead.push_back("["+T[i].to_state+","+T[i].stack_replacement_actions[0]+","+Q[k]+"]");
                        replacementsForHead.push_back("[" + Q[k] + "," + T[i].stack_replacement_actions[1] + "," + Q[j] + "]");
                        PCFG.push_back(make_pair(newHead, replacementsForHead));
                        replacementsForHead.clear();
                        replacementsForHead.push_back(T[i].input_symbol);
                    }
                }
            }
        } else {
            head += T[i].to_state + "] ";
            PCFG.push_back(make_pair(head, replacementsForHead));
        }

    }

    cfg.setP(PCFG);
    return cfg;
}