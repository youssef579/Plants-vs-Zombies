#pragma once

#include <functional>

template <typename elementType>
struct Array {
  elementType *data;
  int size = 0;
  int capacity = 10;

  Array();

  ~Array();

  elementType &operator[](int index);

  void push(elementType &value);

  void erase(std::function<bool(elementType &value)> condition);
};
