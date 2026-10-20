#include <iostream>
#include "Time.hpp"
#include "Customer.hpp"
#include <string>


int main(void) {
	printf("Restaurant Reservation System\n");

	Time time1("1:2");

	std::cout << time1 << std::endl;

	return 1;
}