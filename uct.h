#ifndef __Pm__
#define __Pm__
#include <set>
#include <map>
#include "Source.h"

using namespace std;

class TreeNode{

	int move;
	double wins;
	double visits;
	vector<int> untriedMoves;
	TreeNode* parent;
	vector<TreeNode*> children;

public:
	TreeNode(graph*);
	TreeNode(graph*,TreeNode*,int);
	int getMove();
	double getVisits();
	void setWin(double);
	void incVisit();
	TreeNode* getParent();
	TreeNode* UCTSelectChild();
	TreeNode* addChild(int, graph*);
	void update(double);
	bool isUntriedMovesAvail();
	bool isLeaf();
	int getRandomMove();
	int getBestMove();
};



#endif