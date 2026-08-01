#include <iostream>

#include "Value.h"

int main() {
  // c = a + b
  // d = c * 3
  grad::Value a(2.0f);
  grad::Value b(3.0f);
  grad::Value c = a + b;
  grad::Value d = c * 3;

  d.Backward();

  // print the values and gradients
  std::cout << a.node << '\n';
  std::cout << b.node << '\n';
  std::cout << c.node << '\n';
  std::cout << d.node << '\n';

  return 0;
}
