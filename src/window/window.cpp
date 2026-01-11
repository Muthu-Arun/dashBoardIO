#include "window.h"
#include <memory>
#include <string_view>

namespace Window {
window::window(std::string_view label) : label(label) {}
void window::render() {
  for (auto &widget : widgets) {
    widget.second->copyFromSource();
    widget.second->draw();
  }
}

void window::addWidget(const std::string_view id,
                       std::unique_ptr<Widget::widget> widget) {
  widgets.emplace(id, std::move(widget));
}

void window::removeWidget(const std::string &id) { widgets.erase(id); }
} // namespace Window