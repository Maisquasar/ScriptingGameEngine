#include "../Source/includes/Resources/TxtFile.hpp"

TxtFile::TxtFile(const char* file)
{
	uint32_t size = 0;
	bool sucess = false;
	Content = Loader::ReadFile(file, size, sucess);
	if (!sucess) {
		LOG(LogType::L_ERROR, "Can't load File : %s", file);
		Loaded = true;
	}
	else
	{
		LOG(LogType::INFO, "Sucessfuly load File : %s", file);
		Loaded = true;
	}
}