#include "../Source/includes/Ui/FileExplorer.hpp"
#include "../Source/includes/Core/App.hpp"
File::File(std::string path)
{
	this->Directory = path;
	this->Name = path.substr(path.find_last_of("//\\") + 1);
	if (this->Name.substr(this->Name.find_last_of(".") + 1) == "obj")
	{
		this->Extension = Extension::OBJ;
		if (auto tex = app.GetResourcesManager()->Get<Texture*>("Assets/textures/obj.png")) {
			this->Resource = app.GetResourcesManager()->Get<Model3D*>("Assets/models/" + this->Name);
			this->Tex = tex;
		}
	}
	else if (this->Name.substr(this->Name.find_last_of(".") + 1) == "mtl")
	{
		this->Extension = Extension::MTL;
		if (auto tex = app.GetResourcesManager()->Get<Texture*>("Assets/textures/mtl.png")) {
			this->Tex = tex;
		}
	}
	else if (this->Name.substr(this->Name.find_last_of(".") + 1) == "png")
	{
		this->Extension = Extension::PNG;
		if (auto tex = app.GetResourcesManager()->Get<Texture*>("Assets/textures/" + this->Name)) {
			this->Resource = app.GetResourcesManager()->Get<Texture*>("Assets/textures/" + this->Name);
			this->Tex = tex;
		}
	}
	else if (this->Name.substr(this->Name.find_last_of(".") + 1) == "jpeg")
	{
		this->Extension = Extension::JPEG;
		if (auto tex = app.GetResourcesManager()->Get<Texture*>("Assets/textures/" + this->Name)) {
			this->Resource = app.GetResourcesManager()->Get<Texture*>("Assets/textures/" + this->Name);
			this->Tex = tex;
		}
	}
	else if (this->Name.substr(this->Name.find_last_of(".") + 1) == "jpg")
	{
		this->Extension = Extension::JPG;
		if (auto tex = app.GetResourcesManager()->Get<Texture*>("Assets/textures/" + this->Name)) {
			this->Resource = app.GetResourcesManager()->Get<Texture*>("Assets/textures/" + this->Name);
			this->Tex = tex;
		}
	}
	else if (this->Name.substr(this->Name.find_last_of(".") + 1) == "bmp")
	{
		this->Extension = Extension::BMP;
	}
	else if (this->Name.substr(this->Name.find_last_of(".") + 1) == "txt")
	{
		this->Extension = Extension::TXT;
		if (auto tex = app.GetResourcesManager()->Get<Texture*>("Assets/textures/txt.png")) {
			if (auto file = app.GetResourcesManager()->Get<TxtFile*>("Assets/scenes/" + this->Name))
			{
				this->Resource = file;
			}
			else
			{
				this->Resource = app.GetResourcesManager()->Create<TxtFile*>("Assets/scenes/" + this->Name);
			}
			this->Tex = tex;
		}
	}
	else
	{
		this->Extension = Extension::FOLDER;
		if (auto tex = app.GetResourcesManager()->Get<Texture*>("Assets/textures/file.png"))
			this->Tex = tex;
	}
}

File* File::SearchFolder(std::string path)
{
	if (this->Extension != Extension::FOLDER)
		return nullptr;
	if (!strcmp(path.c_str(), this->Directory.c_str()))
	{
		return this;
	}
	File* file;
	for (auto i : childs)
	{
		file = i->SearchFolder(path);
		if (file != nullptr)
			return file;
	}
	return nullptr;
}

void FileExplorer::Refresh()
{
	Main.Extension = Extension::FOLDER;
	Main.childs.clear();
	FoundFiles("Assets", &Main);
	Main.Directory = "Assets";
	Main.Name = "Assets";
}

void FileExplorer::FoundFiles(std::filesystem::path path, File* file)
{
	for (const auto& entry : std::filesystem::directory_iterator(path))
	{
		file->childs.push_back(new File(entry.path().string()));
		file->childs.back()->parent = file;
		if (entry.is_directory()) {
			if (auto tex = app.GetResourcesManager()->Get<Texture*>("Assets/textures/folder.png"))
				file->childs.back()->Tex = tex;
			file->childs.back()->Extension = Extension::FOLDER;
			FoundFiles(entry.path(), file->childs.back());
		}
	}
}

static int sCooldown = 0;
void FileExplorer::ShowImGui(bool& open)
{
	if (!open)
		return;
	if (ImGui::Begin("File Explorer"), &open)
	{
		if (app.WindowChangeSize && ImGui::GetWindowDockNode())
		{
			auto Size = app.GetWindowSize();
			if (Size.x * Size.y > 0) {
				ImGui::DockBuilderSetNodePos(ImGui::GetWindowDockNode()->ID, ImVec2(0, Size.y - ImGui::GetWindowDockNode()->Size.y));
				ImGui::DockBuilderSetNodeSize(ImGui::GetWindowDockNode()->ID, ImVec2(Size.x, ImGui::GetWindowDockNode()->Size.y));
			}
		}
		if (ImGui::Button("Back") && Current->parent != nullptr)
		{
			Current = Current->parent;
		}
		ImGui::SameLine();
		if (ImGui::Button("Refresh"))
		{
			Current = &Main;
			Refresh();
		}
		ImGui::SameLine();
		char Path[64];
		for (int i = 0; i < Current->Directory.size(); i++)
			Path[i] = Current->Directory[i];
		Path[Current->Directory.size()] = '\0';
		if (ImGui::InputText("Path", (char*)Path, 64, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			for (int i = 0; i < 64; i++)
				if (Path[i] == '/')
					Path[i] = '\\';
			if (auto file = Main.SearchFolder(Path))
				Current = file;
			else
				sCooldown = 120;
		}
		ImGui::SameLine();
		if (ImGui::Button("Open Folder"))
		{
			char buf[128];
			sprintf_s(buf, 128, ("explorer " + std::filesystem::current_path().string() + '\\' + (std::string)Path).c_str());
			system(buf);
		}
		if (sCooldown > 0)
		{
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 0, 0, 1), "folder %s could not be found !", Path);
			sCooldown -= 60 * ImGui::GetIO().DeltaTime;
		}
		ImGui::Separator();

		ImGui::BeginChild("ChildL", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
		for (int i = 0; i < Current->childs.size(); i++)
		{
			ImGui::PushID(i);
			ImGui::BeginGroup();
			if (ImGui::ImageButton((ImTextureID)(Current->childs[i]->Tex->TextureData), ImVec2(86, 86))) {
				if (Current->childs[i]->Extension == Extension::FOLDER) {
					Current = Current->childs[i];
					ImGui::PopID();
					ImGui::EndGroup();
					break;
				}
				else
					app.GetInspector()->Selected = Current->childs[i];
			}
			ImGui::Text(Current->childs[i]->Name.substr(0, 10).c_str());
			ImGui::EndGroup();

			// For auto	back to line.	
			auto windowSize = ImGui::GetWindowSize();
			int rounded = round(windowSize.x / 105);
			if (rounded > 0) {
				if (i % rounded != rounded - 1)
				{
					ImGui::SameLine();
				}
			}
			ImGui::PopID();
		}
		ImGui::EndChild();
	}
	ImGui::End();
}