#include <flutter/dart_project.h>
#include <flutter/flutter_view_controller.h>
#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>

#include "flutter_window.h"
#include "utils.h"

void useSoFile(const char* name) {
    std::ifstream fin(name, std::ios::binary);
    std::ofstream fout("data\\app.so", std::ios::binary);

    bool bRet = true;

    while(!fin.eof()) {
        char szBuf;
        fin.read(&szBuf, sizeof(char));

        if(fin.eof()) break;

        if (fout.bad()) {
            bRet = false;
            break;
        }
        fout.write(&szBuf, sizeof(char));
    }

    fout.close();
    fin.close();
}

int APIENTRY wWinMain(_In_ HINSTANCE instance, _In_opt_ HINSTANCE prev,
                      _In_ wchar_t *command_line, _In_ int show_command)
{
  // Attach to console when present (e.g., 'flutter run') or create a
  // new console when running with a debugger.
  if (!::AttachConsole(ATTACH_PARENT_PROCESS) && ::IsDebuggerPresent())
  {
    CreateAndAttachConsole();
  }

  useSoFile("data\\{{ cookiecutter.project_name }}.so");

  // Initialize COM, so that it is available for use in the library and/or
  // plugins.
  ::CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);

  flutter::DartProject project(L"data");

  std::vector<std::string> command_line_arguments =
      GetCommandLineArguments();

  project.set_dart_entrypoint_arguments(std::move(command_line_arguments));

  FlutterWindow window(project);
  Win32Window::Point origin(10, 10);
  Win32Window::Size size(50, 10);
  if (!window.Create(L"{{ cookiecutter.product_name }}", origin, size))
  {
    return EXIT_FAILURE;
  }
  window.SetQuitOnClose(true);

  ::MSG msg;
  while (::GetMessage(&msg, nullptr, 0, 0))
  {
    ::TranslateMessage(&msg);
    ::DispatchMessage(&msg);
  }

  ::CoUninitialize();
  return EXIT_SUCCESS;
}
