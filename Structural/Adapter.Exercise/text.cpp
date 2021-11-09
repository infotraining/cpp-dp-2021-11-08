#include "text.hpp"

#include "../Composite.Exercise/shape_factories.hpp"

namespace Drawing
{
    void Text::draw() const
    {
        render_at(coord().x, coord().y);
    }

    void Text::set_text(const std::string& txt)
    {
        set_paragraph(txt.c_str());
    }

    std::string Text::get_text() const
    {
        return std::string(get_paragraph());
    }

    bool is_registered = SingletonShapeFactory::instance()
                             .register_creator(Text::id, &std::make_unique<Text>);
}
