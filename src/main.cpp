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

  std::cout << "a = " << a.node->data << '\n';
  std::cout << "b = " << b.node->data << '\n';

  std::cout << "a + b = " << c.node->data << '\n';
  std::cout << "a * b = " << d.node->data << '\n';
  std::cout << "a + 1 = " << e.node->data << '\n';
  std::cout << "1 + a = " << f.node->data << '\n';
  std::cout << "a * 2 = " << g.node->data << '\n';
  std::cout << "2 * a = " << h.node->data << '\n';

  std::cout << "c previous nodes: " << c.node->previous[0] << ", "
            << c.node->previous[1] << '\n';
  std::cout << "d previous nodes: " << d.node->previous[0] << ", "
            << d.node->previous[1] << '\n';
  std::cout << "e previous nodes: " << e.node->previous[0] << ", "
            << e.node->previous[1] << '\n';
  std::cout << "f previous nodes: " << f.node->previous[0] << ", "
            << f.node->previous[1] << '\n';
  std::cout << "g previous nodes: " << g.node->previous[0] << ", "
            << g.node->previous[1] << '\n';
  std::cout << "h previous nodes: " << h.node->previous[0] << ", "
            << h.node->previous[1] << '\n';

  return 0;
}
