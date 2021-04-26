#include "symbolic_image.hpp"

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

Graph SymbolicImage::localize_chain_set(int n)
{
	vector<bool> writes(number_of_cells);
	writes.assign(number_of_cells, false);
	for (int i = 0; i < number_of_cells; i++)
	{
		
		int row = (int)i / cols;
		int col = (int)i % cols;
		double x = x_min + col * delta;
		double y = y_max - row * delta;


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
					vertex_t v = add_vertex(i, g);
					g[i].index = i;
					writes[i] = true;
				}
				if (!writes[cell])
				{
					add_vertex(cell, g);
					g[cell].index = cell;
					writes[cell] = true;
				}
				add_edge_by_label(i, cell, g);
			}
		}
	}
	vector<vector<vertex_t>>& components = find_strong_components();


	if (n > 0)
	{
		for (int i = 0; i < n; i++)
		{
			int old_cols = cols;
			Graph new_g();

			cols *= 2;
			rows *= 2;
			delta *= 0.5;

			number_of_cells = cols * rows;

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

								double xn = x + (double)l * delta / dots_square;
								for (int d = 0; d < dots_square - 1; d++)
								{

									double yn = y - (double)d * delta / dots_square;

									int cell = return_cell(xn, yn);

									if (cell == -1) continue;
									if (cell >= number_of_cells) continue;

									

								}
							}
						}
					}
				}
			}

		}

	}

}

vector<vector<vertex_t>>& SymbolicImage::find_strong_components()
{


	map <vertex_t, int> component;

	int num = strong_components(g, make_assoc_property_map(component));

	vector<vector<vertex_t>> components(num);

	graph_traits <Graph>::vertex_iterator vi, vi_end;

	for (tie(vi, vi_end) = vertices(g); vi != vi_end; ++vi)
	{
		components[component[*vi]].push_back(*vi);

	}

	return components;
}


vector<int>& SymbolicImage::new_coordinates(vertex_t cell, int old_cols)
{
	int row = g.graph()[cell].index / old_cols;
	int col = g.graph()[cell].index % old_cols;

	vector<int> cells(4);

	cells[0] = 2 * row * cols + 2 * col;
	cells[1] = 2 * row * cols + 2 * col + 1;
	cells[2] = (2 * row + 1) * cols + 2 * col;
	cells[3] = (2 * row + 1)* cols + 2 * col + 1;

	return cells;
}