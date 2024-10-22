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

    // При dfa.accept == true проверяем какой это тип
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

    // Функция, которая возвращает лексему, ее тип, индекс в таблице, если она существует, иначе возвращает ошибку
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
        return outSTR; // Возвращаем строку с переносом
    }


    // Проверяет, что строка не пуста и запускает проверку для добавления в таблицу
    void cheker(string& buffer, ofstream& out) {
        if (!buffer.empty()) {
            out << addNewElem(buffer)<<"\n";
            buffer.clear();
        }
    }

    // Разбиваем строку на отдельные слова и каждую отправляем на проверку
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
            else if (Code[i] == 39) { // Если символ - апостроф
                cheker(buffer, out);
                buffer += Code[i];
                i++;
                while (i < Code.size() && Code[i] != 39) {
                    buffer += Code[i];
                    i++;
                }
                buffer += Code[i]; // Добавляем закрывающий апостроф
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
        out << "WORK PROGRAMM"<<'\n'; // Начало файла где мы будем показывать по порядку входа что мы сделали с каждым словом
        while (getline(in, buffer)) { // Считываем файл построчно
            this->Parser(buffer, out);
        }
        hs.seeTable(out); // Выводим таблицу
    }
};
