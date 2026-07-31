#include "Value.h"

namespace grad {

Value::Value(float data, const Operation op) : data(data), grad(0.0f), op(op) {}

Value Value::operator+(const Value& other) const {
  float new_data = this->data + other.data;
  return Value(new_data, Operation::ADD);
}

Value Value::operator*(const Value& other) const {
  float new_data = this->data * other.data;
  return Value(new_data, Operation::MULTIPLY);
}

}  // namespace grad