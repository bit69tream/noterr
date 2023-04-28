#include <algorithm>
#include <filesystem>
#include <iostream>
#include <string>

#include "ui.hxx"

int main(int command_line_arguments_length, char *command_line_arguments[]) {
  if (command_line_arguments_length != 2) {
    std::cerr << "Usage: todo <filename.todo>" << std::endl;
    return 1;
  }

  const std::filesystem::path file_path(command_line_arguments[1]);

  const std::filesystem::path todo_extension(".todo");
  if (file_path.extension() != todo_extension) {
    std::cerr << "Error: provided file is not of a .todo extension." << std::endl;
    return 1;
  }

  ui::ui ui(file_path);
  ui.main_loop();

  return 0;
}

// TODO: look into clang-tidy
