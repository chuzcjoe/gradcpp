#pragma once

#include <array>
#include <memory>

namespace grad {

enum class Operation { NONE, ADD, SUBTRACT, MULTIPLY, DIVIDE };

struct Value {
  Value(float data, const Operation op = Operation::NONE);
  ~Value() = default;

  Value operator+(const Value& other) const;
  Value operator*(const Value& other) const;

  float data;
  float grad;
  Operation op;

 private:
  std::array<std::shared_ptr<Value>, 2> children_;
};

}  // namespace grad