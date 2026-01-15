#include "window.h"

#include <memory>
#include <string_view>

namespace Window {
Window::Window(std::string_view label) : label(label) {}
void Window::render() {
    for (auto&[_, widget]: widgets) {
        widget->copyFromSource();
        widget->draw();
    }
}

void Window::addWidget(const std::string_view id, std::unique_ptr<Widgets::Widget> widget) {
    widgets.emplace(id, std::move(widget));
}

void Window::removeWidget(const std::string& id) {
    widgets.erase(id);
}
}  // namespace Window