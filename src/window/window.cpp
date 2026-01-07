#include "window.h"

namespace Window {
void window::render() {
  for (auto& widget : widgets) {
    widget->draw();
  }
}
} // namespace Window