#include "widget.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
namespace ParseJson {

class HttpWindowWrapper {
protected:
  std::unordered_map<std::string, std::unique_ptr<Widgets::Widget>> widgets;

  std::unordered_map<std::string, std::shared_ptr<int>> MapInt;
  std::unordered_map<std::string, std::shared_ptr<float>> MapFloat;
  std::unordered_map<std::string, std::shared_ptr<std::string>> MapString;
  std::string host_endpoint;

public:
};
} // namespace ParseJson
