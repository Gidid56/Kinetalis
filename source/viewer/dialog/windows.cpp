#include "pch.h"

#include "windows.h"

// Windows headers
#include <windows.h>

std::optional<std::wstring> ShowSaveFileDialog_Windows(
    const wchar_t* dialogTitle,
    const wchar_t* defaultFileName,
    const wchar_t* defaultExtension,
    const COMDLG_FILTERSPEC* filters,
    UINT filterCount)
{
    // Ensure COM is initialized on THIS thread.
    // If your app already initializes COM elsewhere, remove these two lines
    // (or keep them, but handle RPC_E_CHANGED_MODE gracefully).
    HRESULT hrCo = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    bool didInitCOM = SUCCEEDED(hrCo);

    IFileSaveDialog* dialog = nullptr;
    HRESULT hr = CoCreateInstance(
        CLSID_FileSaveDialog,
        nullptr,
        CLSCTX_INPROC_SERVER,
        IID_PPV_ARGS(&dialog));
    if (FAILED(hr))
    {
        if (didInitCOM)
            CoUninitialize();
        return std::nullopt;
    }

    // Title
    if (dialogTitle)
    {
        dialog->SetTitle(dialogTitle);
    }

    // Suggested file name
    if (defaultFileName)
    {
        dialog->SetFileName(defaultFileName);
    }

    // Default extension (used if user doesn't type one)
    if (defaultExtension)
    {
        dialog->SetDefaultExtension(defaultExtension); // e.g. L"csv"
    }

    // Filters (file types)
    if (filters and filterCount > 0)
    {
        dialog->SetFileTypes(filterCount, filters);
        dialog->SetFileTypeIndex(1);
    }

    // Options (overwrite prompt etc.)
    DWORD options = 0;
    dialog->GetOptions(&options);
    options |= FOS_OVERWRITEPROMPT; // "File exists, overwrite?"
    options |= FOS_PATHMUSTEXIST;   // path must exist
    options |= FOS_FORCEFILESYSTEM; // return filesystem paths
    dialog->SetOptions(options);

    // Show dialog (owner HWND can be nullptr if you don't have one handy)
    hr = dialog->Show(nullptr);
    if (FAILED(hr))
    {
        dialog->Release();
        if (didInitCOM)
            CoUninitialize();
        return std::nullopt; // user cancelled or error
    }

    // Retrieve result path
    IShellItem* resultItem = nullptr;
    hr = dialog->GetResult(&resultItem);
    if (FAILED(hr))
    {
        dialog->Release();
        if (didInitCOM)
            CoUninitialize();
        return std::nullopt;
    }

    PWSTR filePath = nullptr;
    hr = resultItem->GetDisplayName(SIGDN_FILESYSPATH, &filePath);
    resultItem->Release();
    dialog->Release();

    if (FAILED(hr) or not filePath)
    {
        if (didInitCOM)
            CoUninitialize();
        return std::nullopt;
    }

    std::wstring path(filePath);
    CoTaskMemFree(filePath);

    if (didInitCOM)
        CoUninitialize();
    return path;
}