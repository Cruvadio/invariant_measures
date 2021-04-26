#pragma once
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/labeled_graph.hpp>
#include <vector>
#include <list>


using namespace boost;
using namespace std;

struct Node
{
	int index;
};


typedef adjacency_list<setS, listS, directedS, Node> preGraph;
typedef labeled_graph< preGraph, int> Graph;
typedef graph_traits<Graph>::vertex_descriptor vertex_t;




class SymbolicImage {

	double a = 0.0;
	double b = -0.35;

	double delta;

	int dots;

	double x_min;
	double x_max;
	double y_min;
	double y_max;


	int cols;
	int rows;

	int iterations_for_balance;
	int iterations_for_localization;

	int number_of_cells;
	int dots_square;

	

	Graph g;



	double func_X(double x, double y, double a, double b);

	double func_Y(double x, double y, double a, double b);


	int return_cell(double x, double y);

	vector<vector<vertex_t>>& find_strong_components();

	Graph localize_chain_set(int n);

	vector<int>& new_coordinates(vertex_t cell, int cols);


	void return_interval(int cell, double& x, double& y);

	int return_old_cell(int cell, int old_cols);

public:

	SymbolicImage(double xMin, double xMax, double yMin, double yMax, int dots, double delta, double a = 0.0, double b = 0.0) :
		x_min(xMin),
		x_max(xMax),
		y_min(yMin),
		y_max(yMax),
		dots(dots),
		delta(delta),
		a(a), b(b),
		g()
	{
		rows = (int)((y_max - y_min) / delta);
		cols = (int)((x_max - x_min) / delta);

		number_of_cells = rows * cols;

		dots_square = ceil(sqrt(dots));
	}


	int return_col(int cell)
	{
		return cell % cols;
	}

	int return_row(int cell)
	{
		return cell / cols;
	}



	vector<vector<int>> find_invariand_measures();


};

