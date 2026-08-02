#include "Graphviz.h"

#include <algorithm>
#include <cstddef>
#include <iomanip>
#include <ostream>
#include <sstream>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

namespace grad {
namespace {

struct Canvas {
  std::vector<std::string> lines;
  std::size_t width = 0;
  std::size_t center = 0;
};

struct PositionedText {
  std::size_t position;
  std::string text;
};

struct ChildCanvases {
  Canvas left;
  Canvas right;
};

struct GraphStep {
  Canvas operation;
  Canvas result;
};

std::size_t DisplayWidth(const std::string& text) {
  return static_cast<std::size_t>(
      std::ranges::count_if(text, [](const unsigned char character) {
        return (character & 0xC0U) != 0x80U;
      }));
}

std::string PadRight(std::string text, const std::size_t width) {
  text.append(width - DisplayWidth(text), ' ');
  return text;
}

std::string TrimRight(std::string text) {
  while (!text.empty() && text.back() == ' ') {
    text.pop_back();
  }
  return text;
}

std::string JoinCells(const std::vector<std::string>& cells) {
  std::string line;
  for (const auto& cell : cells) {
    line += cell;
  }
  return line;
}

std::string Repeat(const std::string& text, const std::size_t count) {
  std::string result;
  result.reserve(text.size() * count);
  for (std::size_t index = 0; index < count; ++index) {
    result += text;
  }
  return result;
}

std::string Place(const std::size_t width, const PositionedText& positioned) {
  std::vector<std::string> cells(width, " ");
  cells[positioned.position] = positioned.text;
  return JoinCells(cells);
}

Canvas MakeBox(const std::vector<std::string>& labels) {
  std::size_t content_width = 0;
  for (const auto& label : labels) {
    content_width = std::max(content_width, DisplayWidth(label));
  }

  Canvas canvas;
  canvas.width = content_width + 4;
  canvas.center = canvas.width / 2;
  canvas.lines.push_back("┌" + Repeat("─", content_width + 2) + "┐");
  for (const auto& label : labels) {
    canvas.lines.push_back("│ " + PadRight(label, content_width) + " │");
  }
  canvas.lines.push_back("└" + Repeat("─", content_width + 2) + "┘");
  return canvas;
}

std::string FormatFloat(const float value) {
  std::ostringstream output;
  output << std::setprecision(6) << value;
  return output.str();
}

std::string OperationSymbol(const Operation operation) {
  switch (operation) {
    case Operation::ADD:
      return "+";
    case Operation::MULTIPLY:
      return "*";
    case Operation::POWER:
      return "^";
    case Operation::NONE:
      return "?";
  }
  return "?";
}

class Renderer {
 public:
  explicit Renderer(std::shared_ptr<Value::Node> root)
      : root_(std::move(root)) {}

  Canvas Render() { return RenderNode(root_, true); }

 private:
  static constexpr std::size_t kSiblingGap = 5;

  [[nodiscard]] static Canvas MakeValueBox(
      const std::shared_ptr<Value::Node>& node, const bool is_root) {
    std::string name = node->label;
    if (is_root) {
      name += name.empty() ? "root" : " (root)";
    }

    std::vector<std::string> labels;
    if (!name.empty()) {
      labels.push_back(name);
    }
    labels.push_back("value = " + FormatFloat(node->data));
    labels.push_back("grad  = " + FormatFloat(node->grad));
    return MakeBox(labels);
  }

  [[nodiscard]] static Canvas MakeReference(
      const std::shared_ptr<Value::Node>& node) {
    const std::string text =
        node->label.empty() ? "↖ reused" : "↖ " + node->label + " (reused)";
    return {.lines = {text}, .width = DisplayWidth(text), .center = 0};
  }

  Canvas RenderNode(const std::shared_ptr<Value::Node>& node,
                    const bool is_root) {
    const bool is_shared = !expanded_.insert(node.get()).second;
    const bool has_inputs = node->previous[0] != nullptr &&
                            node->previous[1] != nullptr &&
                            node->op != Operation::NONE;
    if (is_shared) {
      return MakeReference(node);
    }
    if (!has_inputs) {
      return MakeValueBox(node, is_root);
    }

    Canvas left = RenderNode(node->previous[0], false);
    Canvas right = RenderNode(node->previous[1], false);
    return Combine({.left = std::move(left), .right = std::move(right)},
                   {.operation = MakeBox({OperationSymbol(node->op)}),
                    .result = MakeValueBox(node, is_root)});
  }

  [[nodiscard]] static Canvas Combine(ChildCanvases children,
                                      const GraphStep& step) {
    Canvas& left = children.left;
    Canvas& right = children.right;
    const std::size_t child_height =
        std::max(left.lines.size(), right.lines.size());
    left.lines.insert(left.lines.begin(), child_height - left.lines.size(),
                      std::string(left.width, ' '));
    right.lines.insert(right.lines.begin(), child_height - right.lines.size(),
                       std::string(right.width, ' '));

    Canvas canvas;
    canvas.width = left.width + kSiblingGap + right.width;
    const std::size_t right_offset = left.width + kSiblingGap;
    const std::size_t left_center = left.center;
    const std::size_t right_center = right_offset + right.center;
    canvas.center = (left_center + right_center) / 2;

    for (std::size_t row = 0; row < child_height; ++row) {
      canvas.lines.push_back(PadRight(left.lines[row], left.width) +
                             std::string(kSiblingGap, ' ') +
                             PadRight(right.lines[row], right.width));
    }

    std::vector<std::string> branches(canvas.width, " ");
    branches[left_center] = "└";
    branches[right_center] = "┘";
    for (std::size_t column = left_center + 1; column < right_center;
         ++column) {
      branches[column] = "─";
    }
    branches[canvas.center] = "┬";
    canvas.lines.push_back(JoinCells(branches));
    canvas.lines.push_back(
        Place(canvas.width, {.position = canvas.center, .text = "▼"}));

    AppendCentered(canvas, step.operation);
    canvas.lines.push_back(
        Place(canvas.width, {.position = canvas.center, .text = "│"}));
    canvas.lines.push_back(
        Place(canvas.width, {.position = canvas.center, .text = "▼"}));
    AppendCentered(canvas, step.result);
    return canvas;
  }

  static void AppendCentered(Canvas& destination, const Canvas& source) {
    const std::size_t offset = destination.center - source.center;
    for (const auto& line : source.lines) {
      destination.lines.push_back(
          std::string(offset, ' ') + PadRight(line, source.width) +
          std::string(destination.width - offset - source.width, ' '));
    }
  }

  std::shared_ptr<Value::Node> root_;
  std::unordered_set<const Value::Node*> expanded_;
};

}  // namespace

Graphviz::Graphviz(const Value& root) : root_(root.node) {}

std::string Graphviz::Render() const {
  Renderer renderer(root_);
  const Canvas canvas = renderer.Render();

  std::ostringstream output;
  output << "FORWARD ↓\n\n";
  for (const auto& line : canvas.lines) {
    output << TrimRight(line) << '\n';
  }
  output << "\n↑ BACKWARD\n";
  return output.str();
}

void Graphviz::Print(std::ostream& output) const { output << Render(); }

}  // namespace grad
