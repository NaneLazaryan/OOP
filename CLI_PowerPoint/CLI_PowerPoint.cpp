// CLI_PowerPoint.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

//#include <iostream>
//#include <string>
//#include <algorithm>
//#include "Models/Presentation.h"
//#include "Core/CommandParser.h"
//#include "Core/CommandExecutor.h"
//#include "UI/PresentationRenderer.h"

#include "Models/Appliaction.h"
#include <iostream>

int main()
{
    try {
        cli::Application app;
        app.run();
    }
    catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}