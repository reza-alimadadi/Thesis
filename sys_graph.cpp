#include "sys_graph.h"

/******************************************************************************************
* Name: sys_graph
* Description: this is the constructor of "sys_graph" class. this constructor also read from
*			   files and creates system graph.
* Arguments: spec_file: this file shows the dependency between operators and the flow of program
*						from primary inputs to primary outputs. in this file type of each operator
*						and number of approximation bits is specified too.
*			 input_file: this file specify which of the nets in "spec_file" is primary input
*			 output_file: this file specify which of the nets in "spec_file" is primary output
******************************************************************************************/
sys_graph::sys_graph (std::string spec_file, std::string input_file, std::string output_file) {
	this->spec_file = spec_file;
	this->input_file = input_file;
	this->output_file = output_file;

	read_input();
	read_spec();
	read_output();
	create_graph();
}

/******************************************************************************************
* Name: print_graph
* Description: print system graph
******************************************************************************************/
void sys_graph::print_graph () const {
	for (std::vector<vertex>::const_iterator it = graph.cbegin(); it != graph.cend(); ++it) {
		it->print_vertex();
	}
}

void sys_graph::var_calc (uint64_t stimuli_num) {
	sys_statistics var(graph, stimuli_num);
}

/******************************************************************************************
* Name: read_spec
* Description: this function reads spec file. "spec_tokenize" file will create operators from
*			   the informations that this function passes to "spec_tokenize"
******************************************************************************************/
void sys_graph::read_spec () {
	std::ifstream f(spec_file);
	std::string line;

	while (getline(f, line)) {
		spec_tokenize(line);
	}
}

/******************************************************************************************
* Name: read_input
* Description: this function reads input file. it also creates primary input vertex and add it
*			   to system graph vector
******************************************************************************************/
void sys_graph::read_input() {
	std::ifstream f(input_file);
	std::string line;

	while (getline(f, line)) {
		input_tokenize(line);
	}
}

/******************************************************************************************
* Name: read_output
* Description: this function reads input file. it also creates primary output vertex and add it
*			   to system graph vector
******************************************************************************************/
void sys_graph::read_output () {
	std::ifstream f(output_file);
	std::string line;

	while (getline(f, line)) {
		vertex output_v (line, line, "output");
		graph.push_back(output_v);
	}
}

/******************************************************************************************
* Name: create_graph
* Description: This function will set-up the connectivity of system graph. when spec file is 
*			   is read each operator know its input and output via some nets. this function
*			   search in all vertices to shows which vertex is input of other vertex and vice 
*			   versa. 
*			   This function search for input of vertices and when it's found, this 
*			   vertex is also output of found vertex.
******************************************************************************************/
void sys_graph::create_graph () {
	for (std::vector<vertex>::iterator it = graph.begin(); it != graph.end(); ++it) {
		// vertex is primary output
		if (it->is_primary_output()) {
			vertex* find_op = find_input(it->get_input1_edge());
			if (find_op != NULL) {
				it->add_input1_op(find_op);
				find_op->add_output_op(&*it);
			}
		}
		// vertex is operator (+,*)
		else if (!(it->is_primary_input())) {
			// first operand
			vertex* find_op = find_input(it->get_input1_edge());
			if (find_op != NULL) {
				it->add_input1_op(find_op);
				find_op->add_output_op(&*it);
			}
			// second operand
			find_op = find_input(it->get_input2_edge());
			if (find_op != NULL) {
				it->add_input2_op(find_op);
				find_op->add_output_op(&*it);
			}
		}
	} // end of forv
}

void sys_graph::input_tokenize (std::string line) {
	std::string input_name;
	int64_t const_value = -1;
	
	// extract input name
	size_t line_pointer = line.find(" ");
	if (line_pointer != std::string::npos) {
		input_name = line.substr(0, line_pointer);

		// save after " "
		line = line.substr(line_pointer + 1);

		// extract constant value of input
		const_value = std::stoi(line, nullptr);
	}
	else
		input_name = line.substr();

	vertex input_v (input_name, input_name, "input", const_value);
	graph.push_back(input_v);
}

/******************************************************************************************
* Name: spec_tokenize
* Description: this function get information form line of spec file. spec file has the following
*			   format:
*			   |output_net = operator_name.operator_type.number_approximation_bits 1st_operand, 2nd_operand|
*			   after informations is extracted form line vertex is created according to them.
* Arguments: line: this is the line string that is read from spec file
******************************************************************************************/
void sys_graph::spec_tokenize (std::string line) {
	// extract output
	size_t line_pointer = line.find(" ");
	std::string op_output = line.substr(0, line_pointer);
	
	// save string after "="
	line_pointer = line.find("=");
	line = line.substr(line_pointer + 1);
	line_pointer = line.find(" ");
	line = line.substr(line_pointer + 1);

	// extract operator name
	line_pointer = line.find(".");
	std::string op_name = line.substr(0, line_pointer);

	line = line.substr(line_pointer + 1);
	// extract operator type
	line_pointer = line.find(".");
	std::string op_type = line.substr(0, line_pointer);

	line = line.substr(line_pointer + 1);
	// extract operator approximate range
	int op_approx_width = std::stoi(line, nullptr);

	// save string after operator
	line_pointer = line.find(" ");
	line = line.substr(line_pointer + 1);

	// extract 1st operand
	line_pointer = line.find(",");
	std::string op_input1 = line.substr(0, line_pointer);

	// save string after 1st operator
	line_pointer = line.find(" ");
	line = line.substr(line_pointer + 1);

	// extract 2nd operand
	std::string op_input2 = line.substr();

	vertex op_v(op_name, op_input1, op_input2, op_output, op_type, op_approx_width);
	graph.push_back(op_v);
}

/******************************************************************************************
* Name: find_input
* Description: this function is used to which edge has output with given net
* Arguments: name: name of edge
* Return: pointer to vertex that given edge is its output. in the case that there isn't any
*		  vertex it returns NULL pointer.
******************************************************************************************/
sys_graph::vertex* sys_graph::find_input (std::string name) {
	for (std::vector<vertex>::iterator it = graph.begin(); it != graph.end(); ++it) {
		if(name == it->get_output_edge()) 
			return &*it;
	}
	return NULL;
}

/******************************************************************************************
* Name: vertex
* Description: Constructor of vertex class. this constructor will create operator vertex based on 
*			   input/output edges, type of operator. 
* Arguments: op_name: name of operator
*			 op_input1: name of input 1 edge
*			 op_input2: name of input 2 edge
*			 op_output: name of output edge
*			 op_type: type of operand(adder or multiplier)
*			 op_approx_width: number of approximation bits
*			 op_data_width: data width of operand
******************************************************************************************/
sys_graph::vertex::vertex (std::string op_name, std::string op_input1, std::string op_input2, std::string op_output, 
							std::string op_type, int op_approx_width, int op_data_width /*=32*/) {
	name = op_name;
	input1_edge = op_input1;
	input2_edge = op_input2;
	output_edge = op_output;

	if (op_type == "+")
		// +5 is added to add operators in the cases that overflow is occurred
		// (only fir is checked up to 100 nodes)
		op = new adder(op_data_width + 5, op_approx_width);
	else if (op_type == "*")
		op = new multiplier(op_data_width, op_approx_width);

	input1_op = NULL;
	input2_op = NULL;
	output_op.clear();

	is_const = false;
	visited = false;
}

/******************************************************************************************
* Name: vertex
* Description: Constructor of vertex class. this constructor will create primary input/output
*			   vertex based on its type.
* Arguments: io_name: name of operator
*			 edge: name of input/output edge. if vertex is primary input this edge is output edge.
*				   if vertex is primary output this edge is input edge.
*			 op_type: type (primary input/output)
*			 data_width: data width of primary input/output
******************************************************************************************/	
sys_graph::vertex::vertex (std::string io_name, std::string edge, std::string op_type, 
							int64_t const_value, int data_width /*= 32*/) {
	name = io_name;
	input1_edge.clear();
	input2_edge.clear();
	output_edge.clear();
	is_const = false;
	
	if (op_type == "input") {
		output_edge = edge;
		op = new p_input(data_width);

		if (const_value >= 0) {
			is_const = true;
			accurate_value = (uint64_t) const_value;
			approx_value = (uint64_t) const_value;
		}
	}
	else if (op_type == "output") {
		input1_edge = edge;
		op = new p_output(data_width);
	}

	input1_op = NULL;
	input2_op = NULL;
	output_op.clear();

	visited = false;
}



/******************************************************************************************
* Name: print_vertex
* Description: this function print vertex name and its input and output vertices names.
******************************************************************************************/
void sys_graph::vertex::print_vertex () const {
	std::cout << name << ": ";
	if (input1_op) {
		std::cout << "\t" << "In1: " << input1_op->get_op_name();
		if (input1_op->is_constant_input())
			std::cout << input1_op->get_accurate_value() << std::endl;
		else
			std::cout << std::endl;
	}

	if (input2_op) {
		std::cout << "\t" << "In2: " << input2_op->get_op_name();
		if (input2_op->is_constant_input())
			std::cout << " " << input2_op->get_accurate_value() << std::endl;
		else
			std::cout << std::endl;
	}
	
	if (!output_op.empty()) {
		for (std::vector<vertex*>::const_iterator it = output_op.cbegin(); it != output_op.cend(); ++it) {
			std::cout << "\t" << "Out: " << (*it)->get_op_name() << std::endl;
		}
	}
	std::cout << "-------------------" << std::endl;
}
