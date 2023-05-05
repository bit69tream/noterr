#include <algorithm>
#include <filesystem>
#include <iostream>
#include <string>

#include "ui.hxx"

int main(int command_line_arguments_length, char *command_line_arguments[]) {
  if (command_line_arguments_length != 2) {
    std::cerr << "Usage: noterr <filename.noterr>" << std::endl;
    return 1;
  }

  const std::filesystem::path file_path(command_line_arguments[1]);

  const std::filesystem::path noterr_extension(".noterr");
  if (file_path.extension() != noterr_extension) {
    std::cerr << "Error: provided file is not of a .noterr extension." << std::endl;
    return 1;
  }

  ui::ui ui(file_path);
  ui.main_loop();

  return 0;
}
