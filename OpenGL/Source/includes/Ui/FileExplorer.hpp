#pragma once
#include <string>
#include <vector>
#include <filesystem>
#include <iostream>
#include "../Source/includes/Resources/Texture.hpp"
#include "../Source/includes/Resources/Component.hpp"

enum class Extension
{
	NONE,
	OBJ,
	MTL,
	TXT,
	PNG,
	JPEG,
	JPG,
	BMP,
	FOLDER,
};

class File : public Selectable
{
public:
	File() {}
	File(std::string path);

	~File() {}

	std::string Name;
	std::string Directory;
	Extension Extension;
	Texture* Tex;
	IResource* Resource;

	File* parent;
	std::vector<File*> childs;

	File* SearchFolder(std::string path);

private:

};

class FileExplorer
{
public:
	FileExplorer() {
		Current = &Main;
	}
	~FileExplorer() {}
	File Main;
	File* Current;

	void Refresh();
	void FoundFiles(std::filesystem::path path, File* file);
	void ShowImGui(bool &open);

private:

};
