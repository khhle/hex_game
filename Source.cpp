//Name: Khanh Le
//Date: Feb 17
//




#include <iostream>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <list>
#include <queue>
#include <set>
#include <map>
#include "Source.h"
#include "uct.h"

using namespace std;
const int MAXINT = 32767;
const int SIZE1 = 4;

//Macro for inline function:
inline double probability(){ return 1.0*rand() / RAND_MAX; }; //return a small random number




void node::setValue(int nValue)
{
	value = nValue;
}
void node::setPlayer(int nPlayer)
{
	player = nPlayer;
}

void node::setSpecial()
{
	if (value == 0 || value == SIZE1 - 1 || value == SIZE1* SIZE1 - 1 || value == SIZE1 * (SIZE1 - 1))
		special = -1;
	else if (value < SIZE1)
		special = 1;
	else if (value % SIZE1 == 0)
		special = 2;
	else if (value % SIZE1 == SIZE1 - 1)
		special = 4;
	else if (value / SIZE1 == SIZE1 - 1)
		special = 3;
	else
		special = 0;

	//special = n;
}
int node::getValue()
{
	return value;
}
int node::getPlayer()
{
	return player;
}

int node::getSpecial()
{
	return special;
}

int node::getWeight()
{
	return weight;
}
void node::setWeight(int n)
{
	weight = n;
}
ostream& operator<< (ostream &out, node &cNode)
{
	out << cNode.getValue();
	return out;
}


class CompareGreater {
public:
	bool const operator()(node &nodeX, node &nodeY) {
		return (nodeX.getWeight() > nodeY.getWeight());
	}
};






//Default constructor:
graph::graph()
{
	this->size = 0;
	this->adj = NULL;
}

//Constructor with specific size
graph::graph(int s, double density, int range){
	
	this->size = s*s;
	
	this->adj = new list<int>[size];
	this->cell = new node[size];
	int sizeN = sqrt(size);
	for (int i = 0; i < size; i++)
	{
		cell[i].setValue(i);
		cell[i].setSpecial();
		cell[i].setWeight(1);
		cell[i].setPlayer(-1);	
	}

}


//Destructor
graph::~graph()
{
	//cout << "Des" << endl;
	//If there is nothing to delete, return
	if (!this->size)
		return;
	delete[] adj;
	delete[] cell;
	
}

node* graph::getCell()
{
	return cell;
}

void graph::addEdge(int u, int v)
{
	adj[u].push_back(v);
}

void graph::removeEdge(int u, int v)
{
	cout << u << " and " << v << endl;
	adj[u].remove(v);
}
//Graph::printGraph: print the 2D array
//input: none
//output: none
void graph::printGraph()
{
	int i;

	cout << "Graph size " << this->size << endl;


	int numb = 0;
	for (i = 0; i < this->size; ++i)
	{
		if (this->cell[i].getPlayer() == -1 )
			cout << " " << "." << " ";
		else
			cout << " " << this->cell[i].getPlayer() << " ";
		//cout << " " << this->cell[i].getValue() << " ";
		numb++;
		if (numb == sqrt(size))
		{
			cout << endl;
			//cout << " " ;
			numb = 0;
		}

	}

}


//input:
//output: 0 if player0 win, 1 if player1 win, -1 if none
int graph::isWin()
{
	int found = 0;
	int i = 0;
	int j = 0;
	
	//up down win
	while (i<SIZE1 && !found)
	{
		j = SIZE1*SIZE1 - SIZE1;
		while (j < SIZE1*SIZE1 && !found)
		{
			found = dsearch(i, j,0);
			j++;
		}
		i++;
	}

	//if found then up down win -> return
	if (found)
		return 0;

	i = 0;
	j = SIZE1 - 1;
	//lef right win
	while (i < SIZE1*SIZE1 && !found)
	{
		j = SIZE1 - 1;
		while (j < SIZE1*SIZE1 && !found)
		{
			found = dsearch(i, j,1);
			j += SIZE1;
		}
		i += SIZE1;
	}

	//if found then left right win -> return
	if (found)
		return 1;

	
	// else return -1
	return -1;

}


//fixCell: re-fill the value of adj matrix, if player 2 pick a node, then that node is no longer connected
//input: none
//output: none
void graph::fixCell()
{

	
	for (int i = 0; i < size; i++)
	{
		for (list<int>::iterator it = adj[i].begin(); it != adj[i].end();)
		{
			
			if (cell[*it].getPlayer() != cell[i].getPlayer())

				it = adj[i].erase(it);
			else
				++it;
	
		}
	}



	

	
}


void graph::makeMove(int input, int player)
{
	cell[input].setPlayer(player);
	
	int i = input;
	int sizeN = SIZE1;
	if (cell[i].getSpecial() == 1)
	{
		addEdge(i, i - 1);
		addEdge(i, i + 1);
		addEdge(i, i + sizeN - 1);
		addEdge(i, i + sizeN);
		//cout << i << ": " << i - 1 << " " << i + 1 << " " << i + sizeN - 1 << " " << i + sizeN << endl;
	}
	else if (cell[i].getSpecial() == 3)
	{
		addEdge(i, i - 1);
		addEdge(i, i + 1);
		addEdge(i, i - sizeN + 1);
		addEdge(i, i - sizeN);
		//cout << i << ": " << i - 1 << " " << i + 1 << " " << i - sizeN + 1 << " " << i - sizeN << endl;
	}
	else if (cell[i].getSpecial() == 2)
	{
		addEdge(i, i + 1);
		addEdge(i, i + sizeN);
		addEdge(i, i - sizeN);
		addEdge(i, i - sizeN + 1);
		//cout << i << ": " << i + 1 << " " << i + sizeN << " " << i - sizeN << " " << i - sizeN + 1 << endl;
	}
	else if (cell[i].getSpecial() == 4)
	{
		addEdge(i, i - 1);
		addEdge(i, i + sizeN);
		addEdge(i, i - sizeN);
		addEdge(i, i + sizeN - 1);
		//cout << i << ": " << i - 1 << " " << i + sizeN << " " << i - sizeN << " " << i + sizeN - 1 << endl;
	}
	else if (cell[i].getSpecial() == 0)
	{
		addEdge(i, i + 1);
		addEdge(i, i - 1);
		addEdge(i, i + sizeN);
		addEdge(i, i - sizeN);
		addEdge(i, i - sizeN + 1);
		addEdge(i, i + sizeN - 1);
		//cout << i << ": " << i - 1 << " " << i + 1 << " " << i + sizeN - 1 << " " << i + sizeN << " " << i - sizeN << " " << i - sizeN + 1 << endl;
	}
	else
	{
		if (i == 0)
		{
			addEdge(i, i + 1);
			addEdge(i, i + sizeN);
			//cout << i << ": " << i + 1 << " " << i + sizeN << endl;
		}
		else if (i == sizeN - 1)
		{

			addEdge(i, i - 1);
			addEdge(i, i + sizeN);
			addEdge(i, i + sizeN - 1);
			//cout << i << ": " << i - 1 << " " << i + sizeN - 1 << " " << i + sizeN << endl;
		}
		else if (i == size - 1)
		{

			addEdge(i, i - 1);
			addEdge(i, i - sizeN);
			//cout << i << ": " << i - 1 << " " << i - sizeN << endl;
		}
		else if (i / sizeN == sizeN - 1)
		{

			addEdge(i, i + 1);
			addEdge(i, i - sizeN);
			addEdge(i, i - sizeN + 1);
			//cout << i << ": " << i + 1 << " " << i - sizeN + 1 << " " << i - sizeN << endl;
		}

	}


	for (list<int>::iterator it = adj[input].begin(); it != adj[input].end();)
	{
		
		if (cell[*it].getPlayer() != -1 && cell[*it].getPlayer() != cell[input].getPlayer())
			it = adj[input].erase(it);
		else
			++it;

	}
}


int graph::dsearch(int start,int end,int player) {
	priority_queue<node, vector<node>, CompareGreater> Q;
	vector<int> weights;

	if (cell[start].getPlayer() == -1 || cell[end].getPlayer() == -1)
		return 0;
	

	if (cell[start].getPlayer() != player || cell[end].getPlayer() != player)
		return 0;
	for (int i = 0; i < size;i++)
		weights.push_back(INT_MAX);


	vector <bool> visited(size, false);
	visited[start] = true;
	weights[start] = 0;
	Q.push(cell[start]);
	while (!Q.empty() && !visited[end])
	{
		int current = Q.top().getValue();
		if (current == end)
		{
			//cout << start << " " << current << " " << end << endl;
			return 1;
		}
		Q.pop();
		visited[current] = true;
		//cout << current << endl;
		for (list<int>::iterator it = adj[current].begin(); it != adj[current].end(); ++it)
		{
			//cout << *it << endl;
			if (cell[*it].getPlayer() == player || cell[*it].getPlayer() == -1)
				if (!visited[*it])
				{
					weights[*it] = min(weights[*it], weights[current] + 1);
					Q.push(cell[*it]);
				}
		}
	}

	return 0;
}

graph* graph::clone()
{
	graph* newBoard;

	newBoard = new graph(SIZE1, 0.1, 2);
	node* newCell = newBoard->getCell();
	for (int i = 0; i < size; i++)
		newCell[i].setPlayer(cell[i].getPlayer());

	
	int player;
	for (int i = 0; i < size; i++)
	{
		player = newCell[i].getPlayer();
		if (player != -1)
		{
			newBoard->makeMove(i, player);
		}
	}

	return newBoard;

}

int graph::getRandomMove()
{
	bool isFill = false;
	int i = rand() % (SIZE1*SIZE1);
	while (!isFill)
	{
		if (cell[i].getPlayer() == -1)
		{
			isFill = true;
			return i;
		}
		else
			i = rand() % (SIZE1*SIZE1);
		//cout << "stuck";
	}
	return 0;
}

vector<int> graph::getAvailMove()
{
	vector<int> availMove;
	for (int i = 0; i < size; i++)
	{
		if (cell[i].getPlayer() == -1)
			availMove.push_back(i);
	}



	return availMove;
}

bool graph::isLegalMove(int m)
{
	if (cell[m].getPlayer() == -1)
		return true;
	return false;
}

//Overloading <<
//THIS STILL NEED TO FIX
ostream& operator<< (ostream &out, graph &cGraph)
{

	out << cGraph.cell;
	return out;
}

int AImove(graph* board)
{


	TreeNode * rootNode = new TreeNode(board);
	TreeNode *node = NULL;
	list<TreeNode*> listNode; //Use to keep track of all node in tree -> to destroy later
	int i = 0;

	listNode.push_back(rootNode);
	while (i < 100)
	{
		i++;
		node = rootNode;
		graph* localBoard = board->clone();

		//Select
		while (node->isUntriedMovesAvail() == false && !node->isLeaf()) // node is fully expanded and non - terminal
		{
			node = node->UCTSelectChild();
			localBoard->makeMove(node->getMove(), 0);
		}

		//Expand
		if (node->isUntriedMovesAvail())
		{
			int m = -1;
			bool isLegal = false;
			while (!isLegal)
			{
				m = node->getRandomMove();
				isLegal = localBoard->isLegalMove(m);
			}

			localBoard->makeMove(m, 0);
			node = node->addChild(m, localBoard);
			listNode.push_back(node);
		}

		int randomMove = -1;
		//Rollout
		while (localBoard->isWin() == -1)
		{
			//check becuz getrandom will return some used move
			randomMove = localBoard->getRandomMove();

			localBoard->makeMove(randomMove, 1);

			if (localBoard->isWin() != -1)
				break;

			randomMove = localBoard->getRandomMove();
			localBoard->makeMove(randomMove, 0);


		}

		//Backpropagate
		while (node)
		{
			if (node->getParent() == NULL)
				node->setWin(0);
			else
			{
				if (localBoard->isWin() == 1)
					node->setWin(0);
				else if (localBoard->isWin() == 0)
					node->setWin(1);
				//node->setWin(localBoard->isWin());
			}

			node->incVisit();
			node = node->getParent();

		}

		delete localBoard;

	}

	int bestMove = rootNode->getBestMove();

	//destroy all node in tree
	for (list<TreeNode*>::iterator it = listNode.begin(); it != listNode.end(); ++it)
		delete *it;

	return bestMove;

}



//Main:
int main()
{
	graph* board;
	

	srand(time(0));


	cout << "Game board:" << endl;
	board = new graph(SIZE1, 0.1, 2);
	board->printGraph();
	cout << "Player 1 pick 0, player 2 pick 1" << endl;

	int input;
	int isWin = -1;
	while (isWin == -1)
	{
		cout << "Enter your move: ";
		cin >> input;
		board->makeMove(input, 0);

		input = AImove(board);
		board->makeMove(input, 1);

		board->printGraph();
		isWin = board->isWin();
		cout << " win " << isWin << endl;
	}

	if (isWin == 0)
		cout << "Player 0 Win" << endl;
	else
		cout << "Player 1 Win" << endl;



	//Delete allocated memory to prevent leak
	delete board;
	system("pause");

	return 0;
}