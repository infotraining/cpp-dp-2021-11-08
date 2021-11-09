#ifndef SHAPEGROUP_HPP
#define SHAPEGROUP_HPP

#include <memory>
#include <vector>

#include "shape.hpp"

namespace Drawing
{
    // TODO - implement a composite for shapes
    class ShapeGroup : public CloneableShape<ShapeGroup>
    {
        std::vector<std::unique_ptr<Shape>> shapes_;

    public:
        static constexpr auto id = "ShapeGroup";

        using iterator = std::vector<std::unique_ptr<Shape>>::iterator;
        using const_iterator = std::vector<std::unique_ptr<Shape>>::const_iterator;

        ShapeGroup() = default;

        ShapeGroup(const ShapeGroup& source)
        {
            for(const auto& s : source.shapes_)
            {
                shapes_.push_back(s->clone());
            }
        }

        ShapeGroup& operator=(const ShapeGroup& source)
        {
            ShapeGroup temp(source);
            swap(temp);

            return *this;
        }

        ShapeGroup(ShapeGroup&&) = default;
        ShapeGroup& operator=(ShapeGroup&&) = default;

        void swap(ShapeGroup& other)
        {
            shapes_.swap(other.shapes_);
        }

        void draw() const
        {
            for (const auto& s : shapes_)
                s->draw();
        }

        void move(int dx, int dy)
        {
            for (const auto& s : shapes_)
                s->move(dx, dy);
        }

        void add(std::unique_ptr<Shape> s)
        {
            shapes_.push_back(std::move(s));
        }

        iterator begin()
        {
            return shapes_.begin();
        }

        iterator end()
        {
            return shapes_.end();
        }

        const_iterator begin() const
        {
            return shapes_.begin();
        }

        const_iterator end() const
        {
            return shapes_.end();
        }

        const_iterator cbegin() const
        {
            return shapes_.begin();
        }

        const_iterator cend() const
        {
            return shapes_.end();
        }

        size_t size() const
        {
            return shapes_.size();
        }
    };
}

#endif // SHAPEGROUP_HPP
