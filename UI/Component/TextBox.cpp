#include "TextBox.hpp"
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_color.h>

namespace Engine {
    TextBox::TextBox(std::string hintText, ALLEGRO_FONT* font, float x, float y, float w, float h)
        : IObject(x, y, w, h, 0, 0), hint(hintText), font(font), focused(false), content("") {}

    void TextBox::Draw() const {
        ALLEGRO_COLOR bg = focused ? al_map_rgb(50, 50, 50) : al_map_rgb(30, 30, 30);
        ALLEGRO_COLOR border = focused ? al_map_rgb(255, 255, 0) : al_map_rgb(255, 255, 255);
        ALLEGRO_COLOR textColor = al_map_rgb(255, 255, 255);

        al_draw_filled_rectangle(Position.x, Position.y, Position.x + Size.x, Position.y + Size.y, bg);
        al_draw_rectangle(Position.x, Position.y, Position.x + Size.x, Position.y + Size.y, border, 2);
        std::string text = content.empty() && !focused ? hint : content;
        al_draw_text(font, textColor, Position.x + 5, Position.y + (Size.y - al_get_font_line_height(font)) / 2, 0, text.c_str());
    }

    void TextBox::OnMouseDown(int button, int mx, int my) {
        focused = (mx >= Position.x && mx <= Position.x + Size.x &&
                   my >= Position.y && my <= Position.y + Size.y);
    }

    void TextBox::OnKeyDown(int keyCode) {
        if (!focused) return;
        if (keyCode == ALLEGRO_KEY_BACKSPACE && !content.empty()) {
            content.pop_back();
        }
    }

    void TextBox::OnChar(int unicode) {
        if (!focused) return;
        if (unicode >= 32 && unicode <= 126 && content.size() < 20) {
            content += static_cast<char>(unicode);
        }
    }

    std::string TextBox::GetText() const {
        return content;
    }
}
