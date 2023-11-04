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
    Σ = inputSymbolsPDA;

    // Access the elements of the "Stack symbols" array
    vector<string> stackSymbolsPDA;
    for (const auto& stackSymbol : j["StackAlphabet"]) {
        stackSymbolsPDA.push_back(stackSymbol);
    }
    Γ = stackSymbolsPDA;

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
    δ = transitionsPDA;

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
        for (int j = 0; j < Γ.size(); ++j) {
            VCFG.push_back("["+Q[i]+"," + Γ[j]+"," + Q[i]+"]");
            for (int k = Q.size()-1; k > 0; --k) {
                VCFG.push_back("["+Q[i]+"," + Γ[j]+"," + Q[k]+"]");
            }
        }
    }
    cfg.setV(VCFG);

    // T, Set of terminals
    vector<string> TCFG;
    for (int i = 0; i < Σ.size(); ++i) {
        TCFG.push_back(Σ[i]);
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
    for (int i = 0; i < δ.size(); ++i) {
        string head = "[" + δ[i].from_state + "," + δ[i].stack_symbol + ",";
        vector<string> replacementsForHead;
        replacementsForHead.push_back(δ[i].input_symbol);

        if (!δ[i].stack_replacement_actions.empty()) {
            int bodySize = δ[i].stack_replacement_actions.size();
            int stateCount = Q.size();
            // Generate the production [qZq] → i[qZq][qZq)
            /*for (int j = 0; j < bodySize; ++j) {
                for (int k = 0; k < stateCount; ++k) {
                    string newhead = head + Q[j]+"]";
                    replacementsForHead.push_back("["+δ[i].to_state+","+δ[i].stack_replacement_actions[j]+","+Q[j]+"]");
                    PCFG.push_back(make_pair(newhead, replacementsForHead));
                    replacementsForHead.pop_back();
                }
            }*/
        } else {
            head += δ[i].to_state + "]";
        }

        // Add the generated productions to the PCFG
        PCFG.push_back(make_pair(head, replacementsForHead));
        replacementsForHead.clear();
    }



    cfg.setP(PCFG);

    return cfg;
}