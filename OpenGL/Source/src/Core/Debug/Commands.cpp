#include "../Source/includes/Core/Debug/Commands.hpp"
#include "../Source/includes/Core/App.hpp"

void Commands::CheckCommand(Commands cmd)
{
	switch (cmd.Type)
	{
	case CommandsType::CLEAR:
		Console.ClearConsole();
		break;
	case CommandsType::HELP:
		Console.AddLine(LogType::INFO, { "/clear : Clear The Console.\n       /help : Print Help\n       /shutdown : Crash the Engine\n       /close : close the Engine.\n       /delete {Node_Name} : Delete a Node from the Scene, -A delete all Nodes of the Scene." });
		break;
	case CommandsType::SHUTDOWN:
		assert(1 == 0);
		break;
	case CommandsType::CLOSE:
		app.CloseApp();
		break;
	case CommandsType::C_DELETE:
		if (cmd.Args[0].empty())
		{
			Console.AddLine(LogType::WARNING, { "/delete {Node_Name}" });
		}
		else {
			bool found = false;
			bool deleteAll = false;
			if (!strcmp(cmd.Args[0].c_str(), "-A"))
				deleteAll = true;

			auto childrens = app.SceneNode->GetAllChildrens();
			for (auto child : childrens)
			{
				if (!strcmp(child->Name.c_str(), cmd.Args[0].c_str()) || deleteAll)
				{
					child->Delete();
					found = true;
				}
			}
			if (!found)
				Console.AddLine(LogType::L_ERROR, "%s can't be found", cmd.Args[0].c_str());

		}
		break;
	case CommandsType::NOT_DEFINED:
		Console.AddLine(LogType::L_ERROR, "Command not defined");
		break;
	case CommandsType::NONE:
	default:
		break;
	}
}