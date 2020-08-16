#pragma once
#include "EditorUtils.h"
#include "../panels/VariableCodeProperties.h"

namespace utils
{
    struct WindowSettings
    {
        uint32_t width = 0;
        uint32_t height = 0;
        std::string title;
        Color color = Color::gray;
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
        std::map<Ref<UIView>, ElementCodeProperties>* elementCodeProperties;
    };

    struct MCLayout
    {
        WindowSettings windowSettings;
        std::vector<Ref<UIView>> uiViews;
        std::map<Ref<UIView>, ElementCodeProperties> elementCodeProperties;
    };

    class ProjectGenerator
    {
    public:
        static void GenerateProjectAndVisualStudioSolution(ProjectConfig& config);
        static void GenerateProjectSourceFiles(ProjectConfig& config);
        static MCLayout LoadMCProject(const std::string& path);

    private:
        static void CreateMCLayoutFile(std::string& path, std::vector<Ref<UIView>>& views, WindowSettings& window_settings, std::map<Ref<UIView>, ElementCodeProperties>& element_code_props);
    };
}
