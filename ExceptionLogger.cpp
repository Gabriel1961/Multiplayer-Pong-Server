#include "./ExceptionLogger.h"

void LogException(websocketpp::exception const& e)
{
	std::cout << "Echo failed because: " << "(" << e.what() << ")" << std::endl;
}
