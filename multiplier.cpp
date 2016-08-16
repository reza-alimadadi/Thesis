#include "operator.h"

/******************************************************************************************
* Name: accurate_eval
* Description: accurate evaluation of multiply.
* Arguments: a: 1st input
*			 b: 2nd input
* return: accurate result of multiply
******************************************************************************************/
uint64_t multiplier::accurate_eval (uint64_t a, uint64_t b) {
	return a * b;
}

/******************************************************************************************
* Name: approx_eval
* Description: approximate evaluation of multiply. its algorithm is explained in "A Low-Power, 
*			   High-Performance Approximate Multiplier with Configurable Partial Error Recovery".
* Arguments: a: 1st input
*			 b: 2nd input
* return: approximate result of multiply
******************************************************************************************/
uint64_t multiplier::approx_eval (uint64_t a, uint64_t b) {
	uint64_t res_error = 0;
	uint64_t res_approx = 0;

	for (int i = 0; i < data_width; ++i) {
		uint64_t temp_a, temp_b, temp_err;
		// if current bit is zero don't do any thing
		if ((a & (1 << i)) != 0) {
			temp_a = res_approx;
			temp_b = b << i;

			approx_add(temp_a, temp_b, temp_err, res_approx);
			error_comp(res_error, temp_err, res_error);
		}
	}

	uint64_t mask = ~((uint64_t) exp2(approx_width) - 1);
	res_error &= mask;
	res_approx += res_error;

	return res_approx;
}

/******************************************************************************************
* Name: approx_add
* Description: approximate multiplier is worked based on specific approximate adder. this 
*			   adder is implemented in this function. complete explanation of this adder is 
*			   in "A Low-Power, High-Performance Approximate Multiplier with Configurable 
*			   Partial Error Recovery".
* Arguments: a: 1st input
*			 b: 2nd input
*			 e: output error
*			 s: output sum
******************************************************************************************/
void multiplier::approx_add (uint64_t a, uint64_t b, uint64_t &e, uint64_t &s) {
	uint64_t data_xor, data_and;

	data_xor = a ^ b;
	data_and = (a & b) << 1;

	s = data_xor | data_and;
	e = data_xor & data_and;
}

/******************************************************************************************
* Name: error_comp
* Description: in this approximate multiplier error must be compensated for bits that we don't  
*			   want to have any approximation in it. in this function add compensation is 
*			   implemented.
* Arguments: err_a: 1st error input
*			 err_b: 2nd error input
*			 final_er: output compensated error
******************************************************************************************/
void multiplier::error_comp (uint64_t err_a, uint64_t err_b, uint64_t &final_er) {
	final_er = err_a + err_b;
}
