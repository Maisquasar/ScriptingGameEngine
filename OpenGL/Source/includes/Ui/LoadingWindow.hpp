#pragma once
#include "UiWindow.hpp"

class LoadingWindow : public UiWindow
{
public:
	LoadingWindow();
	~LoadingWindow();
	bool FullyLoaded = false;

	void Show() override;
};
