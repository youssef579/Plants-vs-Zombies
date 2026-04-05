#include<functional>

struct Array {
	int* data;
	int size = 0;
	int capacity = 10;

	void push(int &value);

	void erase(std::function<bool(int &value)> condition);
};