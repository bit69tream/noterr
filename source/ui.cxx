#include <algorithm>
#include <array>
#include <iostream>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string_view>

#include "popup.hxx"
#include "raylib_helper.hxx"
#include "single_line_input.hxx"
#include "ui.hxx"

namespace ui {
  ui::ui(std::filesystem::path file_path)
      : m_file_path(std::move(file_path)) {
    using namespace raylib;
    using namespace std::string_view_literals;

#ifdef NO_RAYLIB_INFO
    SetTraceLogLevel(LOG_ERROR);
#else
    SetTraceLogLevel(LOG_DEBUG);
#endif

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(static_cast<int>(m_width), static_cast<int>(m_height), "todo");
    SetExitKey(KEY_NULL);

    SetWindowTitle(m_file_path.filename().c_str());
    SetMouseCursor(MOUSE_CURSOR_ARROW);

    SetTargetFPS(60);

    m_camera = Camera2D {
      .offset = Vector2 {0, 0},
      .target = Vector2 {0, 0},
      .rotation = 0,
      .zoom = 1,
    };

    constexpr int font_size = 25;
    constexpr int unicode_cyrillic_range_end = 0x04FF;

    // TODO: load themes from config file
    m_theme = theme {
      .font_size = font_size,
      .glyph_spacing = 2,
      .line_spacing = 0,
      .font = LoadFontEx("DejaVuSans.ttf", font_size, nullptr, unicode_cyrillic_range_end),

      .background = WHITE,

      .grid_color = GRAY,
      .grid_tile_size_as_percentage = 0.1f,

      .border = BLACK,
      .border_size = 2,

      .popup_background = LIGHTGRAY,
      .popup_foreground = BLACK,
      .popup_text_padding = 10,

      .placeholder = MAROON,

      .entity_background = WHITE,
      .entity_foreground = BLACK,

      .cursor_width = 2,
      .cursor_color = BLACK,
    };

    m_background_texture_shader = LoadShader(nullptr, "source/background_pattern.fs");

    m_background_shader_screen_resolution_location = GetShaderLocation(m_background_texture_shader, "screenResolutionAndTopLeftPoint");
    m_background_shader_grid_tile_size_as_percentage_location = GetShaderLocation(m_background_texture_shader, "gridTilePercent");

    if (m_background_shader_screen_resolution_location == -1 ||
        m_background_shader_grid_tile_size_as_percentage_location == -1) {
      throw std::runtime_error("fucky-wucky happened");
    }
  }

  ui::~ui() {
    using namespace raylib;

    UnloadShader(m_background_texture_shader);
    UnloadRenderTexture(m_background_texture_for_shader);
    UnloadFont(m_theme.font);
    CloseWindow();
  }

  void ui::update_window_size() {
    using namespace raylib;

    float new_width = static_cast<float>(GetRenderWidth());
    float new_height = static_cast<float>(GetRenderHeight());

    if (new_width != m_width || new_height != m_height) {
      m_width = new_width;
      m_height = new_height;

      UnloadRenderTexture(m_background_texture_for_shader);
      m_background_texture_for_shader = LoadRenderTexture(static_cast<int>(m_width), static_cast<int>(m_height));
      BeginTextureMode(m_background_texture_for_shader);
      {
        DrawRectangle(0, 0, static_cast<int>(m_width), static_cast<int>(m_height), m_theme.background);
      }
      EndTextureMode();
    }

    m_camera.offset.x = m_width / 2;
    m_camera.offset.y = m_height / 2;
  }

  void ui::update_camera() {
    using namespace raylib;

    if (m_state != state::just_looking) {
      return;
    }

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
      Vector2 mouse_delta = GetMouseDelta();
      m_camera.target.x -= mouse_delta.x / m_camera.zoom;
      m_camera.target.y -= mouse_delta.y / m_camera.zoom;
    }

    m_camera.zoom += GetMouseWheelMove() * 0.5f;
    constexpr float maximum_zoom = 2.0f;
    constexpr float minimum_zoom = 0.5f;
    m_camera.zoom = std::clamp(m_camera.zoom, minimum_zoom, maximum_zoom);
  }

  // NOTE: this can probably be written in a better way
  void ui::update_ui_state() {
    using namespace raylib;
    using enum state;

    const Vector2 mouse_position = GetMousePosition();
    const Vector2 mouse_position_in_the_world = GetScreenToWorld2D(mouse_position, m_camera);

    switch (m_state) {
      case popup_menu: {
        if (!IsMouseButtonReleased(MOUSE_BUTTON_RIGHT)) {
          break;
        }

        m_state = just_looking;
        SetMouseCursor(MOUSE_CURSOR_ARROW);

        auto maybe_action = m_popup->get_action();
        if (!maybe_action.has_value()) {
          break;
        }

        execute_popup_action(maybe_action.value());
        delete m_popup.release();
      } break;
      case drawing_new_note: {
        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
          m_started_drawing = true;

          m_note_placeholder.x = mouse_position.x;
          m_note_placeholder.y = mouse_position.y;
          m_note_placeholder.width = m_note_placeholder.height = 0;
        } else if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT) && m_started_drawing) {
          using namespace raylib_helper;

          m_state = just_looking;
          m_started_drawing = false;

          SetMouseCursor(MOUSE_CURSOR_ARROW);

          m_entities.push_back(std::make_unique<note>(map_rectangle_into_world_coordinates(into_proper_rectangle(m_note_placeholder), m_camera), m_theme));
        } else {
          m_note_placeholder.width = mouse_position.x - m_note_placeholder.x;
          m_note_placeholder.height = mouse_position.y - m_note_placeholder.y;
        }
      } break;
      case just_looking:
      case focused_on_entity: {
        bool is_any_mouse_button_pressed = IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) || IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE);
        if (is_any_mouse_button_pressed) {
          for (auto it = m_entities.rbegin(); it != m_entities.rend(); it++) {
            if ((*it)->can_focus(mouse_position_in_the_world)) {
              m_state = focused_on_entity;
              // put focused entity at the end of the vector so it renders on top of everything
              std::swap(*it, m_entities.back());
              goto after_processing_focus;
            }
          }

          m_state = just_looking;
        }
after_processing_focus:

        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
          using namespace std;

          m_state = popup_menu;
          m_popup = make_unique<popup>(
            vector<tuple<string_view, popup_actions>> {
              make_tuple("Restore zoom", popup_actions::restore_zoom),
              make_tuple("New note", popup_actions::create_new_note),
              make_tuple("Quit", popup_actions::quit),
            },
            GetMousePosition(), m_theme);
        }
      } break;
    }
  }

  void ui::execute_popup_action(popup_actions action) {
    using enum popup_actions;

    switch (action) {
      case create_new_note:
        raylib::SetMouseCursor(raylib::MOUSE_CURSOR_RESIZE_ALL);
        m_state = state::drawing_new_note;
        break;
      case restore_zoom:
        m_camera.zoom = 1.0f;
        break;
      case quit:
        m_should_close = true;
        break;
    }
  }

  void ui::draw_background_with_grid_pattern() {
    using namespace raylib;

    ClearBackground(m_theme.grid_color);

    Vector2 top_left_world_point = GetScreenToWorld2D({0, 0}, m_camera);
    Vector4 screen_resolution_and_top_left_world_point = {m_width, m_height, top_left_world_point.x, top_left_world_point.y};

    SetShaderValue(m_background_texture_shader, m_background_shader_screen_resolution_location, &screen_resolution_and_top_left_world_point, SHADER_UNIFORM_VEC4);
    SetShaderValue(m_background_texture_shader, m_background_shader_grid_tile_size_as_percentage_location, &m_theme.grid_tile_size_as_percentage, SHADER_UNIFORM_FLOAT);

    BeginShaderMode(m_background_texture_shader);
    {
      DrawTexture(m_background_texture_for_shader.texture, 0, 0, WHITE);
    }
    EndShaderMode();
  }

  void ui::render() {
    using namespace raylib;
    using enum state;

    BeginDrawing();
    {
      draw_background_with_grid_pattern();

      BeginMode2D(m_camera);
      {
        for (const auto &entity : m_entities) {
          entity->render();
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

  void ui::pass_input_events_to_focused_entity() {
    using namespace raylib;

    if (m_state != state::focused_on_entity) {
      throw std::logic_error("`pass_input_events_to_focused_entity` function should only be called when something is in focus.");
    }

    keyboard_event keyboard_event;
    keyboard_event.key = static_cast<KeyboardKey>(GetKeyPressed());
    keyboard_event.codepoint = GetCharPressed();

    mouse_event mouse_event;
    mouse_event.point = GetScreenToWorld2D(GetMousePosition(), m_camera);

    bool has_any_keyboard_key_been_pressed = keyboard_event.key != 0 || keyboard_event.codepoint != 0;
    bool has_left_mouse_button_been_pressed = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

    if (!has_any_keyboard_key_been_pressed && !has_left_mouse_button_been_pressed) {
      return;
    }

    std::vector<event> events;

    if (has_any_keyboard_key_been_pressed) {
      events.push_back(keyboard_event);
    }

    if (has_left_mouse_button_been_pressed) {
      events.push_back(mouse_event);
    }

    m_entities.back()->send_events(events);
  }

  void ui::main_loop() {
    while (!raylib::WindowShouldClose() && !m_should_close) {
      update_window_size();
      update_camera();
      update_ui_state();

      if (m_state == state::focused_on_entity) {
        pass_input_events_to_focused_entity();
      }

      render();
    }
  }
};  // namespace ui

// TODO: some sort of debug menu
