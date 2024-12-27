#pragma once
#include<vector>
#include<string>
#include<deque>
#include "Token.h"
#include "Tree.h"
#include <map>
#include <fstream>

class Senma {
private:
	Tree* root;
	map<string, string> mp;
	ofstream cout;
public:

	string Descriptions(Tree* node) {
		if (node->child.size() == 1) {
			node->Tr = Descr(node->child[0]);
		}
		else {
			Descr(node->child[0]);
			Descriptions(node->child[1]);
			node->Tr = node->child[0]->Tr + "\n" + node->child[1]->Tr;
		}
		return node->Tr;
	}

	string Descr(Tree* node) {
		node->child[1]->Type = Type(node->child[0]);
		string sbf = VarList(node->child[1]);
		int bf = node->child[1]->count + 1;
		node->Tr = node->child[0]->Type + " " + sbf + " " + to_string(bf) + " DECL";
		return node->Tr;
	}

	string VarList(Tree* node) {
		if (node->child.size() == 1) {
			node->count++;
			node->Tr = Id(node->child[0]);
			if (mp.find(node->child[0]->Tr) != mp.end()) {
				cout << "Repeated creation of a variable\n";
			}
			else {
				node->child[0]->Type = node->Type;
				mp.insert({ node->child[0]->Tr ,node->child[0]->Type });
			}
		}
		else {
			node->count++;
			node->child[2]->Type = node->Type;
			node->Tr = Id(node->child[0]) + " " + VarList(node->child[2]);
			node->count += node->child[2]->count;
			if (mp.find(node->child[0]->Tr) != mp.end()) {
				cout << "Repeated creation of a variable\n";
			}
			else {
				node->child[0]->Type = node->Type;
				mp.insert({ node->child[0]->Tr ,node->child[0]->Type });
			}
		}
		return node->Tr;
	}

	string Type(Tree* node) {
		node->Type = node->child[0]->Name;
		return node->child[0]->Name;
	}

	string Const(Tree* node) {
		node->Type = "INTEGER";
		node->Tr = node->child[0]->Name;
		node->Tr.pop_back();
		return node->Tr;
	}

	string Id(Tree* node) {
		//node->Type = "CHARACTER";
		node->Tr = node->child[0]->Name;
		node->Tr.pop_back();
		return node->Tr;
	}

	string IntNum(Tree* node) {
		node->Tr = node->child[0]->Name;
		node->Tr.pop_back();
		return node->Tr;
	}


	string NumExprMain(Tree* node) {
		if (node->child.size() == 1) {
			node->Tr = SimpleNumExpr(node->child[0]);
			node->Type = node->child[0]->Type;
		}
		else {
			NumExpr(node->child[2]);
			node->Tr = SimpleNumExpr(node->child[0]) + " " + node->child[2]->child[0]->Tr + " " + node->child[1]->Name;
			if (node->child[0]->Type != node->child[2]->Type || node->child[0]->Type != "INTEGER"
				|| node->child[2]->Type != "INTEGER") {
				cout << "Different data type\n";
			}
			if (node->child[2]->child.size() != 1) {
				node->Tr += " " + node->child[2]->Tr;
			}
			node->Type = "INTEGER";
		}
		//node->Type = "INTEGER";
		return node->Tr;
	}

	string NumExpr(Tree* node) {
		if (node->child.size() == 1) {
			node->Tr = SimpleNumExpr(node->child[0]);
			node->Type = node->child[0]->Type;
		}
		else {
			NumExpr(node->child[2]);
			SimpleNumExpr(node->child[0]);
			if (node->child[0]->Type != node->child[2]->Type || node->child[0]->Type != "INTEGER"
				|| node->child[2]->Type != "INTEGER") {
				cout << "Different data type\n";
			}
			if (node->child[2]->child.size() == 3 && node->child[2]->child[0]->Name != "(") {
				node->Tr = node->child[2]->child[0]->Tr + " " + node->child[1]->Name + " " + node->child[2]->Tr;
				//node->Tr += " " + node->child[2]->Tr;
			}
			else {
				node->Tr = node->child[2]->Tr + " " + node->child[1]->Name;
			}
			node->Type = "INTEGER";
		}
		//node->Type = "INTEGER";
		return node->Tr;
	}



	string SimpleNumExpr(Tree* node) {
		if (node->child[0]->Name == "Id") {
			node->Tr = Id(node->child[0]);
			if (mp.find(node->child[0]->Tr) != mp.end()) {
				node->Type = mp.find(node->child[0]->Tr)->second;
			}
			else {
				cout << "The variable does not exist\n";
			}
		}
		else if (node->child[0]->Name == "Const") {
			node->Tr = Const(node->child[0]);
			node->Type = node->child[0]->Type;
		}
		else {
			node->Tr = NumExprMain(node->child[1]);
			node->Type = node->child[1]->Type;
		}
		return node->Tr;
	}

	string StringExprMain(Tree* node) {
		if (node->child.size() == 1) {
			node->Tr = SimpleStringExpr(node->child[0]);
			node->Type = node->child[0]->Type;
		}
		else {
			StringExpr(node->child[2]);
			node->Tr = SimpleStringExpr(node->child[0]) + " " + node->child[2]->child[0]->Tr + " " + node->child[1]->Name;
			if (node->child[2]->child.size() != 1) {
				node->Tr += " " + node->child[2]->Tr;
			}

		}
		node->Type = node->child[0]->Type;
		return node->Tr;
	}

	string StringExpr(Tree* node) {
		if (node->child.size() == 1) {
			node->Tr = SimpleStringExpr(node->child[0]);
			node->Type = node->child[0]->Type;
		}
		else {
			StringExpr(node->child[2]);
			SimpleStringExpr(node->child[0]);

			if (node->child[2]->child.size() == 3) {
				node->Tr = node->child[2]->child[0]->Tr + " " + node->child[1]->Name + " " + node->child[2]->Tr;
				//node->Tr += " " + node->child[2]->Tr;
			}
			else {
				node->Tr = node->child[2]->Tr + " " + node->child[1]->Name;
			}
		}
		node->Type = node->child[0]->Type;
		return node->Tr;

	}

	string SimpleStringExpr(Tree* node) {
		//node->Tr = "'" + String(node->child[1]) + "'";
		node->Type = node->child[1]->Type;
		return node->Tr;
	}


	string Op(Tree* node) {
		if (node->child[2]->Name == "NumExpr") {
			node->Tr = Id(node->child[0]) + " " + NumExprMain(node->child[2]) + " =";
		}
		else {
			node->Tr = Id(node->child[0]) + " " + StringExprMain(node->child[2]) + " =";
		}
		if (mp.find(node->child[0]->Tr) != mp.end()) {
			if (mp.find(node->child[0]->Tr)->second != node->child[2]->Type) {
				cout << "Different data type\n";
			}
		}
		else {
			cout << "The variable does not exist\n";
		}
		node->Type = node->child[2]->Type;
		return node->Tr;
	}

	string Operators(Tree* node) {
		if (node->child.size() == 1) {
			node->Tr = Op(node->child[0]);
		}
		else {
			node->Tr = Op(node->child[0]) + "\n" + Operators(node->child[1]);
		}
		return node->Tr;
	}

	string Begin(Tree* node) {
		node->Tr = node->child[0]->Name + " " + Id(node->child[1]) + " 2 DEFL";
		return node->Tr;
	}

	string End(Tree* node) {
		node->Tr = node->child[0]->Name + " " + Id(node->child[1]) + " 2 DEFL";
		return node->Tr;
	}

	void SenmaAnalize(Tree* node,string name) {
		cout.open(name);
		cout << "Error\n----------------------------\n";
		Begin(node->child[0]);
		Descriptions(node->child[1]);
		Operators(node->child[2]);
		End(node->child[3]);
		node->Tr = node->child[0]->Tr + '\n' + node->child[1]->Tr + '\n' +
			node->child[2]->Tr + '\n' + node->child[3]->Tr;
		if (node->child[0]->child[1]->Tr != node->child[3]->child[1]->Tr) {
			cout << "Different Begin and End\n";
		}
		cout << "----------------------------\nResult\n----------------------------\n";
		cout << node->Tr;
	}

};
