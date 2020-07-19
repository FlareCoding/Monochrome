#pragma once

namespace mc
{
	enum class Shape
	{
		Rectangle, 
		Circle,
		Unknown
	};

	class Rectangle
	{
	public:
		Rectangle() = default;
		Rectangle(float w, float h)
			: width(w), height(h)
		{}

		float width = 10;
		float height = 10;
	};

	class Circle
	{
	public:
		Circle() = default;
		Circle(float radius)
			: radius(radius)
		{}

		float radius = 10;
	};
}
