#pragma once
#include <iostream>
#include "Token.h"
#include <fstream>

using namespace std;

//������� � ���� ������������� ������� ��� ����� ����������� ���� �� ������� 
// �������� �� �� ������� ���� �� ����� ������� ��������� ���� �� ������ ������
//� ������ ��������� ����������� ������� ����������� �� ������ �����
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

    //������� ��� ������� ������� ����� ����� �������� �� ����� � ������� n(����� � ������ ��� ����� �����)
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

    //��� ���������� ������� ���� �� ����� � �������, � ���� ��� ����������� �����������, 
    // � ����� ���� ����� ���� ��������
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
        return hesh % Size;// ������� ������ � ������� ��� �������
    }
    // ����� ��� �������� ������� ������ � ���-�������
    bool contains(Token token) {
        long long hesh = HeshFunction(token);
        for (int i = 0; i < Size; i++) {
            int index = (hesh + i) % Size; // ��������� ������� � ������ ��������
            if (HeshTable[index].Type == " ") {
                break; // ���� ��������� ������ ������, ������ ����� �����������
            }
            if (HeshTable[index].Leksema == token.Leksema) {
                return true; // ����� ������
            }
        }
        return false; // ����� �� ������
    }

    //����� ���� �� ������ ��������� �������
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