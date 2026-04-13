#pragma once
#include <optional>
#include <functional>

template <typename elementType>

struct Array {
  std::optional<elementType>* data;
  int size = 0;
  int capacity = 10;

  Array() {
    data = new std::optional<elementType>[capacity];
  }

  ~Array() {
    delete[] data;
  }

  elementType& operator[](int index) {
    return *data[index];
  }

  void push(elementType& value) {
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
