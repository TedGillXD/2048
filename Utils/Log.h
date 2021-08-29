#pragma once
#include <string>

class Log
{
public:
	static void i(std::string msg);

	static void w(std::string msg);

	static void d(std::string msg);

	static void e(std::string msg);
};

