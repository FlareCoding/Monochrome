#include "PlacementConstraintSystem.h"

namespace mc::utils
{
	std::map<
		std::string,
		VirtualContainer
	> PlacementConstraintSystem::s_containers;


	void PlacementConstraintSystem::registerContainer(const std::string& name, const Size& size) {
		CORE_ASSERT(!hasContainer(name), "Virtual container with name '" + name + "' already registered");
		
		VirtualContainer container;
		container.containerFrame = Frame(Position(), size);

		s_containers[name] = container;
	}
	
	bool PlacementConstraintSystem::hasContainer(const std::string& name) {
		return s_containers.find(name) != s_containers.end();
	}

	VirtualContainer* PlacementConstraintSystem::getContainer(const std::string& name) {
		if (hasContainer(name)) {
			return &s_containers.at(name);
		}

		return nullptr;
	}

	void PlacementConstraintSystem::updateContainer(const std::string& name, const Size& size) {
		if (!hasContainer(name)) {
			return;
		}

		VirtualContainer container;
		container.containerFrame = Frame(Position(), size);

		s_containers[name] = container;
	}
	
	Position VirtualContainer::insertChild(
		uint64_t id,
		const Size& childSize,
		const Position& anchorOrigin,
		const Size& anchorElementSize,
		OverflowDirection overflow
	) {
		// Set the proposed anchor point to be at the anchor's origin
		Point proposedAnchorPoint = {
			anchorOrigin.x,
			anchorOrigin.y
		};

		// anchorElementSize usually refers to the
		// size of the anchor widget or window element,
		// so depending on the overflow parameter, the
		// proposed position has to be adjusted around it.
		switch (overflow) {
		case OverflowDirection::Down: {
			proposedAnchorPoint.y += (int32_t)anchorElementSize.height;
			break;
		}
		case OverflowDirection::Up: {
			proposedAnchorPoint.y -= childSize.height;
			break;
		}
		case OverflowDirection::Right: {
			proposedAnchorPoint.x += (int32_t)anchorElementSize.width;
			break;
		}
		case OverflowDirection::Left: {
			proposedAnchorPoint.x -= childSize.width;
			break;
		}
		default: break;
		}

		//
		// Check for out of bounds conditions
		//
		if (proposedAnchorPoint.y + childSize.height > containerFrame.size.height) {
			// In this case, the child element is going out of bounds
			// of the virtual container from the __bottom__ edge.
			//
			// Attempt to place
			// the child element above.
			proposedAnchorPoint.y = anchorOrigin.y - childSize.height;
		}

		if (proposedAnchorPoint.y < 0) {
			// In this case, the child element is going out of bounds
			// of the virtual container from the __top__ edge.
			//
			// Attempt to place
			// the child element below.
			proposedAnchorPoint.y += anchorOrigin.y + anchorElementSize.height;
		}
		
		if (proposedAnchorPoint.x + childSize.width > containerFrame.size.width) {
			// In this case, the child element is going out of bounds
			// of the virtual container from the __right__ edge.
			//
			// Attempt to place
			// the child element to the left.
			proposedAnchorPoint.x = anchorOrigin.x - childSize.width;
		}

		if (proposedAnchorPoint.x < 0) {
			// In this case, the child element is going out of bounds
			// of the virtual container from the __left__ edge.
			//
			// Attempt to place
			// the child element to the right.
			proposedAnchorPoint.x = anchorOrigin.x + anchorElementSize.width;
		}

		// Insert the child into the children's map
		auto resultingFrame = Frame(proposedAnchorPoint, childSize);
		children[id] = resultingFrame;

		for (auto& [frameId, frame] : children) {
			if (id == frameId) {
				continue;
			}

			Distance resolutionDistance;
			if (framesColliding(resultingFrame, frame, overflow, resolutionDistance)) {
				resultingFrame.position.x += resolutionDistance.x;
				resultingFrame.position.y += resolutionDistance.y;
			}
		}

		return resultingFrame.position;
	}
	
	void VirtualContainer::removeChild(uint64_t id) {
		if (children.find(id) != children.end()) {
			children.erase(id);
		}
	}
	
	bool VirtualContainer::framesColliding(
		Frame& first,
		Frame& second,
		OverflowDirection preferredOverflow,
		Distance& outResolutionDistance
	) {
		int32_t firstMinX = first.position.x;
		int32_t firstMinY = first.position.y;
		int32_t firstMaxX = first.position.x + (int32_t)first.size.width;
		int32_t firstMaxY = first.position.y + (int32_t)first.size.height;

		int32_t secondMinX = second.position.x;
		int32_t secondMinY = second.position.y;
		int32_t secondMaxX = second.position.x + (int32_t)second.size.width;
		int32_t secondMaxY = second.position.y + (int32_t)second.size.height;

		int32_t xDiff = std::min(firstMaxX, secondMaxX) - std::max(firstMinX, secondMinX);
		int32_t yDiff = std::min(firstMaxY, secondMaxY) - std::max(firstMinY, secondMinY);

		bool intersecting = (xDiff > 0) && (yDiff > 0);
		if (intersecting) {
			// Safety measures
			xDiff += 1;
			yDiff += 1;

			bool xDirectionPositive = (firstMinX > secondMinX) && (firstMaxX > secondMaxX);
			if (!xDirectionPositive) {
				xDiff *= -1;
			}

			bool yDirectionPositive = (firstMinY > secondMinY) && (firstMaxY > secondMaxY);
			if (!yDirectionPositive) {
				yDiff *= -1;
			}

			if (std::abs(xDiff) > std::abs(yDiff))
				outResolutionDistance = { 0, yDiff };
			else
				outResolutionDistance = { xDiff, 0 };
		}

		return intersecting;
	}
}
