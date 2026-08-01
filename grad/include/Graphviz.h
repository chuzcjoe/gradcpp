#pragma once

#include <iosfwd>
#include <memory>
#include <string>

#include "Value.h"

namespace grad {

class Graphviz {
 public:
  explicit Graphviz(const Value& root);

  [[nodiscard]] std::string Render() const;
  void Print(std::ostream& output) const;

 private:
  std::shared_ptr<Value::Node> root_;
};

}  // namespace grad
