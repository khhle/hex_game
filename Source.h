#ifndef __Pm2__
#define __Pm2__

#include <iostream>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <list>
#include <queue>
#include <set>
#include <map>

using namespace std;

class node{

	int weight;
	int value;
	int player;
	int special;

public:
	node() { };
	node(int v, int w) : value(v), weight(w) { };
	void setValue(int);
	void setPlayer(int);
	void setSpecial();
	void setWeight(int);
	int getWeight();
	int getValue();
	int getPlayer();
	int getSpecial();
	friend ostream& operator<< (ostream &out, node &cNode);

};



//Class graph: 
// will have a 2D array with specific size
class graph{
private:
	int size;
	list<int> *adj;
	node* cell;


public:

	//Constructor:
	graph();
	graph(int, double, int);

	//Destructor:
	~graph();

	//Method:
	node* getCell();
	void addEdge(int u, int v);
	void removeEdge(int u, int v);
	void printGraph();
	void fixCell();
	int dsearch(int, int, int);
	int isWin();
	void makeMove(int, int);
	graph* clone();
	int getRandomMove();
	friend ostream& operator<< (ostream &out, graph &cGraph);
	vector<int> getAvailMove();
	bool isLegalMove(int);

};



#endif