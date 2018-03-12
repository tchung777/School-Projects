#include <iostream>

class Label{
  static unsigned _counter;
  unsigned _number;

  public:
    Label();
    unsigned number() const;
};

std::ostream &operator<<(std::ostream &os, const Label &lbl);
