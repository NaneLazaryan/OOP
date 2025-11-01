#include "Controller.h"
#include <iostream>

int main()
{
    try {
        cli::Controller controller;
        controller.run();
    }
    catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
