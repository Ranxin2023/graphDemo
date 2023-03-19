#include "graph.h"



Graph::Graph(string filename){
	ifstream inFile(filename,ios::in);
	if (!inFile) {
		cerr << "Cannot open the file" << endl;
		return;
	}
	string input;
	string vertice;
	unsigned vertexIndex=0;
	string currentVertex;
	vector<string>emptyVector;
	vector<string>newVector;
	inFile >> input;
	this->numOfVertex = stoi(input);
	//cout << "number of vertex is" << numOfVertex << "." << endl;
	this->adjacentList = new vector<string>[this->numOfVertex];
	this->vertexName = new string[this->numOfVertex];
	while (inFile>>input) {
		if (input[input.length() - 1] == ':') {
			vertexIndex++;
			vertice = input.substr(0,input.length()-1);
			this->vertexName[vertexIndex-1]=vertice;
			currentVertex = vertice;
			
		}
		else if(input[input.length() - 1] == ','){
			vertice = input.substr(0,input.length()-1);
			this->adjacentList[vertexIndex-1].push_back(vertice);
			newVector = emptyVector;
			newVector.push_back(currentVertex);
			newVector.push_back(vertice);
			this->allEdges.push_back(newVector);
			
		}
		else {
			this->adjacentList[vertexIndex - 1].push_back(input);
			newVector = emptyVector;
			newVector.push_back(currentVertex);
			newVector.push_back(input);
			this->allEdges.push_back(newVector);
		}
	}
	this->currentEdges = this->allEdges;
}

void Graph::printAdjacentList()
{
	int i;
	for (i = 0; i < numOfVertex; i++) {
		cout << "The adjacent list of "<<this->vertexName[i]<<":"<<endl;
		for (string value : this->adjacentList[i]) {
			cout << value << "\t";
		}
		cout << endl;
	}
}

void Graph::printAllEdges() {
	for (vector<string>eachEdge : this->allEdges) {
		cout << eachEdge[0] << "->"<<eachEdge[1]<<endl;
	}

}

void Graph::printCurrentEdges() {
	for (vector<string>eachEdge : this->currentEdges) {
		cout << eachEdge[0] << "->" << eachEdge[1] << endl;
	}
}

unsigned Graph::findVertexIndex(string vertice)
{
	unsigned i;
	for (i = 0; i < this->numOfVertex; i++) {
		if(this->vertexName[i]==vertice)
		return i;
	}
}

bool Graph::whetherInSet(vector<string> discoveredList, string vertice)
{
	for (string value: discoveredList) {
		if (value == vertice)return true;
	}
	return false;
}

queue<string> Graph::findNoIncoming()
{
	bool whetherNoIncoming=true;
	queue<string>noIncoming;
	for (int i = 0; i < this->numOfVertex;i++) {
		for (vector<string> eachEdge : this->currentEdges) {
			if (this->vertexName[i] == eachEdge[1])whetherNoIncoming = false;
		}
		if (whetherNoIncoming)noIncoming.push(this->vertexName[i]);
		whetherNoIncoming = true;
	}
	return noIncoming;
}

vector<vector<string>> Graph::findOutcomingEdges(string vertex)
{
	//cout << "findOutcoming:" <<vertex<< endl;
	vector<vector<string>>outcomingEdges;
	for (vector<string>eachEdge : this->currentEdges) {
		//cout << eachEdge[1] << endl;
		if (eachEdge[0] == vertex) {
			//cout << eachEdge[1] << endl;
			outcomingEdges.push_back(eachEdge);
		}
	}
	/*for (vector<string>eachOutgoing1 : outcomingEdges) {
		cout << eachOutgoing1[0] << "->" << eachOutgoing1[1] << endl;
	}*/
	return outcomingEdges;
}

void Graph::removeEdges(vector<vector<string>>outgoingEdges)
{
	int j = 0;
	for (int i = 0; i < this->allEdges.size(); i++) {
		//cout << "breakPoint2.1" << endl;
		if (outgoingEdges[j] == allEdges[i]) {
			
			int index = findIndexInCurrentEdges(outgoingEdges[j]);
			currentEdges.erase(currentEdges.begin()+index);
			j++;
			/*
			cout << "currentEdges:" << endl;
			for (vector<string>eachOutgoing1 : currentEdges) {
				cout << eachOutgoing1[0] << "->" << eachOutgoing1[1] << endl;
			}*/
			if (j == outgoingEdges.size())break;
		}
	}
	
}

int Graph::getIncomingEdgeCount(string vertex)
{
	int count = 0;
	for (vector<string>eachEdge : currentEdges) {
		if (eachEdge[1] == vertex)count++;
	}
	return count;
}

int Graph::findIndexInCurrentEdges(vector<string> edge)
{
	int index = 0;
	for (vector<string>eachEdge : currentEdges) {
		if (eachEdge == edge)return index;
		index++;
	}
	return -1;
}

vector<string> Graph::BreadthFirstSearch(string start)
{
	vector<string>discoveredSet;
	queue <string>frontierQueue;
	string currentVertice;
	int vertexIndex;
	frontierQueue.push(start);
	discoveredSet.push_back(start);
	while (!frontierQueue.empty()) {
		currentVertice = frontierQueue.front();
		frontierQueue.pop();
		vertexIndex = this->findVertexIndex(currentVertice);
		for (string adjV : this->adjacentList[vertexIndex]) {
			if (!this->whetherInSet(discoveredSet, adjV)) {
				frontierQueue.push(adjV);
				discoveredSet.push_back(adjV);
			}
		}
	}
	return discoveredSet;
}

vector<string> Graph::topologicalSort()
{
	vector<string>resultList;
	queue<string>noIncoming=findNoIncoming();
	vector<vector<string>>outgoingEdges;
	while (!noIncoming.empty()) {
		//cout << "Loop" << endl;
		string curV = noIncoming.front();
		noIncoming.pop();
		resultList.push_back(curV);
		outgoingEdges = findOutcomingEdges(curV);
		/*
		cout<<"breakpoint1"<<endl;
		
		for (vector<string>eachOutgoing1 : outgoingEdges) {
			cout << eachOutgoing1[0] << "->" << eachOutgoing1[1] << endl;
		}
		*/
		if(outgoingEdges.size())
		removeEdges(outgoingEdges);
		for (vector<string> eachOutgoing : outgoingEdges) {
			int inCount = getIncomingEdgeCount(eachOutgoing[1]);
			if (inCount == 0) {
				noIncoming.push(eachOutgoing[1]);
			}
		}
		outgoingEdges.clear();
	}
	return resultList;
}

Graph::~Graph()
{
	delete [] this->adjacentList;
	delete [] this->vertexName;
	cout << "Freed successfully" << endl;
}
