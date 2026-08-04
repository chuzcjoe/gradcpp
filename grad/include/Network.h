#pragma once

#include <random>

#include "Value.h"

namespace grad {

class Module {
 public:
  Module() = default;
  virtual ~Module() = default;

  [[nodiscard]] std::vector<Value*> Parameters() {
    std::vector<Value*> parameters;
    parameters.reserve(ParameterCount());
    AppendParameters(parameters);
    return parameters;
  }

  void ZeroGrad() {
    for (Value* parameter : Parameters()) {
      parameter->node->grad = 0.0f;
    }
  }

  [[nodiscard]] virtual std::size_t ParameterCount() const = 0;
  virtual void AppendParameters(std::vector<Value*>& parameters) = 0;
};

class Neuron : public Module {
 public:
  explicit Neuron(int num_inputs) : bias(0.0f) {
    static thread_local std::mt19937 generator(std::random_device{}());
    std::uniform_real_distribution<float> distribution(-1.0f, 1.0f);

    weights.reserve(num_inputs);
    for (int i = 0; i < num_inputs; ++i) {
      weights.emplace_back(distribution(generator));
    }
    bias = Value(distribution(generator));
  }

  Value operator()(const std::vector<Value>& inputs) {
    if (inputs.size() != weights.size()) {
      throw std::invalid_argument("Input size does not match weights size.");
    }

    Value sum = bias;
    for (size_t i = 0; i < inputs.size(); ++i) {
      sum = sum + (weights[i] * inputs[i]);
    }
    return sum.Tanh();
  }

  [[nodiscard]] std::size_t ParameterCount() const override {
    return weights.size() + 1;
  }

  void AppendParameters(std::vector<Value*>& parameters) override {
    for (auto& weight : weights) {
      parameters.push_back(&weight);
    }
    parameters.push_back(&bias);
  }

  std::vector<Value> weights;
  Value bias;
};

class Layer : public Module {
 public:
  explicit Layer(int num_inputs, int num_neurons) {
    neurons.reserve(num_neurons);
    for (int i = 0; i < num_neurons; ++i) {
      neurons.emplace_back(num_inputs);
    }
  }

  std::vector<Value> operator()(const std::vector<Value>& inputs) {
    std::vector<Value> outputs;
    outputs.reserve(neurons.size());
    for (auto& neuron : neurons) {
      outputs.push_back(neuron(inputs));
    }
    return outputs;
  }

  [[nodiscard]] std::size_t ParameterCount() const override {
    std::size_t count = 0;
    for (const auto& neuron : neurons) {
      count += neuron.ParameterCount();
    }
    return count;
  }

  void AppendParameters(std::vector<Value*>& parameters) override {
    for (auto& neuron : neurons) {
      neuron.AppendParameters(parameters);
    }
  }

  std::vector<Neuron> neurons;
};

class MLP : public Module {
 public:
  explicit MLP(int num_inputs, const std::vector<int>& layer_sizes) {
    layers.reserve(layer_sizes.size());
    int current_inputs = num_inputs;
    for (int size : layer_sizes) {
      layers.emplace_back(current_inputs, size);
      current_inputs = size;
    }
  }

  std::vector<Value> operator()(const std::vector<Value>& inputs) {
    std::vector<Value> current_outputs = inputs;
    for (auto& layer : layers) {
      current_outputs = layer(current_outputs);
    }
    return current_outputs;
  }

  [[nodiscard]] std::size_t ParameterCount() const override {
    std::size_t count = 0;
    for (const auto& layer : layers) {
      count += layer.ParameterCount();
    }
    return count;
  }

  void AppendParameters(std::vector<Value*>& parameters) override {
    for (auto& layer : layers) {
      layer.AppendParameters(parameters);
    }
  }

  std::vector<Layer> layers;
};

}  // namespace grad
