#include "WindowsUtility.h"
#include <algorithm>
#include <tchar.h> 


EXTERN_C IMAGE_DOS_HEADER __ImageBase;

HINSTANCE WindowsUtility::GetCurrentInstance()
{
	return GetModuleHandle(NULL);
}

HINSTANCE WindowsUtility::GetRunningInstance()
{
	return ((HINSTANCE)&__ImageBase);
}

void WindowsUtility::DisplayLastError(TCHAR const* const customError)
{
	DWORD errorCode = GetLastError();

	// Copy Custom Error to Message Buffer
	TCHAR errorMessageBuffer[1024] { 0 };
	uint32_t customErrorLength = 0;
	if (customError)
	{
		customErrorLength = _tcslen(customError);
		if (customErrorLength < sizeof(errorMessageBuffer))
		{
			memcpy(errorMessageBuffer, customError, customErrorLength * sizeof(TCHAR));

			errorMessageBuffer[customErrorLength] = '\n';
			customErrorLength += 1;
		}
	}

	// Format Error Code in Error Message
	if (!FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
						NULL,
						errorCode,
						MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
						errorMessageBuffer + customErrorLength,
						sizeof(errorMessageBuffer) - customErrorLength,
						NULL)) 
	{ 
		// Failed to format Error Message
		MessageBox(NULL, TEXT("FormatMessage failed"), TEXT("ERROR"), MB_OK);
		return;
	}

	MessageBox(NULL, errorMessageBuffer, TEXT("ERROR"), MB_OK);
}
