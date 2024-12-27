#pragma once
#pragma once
#include<vector>
#include<string>
#include<deque>
#include "Token.h"
#include "Tree.h"


struct Situation
{
	string Term;
	vector<string> left;
	deque<string> right;
	vector<Tree*> Node;
	int index;


	bool operator==(Situation second) {
		if (Term != second.Term) {
			return false;
		}
		if (left != second.left) {
			return false;
		}
		if (right != second.right) {
			return false;
		}
		if (index != second.index) {
			return false;
		}
		/*if (Node != second.Node) {
			return false;
		}*/
		return true;
	}
};

struct Grammer {
	string NetermenalName;
	vector<vector<string> > Perehod;
};


class SintaksisAnalizator
{
private:
	vector<Grammer> Gr;
	vector<vector<Situation>> Situations;
	vector<string> Terminal;

public:

	bool IsTerminal(string& S) {
		for (string s : Terminal) {
			if (s == S) {
				return true;
			}
		}
		return false;
	}
	//Найти индек в грамматике
	int IndexGrammer(string S) {
		for (int i = 0; i < Gr.size(); i++) {
			if (S == Gr[i].NetermenalName) {
				return i;
			}
		}
	}
	//Добавить сиуациию на j место и с возможность с ней работать
	void insertINj(bool& flag, int j, Situation& buff, vector<int>& used) {
		for (int i = 0; i < Situations[j].size(); i++) {
			if (Situations[j][i] == buff) {
				return;
			}
		}
		Situations[j].push_back(buff);
		used.push_back(0);
		flag = true;
	}

	void Init() {
		bool flag = true;
		Situation sit;
		sit.Term = "S'";
		sit.index = 0;
		sit.left = {};
		sit.right = { "Program" };
		sit.Node.push_back(new Tree("Program"));
		Situations.push_back({ sit });
		vector<int> used(1, 0);
		for (int k = 0; k < Situations[0].size(); k++) {
			if (used[k] == 0 && Situations[0][k].right.size() != 0) {
				used[k] = 1;
				bool IsTerm = IsTerminal(Situations[0][k].right[0]);
				if (!IsTerm) {
					int i = IndexGrammer(Situations[0][k].right[0]);
					for (int j = 0; j < Gr[i].Perehod.size(); j++) {
						Situation sitBuff;
						sitBuff.Term = Gr[i].NetermenalName;
						sitBuff.index = 0;
						sitBuff.left = {};
						for (int m = Gr[i].Perehod[j].size() - 1; m >= 0; m--) {
							sitBuff.right.push_front(Gr[i].Perehod[j][m]);

							//
							Tree* bf = new Tree(Gr[i].Perehod[j][Gr[i].Perehod[j].size() - 1 - m]);

							Situations[0][k].Node[Situations[0][k].left.size()]->child.push_back(bf);

							sitBuff.Node.push_back(bf);

							//
						}
						insertINj(flag, 0, sitBuff, used);
						//Situations[0].push_back(sitBuff);
						//used.push_back(0);
					}
				}
			}
		}
	}

	SintaksisAnalizator() {
		/*Gr.resize(3);
		Gr[0].NetermenalName = "S";
		Gr[0].Perehod = { {"T","+","S"},{"T"} };

		Gr[1].NetermenalName = "T";
		Gr[1].Perehod = { {"F","*","T"},{"F"} };

		Gr[2].NetermenalName = "F";
		Gr[2].Perehod = { {"n"},{"(","S",")"}};

		Terminal = { "+","*","(",")","n" };*/
		Gr.resize(20);
		//0
		Gr[0].NetermenalName = "Program";
		Gr[0].Perehod = { {"Begin","Descriptions","Operators","End"} };
		//1
		Gr[1].NetermenalName = "Begin";
		Gr[1].Perehod = { {"PROGRAM","Id"} };
		//2
		Gr[2].NetermenalName = "End";
		Gr[2].Perehod = { {"END","Id"} };
		//3
		Gr[3].NetermenalName = "Descriptions";
		Gr[3].Perehod = { {"Descr"},{"Descr","Descriptions"} };
		//4
		Gr[4].NetermenalName = "Operators";
		Gr[4].Perehod = { {"Op" },{"Op","Operators"} };
		//5
		Gr[5].NetermenalName = "Descr";
		Gr[5].Perehod = { {"Type","VarList"} };
		//6
		Gr[6].NetermenalName = "Type";
		Gr[6].Perehod = { {"INTEGER"},{"CHARACTER"} };
		//7
		Gr[7].NetermenalName = "VarList";
		Gr[7].Perehod = { {"Id"},{"Id",",","VarList"}};
		//8
		Gr[8].NetermenalName = "Op";
		Gr[8].Perehod = { {"Id","=","NumExpr"},{"Id","=","StringExpr"} };
		//9
		Gr[9].NetermenalName = "SimpleNumExpr";
		Gr[9].Perehod = { {"Id"},{"Const"},{"(","NumExpr",")"} };
		//10
		Gr[10].NetermenalName = "NumExpr";
		Gr[10].Perehod = { {"SimpleNumExpr"},{"SimpleNumExpr","+","NumExpr"},{"SimpleNumExpr","-","NumExpr"} };
		//11
		Gr[11].NetermenalName = "SimpleStringExpr";
		Gr[11].Perehod = { {"'","String","'",";"}};
		//12
		Gr[12].NetermenalName = "String";
		Gr[12].Perehod = { {"Letter"},{"Digit"},{"Letter","String"},{"Digit","String"} };
		//13
		Gr[13].NetermenalName = "StringExpr";
		Gr[13].Perehod = { {"SimpleStringExpr"},{"SimpleStringExpr","+","StringExpr"} };
		//14
		Gr[14].NetermenalName = "Id";
		Gr[14].Perehod = { {"Letter",";"},{"Letter","Id"}};
		//15
		Gr[15].NetermenalName = "Const";
		Gr[15].Perehod = { {"IntNum",";"}};
		//16
		Gr[16].NetermenalName = "IntNum";
		Gr[16].Perehod = { {"Digit"},{"Digit","IntNum"} };
		//17
		Gr[17].NetermenalName = "Digit";
		Gr[17].Perehod = { {"0"},{"1"}, {"2"}, {"3"}, {"4"}, {"5"}, {"6"}, {"7"}, {"8"}, {"9"} };
		//18
		Gr[18].NetermenalName = "Letter";
		Gr[18].Perehod = { {"a"},{"b"},{"c"},{"d"},{"e"},{"f"},{"g"},{"h"},{"i"},{"j"},{"k"},{"l"},
		{"m"}, {"n"}, {"o"}, {"p"}, {"q"}, {"r"}, {"s"}, {"t"}, {"u"}, {"v"}, {"w"}, {"x"},
		{"y"}, {"z"}, {"A"},{"B"},{"C"},{"D"},{"E"},{"F"},{"G"},{"H"},{"I"},{"J"},{"K"},{"L"},
		{"M"}, {"N"}, {"O"}, {"P"}, {"Q"}, {"R"}, {"S"}, {"T"}, {"U"}, {"V"}, {"W"}, {"X"},
		{"Y"}, {"Z"}, };

		//Gr[19].NetermenalName = "Op";
		//Gr[19].Perehod = { {"Id","=","NumExpr",";"},{"Id","=","StringExpr",";"} };

		Terminal = { "INTEGER","CHARACTER","PROGRAM","END","a","b","c","d","e","f","g","h","i","j","k","l",
		"m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x",
		"y", "z", "A","B","C","D","E","F","G","H","I","J","K","L",
		"M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X",
		"Y", "Z","1","0","2","3","4","5","6","7","8","9","'","(",")",",","=","+","-",";" };
		Init();
	}

	bool Scan(string S) {
		bool flag = false;
		int sz = Situations.size();
		for (int i = 0; i < Situations[sz - 1].size(); i++) {
			if (Situations[sz - 1][i].right.size() != 0 && Situations[sz - 1][i].right[0] == S) {
				Situation buff = Situations[sz - 1][i];

				//
				buff.Node[buff.left.size()]->flag = true;
				//
				buff.left.push_back(buff.right[0]);
				buff.right.pop_front();
				if (!flag) {
					Situations.push_back({ buff });
				}
				else {
					Situations[sz].push_back(buff);
				}
				flag = true;
			}
		}
		return flag;
	}
	bool Predict(vector<int>& used) {
		bool flag = false;
		int sz = Situations.size() - 1;
		for (int k = 0; k < Situations[sz].size(); k++) {
			if (used[k] == 0 && Situations[sz][k].right.size() != 0) {
				used[k] = 1;
				bool IsTerm = IsTerminal(Situations[sz][k].right[0]);
				if (!IsTerm) {
					int i = IndexGrammer(Situations[sz][k].right[0]);
					for (int j = 0; j < Gr[i].Perehod.size(); j++) {
						Situation sitBuff;
						sitBuff.Term = Gr[i].NetermenalName;
						sitBuff.index = sz;
						//sitBuff.left = Situations[sz][k].left;
						for (int m = Gr[i].Perehod[j].size() - 1; m >= 0; m--) {
							sitBuff.right.push_front(Gr[i].Perehod[j][m]);

							Tree* bf = new Tree(Gr[i].Perehod[j][Gr[i].Perehod[j].size() - 1 - m]);

							Situations[sz][k].Node[Situations[sz][k].left.size()]->child.push_back(bf);

							sitBuff.Node.push_back(bf);
						}
						insertINj(flag, sz, sitBuff, used);
						//Situations[sz].push_back(sitBuff);

					}
				}
			}
		}
		return flag;
	}
	bool Complete(vector<int>& used) {
		bool flag = false;
		int sz = Situations.size() - 1;
		for (int k = 0; k < Situations[sz].size(); k++) {
			if (Situations[sz][k].right.size() == 0 && used[k] != 1) {
				used[k] = 1;
				int j = Situations[sz][k].index;
				for (int i = 0; i < Situations[j].size(); i++) {
					if (Situations[j][i].right.size() != 0 && Situations[j][i].right[0] == Situations[sz][k].Term) {
						Situation sitBuff = Situations[j][i];
						sitBuff.left.push_back(sitBuff.right[0]);
						sitBuff.right.pop_front();
						insertINj(flag, sz, sitBuff, used);
					}
				}
			}
		}
		return flag;
	}

	bool Erley(Token token) {
		if (token.Type == "KeyWord") {
			// все слово
			if (Scan(token.Leksema)) {
				bool flag = true;
				vector<int> used(Situations[Situations.size() - 1].size());
				while (flag) {
					flag = false;
					flag = Predict(used);
					flag = Complete(used);
				}
			}
			else {
				cout << "Wrong Sintaksis: " << token.Leksema << "\n";
				exit(0);
			}

		}
		else {
			string buffer = token.Leksema;
			if (token.Type == "String" || token.Type == "NameVar" || token.Type == "Int") {
				token.Leksema += ";";
			}
			string S = "";
			for (int i = 0; i < token.Leksema.size(); i++) {
				if (token.Leksema[i] != ' ') {
					S += token.Leksema[i];
					if (Scan(S)) {
						bool flag = true;
						vector<int> used(Situations[Situations.size() - 1].size());
						while (flag) {
							flag = false;
							flag = Predict(used);
							flag = Complete(used);
						}
					}
					else {
						cout << "Wrong Sintaksis: " << buffer << "\n";
						exit(0);
					}
					S = "";
				}
			}
			// по буквено
		}
	}

	//Вывод лерева(создание отступов в зависимости от глубины)
	void printTree(Tree* root, ofstream& cout, int depth = 0) {
		if (root->Name != ";") {
			for (int i = 0; i < depth; i++) {
				cout << "  ";
			}
			cout << root->Name << "\n";
		}
		for (int i = 0; i < root->child.size(); i++) {
			printTree(root->child[i], cout, depth + 1);
		}


	}
	// Проверка что входной текст дописан до конца
	Tree* returnRoot() {
		int j = Situations.size() - 1;
		for (int i = 0; i < Situations[j].size(); i++) {
			if (Situations[j][i].Term == "S'" && Situations[j][i].index == 0) {
				return Situations[j][i].Node[0];
			}
		}
		cout << "BAD STRING";
		exit(0);
	}


	//Пометка что это нужный нам элемент и потом мы его не удаляем
	bool treeee(Tree* root) {
		for (Tree* s : root->child) {
			if (treeee(s)) {

				root->flag = true;
			}
		}
		return root->flag;
	}

	//Удаляем все что не подошло
	void deleteRoot(Tree* root) {
		for (int i = 0; i < root->child.size(); i++) {
			if (root->child[i]->flag == false) {
				root->child.erase(root->child.begin() + i);
				i--;
				continue;
			}
			deleteRoot(root->child[i]);

		}

	}
	//Убираем все оставшееся что могла появиться(проеряем что все дочернии элементы есть в родительском по грамматике)
	bool clearPerehod(Tree* root) {
		for (int i = 0; i < root->child.size(); i++) {
			if (clearPerehod(root->child[i]) == false) {
				root->child.erase(root->child.begin() + i);
				i--;
			}
		}
		if (!IsTerminal(root->Name)) {
			int i = IndexGrammer(root->Name);
			int flag = 0;
			for (int j = 0; j < Gr[i].Perehod.size(); j++) {
				if (Gr[i].Perehod[j].size() > root->child.size()) {
					flag++;
				}
			}

			if (flag == Gr[i].Perehod.size()) {
				return false;
			}
		}
	}
	//Повторы символов тоже убираем
	void clearPovtor(Tree* root) {
		for (int i = 0; i < root->child.size(); i++) {
			if (root->child[i]->Name == "=") {
				for (int j = i + 1; j < root->child.size(); j++) {
					if (root->child[i]->Name == root->child[j]->Name) {
						root->child.erase(root->child.begin() + j);
						j--;
					}
				}
			}
		}
		for (Tree* s : root->child) {
			clearPovtor(s);
		}
	}
	// Тк изменили граматику(разбили на символы) то собираем их
	void CombineWord(Tree* node, string& S) {
		if (node->child.size() == 0) {
			S += node->Name;
			return;
		}
		for (int i = 0; i < node->child.size(); i++) {
			CombineWord(node->child[i], S);
		}
	}

	void Combine(Tree* root) {
		string S = "";
		if (root->Name == "Id" || root->Name == "Const" || root->Name == "SimpleStringExpr") {
			CombineWord(root, S);
			root->child.clear();
			Tree* leaf = new Tree(S);
			root->child.push_back(leaf);
			return;
		}
		for (int i = 0; i < root->child.size(); i++) {
			Combine(root->child[i]);
		}
	}
	//Объединение всех функция для дерева
	void createTree(Tree* root) {
		treeee(root);
		deleteRoot(root);
		clearPerehod(root);
		clearPovtor(root);
		Combine(root);
		//printTree(root);
	}





};
