#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdexcept>

#include "raylib_helper.hxx"
#include "stb_truetype.h"
#include "theme.hxx"

namespace raylib_helper {
  Rectangle from_vectors(Vector2 position, Vector2 dimensions) {
    return Rectangle {
      .x = position.x,
      .y = position.y,
      .width = dimensions.x,
      .height = dimensions.y,
    };
  }

  Rectangle into_a_rectangle_where_the_top_left_corder_is_actually_a_top_left_corner(Rectangle rectangle) {
    if (rectangle.width < 0) {
      rectangle.width = -rectangle.width;
      rectangle.x -= rectangle.width;
    }

    if (rectangle.height < 0) {
      rectangle.height = -rectangle.height;
      rectangle.y -= rectangle.height;
    }

    return rectangle;
  }

  Rectangle map_rectangle_into_world_coordinates(Rectangle rectangle, Camera2D camera) {
    Vector2 top_left = {.x = rectangle.x, .y = rectangle.y};
    Vector2 world_top_left = GetScreenToWorld2D(top_left, camera);

    rectangle.x = world_top_left.x;
    rectangle.y = world_top_left.y;
    rectangle.width /= camera.zoom;
    rectangle.height /= camera.zoom;

    return rectangle;
  }

  Rectangle subtract_border_from_rectangle(Rectangle rectangle, const ui::theme &theme) {
    rectangle.x += theme.border_size;
    rectangle.y += theme.border_size;
    rectangle.width -= theme.border_size * 2;
    rectangle.height -= theme.border_size * 2;

    return rectangle;
  }

  Rectangle add_border_to_rectangle(Rectangle rectangle, const ui::theme &theme) {
    rectangle.x -= theme.border_size;
    rectangle.y -= theme.border_size;
    rectangle.width += theme.border_size * 2;
    rectangle.height += theme.border_size * 2;

    return rectangle;
  }

  Vector2 get_line_dimensions(std::wstring_view line, const ui::theme &theme) {
    float width = 0;
    float height = 0;

    for (const auto &codepoint : line) {
      const int glyph_index = raylib::GetGlyphIndex(theme.font, codepoint);
      const auto &glyph = theme.font.glyphs[glyph_index];
      const auto &glyph_rectangle = theme.font.recs[glyph_index];
      width += static_cast<float>(glyph.advanceX) + theme.glyph_spacing;
      height = std::max(height, glyph_rectangle.height);
    }
    width -= theme.glyph_spacing;

    return {.x = width + (theme.glyph_spacing * 2), .y = height};
  }

  Font load_font(std::filesystem::path font, int size) {
    unsigned int file_size = 0;
    unsigned char *file_data = LoadFileData(font.c_str(), &file_size);
    if (file_data == nullptr) {
      throw new std::runtime_error("could not load the font file");
    }

    stbtt_fontinfo info = {};
    if (!stbtt_InitFont(&info, file_data, 0)) {
      throw new std::runtime_error("bad font file");
    }

    float scale_factor = stbtt_ScaleForPixelHeight(&info, static_cast<float>(size));

    int ascent = 0, descent = 0, line_gap = 0;
    stbtt_GetFontVMetrics(&info, &ascent, &descent, &line_gap);

    GlyphInfo *glyphs = (GlyphInfo *)RL_MALLOC(static_cast<unsigned long>(info.numGlyphs) * sizeof(GlyphInfo));

    for (int i = 0; i < info.numGlyphs; i++) {
      int width = 0, height = 0;
      int ch = i + 32;
      glyphs[i].value = ch;

      if (ch != 32) {
        glyphs[i].image.data = stbtt_GetCodepointBitmap(&info, scale_factor, scale_factor, ch, &width, &height, &glyphs[i].offsetX, &glyphs[i].offsetY);
      } else {
        glyphs[i].image.data = nullptr;
      }

      stbtt_GetCodepointHMetrics(&info, ch, &glyphs[i].advanceX, nullptr);
      glyphs[i].advanceX = static_cast<int>(static_cast<float>(glyphs[i].advanceX) * scale_factor);

      glyphs[i].image.width = width;
      glyphs[i].image.height = height;
      glyphs[i].image.mipmaps = 1;
      glyphs[i].image.format = PIXELFORMAT_UNCOMPRESSED_GRAYSCALE;

      glyphs[i].offsetY += static_cast<int>(static_cast<float>(ascent) * scale_factor);

      if (ch == 32) {
        Image space = {.data = RL_CALLOC(static_cast<size_t>(glyphs[i].advanceX * size), 2), .width = glyphs[i].advanceX, .height = size, .mipmaps = 1, .format = PIXELFORMAT_UNCOMPRESSED_GRAYSCALE};

        glyphs[i].image = space;
      }
    }

    Font result = {
      .baseSize = size,
      .glyphCount = info.numGlyphs,
#define FONT_TTF_DEFAULT_CHARS_PADDING 4
      .glyphPadding = FONT_TTF_DEFAULT_CHARS_PADDING,
      .texture = {},
      .recs = nullptr,
      .glyphs = glyphs,
    };

    Image atlas = GenImageFontAtlas(result.glyphs, &result.recs, result.glyphCount, result.baseSize, result.glyphPadding, 0);
    result.texture = LoadTextureFromImage(atlas);

    for (int i = 0; i < result.glyphCount; i++) {
      UnloadImage(result.glyphs[i].image);
      result.glyphs[i].image = ImageFromImage(atlas, result.recs[i]);
    }

    UnloadImage(atlas);

    TRACELOG(LOG_INFO, "RAYLIB_HELER: FONT: Data loaded successfully (%i pixel size | %i glyphs)", font.baseSize, font.glyphCount);

    return result;
  }
};  // namespace raylib_helper
