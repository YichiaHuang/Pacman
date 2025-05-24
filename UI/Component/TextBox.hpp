#ifndef TEXTBOX_HPP
#define TEXTBOX_HPP

#include <string>
#include <allegro5/allegro_font.h>
#include "Engine/IControl.hpp"
#include "Engine/IObject.hpp"

namespace Engine {
    class TextBox : public IControl, public IObject {
    protected:
        std::string content;
        std::string hint;
        ALLEGRO_FONT* font;
        bool focused;

    public:
        TextBox(std::string hintText, ALLEGRO_FONT* font, float x, float y, float w, float h);
        void Draw() const override;
        void OnMouseDown(int button, int mx, int my) override;
        void OnKeyDown(int keyCode) override;
        void OnChar(int unicode);
        std::string GetText() const;
    };
}

#endif
