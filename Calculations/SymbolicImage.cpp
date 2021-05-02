#include "symbolic_image.hpp"
#include <iostream>
#include <boost/graph/strong_components.hpp>
#include <map>


double SymbolicImage::func_X(double x, double y, double a, double b)
{
	return x * x - y * y + a;
}

double SymbolicImage::func_Y(double x, double y, double a, double b)
{
	return 2 * x * y + b;
}

int SymbolicImage::return_cell(double x, double y)
{
	double xn = func_X(x, y, a, b);
	double yn = func_Y(x, y, a, b);

	if (xn <= x_min || xn >= x_max || yn <= y_min || yn >= y_max)
		return -1;

	return (int)abs((yn - y_max) / delta) * cols + (int)abs((xn - x_min) / delta);
}

void SymbolicImage::return_interval(int cell, double& x, double& y)
{
	int row = return_row(cell);
	int col = return_col(cell);

	x = x_min + (double)col * delta;
	y = y_max - (double)row * delta;
}

vector<vector<int>> SymbolicImage::localize_chain_set()
{
	vector<bool> writes(number_of_cells);
	writes.assign(number_of_cells, false);
	for (int i = 0; i < number_of_cells; i++)
	{
		
		int row = (int)i / cols;
		int col = (int)i % cols;
		double x, y;
		return_interval(i, x, y);


		for (int l = 0; l < dots_square - 1; l++)
		{
			double xn = x + (double)l * delta / (double)dots_square;
			for (int d = 0; d < dots_square - 1; d++)
			{
				double yn = y - (double)d * delta / (double)dots_square;

				int cell = return_cell(xn, yn);

				if (cell == -1) continue;
				if (cell >= number_of_cells) continue;



				if (!writes[i])
				{
					vertex_t v = add_vertex(i, *g);
					(*g)[i].index = i;
					writes[i] = true;
				}
				if (!writes[cell])
				{
					add_vertex(cell, *g);
					(*g)[cell].index = cell;
					writes[cell] = true;
				}
				add_edge_by_label(i, cell, *g);
			}
		}
	}
	vector<vector<int>>components =  find_strong_components();

		for (int i = 0; i < iterations_for_localization; i++)
		{
			int old_cols = cols;
			Graph* new_g = new Graph();
			cols *= 2;
			rows *= 2;
			delta *= 0.5;

			number_of_cells = cols * rows;

			writes.resize(number_of_cells);
			writes.assign(number_of_cells, false);

			for (auto c : components)
			{
				if (c.size() > 1)
				{
					for (auto v : c)
					{
						vector<int> cells = new_coordinates(v, old_cols);

						for (int m = 0; m < 4; m++)
						{
							double x, y;
							return_interval(cells[m], x, y);



							for (int l = 0; l < dots_square - 1; l++)
							{

								double xn = x + (double)l * delta / (double) dots_square;
								for (int d = 0; d < dots_square - 1; d++)
								{

									double yn = y - (double)d * delta / (double) dots_square;

									int cell = return_cell(xn, yn);

									if (cell == -1) continue;
									if (cell >= number_of_cells) continue;

									if (!writes[cells[m]])
									{
										add_vertex(cells[m], *new_g);
										(*new_g)[cells[m]].index = cells[m];
										writes[cells[m]] = true;
									}

									if (!writes[cell])
									{
										cout << cell << endl;
										add_vertex(cell, *new_g);
										(*new_g)[cell].index = cell;
										writes[cell] = true;
									}

									
									add_edge_by_label(cells[m], cell, *new_g);

								}
							}
						}
					}
				}
			}
			std::cout << "I'M HERE!!! " << i << endl;
			delete g;
			g = new_g;		
			components = find_strong_components();

			std::cout << "I'M HERE!!! " << i << endl;

		}

	return components;
}

vector<vector<int>> SymbolicImage::find_strong_components()
{

	map <vertex_t, int> comp;

	cout << "Calculating strong components..." << endl;
	int num = strong_components(g->graph(), boost::get(&Node::component, g->graph()), 
		vertex_index_map(boost::get(&Node::index, g->graph())));

	vector<vector<int>> components(num);
	cout << "Calculated" << endl;

	graph_traits <Graph>::vertex_iterator vi, vi_end;

	for (tie(vi, vi_end) = vertices(*g); vi != vi_end; ++vi)
	{
		components[g->graph()[*vi].component].push_back(g->graph()[*vi].index);

	}

	return components;

	/*vector<vector<vertex_t>> components;

	return components;*/


}


vector<int> SymbolicImage::new_coordinates(int cell, int old_cols)
{
	int row = cell / old_cols;
	int col = cell % old_cols;

	vector<int> cells(4);

	cells[0] = 2 * row * cols + 2 * col;
	cells[1] = 2 * row * cols + 2 * col + 1;
	cells[2] = (2 * row + 1) * cols + 2 * col;
	cells[3] = (2 * row + 1)* cols + 2 * col + 1;

	return cells;
}


vector<double> SymbolicImage::find_invariant_measures()
{

	vector<vector<int>> components = localize_chain_set();
	vector<double> p(number_of_cells), p1(number_of_cells);


	p.assign(number_of_cells, 0.0);
	p1.assign(number_of_cells, 0.0);

	for (vector<int> c : components)
	{	
		if (c.size() > 1)
		{
			for (int v : c)
			{
				p[v] = 1.0;
			}
		}
	}


	
	for (int n = 0; n < iterations_for_balance; n++)
	{


		for (int i = 0; i < rows; i++)
		{
			double sum_l = 0;
			double sum_h = 0;

			for (int m = 0; m < rows; m++)
			{
				if (m != i)
					sum_h += p[m * cols + i];
			}

			for (int l = 0; l < cols; l++)
			{
				if (l != i)
					sum_l += p[i * cols + l];
			}

			for (int k = 0; k < cols; k++)
			{

				if (k != i && sum_l > 0 && sum_h >= 0)
				{

					if (sum_h > sum_l)
						p1[i * cols + k] = p[i * cols + k] * (sqrt(sum_h / sum_l));
					else
						p1[i * cols + k] = p[i * cols + k];
				}
				else
					p1[i * cols + k] = p[i * cols + k];
	

			}



		}
		p.assign(p1.begin(), p1.end());

		for (int i = 0; i < rows; i++)
		{
			double sum_l = 0;
			double sum_h = 0;

			for (int m = 0; m < rows; m++)
			{
				if (m != i)
					sum_l += p[m * cols + i];
			}

			for (int l = 0; l < cols; l++)
			{
				if (l != i)
					sum_h += p[i * cols + l];
			}

			for (int k = 0; k < cols; k++)
			{

				if (k != i && sum_l > 0 && sum_h >= 0)
				{

					if (sum_h > sum_l)
						p1[k * cols + i] = p[k * cols + i] * (sqrt(sum_h / sum_l));
					else
						p1[k * cols + i] = p[k * cols + i];
				}
				else
					p1[k * cols + i] = p[k * cols + i];


			}

		}
		p.assign(p1.begin(), p1.end());

		double sum = 0.0;

		for (int i = 0; i < number_of_cells; i++)
		{
			sum += p[i];
		}

		for (int i = 0; i < number_of_cells; i++)
		{
			p[i] /= sum;
		}
	}

	//for (int i = 0; i < number_of_cells; i++)
	//{
	//	p[i] /= delta * delta;
	//}

	return p;
}