#ifndef ZIP7_INC_WINE_UTILS_H
#define ZIP7_INC_WINE_UTILS_H

#include "../../../Common/MyString.h"

/*
  Wine/Linux path conversion utilities.

  When 7-Zip runs under Wine on Linux, the file system is accessed
  through Wine's drive mapping (e.g., Z:\home\user\file).
  
  These functions convert between Windows paths (used internally)
  and Unix paths (shown to the user / accepted as input).

  The Wine API functions wine_get_unix_file_name / wine_get_dos_file_name
  are loaded dynamically from ntdll.dll at runtime.
*/

namespace NWineUtils {

// Returns true if running under Wine
bool IsWine();

// Convert Windows path to Unix path for display
// e.g., L"Z:\\home\\user\\Documents" -> "/home/user/Documents"
// Returns true if conversion was successful
bool DosToUnixPath(const UString &dosPath, UString &unixPath);

// Convert Unix path to Windows path for internal use
// e.g., "/home/user/Documents" -> L"Z:\\home\\user\\Documents"
// Returns true if conversion was successful
bool UnixToDosPath(const UString &unixPath, UString &dosPath);

// Convenience inline wrappers
inline UString DosToUnixPath(const UString &dosPath)
{
  UString result;
  DosToUnixPath(dosPath, result);
  return result;
}

inline UString UnixToDosPath(const UString &unixPath)
{
  UString result;
  UnixToDosPath(unixPath, result);
  return result;
}

}

#endif
