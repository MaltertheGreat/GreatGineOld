#pragma once

#include "GGWindow.h"

class GGApplication
{
public:
	GGApplication();

public:
	void Run();

private:
	const std::wstring m_title = L"GreatGine";
	const std::wstring m_version = L"0.0.1";

private:
	GGWindow m_window;
};