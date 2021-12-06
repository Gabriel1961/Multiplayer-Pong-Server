#pragma once
#include "IncludesAndDefines.h"
void LogException(websocketpp::exception const& e);
#define excheck(x) try{x;} catch (websocketpp::exception const& e) {LogException(e);}