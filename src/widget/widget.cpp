#include "widget.h"
#include <string_view>

namespace Widget {
widget::widget(std::string_view _label) : label(_label){}
widget::~widget() {}
} // namespace Widget