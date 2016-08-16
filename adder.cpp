#include "operator.h"

/******************************************************************************************
* Name: adder
* Description: adder class constructor. this function allocate operands, carry and output arrays.
*			   carry and output can overflow so they have one more bit.
* Arguments: n: data width of operation.
*			 k: approximate width of operation
******************************************************************************************/
adder::adder (int n, int k): Operator(n, k) {
	data_a = new int[data_width];
	data_b = new int[data_width];
	carry = new int[data_width + 1];
	data_sum = new int[data_width + 1];
}

/******************************************************************************************
* Name: accurate_eval
* Description: accurate evaluation of adder.
* Arguments: a: 1st input
*			 b: 2nd input
* return: accurate result of add
******************************************************************************************/
uint64_t adder::accurate_eval (uint64_t a, uint64_t b) {
	return a + b;
}

/******************************************************************************************
* Name: approx_eval
* Description: approximate evaluation of adder. its algorithm is explained in "Low-Power Digital 
*			   Signal Processing Using Approximate Adders" paper. there are accurate bit adder
*			   and approximate bit adder in this algorithm. first approximate adder is used 
*			   for least bits then accurate adder is used for other bits.
*			   all the operations are performed in binary so at first/end data should covert from
*			   decimal/binary to binary/decimal.
*			   
* Arguments: a: 1st input
*			 b: 2nd input
* Return: approximate result of add
******************************************************************************************/
uint64_t adder::approx_eval (uint64_t a, uint64_t b) {
	Dec2Bin(a, data_a, data_width);
	Dec2Bin(b, data_b, data_width);
	carry[0] = 0;

	// least bits calculate using approximate adder
	for (int i = 0; i < approx_width; ++i)
		approx_bit_add(data_a[i], data_b[i], carry[i], data_sum[i], carry[i+1]);

	// other bits calculate using accurate adder
	for (int i = approx_width; i < data_width; ++i)
		accurate_bit_add(data_a[i], data_b[i], carry[i], data_sum[i], carry[i+1]);

	data_sum[data_width] = carry[data_width];

	return Bin2Dec(data_sum, data_width + 1);
}

/******************************************************************************************
* Name: approx_bit_add
* Description: this function calculate approximate bit adder according to "Low-Power Digital 
*			   Signal Processing Using Approximate Adders" paper.*			   
* Arguments: a: 1st input
*			 b: 2nd input
*			 c: carry in
*			 s: approximate result
*			 cout: approximate carry out
******************************************************************************************/
void adder::approx_bit_add (int a, int b, int cin, int &s, int &cout) {
	if ((a == 0) && (b == 0) && (cin == 0)) {s = 0; cout = 0;}
	else if ((a == 0) && (b == 0) && (cin == 1)) {s = 0; cout = 0;}
	else if ((a == 0) && (b == 1) && (cin == 0)) {s = 1; cout = 0;}
	else if ((a == 0) && (b == 1) && (cin == 1)) {s = 1; cout = 0;}
	else if ((a == 1) && (b == 0) && (cin == 0)) {s = 0; cout = 1;}
	else if ((a == 1) && (b == 0) && (cin == 1)) {s = 0; cout = 1;}
	else if ((a == 1) && (b == 1) && (cin == 0)) {s = 1; cout = 1;}
	else if ((a == 1) && (b == 1) && (cin == 1)) {s = 1; cout = 1;}
	else {s = -1; cout = -1;}
}

/******************************************************************************************
* Name: accurate_bit_add
* Description: this function calculate accurate bit adder.
* Arguments: a: 1st input
*			 b: 2nd input
*			 c: carry in
*			 s: accurate result
*			 cout: accurate carry out
******************************************************************************************/
void adder::accurate_bit_add (int a, int b, int cin, int &s, int &cout) {
	int temp = a + b + cin;
	s = temp % 2;
	cout = temp / 2;
}

/******************************************************************************************
* Name: Dec2Bin
* Description: convert decimal to binary
* Arguments: dec: decimal input
*			 bin: array that binary is converted
*			 size: size of binary data.
******************************************************************************************/
void adder::Dec2Bin (uint64_t dec, int *bin, int size) {
	int i = 0;

	for (int i = 0; i < size; ++i) {
		bin[i] = dec % 2;
		dec /= 2;
	}
}

/******************************************************************************************
* Name: Bin2Dec
* Description: convert binary to decimal
* Arguments: bin: binary array
*			 size: size of binary array
* Return: value of decimal
******************************************************************************************/
uint64_t adder::Bin2Dec (int *bin, int size) {
	uint64_t exponent = 1;
	uint64_t dec = 0;
	for (int i = 0; i < size; ++i) {
		dec += bin[i] * exponent;
		exponent *= 2;
	}
	return dec;
}

/******************************************************************************************
* Name: ~adder
* Description: adder class destructor.
******************************************************************************************/
adder::~adder () {
	delete[] data_a;
	delete[] data_b;
	delete[] carry;
	delete[] data_sum;
}
