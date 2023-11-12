#include "todo_list.hxx"
#include "raylib.hxx"
#include "raylib_helper.hxx"

namespace ui {
  todo_list::todo_list(raylib::Rectangle bounding_box, std::wstring title, std::vector<task> tasks, const theme &theme)
      : m_theme(theme),
        m_bounding_box(bounding_box),
        m_border_box(bounding_box),
        m_title_bounding_box(std::make_shared<raylib::Rectangle>(bounding_box.x, bounding_box.y, bounding_box.width, m_theme.font_size)),
        m_title(std::move(title), m_title_bounding_box, m_theme),
        m_title_delimiter(title_delimiter()),
        m_tasks(tasks) {
    compute_element_coordinates();
  }

  todo_list::todo_list(raylib::Rectangle bounding_box, const theme &theme)
      : m_theme(theme),
        m_bounding_box(bounding_box),
        m_border_box(bounding_box),
        m_title_bounding_box(std::make_shared<raylib::Rectangle>(bounding_box.x, bounding_box.y, bounding_box.width, m_theme.font_size)),
        m_title(m_title_bounding_box, m_theme),
        m_title_delimiter(title_delimiter()) {
    compute_element_coordinates();
  }

  raylib::Rectangle todo_list::title_delimiter() {
    return raylib::Rectangle {
      .x = m_bounding_box.x + m_theme.border_size,
      .y = m_title_bounding_box->y + m_title_bounding_box->height,
      .width = m_bounding_box.width - (m_theme.border_size * 2),
      .height = m_theme.border_size,
    };
  }

  void todo_list::compute_element_coordinates() {
    using namespace raylib;

    // float max_element_width = std::max(m_title_bounding_box->width, m_body_bounding_box->width);
    float max_element_width = m_title_bounding_box->width;
    if (max_element_width > m_bounding_box.width) {
      m_bounding_box.width = max_element_width;
    }

    m_title_delimiter = title_delimiter();
    // m_bounding_box.height = std::max(new_bounding_box_height(), m_bounding_box.height);
    m_bounding_box.height = m_bounding_box.height;
    m_border_box = raylib_helper::add_border_to_rectangle(m_bounding_box, m_theme);
  }

  void todo_list::render(raylib::Vector2 mouse_position_in_the_world) const {
    using namespace raylib;
    using namespace raylib_helper;

    DrawRectangleRec(m_border_box, m_theme.border);
    DrawRectangleRec(m_bounding_box, m_theme.entity_background);
    DrawRectangleRec(m_title_delimiter, m_theme.border);

    m_title.render(mouse_position_in_the_world, m_title_focused);
  }

  bool todo_list::can_focus(raylib::Vector2 point) const {
    return CheckCollisionPointRec(point, m_bounding_box);
  }

  void todo_list::send_events(std::span<event> events) {
    std::vector<event> title_events;
    // std::vector<event> body_events;

    for (const auto &boxed_event : events) {
      if (std::holds_alternative<mouse_event>(boxed_event)) {
        auto event = std::get<mouse_event>(boxed_event);

        if (raylib::CheckCollisionPointRec(event.point, *m_title_bounding_box)) {
          m_title_focused = true;
          title_events.push_back(event);
        } else {
          m_title_focused = false;
          // body_events.push_back(event);
        }
      } else if (std::holds_alternative<keyboard_event>(boxed_event)) {
        auto event = std::get<keyboard_event>(boxed_event);

        if (m_title_focused) {
          title_events.push_back(event);
        } else {
          // body_events.push_back(event);
        }
      }
    }

    if (!title_events.empty()) {
      m_title.send_events(title_events);
    }

    // if (!body_events.empty()) {
    //   m_body.send_events(body_events);
    // }

    compute_element_coordinates();
  }
};  // namespace ui
