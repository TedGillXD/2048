#include <iostream>
#include "fmt/core.h"
#include "Log.h"

void Log::i(std::string msg)
{
	std::string str = fmt::format("[{0}]info: {1}\n", __TIME__, msg);
	fmt::print(str);
	//ResourcesManager::writeFile("Data\\2048.log", str, std::ios::app);
}

void Log::w(std::string msg)
{
	std::string str = fmt::format("[{0}]warning: {1}\n", __TIME__, msg);
	fmt::print(str);
	//ResourcesManager::writeFile("Data\\2048.log", str, std::ios::app);
}

void Log::d(std::string msg)
{
	std::string str = fmt::format("[{0}]debug: {1}\n", __TIME__, msg);
	fmt::print(str);
	//ResourcesManager::writeFile("Data\\2048.log", str, std::ios::app);
}

void Log::e(std::string msg)
{
	std::string str = fmt::format("[{0}]error: {1}\n", __TIME__, msg);
	fmt::print(str);
	//ResourcesManager::writeFile("Data\\2048.log", str, std::ios::app);
}
