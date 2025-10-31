#include "Controller.h"
#include "Command.h"

#include <iostream>

void Controller::run()
{
	std::cout << "Presentation Editor Ready. Enter commands (or 'exit' to quit):\n";

	std::string line;
	

	while (!m_exit) {
		std::cout << " > ";

		if (!std::getline(std::cin, line)) {
			break; 
		}

		if (line == "exit") {
			std::cout << "Exiting...\n";
			exit();
			break;
		}

		if (line.empty()) {
			continue;
		}

		try {
			std::getline(std::cin, line);
			std::istringstream iss(line);

			Parser parser(iss);
			std::unique_ptr<ICommand> cmd = parser.parse();
			if (!cmd) {
				handleError("Failed to parse command");
				return;
			}

			cmd->execute(presentation);
			std::cout << "Command executed successfully.\n";
		}
		catch (const std::invalid_argument& ex) {
			handleError(std::string("Parse error: ") + ex.what());
		}
		catch (const std::runtime_error& ex) {
			handleError(std::string("Runtime error: ") + ex.what());
		}
		catch (const std::exception& ex) {
			handleError(std::string("Error: ") + ex.what());
		}
	}
}

void Controller::exit()
{
	m_exit = true;
}

void Controller::handleError(const std::string& message)
{
	std::cerr << "ERROR: " << message << "\n";
}
