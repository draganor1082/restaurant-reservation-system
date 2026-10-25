#include "Table/Table.hpp"

Table::Table(unsigned int capacity, bool status) : _capacity(capacity), _isActive(status)
{
	if (capacity == 0) throw std::invalid_argument("Invalid capacity");
	_id = ++_nextId;

}

bool Table::CanAccommodate(unsigned int guests) const{
	return IsActive() && guests > 0 && guests <= GetCapacity();
}

bool Table::operator<(const Table& other) const
{
	return _capacity < other.GetCapacity();
}

bool Table::operator>(const Table& other) const
{
	return _capacity > other.GetCapacity();
}

bool Table::operator==(const Table& other) const
{
	return _capacity == other.GetCapacity();
}
