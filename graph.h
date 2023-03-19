#ifndef GRAPH_H
#define GRAPH_H
#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include<queue>
#include<iterator>
#include<set>
using namespace std;

class Graph {
	private:
		vector<string>* adjacentList;
		vector<vector<string>> allEdges;
		vector<vector<string>> currentEdges;
		unsigned numOfVertex;
		string * vertexName;
		unsigned findVertexIndex(string vertice);
		bool whetherInSet(vector<string>discoveredList, string vertice);
		queue<string>findNoIncoming();
		vector<vector<string>>findOutcomingEdges(string vertex);
		void removeEdges(vector<vector<string>>outgoingEdges);
		int getIncomingEdgeCount(string vertex);
		int findIndexInCurrentEdges(vector<string>edge);
	public:
		Graph(string filename);
		void printAdjacentList();
		void printAllEdges();
		void printCurrentEdges();
		vector<string> BreadthFirstSearch(string start);
		vector<string> topologicalSort();
		~Graph();
};
#endif // !GRAPH_H
