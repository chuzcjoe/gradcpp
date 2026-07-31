#include "Value.h"

#include <ostream>
#include <unordered_set>

namespace grad {

struct Value::Node {
  Node(float data, Operation op) : data(data), op(op) {}

  float data;
  float grad = 0.0f;
  Operation op;

  std::array<std::shared_ptr<Node>, 2> previous{};
};

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

// void Value::BuildTopoOrder(std::vector<std::shared_ptr<Node>>& topo_order)
// const {
//   std::unordered_set<std::shared_ptr<Node>> visited;
//   std::function<void(const std::shared_ptr<Node>&)> dfs =
//       [&](const std::shared_ptr<Node>& node) {
//         if (!node || visited.count(node)) return;
//         visited.insert(node);
//         for (const auto& prev : node->previous) {
//           dfs(prev);
//         }
//         topo_order.push_back(node);
//       };
//   dfs(node);
// }

float Value::data() const { return node->data; }
float Value::grad() const { return node->grad; }
const std::array<std::shared_ptr<Value::Node>, 2>& Value::previous() const {
  return node->previous;
}

std::ostream& operator<<(std::ostream& os,
                         const std::shared_ptr<Value::Node>& node) {
  return os << "[Value(" << node->data << "), grad=" << node->grad << "]";
}

}  // namespace grad