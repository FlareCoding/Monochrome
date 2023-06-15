#pragma once
#include <window/ClassicWindow.h>
#include <widgets/AllWidgets.h>
#include <mcx/McxEngine.h>

// Helper macros
#define ADD_LOAD_WINDOW_FN(fnName, path) \
	static Shared<ClassicWindow> fnName() { \
		return loadWindowFile(path); \
	}

#define ADD_LOAD_WIDGET_FN(fnName, path) \
	static Shared<BaseWidget> fnName() { \
		return loadWindowFile(path); \
	}

#define ADD_LOAD_CONTAINER_WIDGET_FN(fnName, path) \
	static Shared<BaseContainerWidget> fnName() { \
		return std::static_pointer_cast<BaseContainerWidget>(\
			loadWindowFile(path)); \
	}

#define ADD_LOAD_STYLESHEET_FN(fnName, path) \
	static void fnName() { \
		loadStylesheet(path); \
	}

// Mcx file names
#define MCSTUDIO_MCXPATH_MAIN			"main"
#define MCSTUDIO_MCXPATH_STYLESHEET		"stylesheet"

namespace mc::mcstudio {
class McxSourceManager {
public:
	ADD_LOAD_WINDOW_FN(loadMainWindow, MCSTUDIO_MCXPATH_MAIN)
	ADD_LOAD_STYLESHEET_FN(loadStylesheet, MCSTUDIO_MCXPATH_STYLESHEET)

	// Initializes Mcx paths and other resources
	static void initialize();

	// Loads an mcx window file within the mcstudio directory
	static Shared<ClassicWindow> loadWindowFile(const std::string& name);

	// Loads an mcx user widget file within the mcstudio directory
	static Shared<BaseWidget> loadUserWidgetFile(const std::string& name);
	
	// Loads an mcx stylesheet file within the mcstudio directory
	static void loadStylesheet(const std::string& name);

private:
	// Constructs a proper filepath string
	static std::string _constructMcxPath(const std::string& name);
};
} // namespace mc::mcstudio
