#include "widget.h"
#include <string_view>

namespace Widgets {
Widget::Widget(std::string_view _label) : label(_label){}
Widget::~Widget() {}
} // namespace Widget