#pragma once

#include <array>
#include <memory>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace grad {

enum class Operation { NONE, ADD, MULTIPLY, POWER, RELU, TANH };

struct Value {
  Value(float data, const Operation op = Operation::NONE);
  Value(float data, std::string_view label);

  ~Value() = default;

  Value operator+(const Value& other) const;

  Value operator*(const Value& other) const;

  friend Value operator+(float scalar, const Value& value);

  friend Value operator*(float scalar, const Value& value);

  Value Power(const float x) const;

  Value Relu() const;

  Value Tanh() const;

  struct Node {
    Node(float data, Operation op, std::string label = {})
        : data(data), op(op), label(std::move(label)) {}

    float data;
    float grad = 0.0F;
    Operation op;
    std::string label;

    std::array<std::shared_ptr<Node>, 2> previous{};
  };

  [[nodiscard]] float data() const;

  [[nodiscard]] float grad() const;

  [[nodiscard]] const std::string& label() const;

  Value& SetLabel(std::string_view label);

  [[nodiscard]] const std::array<std::shared_ptr<Node>, 2>& previous() const;

  void Backward();

  std::shared_ptr<Node> node;

 private:
  explicit Value(std::shared_ptr<Node> node);

  [[nodiscard]] std::vector<std::shared_ptr<Node>> BuildTopoOrder() const;
};

std::ostream& operator<<(std::ostream& os,
                         const std::shared_ptr<Value::Node>& node);

}  // namespace grad
