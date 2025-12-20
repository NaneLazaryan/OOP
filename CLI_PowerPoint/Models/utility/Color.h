#pragma once
#include <cstdint>

namespace document::shapes::utility
{
	/*class Color
	{
	public:
		Color(size_t r, size_t g, size_t b) : r(r), g(g), b(b) {}

	private:
		size_t r;
		size_t g;
		size_t b;
	};*/

    class Color
    {
    public:
        Color() : r(0), g(0), b(0), a(255) {}
        Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255)
            : r(r), g(g), b(b), a(a) {}

        // Getters
        uint8_t getRed() const { return r; }
        uint8_t getGreen() const { return g; }
        uint8_t getBlue() const { return b; }
        uint8_t getAlpha() const { return a; }

        // Setters
        void setRed(uint8_t red) { r = red; }
        void setGreen(uint8_t green) { g = green; }
        void setBlue(uint8_t blue) { b = blue; }
        void setAlpha(uint8_t alpha) { a = alpha; }

        // Predefined colors
        static Color White() { return Color(255, 255, 255); }
        static Color Black() { return Color(0, 0, 0); }
        static Color Red() { return Color(255, 0, 0); }
        static Color Green() { return Color(0, 255, 0); }
        static Color Blue() { return Color(0, 0, 255); }
        static Color Transparent() { return Color(0, 0, 0, 0); }

        bool operator==(const Color& other) const {
            return r == other.r && g == other.g && b == other.b && a == other.a;
        }

    private:
        uint8_t r, g, b, a; // RGBA components
    };
}