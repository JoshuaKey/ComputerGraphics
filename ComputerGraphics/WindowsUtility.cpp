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

void WindowsUtility::DisplayError(TCHAR const* const customError)
{
	if (customError)
	{
		MessageBox(NULL, customError, TEXT("ERROR"), MB_OK);
	}
	else
	{
		MessageBox(NULL, TEXT("Recieved Error"), TEXT("ERROR"), MB_OK);
	}
}

void WindowsUtility::DisplayLastError(TCHAR const* const customError)
{
	DWORD errorCode = GetLastError();

	// Copy Custom Error to Message Buffer
	constexpr DWORD MESSAGE_BUFFER_SIZE = 1024;
	TCHAR errorMessageBuffer[MESSAGE_BUFFER_SIZE] { 0 };
	size_t customErrorLength = 0;
	if (customError)
	{
		customErrorLength = _tcslen(customError);
		if (customErrorLength < MESSAGE_BUFFER_SIZE)
		{
			memcpy(errorMessageBuffer, customError, customErrorLength * sizeof(TCHAR));

			if (customErrorLength + 1 < MESSAGE_BUFFER_SIZE)
			{
				errorMessageBuffer[customErrorLength] = TEXT('\n');
				customErrorLength += 1;
			}
		}
	}

	// Format Error Code in Error Message
	if (!FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
						NULL,
						errorCode,
						MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
						errorMessageBuffer + customErrorLength,
						MESSAGE_BUFFER_SIZE - static_cast<DWORD>(customErrorLength),
						NULL)) 
	{ 
		// Failed to format Error Message
		MessageBox(NULL, TEXT("FormatMessage failed"), TEXT("ERROR"), MB_OK);
		return;
	}

	MessageBox(NULL, errorMessageBuffer, TEXT("ERROR"), MB_OK);
}
