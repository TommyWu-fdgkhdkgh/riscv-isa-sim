#include "processor.h"
#include <iostream>

class test : public processor_t {
    void gg();
};

void test::gg() {
    std::cout << debug << std::endl;
}
