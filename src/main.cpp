#include <iostream>

#include "Value.h"

int main() {
  grad::Value a(2.0f);
  grad::Value b(3.0f);
  grad::Value c = a + b;
  grad::Value d = a * b;
  grad::Value e = a + 1;
  grad::Value f = 1 + a;
  grad::Value g = a * 2;
  grad::Value h = 2 * a;

  std::cout << "a = " << a.data << '\n';
  std::cout << "b = " << b.data << '\n';

  std::cout << "a + b = " << c.data << '\n';
  std::cout << "a * b = " << d.data << '\n';
  std::cout << "a + 1 = " << e.data << '\n';
  std::cout << "1 + a = " << f.data << '\n';
  std::cout << "a * 2 = " << g.data << '\n';
  std::cout << "2 * a = " << h.data << '\n';

  return 0;
}
