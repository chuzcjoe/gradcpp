#include "Value.h"

namespace grad {

Value::Value(float data, const Operation op)
    : node(std::make_shared<Node>(data, op)) {}

Value::Value(std::shared_ptr<Node> node) : node(std::move(node)) {}

Value Value::operator+(const Value& other) const {
  auto new_node =
      std::make_shared<Node>(node->data + other.node->data, Operation::ADD);
  new_node->previous = {node, other.node};
  return Value(std::move(new_node));
}

Value Value::operator*(const Value& other) const {
  auto new_node = std::make_shared<Node>(node->data * other.node->data,
                                         Operation::MULTIPLY);
  new_node->previous = {node, other.node};
  return Value(std::move(new_node));
}

Value operator+(float scalar, const Value& value) {
  return Value(scalar) + value;
}

Value operator*(float scalar, const Value& value) {
  return Value(scalar) * value;
}

}  // namespace grad