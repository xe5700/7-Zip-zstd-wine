#include "StdAfx.h"
#include "WineUtils.h"

#ifndef Z7_WINE_LINUX

// Stub: no Wine support compiled in
namespace NWineUtils {

bool IsWine() { return false; }

bool DosToUnixPath(const UString &, UString &)
{
  return false;
}

bool UnixToDosPath(const UString &, UString &)
{
  return false;
}

}

#else

#include "../../../Common/MyWindows.h"
#include "../../../Common/StringConvert.h"
#include "WineUtils.h"

namespace NWineUtils {

// Function pointer types for Wine API
typedef char * (CDECL *Func_wine_get_unix_file_name)(LPCWSTR dos);
typedef WCHAR * (CDECL *Func_wine_get_dos_file_name)(LPCSTR str);

static bool g_checkedWine = false;
static bool g_isWine = false;
static Func_wine_get_unix_file_name g_wine_get_unix_file_name = NULL;
static Func_wine_get_dos_file_name g_wine_get_dos_file_name = NULL;

static void InitWineFunctions()
{
  if (g_checkedWine)
    return;
  g_checkedWine = true;

  HMODULE hNtdll = ::GetModuleHandleW(L"kernel32.dll");
  if (!hNtdll)
    return;

  // Check if we're running under Wine by looking for wine-specific export
  g_wine_get_unix_file_name = (Func_wine_get_unix_file_name)
      ::GetProcAddress(hNtdll, "wine_get_unix_file_name");
  g_wine_get_dos_file_name = (Func_wine_get_dos_file_name)
      ::GetProcAddress(hNtdll, "wine_get_dos_file_name");

  if (g_wine_get_unix_file_name && g_wine_get_dos_file_name)
    g_isWine = true;
}

bool IsWine()
{
  InitWineFunctions();
  return g_isWine;
}

bool DosToUnixPath(const UString &dosPath, UString &unixPath)
{
  unixPath.Empty();
  InitWineFunctions();

  if (!g_isWine || !g_wine_get_unix_file_name)
    return false;

  char *unixStr = g_wine_get_unix_file_name(dosPath);
  if (!unixStr)
    return false;

  // Convert UTF-8 Unix path to UString
  unixPath = MultiByteToUnicodeString(unixStr, CP_UTF8);

  // Free the buffer allocated by Wine (HeapAlloc on process heap)
  HANDLE heap = ::GetProcessHeap();
  if (heap)
    ::HeapFree(heap, 0, unixStr);

  return !unixPath.IsEmpty();
}

bool UnixToDosPath(const UString &unixPath, UString &dosPath)
{
  dosPath.Empty();
  InitWineFunctions();

  if (!g_isWine || !g_wine_get_dos_file_name)
    return false;

  // Convert UString to UTF-8 for Wine API
  AString unixPathUtf8 = UnicodeStringToMultiByte(unixPath, CP_UTF8);

  WCHAR *dosStr = g_wine_get_dos_file_name(unixPathUtf8);
  if (!dosStr)
    return false;

  dosPath = dosStr;

  // Free the buffer allocated by Wine
  HANDLE heap = ::GetProcessHeap();
  if (heap)
    ::HeapFree(heap, 0, dosStr);

  return !dosPath.IsEmpty();
}

}

#endif // Z7_WINE_LINUX
