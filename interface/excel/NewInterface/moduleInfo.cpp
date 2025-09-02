#ifdef _WIN32


#include "moduleInfo.h"
#include <windows.h>
#include <strsafe.h>


namespace TP
{
	namespace Excel
	{
			std::string modulePathImpl()
			{
				HMODULE hThisModule = nullptr;

				GetModuleHandleEx(
					GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
					(LPCTSTR)(&modulePath),
					&hThisModule);

				//Get Local Path, increasing buffer size till big enough
				DWORD uiSizeLocalPath = 256; //  DWORD, MAX_PATH;
				char *szLocalPath = 0;
				auto bBigEnough = false;

				do
				{
					delete[] szLocalPath;
					szLocalPath = new char[uiSizeLocalPath + 1];
					const DWORD cb = GetModuleFileNameA(hThisModule,
						szLocalPath,
						uiSizeLocalPath);
					if (cb < uiSizeLocalPath)
					{
						bBigEnough = true;
					}
					else
					{
						uiSizeLocalPath *= 2;
					}
				} while (!bBigEnough);

				auto result = std::string(szLocalPath);
				delete[] szLocalPath;

				return result;
			}

			const std::string& modulePath()
			{
				static const auto modulePath = modulePathImpl();
				return modulePath;
			}

			const std::string& installationPathImpl()
			{
				static const auto installationPath = [] {
					auto p = modulePath();
					const std::string::size_type lastSlashPos = p.rfind('\\');
					p.erase(lastSlashPos + 1);
					return p;
				}();
				return installationPath;
			}

			const std::string& moduleName()
			{
				static const auto moduleName = [] {
					auto p = modulePath();
					const std::string::size_type lastSlashPos = p.rfind('\\');
					p.erase(0, lastSlashPos + 1);
					return p;
				}();
				return moduleName;
			}

			const std::string& installationPath(void)
			{
				static std::string installationPath = installationPathImpl();
				return installationPath;
			}

			void DisplayMessage(const char* lpszFunction, const DWORD dw, const char* lpMsgBuf)
			{
				const char* const couldNotFormat = "Could not format an error message.";
				const size_t cDisplayBuf = 512;
				CHAR lpDisplayBuf[cDisplayBuf] = "";
				if (SUCCEEDED(StringCchPrintfA((LPSTR)lpDisplayBuf, cDisplayBuf,
					"%s failed with error %lu: %s", lpszFunction, dw, lpMsgBuf)))
					MessageBoxA(nullptr, lpDisplayBuf, "Error", MB_OK);
				else
					MessageBoxA(nullptr, couldNotFormat, "Error", MB_OK);
			}

			void DisplayError(const char* lpszFunction)
			{
				// Retrieve the system error message for the last-error code

				LPVOID lpMsgBuf = nullptr;
				const DWORD dw = GetLastError();
				static const DWORD UNHANDLED_EXCEPTION = 3765269347; // SEH exception code 0xE06D7363

				if (UNHANDLED_EXCEPTION == dw)
				{
					DisplayMessage(lpszFunction, dw, "unhandled exception on DLL load");
				}
				else if (FormatMessageA(
					FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
					nullptr, dw, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&lpMsgBuf, 0, nullptr) > 0)
				{
					DisplayMessage(lpszFunction, dw, (LPSTR)lpMsgBuf);
				}
				else
				{
					DisplayMessage(lpszFunction, dw, "unrecognized error code");
				}

				LocalFree(lpMsgBuf);
			}

	}
}

#else // Non windows


#endif // #ifdef _WIN32
