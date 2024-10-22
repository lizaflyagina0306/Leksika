#pragma once
#include <vector>
#include <string>

using namespace std;

class Token
{
public:
	string Type;
	string Leksema;

	Token(string Type, string Leksema) {
		this->Leksema = Leksema;
		this->Type = Type;
	}
	Token() {
		this->Leksema = "";
		this->Type = "";
	}
};

