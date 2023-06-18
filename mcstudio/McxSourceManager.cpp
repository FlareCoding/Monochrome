#include "McxSourceManager.h"

namespace mc::mcstudio {
	void McxSourceManager::initialize() {
		mcx::McxEngine::setRootMcxDirectory("mcx");
	}

	Shared<ClassicWindow> McxSourceManager::loadWindowFile(const std::string& name) {
		auto path = _constructMcxPath(name);

		return mcx::McxEngine::parseWindowFile(path);
	}

	Shared<BaseWidget> McxSourceManager::loadUserWidgetFile(const std::string& name) {
		auto path = _constructMcxPath(name);

		return mcx::McxEngine::parseUserWidgetFile(name);
	}

	std::string McxSourceManager::_constructMcxPath(const std::string& name) {
		return name + ".mcx";
	}
} // namespace mc::mcstudio
