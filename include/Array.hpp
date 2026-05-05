#pragma once
#include <optional>
#include <functional>
#include <utility>

template <typename elementType>

struct Array {
  std::optional<elementType>* data;
  int size = 0;
  int capacity = 10;

  Array() {
    data = new std::optional<elementType>[capacity];
  }

  Array(const Array<elementType> &otherArray) {
    capacity = otherArray.capacity;
    size = otherArray.size;
    data = new std::optional<elementType>[capacity];

    for (int i = 0; i < size; i++)
      data[i] = otherArray.data[i];
  }

  ~Array() {
    delete[] data;
  }

  elementType& operator[](int index) {
    return *data[index];
  }

  void sort(std::function<bool(elementType &value1, elementType &value2)> compare) {
    for (int i = 0; i < size; i++) {
      for (int j = i + 1; j < size; j++) {
        if (compare(*data[j], *data[i]))
          std::swap(data[i], data[j]);
      }
    }
  }

  Array& operator=(const Array& otherArray) {
    if (this == &otherArray) return *this; // Avoid assigning the same array

    delete[] data;
    capacity = otherArray.capacity;
    size = otherArray.size;
    data = new std::optional<elementType>[capacity];

    for (int i = 0; i < size; i++)
      data[i] = otherArray.data[i];

    return *this;
  }

  void push(elementType value) {
    if (size == capacity) {
      capacity *= 2;
      std::optional<elementType>* newData = new std::optional<elementType>[capacity];

      for (int i = 0; i < size; i++)
        newData[i] = data[i];

      delete[] data;
      data = newData;
    }

    data[size] = value;
    size++;
  }

  void erase(std::function<bool(elementType& value)> condition) {
    int lastIndex = 0;

    for (int i = 0; i < size; i++) {
      if (!condition(*data[i])) {
        data[lastIndex] = data[i];
        lastIndex++;
      }
    }

    size = lastIndex;
  }
};
