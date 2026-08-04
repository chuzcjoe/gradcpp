#include <array>
#include <iostream>
#include <vector>

#include "Graphviz.h"
#include "Network.h"
#include "Value.h"

// NOLINTNEXTLINE(bugprone-exception-escape)
int main() {
  // c = a + b
  // d = c * 3
  // grad::Value a(2.0f, "a");
  // grad::Value b(3.0f, "b");
  // grad::Value c = a + b;
  // c.SetLabel("c");
  // grad::Value d = c * 3;
  // d.SetLabel("d");
  // grad::Value e = d + a;
  // e.SetLabel("e");
  // grad::Value f = e.Power(2.0f);
  // f.SetLabel("f");
  // grad::Value g = f.Relu();
  // g.SetLabel("g");

  // g.Backward();

  // grad::Graphviz(g).Print(std::cout);

  grad::Value a(2.0f, "a");
  grad::Value b(3.0f, "b");
  grad::Value c(4.0f, "c");
  grad::Value d = a + b + c;
  d.SetLabel("d");
  d.Backward();
  grad::Graphviz(d).Print(std::cout);

  // inputs: 3
  // layer1: 4 neurons
  // layer2: 4 neurons
  // layer3: 1 neuron
  grad::MLP mlp(3, {4, 4, 1});
  constexpr std::array<std::array<float, 3>, 3> inputs{{
      {2.0f, 3.0f, -1.0f},
      {3.0f, -1.0f, 0.5f},
      {0.5f, 1.0f, 1.0f},
  }};
  constexpr std::array<float, 3> targets{-1.0f, 1.0f, -1.0f};
  constexpr std::size_t training_steps = 20;
  constexpr float learning_rate = 0.1f;

  std::cout << "MLP parameter count: " << mlp.ParameterCount() << '\n';

  const auto parameters = mlp.Parameters();

  // Define a lambda function to perform predictions using the MLP model
  const auto predict = [&mlp, &inputs] {
    std::vector<grad::Value> predictions;
    predictions.reserve(inputs.size());

    for (const auto& [x1, x2, x3] : inputs) {
      predictions.push_back(
          mlp({grad::Value(x1), grad::Value(x2), grad::Value(x3)}).front());
    }
    return predictions;
  };

  // Define a lambda function to calculate the loss based on predictions and
  // targets
  const auto calculate_loss = [&targets](const auto& predictions) {
    grad::Value loss(0.0f);
    for (std::size_t i = 0; i < predictions.size(); ++i) {
      const auto error = predictions[i] + (-targets[i]);
      loss = loss + error.Power(2.0f);
    }
    return loss;
  };

  // Training loop for the MLP model
  for (std::size_t step = 0; step < training_steps; ++step) {
    const auto predictions = predict();
    auto loss = calculate_loss(predictions);
    std::cout << "step " << step << ", loss: " << loss.data() << '\n';

    mlp.ZeroGrad();
    loss.Backward();

    for (grad::Value* parameter : parameters) {
      parameter->node->data -= learning_rate * parameter->grad();
    }
  }

  std::cout << "Network output:\n";
  for (const auto& prediction : predict()) {
    std::cout << prediction.data() << '\n';
  }

  return 0;
}
