#include <Array.hpp>


void Array::push(int& value) {
	if (size == capacity) {
		capacity *= 2;

		int* newData = new int[capacity];

		for (int i = 0; i < size; i++) {
			newData[i] = data[i];
		}

		delete[] data;
		data = newData;

	}
		data[size] = value;
		size++;
}

void Array::erase(std::function<bool(int &value)> condition) {
	int idx = 0;

	for (int i = 0; i < size; i++) {

		if (!condition(data[i])) {

			data[idx] = data[i];
			idx++;

		}
	}

	size = idx;
}

Array::~Array() {
	delete[] data;
}