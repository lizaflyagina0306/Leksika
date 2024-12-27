#pragma once
#include "Token.h"
#include <iostream>
#include "dfa.h"
#include "Hesh.h"
#include<fstream>
#include "Sintaksis.h"
#include "Senmatika.h"
using namespace std;

class LeksikaAnalizator
{
private:
	dfa Dfa;
	vector<string> KeyWords;
	SintaksisAnalizator sintak;
	Senma senmatika;
	Hesh hs;
	Tree* tree = new Tree;
public:

	LeksikaAnalizator(dfa& df, vector<string> KeyWords) {
		this->Dfa = df;
		this->KeyWords = KeyWords;
	}

	//При dfa.accept == true проверяем какой это тип
	string TypeLeksema(string buff) {
		if (buff == "-" || buff == "+" || buff == "=" || buff == "(" || buff == ")" || buff == ",") {
			return "Expresion";
		}
		if (buff[0] == 39) {
			return "String";
		}
		for (int i = 0; i < KeyWords.size(); i++) {
			if (buff == KeyWords[i]) {
				return "KeyWord";
			}
		}
		if (buff[0] >= 'A') {
			return "NameVar";
		}
		return "Int";
	}
	//Функция которая возращает лексему, ее тип, индекс в таблице, если она существует, иначе возращает ошибку
	string addNewElem(string bf) {
		string outSTR;
		if (Dfa.isAccept(bf)) {// Проверяем корректность
			string Type = TypeLeksema(bf);//Определяем тип лексемы
			Token newToken(Type, bf);
			int numHesh = hs.addElement(newToken);//Добавляем в хеш таблицу
			outSTR =  newToken.Type + ' ' + newToken.Leksema + ' ' + to_string(numHesh) + '\n';//Вывод работы программы
			sintak.Erley(newToken); //Вызов синтакс. анализатора
		}
		else {
			outSTR = "WrongString - " + bf + '\n';//Вывод если лексема не может существовать
		}
		return outSTR;
	}

	//Проверяет что строка не пуста и запускает проверку для добавления в таблицу
	void cheker(string& buffer, ofstream& out) {
		if (buffer.size() != 0) {
			out << addNewElem(buffer);
			buffer = "";
		}
	}

	//Разбиваем строку на отдельные слова и каждую отправляем на проверку
	void Parser(string& Code, ofstream& out) {
		string buffer = "";
		for (int i = 0; i < Code.size(); i++) {
			if (Code[i] == '(' || Code[i] == ')' 
				|| Code[i] == '+' || Code[i] == '-' 
				|| Code[i] == '=' || Code[i] == ',')  {
				cheker(buffer, out);
				buffer = Code[i];
				out << addNewElem(buffer);
				buffer = "";
			}
			else if (Code[i] == 39) {
				cheker(buffer, out);
				buffer += Code[i];
				i++;
				while (i < Code.size() && Code[i] != 39) {
					buffer += Code[i];
					i++;
				}
				buffer += Code[i];
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
		//sintak.Erley(*new Token("ss", ";"));
	}

	void StartLeksikaAnalyse(ifstream& in,ofstream& out) {
		string buffer;
		ofstream outTree("Tree.txt");
		out << "WORK PROGRAMM\n";//Начало файла где мы будем показывать по порядку входа что мы сделали с каждым словом
		while (!in.eof()) {// Считываем файл
			getline(in, buffer);
			if(buffer != "")
				this->Parser(buffer, out);
		}
		hs.seeTable(out);//Выводим таблицу
		this->tree = sintak.returnRoot();
		sintak.createTree(tree);
		sintak.printTree(tree,outTree);
		senmatika.SenmaAnalize(tree,"senma.txt");
	}
};

