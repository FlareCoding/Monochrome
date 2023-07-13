#if defined(NDEBUG) && defined(_WIN32)
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif // !NDEBUG

#include <window/ClassicWindow.h>
#include <widgets/AllWidgets.h>
#include <utils/FileDialog.h>
using namespace mc;

#include <fstream>
#include <filesystem>

struct WidgetConfiguration {
    std::string name;
    bool isContainer = false;

    std::vector<
        std::pair<std::string, std::string>
    > publicProperties;
};

static WidgetConfiguration s_widgetConfiguration;

Shared<Entry> createPromptEntry(uint32_t maxWidth = 140);

Shared<StackPanel> createAddPropertiesCell();
Shared<ScrollPanel> createPropertiesListCell();
Shared<StackPanel> createPropertyCell(
    const std::string& type,
    const std::string& name
);

static Shared<StackPanel> s_propertiesListPanel;

void submitAddProperty(Shared<Combobox> typeCombobox, Shared<Entry> namePrompt) {
    if (namePrompt->isEmpty()) {
            namePrompt->borderColor = Color::red;
            return;
        } else {
            namePrompt->borderColor = Color::transparent;
        }

        auto propType = typeCombobox->getSelectedItem();
        auto propName = namePrompt->text;

        auto propCell = createPropertyCell(propType, propName);
        s_propertiesListPanel->addChild(propCell);

        s_widgetConfiguration.publicProperties.push_back({ propType, propName });

        // Clear out the prompt entry
        namePrompt->text = "";

        // Focus on the property list
        s_propertiesListPanel->focus();
}

std::string generateHeaderSource();
std::string generateCppSource();

int main() {
    AppManager::registerApplication("appId-monodev-devtools");

    auto window = MakeRef<ClassicWindow>(680, 530, "MonoDev");
    window->setMinWidth(680);
    window->setMinHeight(530);

    auto rootPanel = MakeRef<StackPanel>();
    rootPanel->backgroundColor = Color(18, 22, 28);
    window->setRootWidget(rootPanel);

    auto widgetNameLabel = MakeRef<Label>();
    widgetNameLabel->marginTop = 12;
    widgetNameLabel->marginBottom = 4;
    widgetNameLabel->text = "Widget Name";
    widgetNameLabel->fontSize = 18;
    widgetNameLabel->horizontalAlignment = HACenter;
    rootPanel->addChild(widgetNameLabel);

    auto widgetNamePrompt = createPromptEntry();
    widgetNamePrompt->placeholder = "widgetName";
    widgetNamePrompt->marginBottom = 8;
    widgetNamePrompt->horizontalAlignment = HACenter;
    widgetNamePrompt->on("changed", [](Shared<Event> e) {
        auto text = e->get<std::string>("text");
        s_widgetConfiguration.name = text;
    });
    rootPanel->addChild(widgetNamePrompt);

    auto isWidgetContainerCheckbox = MakeRef<Checkbox>();
    isWidgetContainerCheckbox->label->text = "Container";
    isWidgetContainerCheckbox->label->fontSize = 12;
    isWidgetContainerCheckbox->marginBottom = 28;
    isWidgetContainerCheckbox->horizontalAlignment = HACenter;
    isWidgetContainerCheckbox->on("valueChanged", [](Shared<Event> e) {
        auto checked = e->get<bool>("checked");
        s_widgetConfiguration.isContainer = checked;
    });
    rootPanel->addChild(isWidgetContainerCheckbox);

    auto propsLabel = MakeRef<Label>();
    propsLabel->marginTop = 12;
    propsLabel->marginBottom = 4;
    propsLabel->text = "Public Properties";
    propsLabel->fontSize = 18;
    propsLabel->horizontalAlignment = HACenter;
    rootPanel->addChild(propsLabel);

    rootPanel->addChild(createAddPropertiesCell());
    rootPanel->addChild(createPropertiesListCell());

    auto generateSourcesButton = MakeRef<Button>();
    generateSourcesButton->label->text = "Generate Source Files";
    generateSourcesButton->horizontalAlignment = HACenter;
    generateSourcesButton->borderColor = Color::transparent;
    generateSourcesButton->backgroundColor = Color(57, 59, 59);
    generateSourcesButton->on("clicked", [](Shared<Event> e) {
        auto fileDialog = utils::FileDialog::create();
        fileDialog->setDefaultPath(std::filesystem::current_path().string());
        auto path = fileDialog->chooseFolderDialog();

        if (path.empty()) {
            return;
        }

        auto headerSource = generateHeaderSource();
        auto cppSource = generateCppSource();
        
        // Create files on the filesystem
        std::ofstream headerFile(path + "/" + s_widgetConfiguration.name + ".h");
        headerFile << headerSource;
        headerFile.close();

        std::ofstream cppFile(path + "/" + s_widgetConfiguration.name + ".cpp");
        cppFile << cppSource;
        cppFile.close();
    });
    rootPanel->addChild(generateSourcesButton);

    AppManager::startApplicationLoop();
    return 0;
}

Shared<Entry> createPromptEntry(uint32_t maxWidth) {
    auto prompt = MakeRef<Entry>();
    prompt->fontSize = 12;
    prompt->fixedWidth = maxWidth;
    prompt->fixedHeight = 20;
    prompt->backgroundColor = Color::transparent;
    prompt->borderColor = Color::transparent;
    prompt->cursorColor = Color::white;
    prompt->textColor = Color::white;
    prompt->focusedBorderColor = Color(0, 240, 216);
    
    return prompt;
}

Shared<StackPanel> createAddPropertiesCell() {
    auto cellPanel = MakeRef<StackPanel>();
    cellPanel->orientation = Horizontal;
    cellPanel->marginLeft = 12;
    cellPanel->marginBottom = 20;
    cellPanel->backgroundColor = Color::transparent;
    cellPanel->horizontalAlignment = HACenter;

    auto typeLabel = MakeRef<Label>();
    typeLabel->text = "Type: ";
    cellPanel->addChild(typeLabel);

    auto propTypeCombobox = MakeRef<Combobox>();
    propTypeCombobox->fixedWidth = 100;
    propTypeCombobox->fixedHeight = 16;
    propTypeCombobox->addItems({
        "int32_t", "uint32_t",
        "std::string", "bool",
        "Color"
    });
    propTypeCombobox->marginRight = 20;
    cellPanel->addChild(propTypeCombobox);

    auto nameLabel = MakeRef<Label>();
    nameLabel->text = "Name: ";
    cellPanel->addChild(nameLabel);

    auto propNamePrompt = createPromptEntry(110);
    propNamePrompt->placeholder = "propName";
    propNamePrompt->marginRight = 12;
    propNamePrompt->on("entered", [propTypeCombobox, propNamePrompt](Shared<Event> e) {
        submitAddProperty(propTypeCombobox, propNamePrompt);
    });
    cellPanel->addChild(propNamePrompt);

    auto addPropButton = MakeRef<Button>();
    addPropButton->label->text = "Add Property";
    addPropButton->fixedHeight = 16;
    addPropButton->borderColor = Color::transparent;
    addPropButton->backgroundColor = Color(57, 59, 59);
    addPropButton->on("clicked", [propTypeCombobox, propNamePrompt](Shared<Event> e) {
        submitAddProperty(propTypeCombobox, propNamePrompt);
    });
    cellPanel->addChild(addPropButton);

    return cellPanel;
}

Shared<ScrollPanel> createPropertiesListCell() {
    auto cellPanel = MakeRef<ScrollPanel>();
    cellPanel->fixedHeight = 200;
    cellPanel->marginLeft = 12;
    cellPanel->marginBottom = 30;
    cellPanel->horizontalAlignment = HACenter;

    s_propertiesListPanel = MakeRef<StackPanel>();
    s_propertiesListPanel->backgroundColor = Color(38, 38, 38);

    cellPanel->addChild(s_propertiesListPanel);
    return cellPanel;
}

Shared<StackPanel> createPropertyCell(
    const std::string& type,
    const std::string& name
) {
    auto cellPanel = MakeRef<StackPanel>();
    cellPanel->orientation = Horizontal;
    cellPanel->marginLeft = 4;
    cellPanel->marginRight = 4;
    cellPanel->marginTop = 2;
    cellPanel->marginBottom = 2;
    cellPanel->backgroundColor = Color::transparent;

    auto typeLabel = MakeRef<Label>();
    typeLabel->text = type;
    typeLabel->color = Color(34, 185, 245);
    typeLabel->fixedWidth = 100;
    typeLabel->alignment = "left";
    typeLabel->marginLeft = 6;
    cellPanel->addChild(typeLabel);

    auto nameLabel = MakeRef<Label>();
    nameLabel->text = name;
    nameLabel->color = Color(202, 252, 222);
    nameLabel->alignment = "left";
    nameLabel->fixedWidth = 160;
    cellPanel->addChild(nameLabel);

    auto removePropButton = MakeRef<Button>();
    removePropButton->label->text = "Remove";
    removePropButton->fixedHeight = 16;
    removePropButton->marginTop = 2;
    removePropButton->borderColor = Color::transparent;
    removePropButton->backgroundColor = Color(57, 59, 59);
    removePropButton->on("clicked", [removePropButton](Shared<Event> e) {
        auto cell = dynamic_cast<StackPanel*>(removePropButton->getParent());
        s_propertiesListPanel->removeChild(cell->getID());

        auto propName = std::dynamic_pointer_cast<Label>(cell->getChild(1))->text.get();

        for (auto it = s_widgetConfiguration.publicProperties.begin();
            it != s_widgetConfiguration.publicProperties.end();
            ++it
        ) {
            if (it->second == propName) {
                s_widgetConfiguration.publicProperties.erase(it);
                break;
            }
        }
    });
    cellPanel->addChild(removePropButton);

    return cellPanel;
}

std::string generateHeaderSource() {
    std::string baseClass = "BaseWidget";
    if (s_widgetConfiguration.isContainer) {
        baseClass = "BaseContainerWidget";
    }

    std::string widgetName = s_widgetConfiguration.name;

    std::string source = "#pragma once\n";
    source.append("#include \"BaseWidget.h\"\n\n");
    source.append("namespace mc {\n");
    source.append("class " + widgetName + " : public " + baseClass + " {\n");
    source.append("public:\n");

    // Constructor
    source.append("\t" + widgetName + "();\n\n");

    // 'getWidgetName' function
    source.append(
        "\tstd::string getWidgetName() const override { return \"" + widgetName + "\"; }\n\n"
    );

    // Public members
    for (auto& [type, name] : s_widgetConfiguration.publicProperties) {
        source.append("\t// @brief TO-DO\n");
        source.append("\tPropertyObserver<" + type + "> " + name + ";\n\n");
    }

    // Protected members
    source.append("protected:\n");
    source.append("\tSize _measureSize() override;\n");
    source.append("\tvoid _onSetComputedSize(const Size& size) override;\n");
    source.append("\tvoid _onArrangeChildren() override;\n\n");

    // Private members
    source.append("private:\n");
    source.append("\tvoid _createVisuals();\n");
    source.append("\tvoid _setupProperties();\n");

    source.append("};\n"); // class ending
    source.append("} // namespace mc\n");
    return source;
}

std::string generateCppSource() {
    std::string widgetName = s_widgetConfiguration.name;
    std::string source =  "#include \"" + widgetName + ".h\"\n\n";
    source.append("namespace mc {\n");

    // Constructor
    source.append("\t" + widgetName + "::" + widgetName + "() {\n");
    source.append("\t\t_createVisuals();\n");
    source.append("\t\t_setupProperties();\n");
    source.append("\t}\n\n");

    // _measureSize()
    source.append("\tSize " + widgetName + "::_measureSize() {\n\t\treturn Size(0, 0);\n\t}\n\n");

    // _onSetComputedSize()
    source.append("\tvoid " + widgetName + "::_onSetComputedSize(const Size& size) {\n\t}\n\n");

    // _onArrangeChildren()
    source.append("\tvoid " + widgetName + "::_onArrangeChildren() {\n\t}\n\n");

    // _createVisuals()
    source.append("\tvoid " + widgetName + "::_createVisuals() {\n\t}\n\n");

    // _setupProperties()
    source.append("\tvoid " + widgetName + "::_setupProperties() {\n");
    
    // Setting up public properties
    for (auto& [type, name] : s_widgetConfiguration.publicProperties) {
        source.append("\t\t" + name + " = " + type + "(); // default value\n");
        source.append("\t\thandleWidgetVisiblePropertyChange(" + name + ");\n\n");
    }

    // Setup default cursor type
    source.append("\t\tcursorType = CursorType::Hand;\n");

    // End the _setupProperties() method
    source.append("\t}\n");

    source.append("} // namespace mc\n");
    return source;
}
