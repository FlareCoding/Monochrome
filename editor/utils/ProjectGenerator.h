#pragma once
#include "EditorUtils.h"

namespace utils
{
    struct WindowSettings
    {
        uint32_t width;
        uint32_t height;
        const char* title;
        Color color;
    };

    struct ProjectConfig
    {
        std::string projectName;
        std::string location;
        std::string uiClassName;
        WindowSettings windowSettings;
        std::vector<Ref<UIView>> uiViews;
        std::string monochromeSourcePath;
        std::string monochromeLibraryDebugPath;
        std::string monochromeLibraryReleasePath;
    };

    class ProjectGenerator
    {
    public:
        static void GenerateProject(ProjectConfig& config);

    private:
        static void CreateMCLayoutFile(std::string& path, std::vector<Ref<UIView>>& views, WindowSettings& window_settings);
    };
}
