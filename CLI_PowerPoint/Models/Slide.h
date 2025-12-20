#pragma once
#include "IObject.h"
#include <vector>
#include <memory>
#include <string>

using namespace document::shapes;

namespace document
{
    class Slide 
    {
    public:
        Slide() : m_id(0) {}
        explicit Slide(int id) : m_id(id) {}

        // Copy constructor for undo/redo support
        Slide(const Slide& other) : m_id(other.m_id)
        {
            for (const auto& obj : other.m_objects) {
                m_objects.push_back(obj->clone());
            }
        }

        ~Slide() = default;

        int getId() const { return m_id; }
        size_t getShapeCount() const { return m_objects.size(); }

        void addShape(std::unique_ptr<IObject> shape);
        void removeShape(size_t index);

        const IObject& getShape(size_t pos) const;
        IObject& getShape(size_t pos);

        bool moveShape(const std::string& name, int x, int y);
        bool resizeShape(const std::string& name, int width, int height);

        void setId(int id) { m_id = id; }

    private:
        int m_id;
        std::vector<std::unique_ptr<IObject>> m_objects;
    };
}

