#ifndef TEXT_HPP
#define TEXT_HPP

#include "paragraph.hpp"
#include "shape.hpp"
#include <string>

namespace Drawing
{
    // TODO - adapt Paragraph class to Shape interface
    class Text : public ShapeBase, private LegacyCode::Paragraph
    {
    public:

        static constexpr const char* id = "Text";

       Text(int x = 0, int y = 0, const std::string& text = "") : ShapeBase{x, y}, LegacyCode::Paragraph{text.c_str()}
       {}

       void draw() const override;

       void set_text(const std::string& txt);
       std::string get_text() const;
    };
}

#endif
