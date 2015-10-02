#include <iostream>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <list>
#include <queue>
#include <set>
#include <map>
#include "uct.h"
#include "Source.h"
using namespace std;

double fRand(double fMin, double fMax);



TreeNode::TreeNode(graph* board)
{
	wins = visits = 0;
	untriedMoves = board->getAvailMove();
	move = -1;
	parent = NULL;
}

TreeNode::TreeNode(graph* board, TreeNode* p, int m)
{
	wins = visits = 0;
	untriedMoves = board->getAvailMove();
	move = m;
	parent = p;
	//children = NULL;
}

TreeNode* TreeNode::UCTSelectChild()
{



	TreeNode *selected = NULL;
	double bestValue = -DBL_MAX;
	double epsilon = 1e-6;
	int count = 0;

	vector<TreeNode*>::iterator i;
	for (i = children.begin(); i != children.end(); ++i)
	{

		double uctValue = (*i)->wins / ((*i)->visits + epsilon) + sqrt(log(visits + 1) / ((*i)->visits + epsilon)) + fRand(0, 1) * epsilon;
		if (uctValue > bestValue)
		{

			selected = children[count];
			bestValue = uctValue;
		}
		count++;
	}




	return selected;

}

TreeNode* TreeNode::addChild(int m, graph* board)
{
	TreeNode* node = new TreeNode(board, this, m);
	for (vector<int>::iterator it = untriedMoves.begin(); it != untriedMoves.end(); ++it)
	{
		if (*it == m)
		{
			untriedMoves.erase(it);
			break;
		}
	}

	children.push_back(node);
	return node;
}

void TreeNode::update(double result)
{
	visits++;
	wins += result;
}


int TreeNode::getMove()
{
	return move;
}

void TreeNode::setWin(double w)
{
	wins += w;
}

void TreeNode::incVisit()
{
	visits++;
}

double TreeNode::getVisits()
{
	return visits;
}

TreeNode* TreeNode::getParent()
{
	return parent;
}

bool TreeNode::isUntriedMovesAvail()
{
	return !untriedMoves.empty();
}

bool TreeNode::isLeaf()
{
	return children.empty();
}

int TreeNode::getRandomMove()
{
	int s = untriedMoves.size();
	int n = rand() % (s);
	return untriedMoves[n];

}

int TreeNode::getBestMove()
{
	vector<TreeNode*>::iterator i;
	double best = 0;
	int bestMove = -1;
	double cur;

	for (i = children.begin(); i != children.end(); ++i)
	{
		cur = (*i)->getVisits();
		if (cur > best)
		{
			best = cur;
			bestMove = (*i)->getMove();
		}
	}

	return bestMove;
}



double fRand(double fMin, double fMax)
{
	double f = (double)rand() / RAND_MAX;
	return fMin + f * (fMax - fMin);
}
