//
// Created by Siebe Mees on 01/11/2023.
//

#include "CFG.h"
#include <iostream>
#include <fstream>
#include "json.hpp"
using namespace std;

using json = nlohmann::json;

CFG::CFG() {}

/*CFG::CFG(const string &filename) {
    // inlezen uit file
    ifstream input(filename);

    json j;
    input >> j;

    // Access the elements of the "Start" array
    string start;
    for (const auto& startvar : j["Start"]) {
        start += startvar;
    }
    S = start;

    // Access the elements of the "Variables" array
    vector<string> variables;
    for (const auto& var : j["Variables"]) {
        variables.push_back(var);
    }
    sort(variables.begin(), variables.end());
    V = variables;

    // Access the elements of the "Terminals" array
    vector<string> terminals;
    for (const auto& terminal : j["Terminals"]) {
        terminals.push_back(terminal);
    }
    sort(terminals.begin(), terminals.end());
    T = terminals;

    // Access the elements of the "Productions" array
    vector<pair<string, vector<string>>> productions;
    for (const auto& rule : j["Productions"]) {
        string head = rule["head"];
        vector<string> body = rule["body"];
        productions.push_back(make_pair(head, body));
    }
    sort(productions.begin(), productions.end());
    P = productions;
}*/

void CFG::print() {
    // V, Set of variables
    cout << "V = {";
    for (int i = 0; i < V.size(); ++i) {
        cout << V[i];
        if (i == V.size() - 1) {
            cout << "}" << endl;
            break;
        } else {
            cout << ", ";
        }
    }

    // T, Set of terminals
    cout << "T = {";
    for (int i = 0; i < T.size(); ++i) {
        cout << T[i];
        if (i == T.size() - 1) {
            cout << "}" << endl;
            break;
        } else {
            cout << ", ";
        }
    }

    // P, Productions
    cout << "P = {" << endl;
    for (int i = 0; i < P.size(); ++i) {
        for (int j = 0; j <  P[i].second.size(); ++j) {
            cout << "    " << P[i].first << "   -> `";
            if (P[i].second.size() == 0) {
                cout << "`" << endl;
            } else {
                cout << P[i].second[j]<< "`" ;
                cout << endl;
            }
        }
    }
    cout << "}" << endl;


    // S, Start symbol
    cout << "S = " << S << endl;
}

// Setters
void CFG::setV(const vector<string> &v) {
    V = v;
}

void CFG::setT(const vector<string> &t) {
    T = t;
}

void CFG::setS(const string &s) {
    S = s;
}

void CFG::setP(const vector<pair<string, vector<string>>> &p) {
    P = p;
}
