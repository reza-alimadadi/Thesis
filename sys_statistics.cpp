#include "sys_graph.h"

/******************************************************************************************
* Name: sys_statistics
* Description: this is constructor of sys_statistics class. this constructor at first sort
*			   system graph using topological sort algorithm and then run system for example
*			   20000 times to calculate the mean and variance of its output error.
* Arguments: graph: reference to system graph
*			 stimuli_num: the number that systems run to calculate variance and mean
******************************************************************************************/
sys_graph::sys_statistics::sys_statistics (std::vector<vertex> &graph, uint64_t stimuli_num) {
	topological_sort(graph);

	// print_graph_tp_sorted();
	statistics_calc(stimuli_num);
}

/******************************************************************************************
* Name: topological_sort
* Description: this function runs topological sort to get the precedence of vertices to 
*			   calculate form primary inputs to primary outputs. this function uses DFS algorithm
*			   on all vertices that DFS didn't visit to calculate topological sort. 
*			   at the end the precedence is saved in "graph_reverse_sorted" vector. it must be 
*			   note that the precedence is saved in reverse and "reverse_iterator" should be 
*			   used on "graph_reverse_sorted" vector.
* Arguments: graph: reference to system graph
******************************************************************************************/
void sys_graph::sys_statistics::topological_sort (std::vector<vertex> &graph) {
	for (std::vector<vertex>::iterator it = graph.begin(); it != graph.end(); ++it)
		it->set_op_visited(false);

	for (std::vector<vertex>::iterator it = graph.begin(); it != graph.end(); ++it)
		if (it->get_op_visited() == false)
			dfs(*it);
}

/******************************************************************************************
* Name: dfs
* Description: this function perform DFS algorithm. when this function is called it call DFS 
*			   for all of its output vertices until there is no visited vertex or that vertex 
*			   doesn't have any output which means that this vertex is primary output.
* Arguments: v: vertex that DFS is performed on it.
******************************************************************************************/
void sys_graph::sys_statistics::dfs (vertex &v) {
	v.set_op_visited(true);

	for (std::vector<vertex*>::iterator it = v.get_output_op().begin(); it != v.get_output_op().end(); ++it) {
		if ((*it)->get_op_visited() == false) 
			dfs(**it);
	}
	graph_reverse_sorted.push_back(&v);
}

/******************************************************************************************
* Name: statistics_calc
* Description: this function is to calculate mean and variance of system outputs error. this 
*			   function uses "sys_calc" function that calculate output error with random inputs
*			   and this function calculate mean and variance based on those calculations.
* Arguments: stimuli_num: number of runs that system calculate output error to evaluate mean and 
*			 variance
******************************************************************************************/
void sys_graph::sys_statistics::statistics_calc (uint64_t stimuli_num) {
	// these maps are used to save the mean and variance of all outputs in the case that 
	// system has multiple primary output
	// we used var = E(x^2) - E(x)^2 formula to calculate variance
	std::map<std::string, double> MSE_outputs, MEAN_outputs;
	std::map<std::string, double> VAR_outputs;

	srand(time(NULL));
	for (int i = 0; i < stimuli_num; ++i) {
		std::map<std::string, int64_t> diff_outputs;
		sys_calc(diff_outputs);
		
		// for all of outputs
		for (std::map<std::string, int64_t>::iterator it = diff_outputs.begin(); it != diff_outputs.end(); ++it) {
			// find can be interpret that this object exit in both of "Mean" and "MSE"
			std::map<std::string, double>::iterator statistics_it = MEAN_outputs.find(it->first);
			if (statistics_it != MEAN_outputs.end()) {
				MEAN_outputs[it->first] += it->second;
				MSE_outputs[it->first] += it->second * it->second;
			}
			else {
				MEAN_outputs[it->first] = it->second;
				MSE_outputs[it->first] = it->second * it->second;
			}
			// std::cout << "Diff " << it->first << ": " << it->second << std::endl;
			// std::cout << "***************************************" << std::endl;
		}
	}
	for (std::map<std::string, double>::iterator it = MEAN_outputs.begin(); it != MEAN_outputs.end(); ++it) {
		MEAN_outputs[it->first] /= stimuli_num;
		MSE_outputs[it->first] /= stimuli_num;
		VAR_outputs[it->first] = MSE_outputs[it->first] - MEAN_outputs[it->first] * MEAN_outputs[it->first];
		
		// std::cout << "Mean " << it->first << ": " << MEAN_outputs[it->first] << std::endl;
		// std::cout << "MSE " << it->first << ": " << MSE_outputs[it->first] << std::endl;
		std::cout << "Calculated Var " << it->first << ": " << std::fixed << VAR_outputs[it->first] << std::endl;
		// std::cout << "***************************************" << std::endl;
	}
}

/******************************************************************************************
* Name: sys_calc
* Description: this function uses the precedence thats is executed from topological sort function 
*			   to feed system and get the outputs error. system graph has three type of vertices. 
*			   (primary inputs, primary outputs and operators) 
*			   primary inputs should get inputs value randomly. 
*			   operator perform their function in approximate and accurate manner. when each 
*			   operator wants to calculate its output it gets inputs value from "input1/2_op" 
*			   that is guaranteed from topological sort which its value is ready.
*			   primary outputs save their value in a map to send it back to "statistics_calc" 
*			   function.
* Arguments: diff: map that saves outputs error and pass it to "statistics_calc" function.
******************************************************************************************/
void sys_graph::sys_statistics::sys_calc (std::map<std::string, int64_t> &diff) {
	for (std::vector<vertex*>::reverse_iterator it = graph_reverse_sorted.rbegin(); it != graph_reverse_sorted.rend(); ++it) {
		if ((*it)->is_primary_input()) {
			if (!(*it)->is_constant_input()) {
				uint64_t num = rand () % 32768;

				(*it)->op_accurate_calc(num);
				(*it)->op_approx_calc(num);

				// std::cout << (*it)->get_op_name() << ": " << num << std::endl;
			}
			else
				;// std::cout << (*it)->get_op_name() << ": " << (*it)->get_accurate_value() << std::endl;
		}
		else if ((*it)->is_primary_output()) {
			uint64_t in_acc = (*it)->get_input1_op()->get_accurate_value();
			(*it)->op_accurate_calc(in_acc);
			
			uint64_t in_app = (*it)->get_input1_op()->get_approx_value();
			(*it)->op_approx_calc(in_app);

			// std::cout << "APP " << (*it)->get_op_name() << ": " << in_app << std::endl;
			// std::cout << "ACC " << (*it)->get_op_name() << ": " << in_acc << std::endl;

			diff[(*it)->get_op_name()] = int64_t (in_app - in_acc);
		}
		// operator with 2 operand (*,+)
		else {
			uint64_t in1_acc = (*it)->get_input1_op()->get_accurate_value();
			uint64_t in2_acc = (*it)->get_input2_op()->get_accurate_value();
			(*it)->op_approx_calc(in1_acc, in2_acc);
			
			uint64_t in1_app = (*it)->get_input1_op()->get_approx_value();
			uint64_t in2_app = (*it)->get_input2_op()->get_approx_value();
			(*it)->op_accurate_calc(in1_app, in2_app);
		}
	}
}

/******************************************************************************************
* Name: print_graph_tp_sorted
* Description: print graph in precedence that calculation should be  performed.
******************************************************************************************/
void sys_graph::sys_statistics::print_graph_tp_sorted () const {
	for (std::vector<vertex*>::const_reverse_iterator it = graph_reverse_sorted.rbegin(); it != graph_reverse_sorted.rend(); ++it)
		std::cout << (*it)->get_op_name() << std::endl;
}
