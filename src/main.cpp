#include <iostream>

#include "Graphviz.h"
#include "Value.h"

int main() {
  // c = a + b
  // d = c * 3
  grad::Value a(2.0f, "a");
  grad::Value b(3.0f, "b");
  grad::Value c = a + b;
  c.SetLabel("c");
  grad::Value d = c * 3;
  d.SetLabel("d");
  grad::Value e = d + a;
  e.SetLabel("e");
  grad::Value f = e.Power(2.0f);

  f.Backward();

  grad::Graphviz(f).Print(std::cout);

  return 0;
}
