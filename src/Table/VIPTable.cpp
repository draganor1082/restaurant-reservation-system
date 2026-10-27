#include "Table/VIPTable.hpp"

VIPTable::VIPTable(unsigned int capacity, double extraFee, bool status) : Table(capacity, status)
{
    SetEctraFee(extraFee);
}

void VIPTable::SetEctraFee(double amount)
{
    if (amount < 0)
        throw std::invalid_argument("Fees can not be less than 0");
    _extraFee = amount;
}
