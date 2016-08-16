#ifndef _Operator_H_
#define _Operator_H_

#include <cstdint>
#include <cmath>

/******************************************************************************************
* Name: Operator
* Description: this is a basic class that all of operators such as adder, multiplier and etc...
*			   inherit form this class. 
******************************************************************************************/
class Operator {
	protected:
		int data_width, approx_width;
	public:
		Operator (int n = 32, int k = 0): data_width(n), approx_width(k) {}
		virtual uint64_t approx_eval (uint64_t a = 0, uint64_t b = 0) {return a;}
		virtual uint64_t accurate_eval (uint64_t a = 0, uint64_t b = 0) {return a;}
};

/******************************************************************************************
* Name: p_input
* Description: primary input class. to use the primary input as operator this class was 
*			   necessary. the approximation and accurate evaluate version of this class act in 
*			   same manner.
******************************************************************************************/
class p_input: public Operator {
	public:
		p_input (int n = 32): Operator(n) {}
		uint64_t approx_eval (uint64_t a = 0, uint64_t b = 0) {return a;}
		uint64_t accurate_eval (uint64_t a = 0, uint64_t b = 0) {return a;};
};

/******************************************************************************************
* Name: p_output
* Description: primary input class. to use the primary output as operator this class was 
*			   necessary. the approximation and accurate evaluate version of this class act in 
*			   same manner.
******************************************************************************************/
class p_output: public Operator {
	public:
		p_output(int n = 32): Operator(n) {}
		uint64_t approx_eval (uint64_t a = 0, uint64_t b = 0) {return a;}
		uint64_t accurate_eval (uint64_t a = 0, uint64_t b = 0) {return a;};
};

/******************************************************************************************
* Name: adder
* Description: this class implement adder. this adder has approximate and accurate evaluation
*			   that program can uses.
* Note: in this class implements only 5th approximate version of "Impact" paper.
*		for other versions of this paper refer to $thesis_code/legacy/operator_only/ directory
******************************************************************************************/
class adder: public Operator {
	private:
		int *data_a, *data_b;
		int *carry;
		int *data_sum;

		void approx_bit_add (int a, int b, int cin, int &s, int &cout);
		void accurate_bit_add (int a, int b, int cin, int &s, int &cout);
		uint64_t Bin2Dec (int *bin, int size);
		void Dec2Bin (uint64_t dec, int *bin, int size);

	public:
		adder (int n = 32, int k = 0);
		uint64_t approx_eval (uint64_t a = 0, uint64_t b = 0);
		uint64_t accurate_eval (uint64_t a = 0, uint64_t b = 0);
		~adder ();
};

/******************************************************************************************
* Name: multiplier
* Description: this class implements multiplier. this adder has approximate and accurate evaluation
*			   that program can uses.
* Note: only "add" version of "A Low-Power, High-Performance Approximate Multiplier with 
		Configurable Partial Error Recovery" is implemented. for other implementations refer to the paper.
******************************************************************************************/
class multiplier: public Operator {
	private:
		void approx_add (uint64_t a, uint64_t b, uint64_t &e, uint64_t &s);
		void error_comp (uint64_t err_a, uint64_t err_b, uint64_t &final_er);

	public:
		multiplier (int n = 32, int k = 0): Operator(n, k) {}
		uint64_t approx_eval (uint64_t a = 0, uint64_t b = 0);
		uint64_t accurate_eval (uint64_t a = 0, uint64_t b = 0);
};

#endif