#include <iostream>
#include "sys_graph.h"

using namespace std;


int main(int argc, char *argv[]) {

	string a(argv[1]);
	string b(argv[2]);
	string c(argv[3]);

	sys_graph sys(a, b, c);
	sys.var_calc(20000);

	// int add_data_width = 48;
	// int mult_data_width = 32;

	// Operator *m0 = new multiplier(mult_data_width, 9);
	// Operator *m1 = new multiplier(mult_data_width, 1);
	// Operator *m2 = new multiplier(mult_data_width, 3);
	// Operator *m3 = new multiplier(mult_data_width, 8);
	// Operator *m4 = new multiplier(mult_data_width, 7);
	// Operator *m5 = new multiplier(mult_data_width, 6);
	// Operator *m6 = new multiplier(mult_data_width, 3);
	// Operator *m7 = new multiplier(mult_data_width, 5);
	// Operator *m8 = new multiplier(mult_data_width, 5);
	// Operator *a0 = new adder(add_data_width, 3);
	// Operator *a1 = new adder(add_data_width, 4);
	// Operator *a2 = new adder(add_data_width, 10);
	// Operator *a3 = new adder(add_data_width, 8);
	// Operator *a4 = new adder(add_data_width, 8);
	// Operator *a5 = new adder(add_data_width, 1);
	// Operator *a6 = new adder(add_data_width, 6);
	// Operator *a7 = new adder(add_data_width + 1, 8);

	// uint64_t i17 = 1990;
	// uint64_t i16 = 868;
	// uint64_t i15 = 27252;
	// uint64_t i14 = 29773;
	// uint64_t i13 = 32174;
	// uint64_t i12 = 31227;
	// uint64_t i11 = 19494;
	// uint64_t i10 = 31918;
	// uint64_t i9 = 31864;
	// uint64_t i8 = 9635;
	// uint64_t i7 = 29917;
	// uint64_t i6 = 25490;
	// uint64_t i5 = 23600;
	// uint64_t i4 = 16511;
	// uint64_t i3 = 23867;
	// uint64_t i2 = 14109;
	// uint64_t i1 = 11359;
	// uint64_t i0 = 28445;

	// uint64_t t0 = m0->approx_eval(i0, i1);
	// std::cout << "t0: " << t0 << std::endl;
	// uint64_t t1 = m1->approx_eval(i2, i3);
	// std::cout << "t1: " << t1 << std::endl;
	// uint64_t t2 = m2->approx_eval(i4, i5);
	// std::cout << "t2: " << t2 << std::endl;
	// uint64_t t3 = m3->approx_eval(i6, i7);
	// std::cout << "t3: " << t3 << std::endl;
	// uint64_t t4 = m4->approx_eval(i8, i9);
	// std::cout << "t4: " << t4 << std::endl;
	// uint64_t t5 = m5->approx_eval(i10, i11);
	// std::cout << "t5: " << t5 << std::endl;
	// uint64_t t6 = m6->approx_eval(i12, i13);
	// std::cout << "t6: " << t6 << std::endl;
	// uint64_t t7 = m7->approx_eval(i14, i15);
	// std::cout << "t7: " << t7 << std::endl;
	// uint64_t t8 = m8->approx_eval(i16, i17);
	// std::cout << "t8: " << t8 << std::endl;
	// uint64_t k0 = a0->approx_eval(t0, t1);
	// std::cout << "k0: " << k0 << std::endl;
	// uint64_t k1 = a1->approx_eval(k0, t2);
	// std::cout << "k1: " << k1 << std::endl;
	// uint64_t k2 = a2->approx_eval(k1, t3);
	// std::cout << "k2: " << k2 << std::endl;
	// uint64_t k3 = a3->approx_eval(k2, t4);
	// std::cout << "k3: " << k3 << std::endl;
	// uint64_t k4 = a4->approx_eval(k3, t5);
	// std::cout << "k4: " << k4 << std::endl;
	// uint64_t k5 = a5->approx_eval(k4, t6);
	// std::cout << "k5: " << k5 << std::endl;
	// uint64_t k6 = a6->approx_eval(k5, t7);
	// std::cout << "k6: " << k6 << std::endl;
	// uint64_t o0 = a7->approx_eval(k6, t8);
	// std::cout << "o0: " << o0 << std::endl;


	return 0;
}