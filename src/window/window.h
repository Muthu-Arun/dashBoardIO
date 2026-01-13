#pragma once
#include <memory>
#include <string_view>
#include <unordered_map>

#include "widget.h"
namespace Window
{
class Window
{
public:
    std::string label;  // Window Name
    std::unordered_map<std::string, std::unique_ptr<Widgets::Widget>> widgets;

    Window(std::string_view label);
    Window(const Window&) = delete;
    Window(Window&&) = delete;
    void render();
    void addWidget(const std::string_view id, std::unique_ptr<Widgets::Widget> widget);
    void removeWidget(const std::string& id);
};
}  // namespace Window