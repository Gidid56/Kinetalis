#pragma once
#include <shobjidl.h> // IFileSaveDialog

std::optional<std::wstring> ShowSaveFileDialog_Windows(
    const wchar_t* dialogTitle,
    const wchar_t* defaultFileName,
    const wchar_t* defaultExtension,
    const COMDLG_FILTERSPEC* filters,
    UINT filterCount);
