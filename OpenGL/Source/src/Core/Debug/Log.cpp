#include "../Source/includes/Core/Debug/Log.hpp"
#include "../Source/includes/Core/App.hpp"
using namespace Debug;

Debug::Log Console;
void Log::OpenFile(std::filesystem::path const& filename)
{
	_file.open(filename);
	if (_file.fail())
		std::cout << "Log : failed" << std::endl;

}

void Log::Print(LogType type, const char* format, ...)
{
	switch (type)
	{
	case LogType::INFO:
		std::cout << "Log : " << format << std::endl;
		if (_file)
		{
			_file << "Log : " << format << std::endl;
		}
		break;
	case LogType::WARNING:
		std::cout << "\033[1;33m Warning : " << format << "\033[0m" << std::endl;
		if (_file)
		{
			_file << "Warning : " << format << std::endl;
		}
		break;
	case LogType::L_ERROR:
		std::cout << "\033[1;31m Error : " << format << "\033[0m" << std::endl;
		if (_file)
		{
			_file << "Error : " << format << std::endl;
		}
		break;
	default:
		break;
	}
}

void Debug::Debug_Log(LogType type, const char* format)
{
	switch (type)
	{
	case LogType::INFO:
		std::cout << "Log : " << format << std::endl;
		break;
	case LogType::WARNING:
		std::cout << "\033[1;33m Warning : " << format << "\033[0m" << std::endl;
		break;
	case LogType::L_ERROR:
		std::cout << "\033[1;31m Error : " << format << "\033[0m" << std::endl;
		break;
	default:
		break;
	}
}

void Log::DrawConsole(bool &open)
{
	if (_ConsoleText.size() > 50)
		_ConsoleText.erase(_ConsoleText.begin(), _ConsoleText.begin() + _ConsoleText.size() - 50);
	if (!open)
		return;	
	if (ImGui::Begin("Console", &open))
	{
		ImGui::BeginChild("Text");
		for (int i = 0; i < _ConsoleText.size(); i++)
		{
			if (_ConsoleText[i].Color.x == 1 && _ConsoleText[i].Color.y == 1 && _ConsoleText[i].Color.z == 1 && _ConsoleText[i].Color.z == 1)
				ImGui::Text(_ConsoleText[i].Text.c_str());
			else
				ImGui::TextColored(_ConsoleText[i].Color, _ConsoleText[i].Text.c_str());
		}
		// Set The Scroll Y one time.
		if (_scrollY != 0) {
			ImGui::SetScrollY(_scrollY);
			_scrollY = 0;
		}

		ImGui::EndChild();
		ImGui::Separator();
		if (ImGui::InputText("Input", _input, 64, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			// Add text to the line.
			AddLine(LogType::INFO, _input);
			// Scroll down.
			_scrollY = 20000;
			// Check if it's a command.
			AlreadyGet = false;
			_actualCommand = Commands::StringToCommands(_input);
			// Clear the input.
			_input[0] = '\0';
		}
		ImGui::SetItemDefaultFocus();
	}
	ImGui::End();
}

Commands Log::GetActualCommand()
{
	AlreadyGet = true;
	return _actualCommand;
}

void Debug::Log::ClearConsole()
{
	_ConsoleText.clear();
}
