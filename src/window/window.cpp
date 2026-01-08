#include "window.h"
#include <memory>

namespace Window {
void window::render() {
  for (auto& widget : widgets) {
    widget.second->draw();
  }
}

void window::addWidget(const std::string &id, std::unique_ptr<Widget::widget> widget){
  widgets.emplace(id, std::move(widget));
}

void window::removeWidget(const std::string &id){
  widgets.erase(id);
}
} // namespace Window