#pragma once

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>

// Provides Utility methods for interfacing with Windows API
namespace WindowsUtility
{
	// Returns the HINSTANCE of the current "Instance" (aka Executable or Library) this code is located in.
	// This HISNTANCE may be separate for the current running Executable if it is ran from a different Executable.
	// For more information: https://devblogs.microsoft.com/oldnewthing/20050418-59/?p=35873
	HINSTANCE GetCurrentInstance();

	// Returns the HINSTANCE of the current Exectuable this code is being ran from.
	// For more information: https://devblogs.microsoft.com/oldnewthing/20041025-00/?p=37483
	HINSTANCE GetRunningInstance();
};

