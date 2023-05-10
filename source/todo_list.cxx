#include "todo_list.hxx"
#include "raylib.hxx"
#include "raylib_helper.hxx"

namespace ui {
  todo_list::todo_list(raylib::Rectangle bounding_box, std::wstring title, std::vector<task> tasks, const theme &theme)
      : m_theme(theme),
        m_bounding_box(raylib_helper::subtract_border_from_rectangle(bounding_box, m_theme)),
        m_title_bounding_box(std::make_shared<raylib::Rectangle>(bounding_box.x, bounding_box.y, bounding_box.width, bounding_box.height)),
        m_title(std::move(title), m_title_bounding_box, m_theme),
        m_title_delimiter(title_delimiter()),
        m_tasks(tasks) {
  }

  todo_list::todo_list(raylib::Rectangle bounding_box, const theme &theme)
      : m_theme(theme),
        m_bounding_box(raylib_helper::subtract_border_from_rectangle(bounding_box, m_theme)),
        m_title_bounding_box(std::make_shared<raylib::Rectangle>(bounding_box.x, bounding_box.y, bounding_box.width, bounding_box.height)),
        m_title(m_title_bounding_box, m_theme),
        m_title_delimiter(title_delimiter()) {
  }

  raylib::Rectangle todo_list::title_delimiter() {
    return raylib::Rectangle {
      .x = m_bounding_box.x + m_theme.border_size,
      .y = m_title_bounding_box->y + m_title_bounding_box->height,
      .width = m_bounding_box.width - (m_theme.border_size * 2),
      .height = m_theme.border_size,
    };
  }

  void todo_list::render() const {
  }

  bool todo_list::can_focus(raylib::Vector2 point) const {
    (void)point;
    return false;
  }

  void todo_list::send_events(std::span<event> events) {
    (void)events;
  }
};  // namespace ui
