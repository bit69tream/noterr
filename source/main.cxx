#include <algorithm>
#include <filesystem>
#include <iostream>
#include <string>

#include "ui.hxx"

int main(int command_line_arguments_length, char *command_line_arguments[]) {
  if (command_line_arguments_length != 2) {
    std::cerr << "Usage: todo <filename>" << std::endl;
    return 1;
  }

  const std::filesystem::path file_path(command_line_arguments[1]);
  ui::ui ui(file_path);
  ui.main_loop();

  return 0;
}
