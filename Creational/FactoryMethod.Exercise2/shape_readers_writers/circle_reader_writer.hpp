#ifndef CIRCLEREADERWRITER_H
#define CIRCLEREADERWRITER_H

#include "shape_reader_writer.hpp"
#include "../circle.hpp"

namespace Drawing
{
    namespace IO
    {
        class CircleReaderWriter : public ShapeReaderWriter
        {
        public:
            void read(Shape& shp, std::istream& in) override
            {
                Circle& c = static_cast<Circle&>(shp);

                Point pt;
                int r;

                in >> pt >> r;

                c.set_coord(pt);
                c.set_radius(r);
            }

            void write(const Shape& shp, std::ostream& out) override
            {
                // TODO
            }
        };
    }
}

#endif // CIRCLEREADERWRITER_H
