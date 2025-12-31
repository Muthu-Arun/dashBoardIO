#include <cstddef>
#include <vector>
namespace Utils {
    template<typename T>
    class CircularBuffer{
        private:
            std::vector<T> _buffer;
            size_t index = 0, size;
        public:
            CircularBuffer(size_t s) : _buffer(s), size(s) {}
            void insert(T value){
                _buffer[index] = value;
                index = (index + 1) % size;
            }
            T get(){
                if(index - 1 > 0) [[likely]]{
                    return _buffer[index - 1];
                }
                else{
                    return _buffer[size - 1];
                }
            }
    };

}