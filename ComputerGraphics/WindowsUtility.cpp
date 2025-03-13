#include "WindowsUtility.h"

EXTERN_C IMAGE_DOS_HEADER __ImageBase;

HINSTANCE WindowsUtility::GetCurrentInstance()
{
	return GetModuleHandle(NULL);
}

HINSTANCE WindowsUtility::GetRunningInstance()
{
	return ((HINSTANCE)&__ImageBase);
}
