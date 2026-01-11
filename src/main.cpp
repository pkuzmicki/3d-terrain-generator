#include "core/app_panel.h"
#include <glm/glm.hpp>
#include <iostream>

//cmake -G Ninja ..
//ninja

int main() {
    AppPanel& ap_ = AppPanel::getInstance();
    ap_.init();
    ap_.run();  
    return 0;
}