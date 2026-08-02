#include "Value.h"

#include <algorithm>
#include <ostream>
#include <unordered_set>

namespace grad {

Value::Value(float data, const Operation op)
    : node(std::make_shared<Node>(data, op)) {}

Value::Value(float data, const std::string_view label)
    : node(std::make_shared<Node>(data, Operation::NONE, std::string(label))) {}

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

Value Value::Power(const float x) const {
  auto new_node =
      std::make_shared<Node>(std::pow(node->data, x), Operation::POWER);
  auto power_out = Value(x);
  new_node->previous = {node, power_out.node};
  return Value(std::move(new_node));
}

Value Value::Relu() const {
  auto new_node =
      std::make_shared<Node>(std::max(0.0f, node->data), Operation::RELU);
  new_node->previous = {node, nullptr};
  return Value(std::move(new_node));
}

float Value::data() const { return node->data; }
float Value::grad() const { return node->grad; }
const std::string& Value::label() const { return node->label; }

Value& Value::SetLabel(const std::string_view label) {
  node->label = label;
  return *this;
}

const std::array<std::shared_ptr<Value::Node>, 2>& Value::previous() const {
  return node->previous;
}

std::vector<std::shared_ptr<Value::Node>> Value::BuildTopoOrder() const {
  std::vector<std::shared_ptr<Node>> order;
  std::unordered_set<const Node*> visited;

  const auto visit = [&visited, &order](
                         auto&& self,
                         const std::shared_ptr<Node>& current) -> void {
    if (current == nullptr || !visited.insert(current.get()).second) {
      return;
    }

    for (const auto& previous : current->previous) {
      self(self, previous);
    }

    order.push_back(current);
  };

  visit(visit, node);

  return order;
}

// Only the root node should call this function.
void Value::Backward() {
  auto topo_order = BuildTopoOrder();
  node->grad = 1.0f;

  std::ranges::reverse(topo_order);
  for (const auto& current : topo_order) {
    switch (current->op) {
      case Operation::ADD:
        current->previous[0]->grad += current->grad;
        current->previous[1]->grad += current->grad;
        break;
      case Operation::MULTIPLY:
        current->previous[0]->grad +=
            current->previous[1]->data * current->grad;
        current->previous[1]->grad +=
            current->previous[0]->data * current->grad;
        break;
      case Operation::POWER:
        current->previous[0]->grad +=
            current->previous[1]->data *
            std::pow(current->previous[0]->data,
                     current->previous[1]->data - 1.0f) *
            current->grad;
        break;
      case Operation::RELU:
        current->previous[0]->grad +=
            (current->previous[0]->data > 0.0f ? 1.0f : 0.0f) * current->grad;
        break;
      case Operation::NONE:
        break;
    }
  }
}

std::ostream& operator<<(std::ostream& os,
                         const std::shared_ptr<Value::Node>& node) {
  return os << "[Value(" << node->data << "), grad=" << node->grad << "]";
}

}  // namespace grad
