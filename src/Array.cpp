#include <Array.hpp>

// push function
template <typename elementType>
void Array<elementType>::push(elementType &value) {
  if (size == capacity) {
    capacity *= 2;
    elementType *newData = new elementType[capacity];

    for (int i = 0; i < size; i++)
      newData[i] = data[i];

    delete[] data;
    data = newData;
  }

  data[size] = value;
  size++;
}

// erase function
template <typename elementType>
void Array<elementType>::erase(std::function<bool(elementType &value)> condition) {
  int lastIndex = 0;

  for (int i = 0; i < size; i++) {
    if (!condition(data[i])) {
      data[lastIndex] = data[i];
      lastIndex++;
    }
  }

  size = lastIndex;
}

// destructor
template <typename elementType>
Array<elementType>::~Array() {
  delete[] data;
}

// constructor
template <typename elementType>
Array<elementType>::Array() {
  data = new elementType[capacity];
}

// operator overloading
template <typename elementType>
elementType &Array<elementType>::operator[](int index) {
  return data[index];
}
