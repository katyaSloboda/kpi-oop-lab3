// Lab3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "pch.h"
#include "graph.h"

using namespace std;

int main()
{
	Node<int, size_t> node;
	cout << "Enter node info (press 0 to finish entering):\n";
	cin >> node;
	cout << node;

	Graph<Node<int, size_t>, int, size_t, size_t> graph;
	cout << "Enter graph info (press 0 to finish entering node,\n";
	cout << "press -1 to finish entering graph):\n";
	cin >> graph;
	cout << graph;

	int parents1[] = { 1,2,3 };
	int parents2[] = { 1 };
	Node<int, size_t> node1(5, parents1, sizeof(parents1) / sizeof(parents1[0]));
	Node<int, size_t> node2(2, parents2, sizeof(parents2) / sizeof(parents2[0]));
	node1.Print();
	node2.Print();

	graph.Add(node1);
	graph.Add(node2);

	Graph<Node<int, size_t>, int, size_t, size_t> path = graph.Path(node1, node2);
	path.Print();

	graph.Print();
	graph.Info();
	graph.IterateNodes(node2, Graph<Node<int, size_t>, int, size_t, size_t>());

	graph.Remove(node1);
	graph.Print();
	graph.Info();
}
