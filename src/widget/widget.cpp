#include "widget.h"
#include "imgui.h"
#include <cstddef>
#include <memory>
#include <mutex>
#include <string_view>

namespace Widgets {
Widget::Widget(std::string_view _label) : label(_label) {}
Widget::~Widget() {}
TextInput::TextInput(std::string_view label, std::shared_ptr<std::string> &src,
                     std::mutex &src_mtx, size_t)
    : Widget(label), src(src), src_mtx(src_mtx) {
  data.resize(string_capacity);
}
TextInput::~TextInput() {}
void TextInput::draw() {
  ImGui::InputText(label.c_str(), data.data(), string_capacity);
}
void TextInput::copyFromSource() {
  if (is_data_available.load()) {
    is_being_copied.store(true);
    std::lock_guard<std::mutex> lock(src_mtx);
    data = *src;
    is_data_available.store(false);
    is_being_copied.store(false);
  }
}
} // namespace Widgets