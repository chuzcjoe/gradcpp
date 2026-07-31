#include <iostream>

#include "Value.h"

int main() {
  // c = a + b
  // d = c * 3
  grad::Value a(2.0f);
  grad::Value b(3.0f);
  grad::Value c = a + b;
  grad::Value d = c * 3;

  auto topo_order = d.BuildTopoOrder();

  std::cout << "c previous: " << c.previous()[0] << ", " << c.previous()[1]
            << '\n';
  std::cout << "d previous: " << d.previous()[0] << ", " << d.previous()[1]
            << '\n';

  std::cout << "Topo order of node C in the computation graph:\n";
  for (const auto& node : topo_order) {
    std::cout << node << '\n';
  }

  return 0;
}
