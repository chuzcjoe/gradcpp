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

  std::cout << "a = " << a.data() << '\n';
  std::cout << "b = " << b.data() << '\n';

  std::cout << "a + b = " << c.data() << '\n';
  std::cout << "a * b = " << d.data() << '\n';
  std::cout << "a + 1 = " << e.data() << '\n';
  std::cout << "1 + a = " << f.data() << '\n';
  std::cout << "a * 2 = " << g.data() << '\n';
  std::cout << "2 * a = " << h.data() << '\n';

  std::cout << "c previous nodes: " << c.previous()[0] << ", "
            << c.previous()[1] << '\n';
  std::cout << "d previous nodes: " << d.previous()[0] << ", "
            << d.previous()[1] << '\n';
  std::cout << "e previous nodes: " << e.previous()[0] << ", "
            << e.previous()[1] << '\n';
  std::cout << "f previous nodes: " << f.previous()[0] << ", "
            << f.previous()[1] << '\n';
  std::cout << "g previous nodes: " << g.previous()[0] << ", "
            << g.previous()[1] << '\n';
  std::cout << "h previous nodes: " << h.previous()[0] << ", "
            << h.previous()[1] << '\n';

  return 0;
}
