#pragma once
#include <iostream>
#include "Token.h"
#include <fstream>

using namespace std;

//“аблица в виде динамического массива где после определени€ хеша мы пробуем 
// добавить ее по индексу если он зан€т пробуем следующие пока не найдем нужный
//¬ случае полностью заполненной таблицы увеличиваем ее размер вдвое
class Hesh
{
private:
    Token* HeshTable;
    int Size;
    int AlrdInsert = 0;
public:

    Hesh(int n) {
        this->Size = n;
        HeshTable = new Token[n];
    }

    Hesh() {
        this->Size = 1007;
        HeshTable = new Token[1007];
    }

    //ѕроста€ хеш функци€ котора€ номер буквы умножает на число в степени n(номер в строке где стоит буква)
    long long HeshFunction(Token& token) {
        long long sum = 0;
        long long p = 1;
        long long pow = 31;
        for (int i = 0; i < token.Leksema.size(); i++) {
            sum += p * token.Leksema[i];
            sum %= Size;
            p = (p * pow) % Size;
        }
        return sum;
    }

    //ѕри добавлении смотрим есть ли место в таблице, и если что увеличиваем вместимость, 
    // а потом ищем место куда добавить
    int addElement(Token token) {
        long long hesh = HeshFunction(token);
        if (AlrdInsert == Size) {
            Token* buff = new Token[2 * Size];
            for (int i = 0; i < Size; i++) {
                buff[i] = HeshTable[i];
            }
            delete HeshTable;
            HeshTable = buff;
            Size *= 2;
        }
        while (HeshTable[hesh % Size].Type != "") {
            hesh++;
        }
        HeshTable[hesh % Size] = token;
        return hesh % Size;// выводим индекс в таблице дл€ задани€
    }
    // ћетод дл€ проверки наличи€ токена в хеш-таблице
    bool contains(Token token) {
        long long hesh = HeshFunction(token);
        for (int i = 0; i < Size; i++) {
            int index = (hesh + i) % Size; // ѕробируем индексы в случае коллизий
            if (HeshTable[index].Type == " ") {
                break; // ≈сли встретили пустую €чейку, значит токен отсутствует
            }
            if (HeshTable[index].Leksema == token.Leksema) {
                return true; // “окен найден
            }
        }
        return false; // “окен не найден
    }

    //¬ывод всех не пустых элементов таблицы
    void seeTable(ofstream& out) {
        string buffer;
        out << "\nHESH TABLE\nIndex  Type  Leksema\n";
        for (int i = 0; i < Size; i++) {
            if (HeshTable[i].Type != "") {
                buffer = to_string(i) + " | " + HeshTable[i].Type + " | " + HeshTable[i].Leksema + '\n';
                out << buffer;
            }
        }
    }

};