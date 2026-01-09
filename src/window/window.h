#include "widget.h"
#include <memory>
#include <unordered_map>
namespace Window {
class window {
public:
  std::unordered_map<std::string, std::unique_ptr<Widget::widget>> widgets;
  void render();
  void addWidget(const std::string &id, std::unique_ptr<Widget::widget> widget);
  void removeWidget(const std::string &id);
};
} // namespace Window