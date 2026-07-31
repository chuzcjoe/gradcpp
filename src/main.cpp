#include <iostream>

#include "Value.h"

int main() {
  grad::Value a(2.0f);
  grad::Value b(3.0f);
  grad::Value c = a + b;
  grad::Value d = a * b;

  std::cout << "a + b = " << c.data << '\n';
  std::cout << "a * b = " << d.data << '\n';

  return 0;
}
