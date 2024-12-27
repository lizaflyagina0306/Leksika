#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "Token.h"
#include <fstream>

using namespace std;

//������� � ���� ������������� ������� ��� ����� ����������� ���� �� ������� �������� �� �� ������� ���� �� ����� ������� ��������� ���� �� ������ ������
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

	void updateHeshtable() {
		Token* oldHesh = new Token[AlrdInsert];
		int i = 0;
		for (int j = 0; j < Size/2; j++) {
			if (HeshTable[j].Type != "") {
				oldHesh[i] = HeshTable[j];
				i++;
			}
		}
		long long hesh;
		for (int i = 0; i < AlrdInsert; i++) {
			hesh = HeshFunction(oldHesh[i]);
			while (HeshTable[hesh % Size].Type != "") {
				hesh++;
			}
			HeshTable[hesh % Size] = oldHesh[i];
		}
	}


	//��� ���������� ������� ���� �� ����� � �������, � ���� ��� ����������� �����������, � ����� ���� ����� ���� ��������
	int addElement(Token token) {
		long long hesh = HeshFunction(token);
		if (AlrdInsert == Size) {
			Token* buff = new Token[2*Size];
			for (int i = 0; i < Size; i++) {
				buff[i] = HeshTable[i];
			}
			delete [] HeshTable;
			HeshTable = buff;
			Size *= 2;
			updateHeshtable();
		}
		while (HeshTable[hesh % Size].Type != "" && HeshTable[hesh % Size].Leksema != token.Leksema) {
			hesh++;
		}
		HeshTable[hesh % Size]= token;
		AlrdInsert++;
		return hesh % Size;// ������� ������ � ������� ��� �������	
	}

	//����� ���� �� ������ ��������� ��������
	void seeTable(ofstream& out) {
		string buffer;
		out << "\nHESH TABLE\nType  Leksema  Hesh \n";
		for (int i = 0; i < Size; i++) {
			if (HeshTable[i].Type != "") {
				buffer = HeshTable[i].Type + ' ' + HeshTable[i].Leksema + ' ' + to_string(i) + '\n';
				out << buffer;
			}
		}
	}

};

