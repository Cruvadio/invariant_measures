#include "symbolic_image.hpp"
#include <iostream>
#include <boost/graph/strong_components.hpp>
#include <map>



template < typename G>
typename subgraph< G >::vertex_descriptor add_vertex(
	typename subgraph< G >::vertex_descriptor u_global, int global_label, subgraph< G >& g)
{
	BOOST_ASSERT(!g.is_root());
	typename subgraph< G >::vertex_descriptor u_local;
	bool exists_local;
	boost::tie(u_local, exists_local) = g.find_vertex(u_global);
	if (!exists_local)
	{
		typename subgraph< G >::vertex_descriptor v_global;
		typename subgraph< G >::edge_descriptor e_global;
		// call recursion for parent subgraph
		if (!g.parent().is_root())
			add_vertex(u_global, global_label, g.parent());

		u_local = add_vertex(global_label, global_label, g.m_graph);
		g.m_global_vertex.push_back(u_global);
		g.m_local_vertex[u_global] = u_local;

		subgraph< G >& r = g.root();

		// remember edge global and local maps
		{
			typename subgraph< G >::out_edge_iterator ei, ei_end;
			for (boost::tie(ei, ei_end) = out_edges(u_global, r); ei != ei_end;
				++ei)
			{
				e_global = *ei;
				v_global = target(e_global, r);
				if (g.find_vertex(v_global).second == true)
					g.local_add_edge(
						u_local, g.global_to_local(v_global), e_global);
			}
		}
		if (is_directed(g))
		{ // not necessary for undirected graph
			typename subgraph< G >::vertex_iterator vi, vi_end;
			typename subgraph< G >::out_edge_iterator ei, ei_end;
			for (boost::tie(vi, vi_end) = vertices(r); vi != vi_end; ++vi)
			{
				v_global = *vi;
				if (v_global == u_global)
					continue; // don't insert self loops twice!
				if (!g.find_vertex(v_global).second)
					continue; // not a subgraph vertex => try next one
				for (boost::tie(ei, ei_end) = out_edges(*vi, r); ei != ei_end;
					++ei)
				{
					e_global = *ei;
					if (target(e_global, r) == u_global)
					{
						g.local_add_edge(
							g.global_to_local(v_global), u_local, e_global);
					}
				}
			}
		}
	}
	return u_local;
}



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

vector<vector<vertex_t>> SymbolicImage::localize_chain_set()
{
	vector<bool> writes(number_of_cells);
	writes.assign(number_of_cells, false);
	map<int, vertex_t> vertices;

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



				//if (!writes[i] || !writes[cell])
				//{
				//	if (!writes[i])
				//	{
				//		vertex_t v = add_vertex(g);
				//		put(vertex_index, g, v, i);
				//		put(vertex_index1, g, v, i);
				//		vertices[i] = v;
				//		writes[i] = true;
				//	}
				//	if (!writes[cell])
				//	{
				//		vertex_t v = add_vertex(g);
				//		put(vertex_index, g, v, cell);
				//		put(vertex_index1, g, v, cell);
				//		vertices[cell] = v;
				//		writes[cell] = true;
				//	}
				//	add_edge(vertices[i], vertices[cell], g);
				//}

				add_edge(i, cell, g);
			}
		}
	}
	vector<vector<vertex_t>>components =  find_strong_components(g);
	cout << vertices.size() << endl;



	vertices.clear();
	for (int i = 0; i < iterations_for_localization; i++)
	{
		int old_cols = cols;
		Graph new_g;
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
					vector<int> cells = new_coordinates(boost::get(vertex_index, g, v), old_cols);

					for (int m = 0; m < 4; m++)
					{
						double x, y;
						return_interval(cells[m], x, y);



						for (int l = 0; l < dots_square - 1; l++)
						{

							double xn = x + (double)l * delta / (double)dots_square;
							for (int d = 0; d < dots_square - 1; d++)
							{

								double yn = y - (double)d * delta / (double)dots_square;

								int cell = return_cell(xn, yn);

								if (cell == -1) continue;
								if (cell >= number_of_cells) continue;

								if (vertices.find(cells[m]) != vertices.end())
								{
									vertex_t v = add_vertex(new_g);
									boost::put(vertex_index1, new_g, v, cells[m]);
									vertices[cells[m]] = v;
								}

								if (vertices.find(cell) != vertices.end())
								{
									vertex_t v = add_vertex(new_g);
									boost::put(vertex_index1, new_g, v, cell);
									vertices[cell] = v;
								}


								add_edge(vertices[cells[m]], vertices[cell], new_g);

							}
						}
					}
				}
			}
		}
		std::cout << "I'M HERE!!! " << i << endl;
		components = find_strong_components(new_g);
		g = new_g;
		std::cout << "I'M HERE!!! " << i << endl;

	}

	return components;
}



vector<vector<vertex_t>> SymbolicImage::find_strong_components(Graph &g)
{

	int num = strong_components(g, boost::get(vertex_component_t(), g)
		/*vertex_index_map(boost::get(vertex_index, g))*/);

	cout << num << endl;

	vector<vector<vertex_t>> components(num);

	graph_traits <Graph>::vertex_iterator vi, vi_end;

	for (tie(vi, vi_end) = vertices(g); vi != vi_end; ++vi)
	{
		components[boost::get(vertex_component_t(), g, *vi)].push_back(*vi);
	}

	return components;

	/*vector<vector<vertex_t>> components;

	return components;*/


}

vector<vertex_t> SymbolicImage::max_component(vector<vector<vertex_t>>& components)
{
	vector<vertex_t> &m_c = components[0];
	for (auto c : components)
	{
		//cout << c.size() << endl;
		if (c.size() > m_c.size())
		{
			m_c = c;
		}
	}

	return m_c;
}


Graph SymbolicImage::return_component_subgraph(vector<vector<vertex_t>>& components)
{
	vector<vertex_t> c = max_component(components);


	Graph s = g.create_subgraph();

	for (auto v : c)
	{
		add_vertex(v, s);
	}


	return s;
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

	vector<vector<vertex_t>> components = localize_chain_set();
	Graph s = return_component_subgraph(components);
	vector<double> p(number_of_cells * number_of_cells), p1(number_of_cells * number_of_cells);


	p.assign(number_of_cells * number_of_cells, 0.0);
	p1.assign(number_of_cells * number_of_cells, 0.0);

	auto rec_edges = edges(s);
	auto rec_vertices = vertices(s);
	for (auto ei = rec_edges.first; ei != rec_edges.second; ++ei)
	{
		int i = get(vertex_index, g, source(s.local_to_global(*ei), g));
		int j = get(vertex_index, g, target(s.local_to_global(*ei), g));

		p[i * number_of_cells + j] = p1[i * number_of_cells + j] = 1.0;
	}


	
	for (int n = 0; n < iterations_for_balance; ++n)
	{
		double sum = 0.0, x;


		for (auto ei = rec_edges.first; ei != rec_edges.second; ++ei)
		{
			int i = get(vertex_index, g, source(s.local_to_global(*ei), g));
			int j = get(vertex_index, g, target(s.local_to_global(*ei), g));

			sum += p[i * number_of_cells + j];
		}


		for (auto ei = rec_edges.first; ei != rec_edges.second; ++ei)
		{
			int i = get(vertex_index, g, source(s.local_to_globa
				l(*ei), g));
			int j = get(vertex_index, g, target(s.local_to_global(*ei), g));

			p[i * number_of_cells + j] /= sum;
		}

		for (auto vi = rec_vertices.first; vi != rec_vertices.second; ++vi)
		{
			
			double sum_l = 0.0, sum_h = 0.0;
			auto i_edges = out_edges(*vi, s);
			int i = boost::get(vertex_index, g, s.local_to_global(*vi));
			for (auto ej = i_edges.first; ej != i_edges.second; ++ej)
			{
				int j = boost::get(vertex_index, g, s.local_to_global(target(*ej, s)));
				sum_h += p[j * number_of_cells + i];
				sum_l += p[i * number_of_cells + j];
			}

			for (auto ej = i_edges.first; ej != i_edges.second; ++ej)
			{
				int j = boost::get(vertex_index, g, s.local_to_global(target(*ej, s)));
				if (j != i && sum_l > 0 && sum_h >= 0)
				{
					p1[i * number_of_cells + j] = p[i * number_of_cells + j] * sqrt(sum_h / sum_l);
				}
				else
					p1[i * number_of_cells + j] = p[i * number_of_cells + j];
			}
		}
		p.assign(p1.begin(), p1.end());

//#pragma omp parallel for reduction (+:sum) private(x)
//		for (int i = 0; i < number_of_cells * number_of_cells; i++)
//		{
//			x = p[i];
//			sum += x;
//		}
//#pragma omp parallel for
//		for (int i = 0; i < number_of_cells * number_of_cells; i++)
//		{
//			p[i] /= sum;
//		}
//
//		for (int i = 0; i < number_of_cells; ++i)
//		{
//			double sum_l = 0;
//			double sum_h = 0;
//#pragma omp parallel for reduction (+:sum_h) private(x)
//			for (int m = 0; m < number_of_cells; ++m)
//			{
//				x = p[m * number_of_cells + i];
//				if (m != i)
//					sum_h += x;
//			}
//#pragma omp parallel for reduction (+:sum_l) private(x)
//			for (int l = 0; l < number_of_cells; ++l)
//			{
//				x = p[i * number_of_cells + l];
//				if (l != i)
//					sum_l += x;
//			}
//#pragma omp parallel for
//			for (int k = 0; k < number_of_cells; ++k)
//			{
//
//				if (k != i && sum_l > 0 && sum_h >= 0)
//				{
//
//					//if(sum_h > sum_l)
//						p1[i * number_of_cells + k] = p[i * number_of_cells + k] * (sqrt(sum_h / sum_l));
//					//else
//						//p1[i * number_of_cells + k] = p[i * number_of_cells + k];
//				}
//				else
//					p1[i * number_of_cells + k] = p[i * number_of_cells + k];
//	
//
//			}
//
//
//
//		}
//		p.assign(p1.begin(), p1.end());

		//for (int i = 0; i < number_of_cells; i++)
		//{
		//	double sum_l = 0;
		//	double sum_h = 0;

		//	for (int m = 0; m < number_of_cells; ++m)
		//	{
		//		if (m != i)
		//			sum_l += p[m * number_of_cells + i];
		//	}

		//	for (int l = 0; l < number_of_cells; ++l)
		//	{
		//		if (l != i)
		//			sum_h += p[i * number_of_cells + l];
		//	}

		//	for (int k = 0; k < number_of_cells; ++k)
		//	{

		//		if (k != i && sum_l > 0 && sum_h >= 0)
		//		{

		//				p1[k * number_of_cells + i] = p[k * number_of_cells + i] * (sqrt(sum_h / sum_l));
		//		/*	else
		//				p1[k * number_of_cells + i] = p[k * number_of_cells + i];*/
		//		}
		//		else
		//			p1[k * number_of_cells + i] = p[k * number_of_cells + i];


		//	}

		//}
		//p.assign(p1.begin(), p1.end());




	}

	vector<double> measures(number_of_cells);

	for (int i = 0; i < number_of_cells; ++i)
	{
		double sum = 0.0;
		for (int j = 0; j < number_of_cells; ++j)
		{
			sum += p[i * number_of_cells + j];
		}
		measures[i] = sum;
	}
	//for (int i = 0; i < number_of_cells; i++)
	//{
	//	p[i] /= delta * delta;
	//}

	return measures;
}