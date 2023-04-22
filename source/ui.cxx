#include <algorithm>
#include <array>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string_view>

#include "popup.hxx"
#include "raylib_helper.hxx"
#include "ui.hxx"

namespace ui {
  ui::ui(std::filesystem::path file_path) : m_file_path(std::move(file_path)) {
    using namespace raylib;
    using namespace std::string_view_literals;

#ifdef NO_RAYLIB_INFO
    SetTraceLogLevel(LOG_ERROR);
#else
    SetTraceLogLevel(LOG_DEBUG);
#endif

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow((int)m_width, (int)m_height, "todo");
    SetExitKey(KEY_NULL);

    SetWindowTitle(m_file_path.filename().c_str());

    m_camera = Camera2D {
      .offset = Vector2 {0, 0},
      .target = Vector2 {0, 0},
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

    SetMouseCursor(MOUSE_CURSOR_ARROW);
  }

  ui::~ui() { raylib::CloseWindow(); }

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
    using enum state;

    const Vector2 mouse_position = GetMousePosition();

    if (m_state == just_looking && IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
      using namespace std;

      m_state = popup_menu;
      m_popup = make_unique<popup>(
        vector<tuple<string_view, popup_actions>> {
          make_tuple("New note", popup_actions::create_new_note),
          make_tuple("Quit", popup_actions::quit),
        },
        GetMousePosition(), m_theme);
    }

    if (m_state == popup_menu && IsMouseButtonReleased(MOUSE_BUTTON_RIGHT)) {
      m_state = just_looking;
      SetMouseCursor(MOUSE_CURSOR_ARROW);
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
        using namespace raylib_helper;

        m_state = just_looking;
        m_started_drawing = false;

        m_notes.push_back(note(
          map_rectangle_into_world_coordinates(
            into_proper_rectangle(m_note_placeholder), m_camera),
          "sample text",
          "I'd just like to interject for a moment. What you're refering to as Linux, is in fact, GNU/Linux, or as I've recently taken to calling it, GNU plus Linux. Linux is not an operating system unto itself, but rather another free component of a fully functioning GNU system made useful by the GNU corelibs, shell utilities and vital system components comprising a full OS as defined by POSIX.\n\nMany computer users run a modified version of the GNU system every day, without realizing it. Through a peculiar turn of events, the version of GNU which is widely used today is often called Linux, and many of its users are not aware that it is basically the GNU system, developed by the GNU Project.\n\nThere really is a Linux, and these people are using it, but it is just a part of the system they use. Linux is the kernel: the program in the system that allocates the machine's resources to the other programs that you run. The kernel is an essential part of an operating system, but useless by itself; it can only function in the context of a complete operating system. Linux is normally used in combination with the GNU operating system: the whole system is basically GNU with Linux added, or GNU/Linux. All the so-called Linux distributions are really distributions of GNU/Linux!",
          m_theme));

        std::stable_sort(m_notes.begin(), m_notes.end(), [](const note &first, const note &second) {
          return first > second;
        });

        SetMouseCursor(MOUSE_CURSOR_ARROW);
      } else {
        m_note_placeholder.width = mouse_position.x - m_note_placeholder.x;
        m_note_placeholder.height = mouse_position.y - m_note_placeholder.y;
      }
    }
  }

  void ui::execute_popup_action(popup_actions action) {
    using enum popup_actions;

    switch (action) {
      case create_new_card:
        throw std::runtime_error("TODO: cards.");
        break;
      case create_new_note:
        raylib::SetMouseCursor(raylib::MOUSE_CURSOR_RESIZE_ALL);
        m_state = state::drawing_new_note;
        break;
      case quit:
        m_should_close = true;
        break;
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
        DrawRectangleRec(raylib_helper::into_proper_rectangle(m_note_placeholder),
                         m_theme.placeholder);
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
};  // namespace ui
