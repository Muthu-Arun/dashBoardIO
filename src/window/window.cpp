#include "window.h"

#include <algorithm>
#include <memory>
#include <string>
#include <string_view>

namespace Window {
Window::Window(std::string_view label) : label(label) {}
void Window::render() {
    for (auto&[_, widget]: widgets) {
        widget->draw();
    }
}

void Window::addWidget(const std::string_view id, std::unique_ptr<Widgets::Widget> widget) {
    widgets.emplace(id, std::move(widget));
}

void Window::updateWidget(const std::string& id, std::unique_ptr<Widgets::Widget> widget) {
    widgets[id] = std::move(widget);
}

bool Window::isWidgetPresent(const std::string& id){
    return widgets.find(id) != widgets.end();
}
void Window::removeWidget(const std::string& id) {
    widgets.erase(id);
}
}  // namespace Window