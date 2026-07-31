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

  struct Node;

  [[nodiscard]] float data() const;
  [[nodiscard]] float grad() const;
  [[nodiscard]] const std::array<std::shared_ptr<Node>, 2>& previous() const;

  std::shared_ptr<Node> node;

  // void BuildTopoOrder(const std::shared_ptr<Node>& start_node,
  //                     std::vector<std::shared_ptr<Node>>& topo_order) const;

 private:
  explicit Value(std::shared_ptr<Node> node);
};

std::ostream& operator<<(std::ostream& os,
                         const std::shared_ptr<Value::Node>& node);

}  // namespace grad