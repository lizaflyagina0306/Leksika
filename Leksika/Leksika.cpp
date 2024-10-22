#include <iostream>
#include <vector>
#include <string>
#include "Token.h"
#include "LeksikaAnalizator.h"
#include "dfa.h"
#include <fstream>

//Считываем автомат с файла
dfa DfaReader(string fileName,vector<string>& KeyWords) {
    int count;
    ifstream fin(fileName);
    //считывает количество состояний автомата
    fin >> count;
    vector<char> alphabet;
    vector<int> finalStates;
    vector<vector<int> > transitFunc(count);
    string buff;
    //считывает алфавит
    getline(fin, buff);
    getline(fin, buff);
    for (int i = 0; i < buff.size(); i += 2) {
        alphabet.push_back(buff[i]);
    }
    alphabet.push_back(' ');
    //считывает финальные состояния
    getline(fin, buff);
    string bf = "";
    for (int i = 0; i < buff.size(); i++) {
        if (buff[i] != ' ') {
            bf += buff[i];
        }
        else {
            finalStates.push_back(stoi(bf));
            bf = "";
        }
    }
    finalStates.push_back(stoi(bf));
    bf = "";
    //считывает матрицу переходов между состояниями
    for (int j = 0; j < count; j++) {
        getline(fin, buff);
        for (int i = 0; i < buff.size(); i++) {
            if (buff[i] != ' ') {
                bf += buff[i];
            }
            else {
                transitFunc[j].push_back(stoi(bf));
                bf = "";
            }
        }
        transitFunc[j].push_back(stoi(bf));
        bf = "";
    }
    //считавыет количество и сами ключевые слова
    int countKeyWord;
    fin >> countKeyWord;
    for (int i = 0; i < countKeyWord; i++) {
        fin >> buff;
        KeyWords.push_back(buff);
    }
    dfa df(count, alphabet, finalStates, transitFunc);
    return df;
}

int main()
{
    Hesh hs(123);
    vector<string> KeyWords;
    dfa df = DfaReader("alphabet.txt", KeyWords);
    LeksikaAnalizator leksika(df,KeyWords);
    ifstream in("TestLeksika.txt");
    ofstream out("OutTestLeksika.txt");
    leksika.StartLeksikaAnalyse(in, out);//Запуск обработки файла
}