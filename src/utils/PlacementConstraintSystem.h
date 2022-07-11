#pragma once
#include <core/Core.h>
#include <widgets/Layer.h>
#include <vector>
#include <map>

#define MAIN_SCREEN_CONTAINER_NAME "mcScreenContainer"

namespace mc::utils
{
	struct VirtualContainer
	{
		Frame containerFrame;
		std::vector<Frame> children;

		Position insertChild(
			const Size& childSize,
			const Position& anchorOrigin,
			const Size& anchorElementSize,
			OverflowDirection overflow
		);
	};

	class PlacementConstraintSystem
	{
	public:
		static void registerContainer(const std::string& name, const Size& size);
		static bool hasContainer(const std::string& name);
		static VirtualContainer* getContainer(const std::string& name);

	private:
		static std::map<std::string, VirtualContainer> s_containers;
	};
}
