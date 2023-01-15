#pragma once
#include <iostream>
#include <filesystem>
#include <fstream>
#include <map>
#include <string>
#include <stdio.h>
#include <stdarg.h>
#include <ImGui/imgui.h>
#include <GLFW/glfw3.h>
#include "Commands.hpp"
#include <Windows.h>
#include "../Source/includes/Core/DataStructure/ThreadManager.hpp"
#include <deque>
enum class GameState
{
	GS_EDITOR,
	GS_PLAY,
};

template<typename ... Args>
std::string string_format(const std::string& format, Args ... args)
{
	int size_s = std::snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
	if (size_s <= 0) { throw std::runtime_error("Error during formatting."); }
	auto size = static_cast<size_t>(size_s);
	std::unique_ptr<char[]> buf(new char[size]);
	std::snprintf(buf.get(), size, format.c_str(), args ...);
	return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
}

enum class LogType
{
	INFO,
	WARNING,
	L_ERROR,
};

namespace Debug
{
	struct ConsoleText
	{
		ImVec4 Color;
		std::string Text;
	};

	class Log
	{
	private:
		std::ofstream _file;
		std::deque<ConsoleText> _ConsoleText;
		float _scrollY;
		char _input[64];
		Commands _actualCommand;
		HANDLE  hConsole;
	public:
		Log() { hConsole = GetStdHandle(STD_OUTPUT_HANDLE); }
		void OpenFile(std::filesystem::path const& filename);
		void Print(LogType type, const char* format, ...);

		bool AlreadyGet = true;

		void ClearConsole();

		Commands GetActualCommand();

		template <typename ...Args>	void PrintConsole(const char* file, int line, LogType type, std::string format, Args ... args)
		{
			std::string text;
			text = string_format("%s(%d): ", file, line);
			text += string_format(format, args...);

			switch (type)
			{
			case LogType::INFO:
				SetConsoleTextAttribute(hConsole, 15);
				_ConsoleText.push_back(ConsoleText{ ImVec4(1, 1, 1, 1), text });
				break;
			case LogType::WARNING:
				SetConsoleTextAttribute(hConsole, 14);
				_ConsoleText.push_back(ConsoleText{ ImVec4(1, 0.5f, 0, 1), text });
				break;
			case LogType::L_ERROR:
				SetConsoleTextAttribute(hConsole, 4);
				_ConsoleText.push_back(ConsoleText{ ImVec4(1, 0, 0, 1), text });
				break;
			default:
				break;
			}

			_scrollY = 20000;
			std::cout << text << std::endl;
			if (_file)
				_file << text << std::endl;
			SetConsoleTextAttribute(hConsole, 15);
		}

		template <typename ...Args>	void AddLine(LogType type, std::string format, Args ... args)
		{
			std::string text;
			text += string_format(format, args...);
			if (_ConsoleText.size() > 50)
				_ConsoleText.erase(_ConsoleText.begin());
			switch (type)
			{
			case LogType::INFO:
				text = "[INFO] " + text;
				_ConsoleText.push_back(ConsoleText{ ImVec4(1, 1, 1, 1), text });
				break;
			case LogType::WARNING:
				text = "[WARNING] " + text;
				_ConsoleText.push_back(ConsoleText{ ImVec4(1, 0.5f, 0, 1), text });
				break;
			case LogType::L_ERROR:
				text = "[ERROR] " + text;
				_ConsoleText.push_back(ConsoleText{ ImVec4(1, 0, 0, 1), text });
				break;
			default:
				break;
			}

			_scrollY = 20000;
		}

		void DrawConsole(bool& open);

	};

	void Debug_Log(LogType type, const char* format);
};


#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
extern Debug::Log Console;
#define LOG(t, x, ...) Console.PrintConsole(__FILENAME__, __LINE__, t, x, __VA_ARGS__);