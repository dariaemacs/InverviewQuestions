// Пример того, как можно использовать паттерн CRTP для создания operator>
// Для любого класса, который поддерживает operator<

#include <cassert>

// Отсортированные объекты
template <class T> 
struct Ordered{
  
  bool operator>(T const& rhs) const {
    T const& self = static_cast<T const&>(*this);
    return rhs < self; 
  }
};

// Класс целочисленной переменной
class Int : public Ordered<Int>{
public:
  explicit Int(int x) : value(x) {}

  bool operator<(Int const& rhs) const {
    return this->value < rhs.value;
  }
  
  int value;
};

int main() {
  assert(Int(2) < Int(4));
  assert(Int(3) > Int(1));
}
