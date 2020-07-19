#pragma once
#include "../Graphics/Color.h"

namespace mc
{
	typedef struct Point
	{
		union
		{
			float x = 0;
			float width;
		};

		union
		{
			float y = 0;
			float height;
		};

		Point operator+(const Point& rhs) { return Point{ x + rhs.x, y + rhs.y }; }
		Point operator-(const Point& rhs) { return Point{ x - rhs.x, y - rhs.y }; }
		Point operator*(const Point& rhs) { return Point{ x * rhs.x, y * rhs.y }; }
		Point operator/(const Point& rhs) { return Point{ x / rhs.x, y / rhs.y }; }

		Point operator+=(const Point& rhs) { return Point{ x += rhs.x, y += rhs.y }; }
		Point operator-=(const Point& rhs) { return Point{ x -= rhs.x, y -= rhs.y }; }
		Point operator*=(const Point& rhs) { return Point{ x *= rhs.x, y *= rhs.y }; }
		Point operator/=(const Point& rhs) { return Point{ x /= rhs.x, y /= rhs.y }; }

		// Corrects coordinates match display's dpi settings
		inline void AdjustToDpi(uint32_t dpi)
		{
			static float default_dpi = 96.0f;

			x = x * (float)dpi / default_dpi;
			y = y * (float)dpi / default_dpi;
		}

	} Point, Size, Position;

	struct Frame
	{
		Size		size{ 40, 40 };
		Position	position{ 0, 0 };

		Frame() {}
		Frame(float x, float y, float width, float height) : size(Size{ width, height }), position(Position{ x, y }) {}
		Frame(Position position, Size size) : size(size), position(position) {}

		inline const Position GetTopRightCorner()		const { return Position{ position.x + size.width, position.y }; }
		inline const Position GetBottomRightConer()		const { return Position{ position.x + size.width, position.y + size.height }; }
		inline const Position GetBottomLeftCorner()		const { return Position{ position.x, position.y + size.height }; }

		// Returns whether or not the point is inside the frame's region
		inline const bool DoesContain(Point p) const
		{
			if (p.x > position.x&& p.x < position.x + size.width &&
				p.y > position.y&& p.y < position.y + size.height)
				return true;
			return false;
		}

		// Corrects frame coordinates and size to match display's dpi settings
		inline void AdjustToDpi(uint32_t dpi)
		{
			static float default_dpi = 96.0f;

			position.x = position.x * (float)dpi / default_dpi;
			position.y = position.y * (float)dpi / default_dpi;

			size.width = size.width * (float)dpi / default_dpi;
			size.height = size.height * (float)dpi / default_dpi;
		}
	};

	class Layer
	{
	public:
		Color color = Color::white;
		Frame frame = Frame();
	};
}
