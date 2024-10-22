#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include <queue>
#include <set>

using namespace std;
class dfa
{
private:
	int countStates;
	vector<char> alphabet;
	vector<int> finalStates;
	vector<vector<int>> transitFunction;
	int getState(int state, char c);
	set<int> inverse(int state, int k);
public:
	dfa(int countStates, vector<char> alphabet, vector<int> finalStates, vector<vector<int>> transitFunction);
	dfa() {}
	bool isAccept(string s);
	dfa minimize();
	vector<vector<int>> quotientSet();
};

