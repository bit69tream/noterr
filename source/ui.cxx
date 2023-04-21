#include <iostream>
#include <algorithm>
#include <array>
#include <stdexcept>
#include <string_view>
#include <optional>

#include "ui.hxx"
#include "popup.hxx"
#include "raylib_helper.hxx"

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

    // TODO: load themes from config file
    m_theme = theme {
      .font_size = 20,
      .font_spacing = 2,
      .font = GetFontDefault(),

      .background = WHITE,

      .border = BLACK,
      .border_size = 2,

      .popup_background = LIGHTGRAY,
      .popup_foreground = BLACK,
      .popup_text_padding = 10,

      .placeholder = MAROON,

      .note_background = RAYWHITE,
      .note_foreground = BLACK,
    };
  }

  ui::~ui() {
    raylib::CloseWindow();
  }

  void ui::update_window_size() {
    m_width = raylib::GetRenderWidth();
    m_height = raylib::GetRenderHeight();

    m_camera.offset.x = m_width / 2;
    m_camera.offset.y = m_height / 2;
  }

  void ui::update_camera() {
    if (m_state != state::just_looking) {
      return;
    }

    if (IsMouseButtonDown(raylib::MOUSE_BUTTON_LEFT)) {
      raylib::Vector2 mouse_delta = raylib::GetMouseDelta();
      m_camera.target.x -= mouse_delta.x / m_camera.zoom;
      m_camera.target.y -= mouse_delta.y / m_camera.zoom;
    }

    m_camera.zoom += raylib::GetMouseWheelMove() * 0.1;
    m_camera.zoom = std::clamp(m_camera.zoom, 0.1f, 1.0f);
  }

  // NOTE: this can probably be written in a better way
  void ui::update_ui_state() {
    using namespace raylib;
    using namespace std::string_view_literals;
    using enum state;

    const Vector2 mouse_position = GetMousePosition();

    if (m_state == just_looking &&
        IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
      m_state = popup_menu;
      m_popup = std::make_unique<popup>(std::vector<std::tuple<std::string_view, popup_actions>>{
          std::make_tuple("New card"sv, popup_actions::create_new_card),
          std::make_tuple("New note", popup_actions::create_new_note),
          std::make_tuple("Quit", popup_actions::quit),
        },
        GetMousePosition(),
        m_theme);
    }

    if (m_state == popup_menu &&
        IsMouseButtonReleased(MOUSE_BUTTON_RIGHT)) {
      m_state = just_looking;
      SetMouseCursor(MOUSE_CURSOR_DEFAULT);
      auto maybe_action = m_popup->get_action();
      if (maybe_action.has_value()) {
        execute_popup_action(maybe_action.value());
      }
      delete m_popup.release();
    }

    if (m_state == drawing_new_note) {
      if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
        m_note_placeholder.x = mouse_position.x;
        m_note_placeholder.y = mouse_position.y;
        m_note_placeholder.width = m_note_placeholder.height = 0;
        m_started_drawing = true;
      } else if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT) && m_started_drawing) {
        m_state = just_looking;
        m_started_drawing = false;
        m_notes.push_back(note (raylib_helper::map_rectangle_into_world_coordinates(raylib_helper::into_proper_rectangle(m_note_placeholder), m_camera),
                                 "sample text",
                                 "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. "
                                 "Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. "
                                 "Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. "
                                 "Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.",
                                 m_theme));
        SetMouseCursor(MOUSE_CURSOR_DEFAULT);
      } else {
        m_note_placeholder.width = mouse_position.x - m_note_placeholder.x;
        m_note_placeholder.height = mouse_position.y - m_note_placeholder.y;
      }
    }
  }

  void ui::execute_popup_action(popup_actions action) {
    using enum popup_actions;

    switch (action) {
    case create_new_card: throw std::runtime_error("todo"); break;
    case create_new_note:
      raylib::SetMouseCursor(raylib::MOUSE_CURSOR_RESIZE_ALL);
      m_state = state::drawing_new_note;
      break;
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
        for (const auto &note : m_notes) {
          note.render();
        }
      }
      EndMode2D();

      if (m_state == popup_menu) {
        m_popup->render();
      } else if (m_state == drawing_new_note && m_started_drawing) {
        DrawRectangleRec(raylib_helper::into_proper_rectangle(m_note_placeholder), m_theme.placeholder);
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
