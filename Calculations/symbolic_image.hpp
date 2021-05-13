#pragma once
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/labeled_graph.hpp>
#include <boost/graph/subgraph.hpp>
#include <vector>
#include <list>

using namespace boost;
using namespace std;

struct vertex_component_t
{
	typedef vertex_property_tag kind;
};


typedef property<vertex_index_t, size_t, property <vertex_component_t, int, property<vertex_index1_t, int>>> VertexProperty;
typedef adjacency_list<setS, vecS, directedS, VertexProperty, property<edge_index_t, size_t>> adj_list;
//typedef labeled_graph<adj_list, int> preGraph;
typedef subgraph<adj_list> Graph;
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

	vector<vector<vertex_t>> find_strong_components(Graph &);

	vector<vector<vertex_t>> localize_chain_set();

	vector<int> new_coordinates(int cell, int cols);


	void return_interval(int cell, double& x, double& y);

	int return_old_cell(int cell, int old_cols);


	Graph return_component_subgraph(vector<vector<vertex_t>>& components);
	vector<vertex_t> max_component(vector<vector<vertex_t>>& components);
public:

	SymbolicImage(
		double xMin,
		double xMax,
		double yMin,
		double yMax,
		int dots,
		double delta,
		int it_loc,
		int it_bal,
		double a = 0.0,
		double b = 0.0
	) :
		x_min(xMin),
		x_max(xMax),
		y_min(yMin),
		y_max(yMax),
		dots(dots),
		delta(delta),
		iterations_for_localization(it_loc),
		iterations_for_balance(it_bal),
		a(a), b(b) //g()
	{
		rows = (int)((y_max - y_min) / delta);
		cols = (int)((x_max - x_min) / delta);

		number_of_cells = rows * cols;
		g = Graph(number_of_cells);
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



	vector<double> find_invariant_measures();

};

