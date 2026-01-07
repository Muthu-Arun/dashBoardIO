#include <deque>
#include <string>
namespace Widget {
class widget {
public:
  virtual void draw() = 0;
  virtual void action();
  virtual ~widget() = 0;
};
template <typename _data_type> class graph : public widget {
public:
  std::deque<_data_type> data;
  void draw() override {}
  ~graph() {}
};

class button : public widget {
public:
  void draw() override {}
  void action() override {}
};

template <typename _data_type = std::string> class text : public widget {
public:
  _data_type data; // Copying data to prevent race conditions or frequent locking
  void draw() override {}
};

} // namespace Widget