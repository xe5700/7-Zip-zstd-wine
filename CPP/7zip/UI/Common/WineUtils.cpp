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

static void FallbackUnixToDosPath(const UString &unixPath, UString &dosPath)
{
  if (unixPath.IsEmpty())
  {
    dosPath.Empty();
    return;
  }

  if (unixPath[0] == L'/')
  {
    dosPath = L"Z:" + unixPath;
    dosPath.Replace(L'/', L'\\');
    return;
  }

  // Already in DOS format or unknown format
  dosPath = unixPath;
}

bool DosToUnixPath(const UString &dosPath, UString &unixPath)
{
  unixPath.Empty();
  InitWineFunctions();

  if (dosPath.Len() >= 2 && dosPath[1] == L':')
  {
    // Path has drive prefix (e.g. "G:\...").
    // Use drive root approach: resolve just the drive root via Wine API,
    // then manually append the rest. This avoids symlink/mount resolution
    // issues that can occur when Wine resolves the full path.
    UString unixPrefix;
    if (g_isWine && g_wine_get_unix_file_name)
    {
      UString driveRoot = UString(dosPath[0]) + L":\\";
      char *unixStr = g_wine_get_unix_file_name(driveRoot);
      if (unixStr)
      {
        unixPrefix = MultiByteToUnicodeString(unixStr, CP_UTF8);
        HANDLE heap = ::GetProcessHeap();
        if (heap)
          ::HeapFree(heap, 0, unixStr);
      }
    }
    if (unixPrefix.IsEmpty())
    {
      if (dosPath[0] == L'Z' || dosPath[0] == L'z')
        unixPrefix = L"/";
      else
        unixPrefix = L"/" + UString(dosPath[0]) + L"/";
    }
    if (!unixPrefix.IsEmpty() && unixPrefix.Back() == L'/')
      unixPrefix.DeleteBack();
    unixPath = unixPrefix;
    UString rest = dosPath.Ptr(2);
    rest.Replace(L'\\', L'/');
    unixPath += rest;
    return !unixPath.IsEmpty();
  }

  // No drive prefix. Try Wine API on the full path.
  if (g_isWine && g_wine_get_unix_file_name)
  {
    char *unixStr = g_wine_get_unix_file_name(dosPath);
    if (unixStr)
    {
      unixPath = MultiByteToUnicodeString(unixStr, CP_UTF8);
      HANDLE heap = ::GetProcessHeap();
      if (heap)
        ::HeapFree(heap, 0, unixStr);
      if (!unixPath.IsEmpty())
        return true;
    }
  }

  // Fallback: just replace backslashes
  unixPath = dosPath;
  unixPath.Replace(L'\\', L'/');
  return !unixPath.IsEmpty();
}

bool UnixToDosPath(const UString &unixPath, UString &dosPath)
{
  dosPath.Empty();
  InitWineFunctions();

  if (g_isWine && g_wine_get_dos_file_name)
  {
    // Convert UString to UTF-8 for Wine API
    AString unixPathUtf8 = UnicodeStringToMultiByte(unixPath, CP_UTF8);

    WCHAR *dosStr = g_wine_get_dos_file_name(unixPathUtf8);
    if (dosStr)
    {
      dosPath = dosStr;

      // Free the buffer allocated by Wine
      HANDLE heap = ::GetProcessHeap();
      if (heap)
        ::HeapFree(heap, 0, dosStr);

      if (!dosPath.IsEmpty())
        return true;
    }
  }

  // Fallback: do simple string conversion
  FallbackUnixToDosPath(unixPath, dosPath);
  return !dosPath.IsEmpty();
}

}

#endif // Z7_WINE_LINUX
