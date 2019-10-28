#ifndef GRAPH_H
#define GRAPH_H
#include <iostream>

using namespace std;

template <class Type, class SizeT = size_t>
class Node {
	SizeT parentsSize_;
	Type * parentsData_;

public:
	Type Num;

	Node();
	Node(Type num, Type parents[], SizeT parentsSize);
	Node(const Node<Type, SizeT>& node);
	~Node() {
		delete[] parentsData_;
	};

	Node<Type, SizeT>& operator =(const Node<Type, SizeT>& g);
	bool operator ==(const Node<Type, SizeT>& g);

	friend istream& operator >>(istream & is, Node<Type, SizeT> & node) {
		Type c = 1;
		is >> node.Num;
		while (c != 0)
		{
			is >> c;
			while (is.fail())
			{
				is.clear();
				is.ignore(numeric_limits<streamsize>::max(), '\n');
				is >> c;
			}
			if (c != 0)
				node.AddParent(c);
		}
		return is;
	};
	friend ostream& operator <<(ostream & os, Node<Type, SizeT> const& node) {
		os << "Num: " << node.Num << ",\tParents: ";

		SizeT parentsSize = node.GetParentsSize();
		Type * parentsData = node.GetParentsData();

		for (SizeT i = 0; i != parentsSize; ++i)
			os << parentsData[i] << ",\t";
		os << "\n\n";
		return os;
	};

	SizeT GetParentsSize() const { return parentsSize_; };
	Type * GetParentsData() const { return parentsData_; };
	int IndexOf(Type parent);
	void AddParent(Type parent);
	void SetParents(Type parents[], SizeT parentsSize);
	bool CheckParents(Type parents[], SizeT parentsSize);
	void Swap(Node<Type, SizeT>& node);
	void Print();
};

template <class Type, class ParentType, class SizeT = size_t, class ParentSizeT = size_t>
class Graph {
	SizeT nodesSize_;
	Type * nodesData_;

public:
	Graph();
	Graph(const Graph<Type, ParentType, SizeT, ParentSizeT>& graph);
	~Graph() {
		delete[] nodesData_;
	};

	Graph<Type, ParentType, SizeT, ParentSizeT>& operator =(const Graph<Type, ParentType, SizeT, ParentSizeT>& graph);

	friend istream& operator >>(istream & is, Graph<Type, ParentType, SizeT, ParentSizeT> & graph) {
		ParentType c = 1;
		while (true)
		{
			Type node;
			is >> node.Num;
			while (is.fail() || node.Num == 0)
			{
				is.clear();
				is.ignore(numeric_limits<streamsize>::max(), '\n');
				is >> node.Num;
			}
			if (node.Num == -1)
				break;

			while (true)
			{
				is >> c;
				while (is.fail())
				{
					is.clear();
					is.ignore(numeric_limits<streamsize>::max(), '\n');
					is >> c;
				}
				if (c == 0 || c == -1)
					break;
				else
					node.AddParent(c);
			}
			graph.Add(node);
			if (c == -1)
				break;
		}
		return is;
	};
	friend ostream& operator <<(ostream & os, Graph<Type, ParentType, SizeT, ParentSizeT> const& graph) {
		os << "nodesSize_: " << graph.nodesSize_ << "\n";
		for (SizeT i = 0; i != graph.nodesSize_; ++i)
		{
			Type node = graph.nodesData_[i];
			os << "Num: " << node.Num << ",\tParents: ";

			ParentSizeT parentsSize = node.GetParentsSize();
			ParentType * parentsData = node.GetParentsData();

			for (ParentSizeT i = 0; i != parentsSize; ++i)
				os << parentsData[i] << ",\t";
			os << "\n\n";
		}
		return os;
	};

	void Shift(Type node);
	void Add(Type node);
	void Remove(Type node);
	void Clear();
	void Print();
	void Info();
	void Swap(Graph<Type, ParentType, SizeT, ParentSizeT>& graph);
	int IndexOf(ParentType num);
	Type Find(ParentType num);
	Graph<Type, ParentType, SizeT, ParentSizeT> FindParentNodes(ParentType num);
	Graph<Type, ParentType, SizeT, ParentSizeT> FindChildrenNodes(ParentType num);
	Graph<Type, ParentType, SizeT, ParentSizeT> Path(Type node1, Type node2, bool inverseChild = false);
	void IterateNodes(Type startNode, Graph<Type, ParentType, SizeT, ParentSizeT> checkedNodes);
};

// Node constructors
template <class Type, class SizeT>
Node<Type, SizeT>::Node()
	: Num(0), parentsSize_(0), parentsData_(new Type[parentsSize_]) {}

template <class Type, class SizeT>
Node<Type, SizeT>::Node(Type num, Type parents[], SizeT parentsSize)
	: Num(num), parentsSize_(parentsSize), parentsData_(new Type[parentsSize_]) {
	for (SizeT i = 0; i != parentsSize_; ++i)
		parentsData_[i] = parents[i];
}
template <class Type, class SizeT>
Node<Type, SizeT>::Node(const Node<Type, SizeT>& node)
	: Num(node.Num), parentsSize_(node.parentsSize_), parentsData_(new int[parentsSize_]) {
	for (SizeT i = 0; i != parentsSize_; ++i)
		parentsData_[i] = node.parentsData_[i];
}

// Node operators
template <class Type, class SizeT>
Node<Type, SizeT> & Node<Type, SizeT>::operator =(const Node<Type, SizeT> &node) {
	if (this != &node)
		Node<Type>(node).Swap(*this);
	return *this;
}
template <class Type, class SizeT>
bool Node<Type, SizeT>::operator ==(const Node<Type, SizeT> &node) {
	if (Num != node.Num ||
		!CheckParents(node.parentsData_, node.parentsSize_))
		return false;

	return true;
}

// Node methods
// Find index of parent
template <class Type, class SizeT>
int Node<Type, SizeT>::IndexOf(Type parent) {
	for (SizeT i = 0; i != parentsSize_; ++i)
		if (parentsData_[i] == parent)
			return i;

	return -1;
}
// Add parent
template <class Type, class SizeT>
void Node<Type, SizeT>::AddParent(Type parent) {
	if (IndexOf(parent) != -1)
	{
		cout << "This node already exist\n\n";
		return;
	}

	Type * parents = new Type[parentsSize_ + 1];
	for (SizeT i = 0; i != parentsSize_; ++i)
		parents[i] = parentsData_[i];
	if (parentsSize_ != 0)
		delete[] parentsData_;

	parents[parentsSize_] = parent;
	parentsData_ = parents;
	++parentsSize_;
}
// Set parents nums
template <class Type, class SizeT>
void Node<Type, SizeT>::SetParents(Type parents[], SizeT parentsSize) {
	if (parentsSize_ != 0)
		delete[] parentsData_;
	parentsSize_ = parentsSize;
	parentsData_ = new Type[parentsSize_];

	for (SizeT i = 0; i != parentsSize_; ++i)
		parentsData_[i] = parents[i];
}
// Check parents of node
template <class Type, class SizeT>
bool Node<Type, SizeT>::CheckParents(Type parents[], SizeT parentsSize) {
	if (parentsSize_ != parentsSize)
		return false;

	for (SizeT i = 0; i != parentsSize_; ++i)
		if (parentsData_[i] != parents[i])
			return false;

	return true;
}
// Swap two nodes
template <class Type, class SizeT>
void Node<Type, SizeT>::Swap(Node<Type, SizeT>& node) {
	Type const n = Num;
	Num = node.Num;
	node.Num = n;

	SizeT const s = parentsSize_;
	parentsSize_ = node.parentsSize_;
	node.parentsSize_ = s;

	Type * const d = parentsData_;
	parentsData_ = node.parentsData_;
	node.parentsData_ = d;
}
// Print info about node
template <class Type, class SizeT>
void Node<Type, SizeT>::Print() {
	cout << "Num: " << Num << "\n";
	cout << "parentsSize_: " << parentsSize_ << "\n";
	for (SizeT i = 0; i != parentsSize_; ++i)
		cout << parentsData_[i] << "\t";
	cout << "\n\n";
}

// Graph constructors
template <class Type, class ParentType, class SizeT, class ParentSizeT>
Graph<Type, ParentType, SizeT, ParentSizeT>::Graph() : nodesSize_(0), nodesData_(new Type[nodesSize_]) {}

template <class Type, class ParentType, class SizeT, class ParentSizeT>
Graph<Type, ParentType, SizeT, ParentSizeT>::Graph(const Graph<Type, ParentType, SizeT, ParentSizeT>& graph)
	: nodesSize_(graph.nodesSize_), nodesData_(new Type[nodesSize_]) {
	for (SizeT i = 0; i != nodesSize_; ++i)
		nodesData_[i] = graph.nodesData_[i];
}

// Graph operators
template <class Type, class ParentType, class SizeT, class ParentSizeT>
Graph<Type, ParentType, SizeT, ParentSizeT>& Graph<Type, ParentType, SizeT, ParentSizeT>::operator =(const Graph<Type, ParentType, SizeT, ParentSizeT>& graph) {
	if (this != &graph)
		Graph<Type, ParentType, SizeT, ParentSizeT>(graph).Swap(*this);
	return *this;
}

// Graph methods
// Add node to the beginning of the array
template <class Type, class ParentType, class SizeT, class ParentSizeT>
void Graph<Type, ParentType, SizeT, ParentSizeT>::Shift(Type node) {
	if (Find(node.Num).Num != 0)
	{
		cout << "This node already exist\n\n";
		return;
	}

	Type * nodes = new Type[nodesSize_ + 1];
	nodes[0] = node;
	for (SizeT i = 0; i != nodesSize_; ++i)
		nodes[i + 1] = nodesData_[i];

	if (nodesSize_ != 0)
		delete[] nodesData_;

	nodesData_ = nodes;
	++nodesSize_;
}
// Add node to the end of the array
template <class Type, class ParentType, class SizeT, class ParentSizeT>
void Graph<Type, ParentType, SizeT, ParentSizeT>::Add(Type node) {
	if (Find(node.Num).Num != 0)
	{
		cout << "This node already exist\n\n";
		return;
	}

	Type * nodes = new Type[nodesSize_ + 1];
	for (SizeT i = 0; i != nodesSize_; ++i)
		nodes[i] = nodesData_[i];
	if (nodesSize_ != 0)
		delete[] nodesData_;

	nodes[nodesSize_] = node;
	nodesData_ = nodes;
	++nodesSize_;
}
// Remove existing node
template <class Type, class ParentType, class SizeT, class ParentSizeT>
void Graph<Type, ParentType, SizeT, ParentSizeT>::Remove(Type node) {
	if (nodesSize_ == 0)
	{
		cout << "Graph is empty\n\n";
		return;
	}

	int i = IndexOf(node.Num);
	if (i == -1)
	{
		cout << "This node not exist\n\n";
		return;
	}

	Type * nodes = new Type[nodesSize_ - 1];
	int count1 = 0, count2 = 0;
	while (count1 != nodesSize_ - 1 || count2 != nodesSize_)
	{
		if (i != count2)
		{
			nodes[count1] = nodesData_[count2];
			++count1;
		}
		++count2;
	}

	if (nodesSize_ != 0)
		delete[] nodesData_;

	nodesData_ = nodes;
	--nodesSize_;
}
// Clear graph
template <class Type, class ParentType, class SizeT, class ParentSizeT>
void Graph<Type, ParentType, SizeT, ParentSizeT>::Clear() {
	nodesSize_ = 0;
	delete[] nodesData_;
}
// Print graph structure
template <class Type, class ParentType, class SizeT, class ParentSizeT>
void Graph<Type, ParentType, SizeT, ParentSizeT>::Print() {
	cout << "Graph:\n";
	cout << "nodesSize_: " << nodesSize_ << "\n";
	for (SizeT i = 0; i != nodesSize_; ++i)
	{
		Type node = nodesData_[i];
		cout << "Num: " << node.Num << ";\t";

		cout << "Parents: ";
		SizeT parentsSize = node.GetParentsSize();
		ParentType * parents = node.GetParentsData();
		for (SizeT j = 0; j != parentsSize; ++j)
			cout << parents[j] << ",\t";

		cout << "\n";
	}
	cout << "\n";
}
// Print info about graph
template <class Type, class ParentType, class SizeT, class ParentSizeT>
void Graph<Type, ParentType, SizeT, ParentSizeT>::Info() {
	cout << "Number of graph nodes: " << nodesSize_ << "\n\n";
}
// Get index of node
template <class Type, class ParentType, class SizeT, class ParentSizeT>
int Graph<Type, ParentType, SizeT, ParentSizeT>::IndexOf(ParentType num) {
	for (SizeT i = 0; i != nodesSize_; ++i)
		if (nodesData_[i].Num == num)
			return i;

	return -1;
}
// Find a node in the graph
template <class Type, class ParentType, class SizeT, class ParentSizeT>
Type Graph<Type, ParentType, SizeT, ParentSizeT>::Find(ParentType num) {
	for (SizeT i = 0; i != nodesSize_; ++i)
		if (nodesData_[i].Num == num)
			return nodesData_[i];

	return Type();
}
// Swap graphs
template <class Type, class ParentType, class SizeT, class ParentSizeT>
void Graph<Type, ParentType, SizeT, ParentSizeT>::Swap(Graph& graph) {
	SizeT const s = nodesSize_;
	nodesSize_ = graph.nodesSize_;
	graph.nodesSize_ = s;

	Type * const d = nodesData_;
	nodesData_ = graph.nodesData_;
	graph.nodesData_ = d;
}
// Get parent nodes
template <class Type, class ParentType, class SizeT, class ParentSizeT>
Graph<Type, ParentType, SizeT, ParentSizeT> Graph<Type, ParentType, SizeT, ParentSizeT>::FindParentNodes(ParentType num) {
	Graph<Type, ParentType, SizeT, ParentSizeT> newGraph;
	if (nodesSize_ == 0)
		return newGraph;

	Type node = Find(num);
	if (node.Num == 0)
		return newGraph;

	SizeT parentsSize = node.GetParentsSize();
	ParentType * parents = node.GetParentsData();

	for (SizeT j = 0; j != parentsSize; ++j)
	{
		ParentType parentNum = parents[j];
		Type parentNode = Find(parentNum);
		if (node.Num != 0)
			newGraph.Add(parentNode);
	}

	return newGraph;
}
// Get children nodes
template <class Type, class ParentType, class SizeT, class ParentSizeT>
Graph<Type, ParentType, SizeT, ParentSizeT> Graph<Type, ParentType, SizeT, ParentSizeT>::FindChildrenNodes(ParentType num) {
	Graph<Type, ParentType, SizeT, ParentSizeT> newGraph;
	if (nodesSize_ == 0)
		return newGraph;

	Type node = Find(num);
	if (node.Num == 0)
		return newGraph;

	for (SizeT i = 0; i != nodesSize_; ++i)
	{
		Type chieldNode = nodesData_[i];
		if (chieldNode.CheckParents(new ParentType[1]{ node.Num }, 1))
			newGraph.Add(chieldNode);
	}

	return newGraph;
}
// Get path between two nodes
template <class Type, class ParentType, class SizeT, class ParentSizeT>
Graph<Type, ParentType, SizeT, ParentSizeT> Graph<Type, ParentType, SizeT, ParentSizeT>::Path(Type node1, Type node2, bool inverseChild) {
	Graph<Type, ParentType, SizeT, ParentSizeT> newGraph;
	newGraph.Add(node1);

	// search by children
	Graph<Type, ParentType, SizeT, ParentSizeT> children = FindChildrenNodes(node1.Num);
	for (SizeT i = 0; i != children.nodesSize_; ++i)
		if (children.nodesData_[i].Num == node2.Num)
		{
			if (inverseChild)
				newGraph.Shift(node2);
			else
				newGraph.Add(node2);
			return newGraph;
		}
		else
		{
			Graph<Type, ParentType, SizeT, ParentSizeT> nchildren = Path(children.nodesData_[i], node2, inverseChild);
			if (nchildren.IndexOf(node2.Num) != -1)
			{
				if (inverseChild)
				{
					for (SizeT j = 0; j != newGraph.nodesSize_; ++j)
						nchildren.Add(newGraph.nodesData_[j]);
					return nchildren;
				}
				else
				{
					for (SizeT j = 0; j != nchildren.nodesSize_; ++j)
						newGraph.Add(nchildren.nodesData_[j]);
					return newGraph;
				}
			}
		}

	// search by parents
	Graph<Type, ParentType, SizeT, ParentSizeT> parents = FindParentNodes(node1.Num);
	for (SizeT i = 0; i != parents.nodesSize_; ++i)
		if (parents.nodesData_[i].Num == node2.Num)
		{
			newGraph.Shift(node2);
			return newGraph;
		}
		else
		{
			Graph<Type, ParentType, SizeT, ParentSizeT> nparents = Path(parents.nodesData_[i], node2, true);
			if (nparents.IndexOf(node2.Num) != -1)
			{
				for (SizeT j = 0; j != newGraph.nodesSize_; ++j)
					nparents.Add(newGraph.nodesData_[j]);
				return nparents;
			}
		}

	newGraph.Remove(node1);
	return newGraph;
}
template <class Type, class ParentType, class SizeT, class ParentSizeT>
void Graph<Type, ParentType, SizeT, ParentSizeT>::IterateNodes(Type startNode, Graph<Type, ParentType, SizeT, ParentSizeT> checkedNodes) {
	// by children
	Graph<Type, ParentType, SizeT, ParentSizeT> children = FindChildrenNodes(startNode.Num);
	for (SizeT i = 0; i != children.nodesSize_; ++i)
		if (checkedNodes.IndexOf(children.nodesData_[i].Num) == -1)
		{
			IterateNodes(children.nodesData_[i], checkedNodes);
			checkedNodes.Add(children.nodesData_[i]);
		}

	// by parents
	Graph<Type, ParentType, SizeT, ParentSizeT> parents = FindParentNodes(startNode.Num);
	for (SizeT i = 0; i != parents.nodesSize_; ++i)
		if (checkedNodes.IndexOf(parents.nodesData_[i].Num) == -1)
		{
			IterateNodes(parents.nodesData_[i], checkedNodes);
			checkedNodes.Add(parents.nodesData_[i]);
		}
}

#endif //GRAPH_H