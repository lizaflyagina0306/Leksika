#pragma once
#include "Token.h"
#include <iostream>
#include "dfa.h"
#include "Hesh.h"
#include <fstream>
using namespace std;

class LeksikaAnalizator {
private:
    dfa Dfa;
    vector<string> KeyWords;
    Hesh hs;

public:
    LeksikaAnalizator(dfa& df, vector<string> KeyWords) {
        this->Dfa = df;
        this->KeyWords = KeyWords;
    }

    // ��� dfa.accept == true ��������� ����� ��� ���
    string TypeLeksema(string buff) {
        if (buff == "-" || buff == "+" || buff == "=" || buff == "(" || buff == ")") {
            return "Expresion";
        }
        if (buff[0] == 39) {
            return "String";
        }
        for (const auto& keyword : KeyWords) {
            if (buff == keyword) {
                return "KeyWord";
            }
        }
        if (buff[0] >= 'A') {
            return "NameVar";
        }
        return "IntNum";
    }

    // �������, ������� ���������� �������, �� ���, ������ � �������, ���� ��� ����������, ����� ���������� ������
    string addNewElem(string bf) {
        string outSTR;
        if (Dfa.isAccept(bf)) {
            string Type = TypeLeksema(bf);
            Token newToken(Type, bf);

            if (hs.contains(newToken)) {
                outSTR = newToken.Type + '|' + newToken.Leksema;
            }
            else {
                int numHesh = hs.addElement(newToken);
                outSTR = newToken.Type + '|' + newToken.Leksema + '|' + to_string(numHesh);
            }
        }
        else {
            outSTR = "!ERROR! | " + bf;
        }
        return outSTR; // ���������� ������ � ���������
    }


    // ���������, ��� ������ �� ����� � ��������� �������� ��� ���������� � �������
    void cheker(string& buffer, ofstream& out) {
        if (!buffer.empty()) {
            out << addNewElem(buffer)<<"\n";
            buffer.clear();
        }
    }

    // ��������� ������ �� ��������� ����� � ������ ���������� �� ��������
    void Parser(string& Code, ofstream& out) {
        string buffer = "";
        for (int i = 0; i < Code.size(); i++) {
            if (Code[i] == '(' || Code[i] == ')' ||
                Code[i] == '+' || Code[i] == '-' ||
                Code[i] == '=' || Code[i] == ',') {
                cheker(buffer, out);
                buffer = Code[i];
                out << addNewElem(buffer)<<'\n';
                buffer = "";
            }
            else if (Code[i] == 39) { // ���� ������ - ��������
                cheker(buffer, out);
                buffer += Code[i];
                i++;
                while (i < Code.size() && Code[i] != 39) {
                    buffer += Code[i];
                    i++;
                }
                buffer += Code[i]; // ��������� ����������� ��������
                cheker(buffer, out);
            }
            else if (Code[i] == ' ') {
                cheker(buffer, out);
            }
            else {
                buffer += Code[i];
            }
        }
        cheker(buffer, out);
    }

    void StartLeksikaAnalyse(ifstream& in, ofstream& out) {
        string buffer;
        out << "WORK PROGRAMM"<<'\n'; // ������ ����� ��� �� ����� ���������� �� ������� ����� ��� �� ������� � ������ ������
        while (getline(in, buffer)) { // ��������� ���� ���������
            this->Parser(buffer, out);
        }
        hs.seeTable(out); // ������� �������
    }
};
