#include <algorithm>
#include <array>
#include <stdexcept>
#include <string_view>
#include <optional>

#include "ui.hxx"
#include "ui_popup.hxx"

namespace ui {
  ui::ui(std::filesystem::path file_path) : m_file_path(std::move(file_path)) {
    using namespace raylib;
    using namespace std::string_view_literals;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow((int)m_width, (int)m_height,
               "todo");
    SetExitKey(KEY_NULL);

    SetWindowTitle(m_file_path.filename().c_str());

    m_camera = Camera2D{
      .offset = Vector2{0, 0},
      .target = Vector2{0, 0},
      .rotation = 0,
      .zoom = 1,
    };

    m_font = GetFontDefault();
  }

  ui::~ui() {
    raylib::CloseWindow();
  }

  void ui::update_window_size() {
    m_width = raylib::GetRenderWidth();
    m_height = raylib::GetRenderHeight();
  }

  void ui::update_camera() {
    if (m_state != state::just_looking) {
      return;
    }

    if (IsMouseButtonDown(raylib::MOUSE_BUTTON_LEFT)) {
      raylib::Vector2 mouse_delta = raylib::GetMouseDelta();
      m_camera.offset.x += mouse_delta.x;
      m_camera.offset.y += mouse_delta.y;
    }

    m_camera.zoom += raylib::GetMouseWheelMove() * 0.1;
    m_camera.zoom = std::clamp(m_camera.zoom, 0.1f, 1.0f);
  }

  void ui::update_ui_state() {
    using namespace raylib;
    using namespace std::string_view_literals;
    using enum state;

    if (m_state == just_looking &&
        IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
      m_state = popup_menu;
      m_popup = std::make_unique<popup>(std::vector<std::tuple<std::string_view, popup_actions>>{
          std::make_tuple("New card"sv, popup_actions::create_new_card),
          std::make_tuple("New note", popup_actions::create_new_note),
          std::make_tuple("Quit", popup_actions::quit),
        },
        GetMousePosition(),
        m_font, m_font_size, m_font_spacing, m_text_padding);
    }

    if (m_state == popup_menu &&
        IsMouseButtonReleased(MOUSE_BUTTON_RIGHT)) {
      m_state = just_looking;
      auto maybe_action = m_popup->get_action();
      if (maybe_action.has_value()) {
        execute_action(maybe_action.value());
      }
      delete m_popup.release();
    }
  }

  void ui::execute_action(popup_actions action) {
    using enum popup_actions;

    switch (action) {
    case create_new_card: throw std::runtime_error("todo"); break;
    case create_new_note: throw std::runtime_error("todo"); break;
    case quit: m_should_close = true; break;
    }
  }

  void ui::render() {
    using namespace raylib;
    using enum state;

    BeginDrawing();
    {
      ClearBackground(WHITE);

      BeginMode2D(m_camera);
      {

      }
      EndMode2D();

      if (m_state == popup_menu) {
        m_popup->render();
      }

      DrawFPS(0, 0);
    }
    EndDrawing();
  }

  void ui::main_loop() {
    while (!raylib::WindowShouldClose() && !m_should_close) {
      update_window_size();
      update_camera();
      update_ui_state();

      render();
    }
  }
};
