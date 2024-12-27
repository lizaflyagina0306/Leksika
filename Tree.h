#pragma once
#pragma once
#include <vector>
#include<string>
#include<iostream>

using namespace std;

class Tree {
private:
public:
	string Name;
	vector<Tree*> child;
	bool flag = false;
	string Type = "";
	string sval = "";
	string Tr = "";
	int count = 0;

	Tree() {
	}
	Tree(string Name) {
		this->Name = Name;
	}

	void AddTree(Tree* NewChild) {
		child.push_back(NewChild);
	}




};
