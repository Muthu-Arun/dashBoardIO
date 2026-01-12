#pragma once
#include "widget.h"
#include <memory>
#include <string_view>
#include <unordered_map>
namespace Window {
class window {

public:

  std::string label; // Window Name
  std::unordered_map<std::string, std::unique_ptr<Widgets::Widget>> widgets;
  
  window(std::string_view label);
  window(const window&) = delete;
  window(window&&) = delete;
  void render();
  void addWidget(const std::string_view id, std::unique_ptr<Widgets::Widget> widget);
  void removeWidget(const std::string &id);
};
} // namespace Window