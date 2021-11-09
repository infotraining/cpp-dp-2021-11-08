#include "text_reader_writer.hpp"
#include "../shape_factories.hpp"
#include "../text.hpp"

using namespace std;
using namespace Drawing;
using namespace IO;

namespace
{
    bool is_registered = SingletonShapeRWFactory::instance()
                             .register_creator(make_type_index<Text>(), &make_unique<TextReaderWriter>);
}

void TextReaderWriter::read(Shape& shp, istream& in)
{
    Text& sqr = static_cast<Text&>(shp);

    Point pt;
    string text;

    in >> pt >> text;

    sqr.set_text(text);
    sqr.set_coord(pt);
}

void TextReaderWriter::write(const Shape& shp, ostream& out)
{
    const Text& square = static_cast<const Text&>(shp);

    out << Text::id << " " << square.coord() << " " << square.get_text() << endl;
}
