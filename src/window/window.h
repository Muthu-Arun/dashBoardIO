#include "widget.h"
#include <map>
#include <memory>
#include <unordered_map>
#include <vector>
namespace Window {
class window {
public:
  std::unordered_map<std::string, std::unique_ptr<Widget::widget>> widgets;
  void render();
  void addWidget(const std::string &id, std::unique_ptr<Widget::widget> widget);
  void removeWidget();
};
} // namespace Window