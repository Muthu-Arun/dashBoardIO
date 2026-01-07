#include "widget.h"
#include <memory>
#include <vector>
namespace Window {
class window {
public:
  std::vector<std::unique_ptr<Widget::widget>> widgets;
  void render();

};
} // namespace Window