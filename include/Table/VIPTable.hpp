#pragma once

#include "Table.hpp"

class VIPTable : public Table {
private:
	double _extraFee;
public:
	VIPTable(unsigned int capacity, double extraFee, bool status = true);

	double GetExtraFee() const { return _extraFee; }
	void SetEctraFee(double amount);
};