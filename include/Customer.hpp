#pragma once
#include <string>

class Customer {
private:
	unsigned int _id;
	std::string _name;
	std::string _phoneNumber;
	bool _isVIP = false;

	inline static unsigned int _totalNumberOfCustomers = 0;

public:
	Customer(const std::string& name, const std::string& phoneNumber) : _name(name), _phoneNumber(phoneNumber) {
		_id = ++_totalNumberOfCustomers;
	}

	unsigned int GetId() const { return _id; }
	const std::string& GetName() const { return _name; }
	const std::string& GetPhone() const { return _phoneNumber; }
	bool IsVIP() const { return _isVIP; }
	void SetVIP(bool status) { _isVIP = status; }

	Customer& operator=(const Customer& other) = delete;
	Customer(const Customer& other) = delete;

};