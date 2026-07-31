#pragma once

#include <array>
#include <memory>

namespace grad {

enum class Operation { NONE, ADD, MULTIPLY };

struct Value {
  Value(float data, const Operation op = Operation::NONE);
  ~Value() = default;

  Value operator+(const Value& other) const;
  Value operator*(const Value& other) const;

  friend Value operator+(float scalar, const Value& value);
  friend Value operator*(float scalar, const Value& value);

  struct Node {
    Node(float data, Operation op) : data(data), grad(0.0f), op(op) {}

    float data;
    float grad;
    Operation op;

    std::array<std::shared_ptr<Node>, 2> previous{};
  };

  std::shared_ptr<Node> node;

 private:
  explicit Value(std::shared_ptr<Node> node);
};

}  // namespace grad