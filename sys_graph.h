#ifndef SYS_GRAPH_H_
#define SYS_GRAPH_H_

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <stack>
#include <cstdlib>
#include <ctime>
#include "operator.h"

/******************************************************************************************
* Name: sys_graph
* Description: main program create an object form this class. this class should create 
* 			   DFG based on input file then it can calculate output variance.
******************************************************************************************/
class sys_graph {
	// sys_graph private classes
	private:
		/*********************************************************************************
		* Name: vertex
		* Description: Each operator and primary input/output is object of this class. Operators 
		*			   are created from "operator.h" library that has both approximate and accurate
		*			   version of them.
		*			   In this class each object specify its input/output object so it can 
		*			   show the whole connectivity. In addition to that this class saves 
		*			   value that has been calculated.
		*********************************************************************************/
		class vertex {
			private:
				// name and edges that is got from input folder
				std::string name;
				std::string input1_edge, input2_edge;
				std::string output_edge;
				// vertex is either operator or primary input/output; operator type is in spec file.
				Operator* op;
				// pointers to other operator that evaluate input value or gets value from this operator
				// I assumed that all of operator has at most 2 operand
				// some operations output varies when order of its operands change
				// separation of input operand could solve this problem
				vertex* input1_op;
				vertex* input2_op;
				// output of operator can feed many other operators so I used vector to save all of those operators
				std::vector<vertex*> output_op;
				// used in sys_statistics in topological sort
				bool visited;
				// if the node is input and has constant value this flag sets.
				bool is_const;
				// used in sys_statistics class in mean and variance calculation
				uint64_t accurate_value, approx_value;

			public:
				vertex (std::string op_name, std::string op_input1, std::string op_input2, std::string op_output, 
						std::string op_type, int op_approx_width, int op_data_width = 32);
				vertex (std::string io_name, std::string edge, std::string op_type, int64_t const_value = -1, int data_width = 32);

				void add_input1_op (vertex *in_op) {this->input1_op = in_op;}
				void add_input2_op (vertex *in_op) {this->input2_op = in_op;}
				void add_output_op (vertex *out_op) {this->output_op.push_back(out_op);}

				vertex* get_input1_op () const {return input1_op;}
				vertex* get_input2_op () const {return input2_op;}
				std::vector<vertex*>& get_output_op () {return output_op;}

				std::string get_op_name () const {return name;}
				std::string get_input1_edge () const {return input1_edge;}
				std::string get_input2_edge () const {return input2_edge;}
				std::string get_output_edge () const {return output_edge;}

				bool is_primary_input () const {return input1_edge.empty() && input2_edge.empty();}
				bool is_primary_output () const {return output_edge.empty();}
				bool is_constant_input () const {return is_const;}

				bool get_op_visited () const {return visited;}
				void set_op_visited (bool state) {visited = state;}

				void op_approx_calc (uint64_t a = 0, uint64_t b = 0) {approx_value = op->approx_eval(a, b);}
				void op_accurate_calc (uint64_t a = 0, uint64_t b = 0) {accurate_value = op->accurate_eval(a, b);}

				uint64_t get_approx_value () const {return approx_value;}
				uint64_t get_accurate_value () const {return accurate_value;}

				void print_vertex () const;
		};

		/*********************************************************************************
		* Name: sys_statistics
		* Description: This class calculate mean and variance of graph. This class can also 
		*			   calculate mean and variance from multiple output systems.
		*			   At first this class run topological sort to get the order that input 
		*			   must flow through graph until its output. then it feeds random inputs 
		*			   to the system graph and calculate outputs mean and variance.
		*********************************************************************************/
		class sys_statistics {
			private:
				// this vector contains reverse ordered of system graph
				// after topological sort is executed on system graph pointer of actual vertices are
				// stored in this vector
				// the precedence of vertices are saved in reverse and to access them reverse_iterator
				// should be used.
				std::vector<vertex*> graph_reverse_sorted;

			private:
				void topological_sort (std::vector<vertex> &graph);
				void dfs (vertex &v);

				void statistics_calc (uint64_t stimuli_num);
				void sys_calc (std::map<std::string, int64_t> &diff);

				void print_graph_tp_sorted () const;
			public:
				sys_statistics (std::vector<vertex> &graph, uint64_t stimuli_num = 20000);
		};

	// sys_graph private variables
	private:
		// vector of operators and primary input/output of system
		std::vector<vertex> graph;
		// name of three files
		// these three files specify specifications of system
		std::string spec_file, input_file, output_file;

	// sys_graph private functions
	private:
		void read_spec ();
		void read_input ();
		void read_output ();
		void create_graph ();
		void spec_tokenize (std::string line);
		void input_tokenize (std::string line);
		sys_graph::vertex* find_input (std::string name); 
		
	public:
		sys_graph (std::string spec_file, std::string input_file, std::string output_file);
		void var_calc (uint64_t stimuli_num = 20000);
		void print_graph () const;
};

#endif