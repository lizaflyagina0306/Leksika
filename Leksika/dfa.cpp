﻿#include "dfa.h"
#include <string>

dfa::dfa(int countStates, vector<char> alphabet, vector<int> finalStates, vector<vector<int>> transitFunction)
{
	this->countStates = countStates;
	this->alphabet = alphabet;
	this->finalStates = finalStates;
	this->transitFunction.resize(countStates);
	for (int i = 0; i < countStates; i++)
		this->transitFunction[i] = transitFunction[i];
}


// ������� ��������� �������� �� �������� ��������� state � �������� ������� c
int dfa::getState(int state, char c)
{
	int i = 0;
	while (alphabet[i] != c)
	{
		i++;
		if (i == alphabet.size())
			return -1;
	}
	return transitFunction[state][i];
}


// ���������, ����������� �� ������ s ���������
bool dfa::isAccept(string s)
{
	int state = 0;
	for (char c : s)
	{
		state = getState(state, c);
		if (state == -1)
			return false;
	}
	if (find(finalStates.begin(), finalStates.end(), state) != finalStates.end())
		return true;
	return false;
}


// ������� ��� ���������, � ������� ������� ��������� �� k-�� ����������� ������� � ��������� state
set<int> dfa::inverse(int state, int k)
{
	set<int> result;
	for (int i = 0; i < countStates; i++)
		if (transitFunction[i][k] == state)
			result.insert(i);
	return result;
}


// ������ ��������� ������� ��������������� �� ��������� ���������
vector<vector<int>> dfa::quotientSet()
{
	vector<vector<int>> table(countStates, vector<int>(countStates, 0));
	queue<pair<int, int>> q;
	for (int i = 0; i < countStates; i++)
	{
		if (find(finalStates.begin(), finalStates.end(), i) == finalStates.end())
			for (int x : finalStates)
			{
				table[i][x] = 1;
				table[x][i] = 1;
				q.push(pair<int, int>(i, x));
			}
	}
	while (!q.empty())
	{
		pair<int, int> p = q.front();
		q.pop();
		int x = p.first;
		int y = p.second;
		for (int i = 0; i < alphabet.size(); i++)
		{
			auto s1 = inverse(x, i);
			auto s2 = inverse(y, i);
			if (s1.size() != 0 && s2.size() != 0)
			{
				for (int j : s1)
					for (int k : s2)
						if (table[j][k] != 1)
						{
							table[j][k] = 1;
							table[k][j] = 1;
							q.push(pair<int, int>(j, k));
						}
			}
		}
	}
	vector<int> mark(countStates, 0);
	vector<vector<int>> result;
	vector<int> t;
	for (int i = 0; i < countStates; i++)
	{
		if (mark[i] == 0)
		{
			mark[i] = 1;
			t.push_back(i);
			for (int j = i + 1; j < countStates; j++)
				if (table[i][j] == 0)
				{
					mark[j] = 1;
					t.push_back(j);
				}
			result.push_back(t);
			t.clear();
		}

	}
	return result;
}


// ������ ������������� ��� � ����������� ������ ���������
dfa dfa::minimize()
{
	auto q = quotientSet();
	vector<int> minFinalStates;
	int minCountStates = q.size();
	vector<vector<int>> minTransitFunction(minCountStates, vector<int>(alphabet.size(), -1));
	for (int i = 0; i < q.size(); i++)
		for (int j : q[i])
		{
			if (find(finalStates.begin(), finalStates.end(), j) != finalStates.end())
			{
				minFinalStates.push_back(i);
				break;
			}
		}
	for (int i = 0; i < minCountStates; i++)
	{
		for (int j = 0; j < alphabet.size(); j++)
		{
			int k = transitFunction[q[i][0]][j];
			for (int l = 0; l < minCountStates; l++)
				if (find(q[l].begin(), q[l].end(), k) != q[l].end())
				{
					minTransitFunction[i][j] = l;
					break;
				}
		}
	}
	auto minDfa = dfa(minCountStates, alphabet, minFinalStates, minTransitFunction);
	return minDfa;
}
