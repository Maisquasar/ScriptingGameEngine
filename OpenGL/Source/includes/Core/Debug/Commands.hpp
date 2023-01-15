#pragma once
#include <vector>
#include <string>

enum class CommandsType
{
	CLEAR,
	HELP,
	SHUTDOWN,
	CLOSE,
	C_DELETE,
	NOT_DEFINED,
	NONE,
};

class Commands
{
public:
	Commands() { Type = CommandsType::CLEAR; };
	~Commands() {};
	CommandsType Type;
	std::vector<std::string> Args;

	static Commands StringToCommands(std::string cmd)
	{
		Commands result;
		int i = 0;
		std::string first;
		while (cmd[i] != '\0' && cmd[i] != '\n' && cmd[i] != ' ')
		{
			first.push_back(cmd[i]);
			i++;
		}
		result.Type = StringToCommandsType(first);
		std::string arg;
		while (cmd[i] != '\0' && cmd[i] != '\n')
		{
			i++;
			if (cmd[i] == ' ') {
				result.Args.push_back(arg);
				arg.clear();
				continue;
			}
			arg.push_back(cmd[i]);
		}
		result.Args.push_back(arg);
		return result;
	}

	static CommandsType StringToCommandsType(std::string cmd)
	{
		if (cmd.substr(1) == "clear") return CommandsType::CLEAR;
		if (cmd.substr(1) == "help") return CommandsType::HELP;
		if (cmd.substr(1) == "shutdown") return CommandsType::SHUTDOWN;
		if (cmd.substr(1) == "close") return CommandsType::CLOSE;
		if (cmd.substr(1) == "delete") return CommandsType::C_DELETE;
		else if (cmd[0] == '/') return CommandsType::NOT_DEFINED;
		else return CommandsType::NONE;
	}

	static void CheckCommand(Commands cmd);
private:
};