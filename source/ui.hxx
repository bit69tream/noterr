#pragma once

#include <string>
#include <filesystem>
#include <vector>

#include "raylib.hxx"
#include "ui_popup.hxx"

namespace ui {
  enum class state {
    just_looking,
    popup_menu,
  };

  class ui {
  public:
    ui(std::filesystem::path file_path);
    ~ui();

    void main_loop();

  private:
    std::filesystem::path m_file_path;

    raylib::Camera2D m_camera{};

    std::unique_ptr<popup> m_popup;

    float m_width{640};
    float m_height{480};

    state m_state{state::just_looking};

    float m_font_size = 20;
    float m_font_spacing = 1;
    float m_text_padding = 10;
    raylib::Font m_font;

    bool m_should_close{false};

    void update_window_size();
    void update_camera();
    void update_ui_state();

    void render();

    void execute_action(popup_actions action);
  };
};
