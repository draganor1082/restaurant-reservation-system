#pragma once

class Table {
private:
	inline static unsigned int _nextId = 0;

	unsigned int _id;
	unsigned int _capacity;
	bool _isActive;
protected:
	Table(unsigned int capacity, bool status = true);
public:

	Table(const Table& other) = delete;
	Table& operator=(const Table& other) = delete;


	virtual ~Table() = default;

	unsigned int GetId()  const { return _id; }
	unsigned int GetCapacity() const { return _capacity; }
	bool IsActive() const { return _isActive; }
	void SetActive(bool status) { _isActive = status; }

	virtual bool CanAccommodate(unsigned int guests) const;

	bool operator<(const Table& other) const;
	bool operator>(const Table& other) const;
	bool operator==(const Table& other) const;
};