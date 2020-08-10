#include "ProjectGenerator.h"
#include <fstream>

#include "../rapidxml/rapidxml.hpp"
#include "../rapidxml/rapidxml_print.hpp"
using namespace rapidxml;

namespace utils
{
#define MC_ENUM_ELEM_TO_STRING(elem) "mc::"##elem

    static std::wstring StringToWidestring(const std::string& as)
    {
        if (as.empty())    return std::wstring();

        size_t reqLength = ::MultiByteToWideChar(CP_UTF8, 0, as.c_str(), (int)as.length(), 0, 0);
        std::wstring ret(reqLength, L'\0');

        ::MultiByteToWideChar(CP_UTF8, 0, as.c_str(), (int)as.length(), &ret[0], (int)ret.length());
        return ret;
    }

    Widget GetWidgetType(UIView* view);
    std::string WidgetTypeToString(Widget type);
    void AddWidgetPropertyNodes(Ref<xml_document<>>& doc, xml_node<>*& view_node, Widget type, Ref<UIView>& view);

    void AddBasicPropertyNodes(Ref<xml_document<>>& doc, xml_node<>*& view_node, Ref<UIView>& view);
    void AddLabelPropertyNodes(Ref<xml_document<>>& doc, xml_node<>*& view_node, Ref<UILabel>& label);
    void AddButtonPropertyNodes(Ref<xml_document<>>& doc, xml_node<>*& view_node, Ref<UIButton>& button);
    void AddCheckboxPropertyNodes(Ref<xml_document<>>& doc, xml_node<>*& view_node, Ref<UICheckbox>& checkbox);
    void AddSliderPropertyNodes(Ref<xml_document<>>& doc, xml_node<>*& view_node, Ref<UISlider>& slider);

    void UIViewToXML(Ref<xml_document<>>& doc, xml_node<>*& root_node, Ref<UIView>& view, std::map<Ref<UIView>, ElementCodeProperties>& element_code_props);
    void AddWindowSettingsNode(Ref<xml_document<>>& doc, xml_node<>*& root_node, WindowSettings& window_settings);

    void ProjectGenerator::GenerateProject(ProjectConfig& config)
    {
        // Create monochrome UI layout file
        CreateMCLayoutFile(config.location + "\\" + config.projectName + ".mc", config.uiViews, config.windowSettings, *config.elementCodeProperties);

        // Arg1 --> Target Path
        // Arg2 --> Project Name
        // Arg3 --> Class Name
        // Arg4 --> Monochrome Source Path
        // Arg5 --> Monochrome Library Debug Path
        // Arg6 --> Monochrome Library Release Path

        std::string cmd = std::string("python project_source_generator.py " +
            std::string("\"") + config.location + "\"" + " \"" + config.projectName + "\" \"" + config.uiClassName + "\" " +
            "\"" + config.monochromeSourcePath + "\" \"" + config.monochromeLibraryDebugPath + "\" \"" + config.monochromeLibraryReleasePath + "\""
        );

        std::system(cmd.c_str());
    }

    void ProjectGenerator::CreateMCLayoutFile(std::string& path, std::vector<Ref<UIView>>& views, WindowSettings& window_settings, std::map<Ref<UIView>, ElementCodeProperties>& element_code_props)
    {
        std::ofstream file(path);
        Ref<xml_document<>> document = MakeRef<xml_document<>>();

        // Declaration
        xml_node<>* decl = document->allocate_node(node_declaration);
        decl->append_attribute(document->allocate_attribute("version", "1.0"));
        decl->append_attribute(document->allocate_attribute("encoding", "utf-8"));
        document->append_node(decl);

        // Root node
        xml_node<>* root_node = document->allocate_node(node_element, "mcscene");

        AddWindowSettingsNode(document, root_node, window_settings);

        for (auto& view : views)
            UIViewToXML(document, root_node, view, element_code_props);

        document->append_node(root_node);
        file << *document;
        file.close();
    }

    void UIViewToXML(Ref<xml_document<>>& doc, xml_node<>*& root_node, Ref<UIView>& view, std::map<Ref<UIView>, ElementCodeProperties>& element_code_props)
    {
        xml_node<>* view_node = doc->allocate_node(node_element, "uiview");

        Widget widget_type = GetWidgetType(view.get());
        view_node->append_attribute(doc->allocate_attribute("type", doc->allocate_string(WidgetTypeToString(widget_type).c_str())));

        // Element Code Properties
        if (element_code_props.find(view) != element_code_props.end())
        {
            view_node->append_attribute(doc->allocate_attribute("visibility", doc->allocate_string(element_code_props[view].visibility.c_str())));
            view_node->append_attribute(doc->allocate_attribute("name", doc->allocate_string(element_code_props[view].name.c_str())));

            // Event handlers data
            if (element_code_props[view].eventHandlerDataMap.size())
            {
                xml_node<>* event_handlers_data_node = doc->allocate_node(node_element, "event_handlers");
                for (auto& [type, data] : element_code_props[view].eventHandlerDataMap)
                {
                    xml_node<>* handler_node = doc->allocate_node(node_element, "handler");
                    handler_node->append_attribute(doc->allocate_attribute("type", doc->allocate_string(type.c_str())));
                    handler_node->append_attribute(doc->allocate_attribute("generate_member_fn", doc->allocate_string(std::to_string(data.generateClassFunction).c_str())));

                    if (data.generateClassFunction)
                    {
                        xml_node<>* member_fn_name_node = doc->allocate_node(node_element, "name");
                        member_fn_name_node->value(doc->allocate_string(data.classFunctionName.c_str()));

                        xml_node<>* member_fn_visibility_node = doc->allocate_node(node_element, "visibility");
                        member_fn_visibility_node->value(doc->allocate_string(data.classFunctionVisibility.c_str()));

                        xml_node<>* member_fn_node = doc->allocate_node(node_element, "member_fn");
                        member_fn_node->append_node(member_fn_name_node);
                        member_fn_node->append_node(member_fn_visibility_node);
                        handler_node->append_node(member_fn_node);
                    }

                    xml_node<>* return_status_node = doc->allocate_node(node_element, "return_status");
                    if (data.returnStatus._Equal("Handled"))
                        return_status_node->value(doc->allocate_string("EVENT_HANDLED"));
                    else
                        return_status_node->value(doc->allocate_string("EVENT_UNHANDLED"));

                    handler_node->append_node(return_status_node);
                    event_handlers_data_node->append_node(handler_node);
                }
                view_node->append_node(event_handlers_data_node);
            }
        }

        AddWidgetPropertyNodes(doc, view_node, widget_type, view);

        for (auto& child : view->subviews)
            UIViewToXML(doc, view_node, child, element_code_props);

        root_node->append_node(view_node);
    }

    Widget GetWidgetType(UIView* view)
    {
        if (CheckType<UILabel>(view))       return Widget::Label;
        if (CheckType<UIButton>(view))      return Widget::Button;
        if (CheckType<UICheckbox>(view))    return Widget::Checkbox;
        if (CheckType<UISlider>(view))      return Widget::Slider;

        return Widget::Unknown;
    }

    std::string WidgetTypeToString(Widget type)
    {
        switch (type)
        {
        case Widget::Label:     return "UILabel";
        case Widget::Button:    return "UIButton";
        case Widget::Checkbox:  return "UICheckbox";
        case Widget::Slider:    return "UISlider";

        default: return "UIView";
        }
    }

    void AddWidgetPropertyNodes(Ref<xml_document<>>& doc, xml_node<>*& view_node, Widget type, Ref<UIView>& view)
    {
        AddBasicPropertyNodes(doc, view_node, view);

        switch (type)
        {
        case Widget::Label:     return AddLabelPropertyNodes(doc, view_node, std::dynamic_pointer_cast<UILabel>(view));
        case Widget::Button:    return AddButtonPropertyNodes(doc, view_node, std::dynamic_pointer_cast<UIButton>(view));
        case Widget::Checkbox:  return AddCheckboxPropertyNodes(doc, view_node, std::dynamic_pointer_cast<UICheckbox>(view));
        case Widget::Slider:    return AddSliderPropertyNodes(doc, view_node, std::dynamic_pointer_cast<UISlider>(view));
        default: break;
        }
    }

    void AddBasicPropertyNodes(Ref<xml_document<>>& doc, xml_node<>*& view_node, Ref<UIView>& view)
    {
        xml_node<>* width_node = doc->allocate_node(node_element, "width");
        width_node->value(doc->allocate_string(std::to_string(view->layer.frame.size.width).c_str()));

        xml_node<>* height_node = doc->allocate_node(node_element, "height");
        height_node->value(doc->allocate_string(std::to_string(view->layer.frame.size.height).c_str()));

        xml_node<>* xpos_node = doc->allocate_node(node_element, "x");
        xpos_node->value(doc->allocate_string(std::to_string(view->layer.frame.position.x).c_str()));

        xml_node<>* ypos_node = doc->allocate_node(node_element, "y");
        ypos_node->value(doc->allocate_string(std::to_string(view->layer.frame.position.y).c_str()));

        xml_node<>* size_node = doc->allocate_node(node_element, "size");
        size_node->append_node(width_node);
        size_node->append_node(height_node);

        xml_node<>* position_node = doc->allocate_node(node_element, "position");
        position_node->append_node(xpos_node);
        position_node->append_node(ypos_node);

        xml_node<>* frame_node = doc->allocate_node(node_element, "frame");
        frame_node->append_node(size_node);
        frame_node->append_node(position_node);

        xml_node<>* color_node = doc->allocate_node(node_element, "color");
        color_node->append_attribute(doc->allocate_attribute("r", doc->allocate_string(std::to_string(view->layer.color.r).c_str())));
        color_node->append_attribute(doc->allocate_attribute("g", doc->allocate_string(std::to_string(view->layer.color.g).c_str())));
        color_node->append_attribute(doc->allocate_attribute("b", doc->allocate_string(std::to_string(view->layer.color.b).c_str())));
        color_node->append_attribute(doc->allocate_attribute("a", doc->allocate_string(std::to_string(view->layer.color.alpha).c_str())));

        xml_node<>* layer_node = doc->allocate_node(node_element, "layer");
        layer_node->append_node(frame_node);
        layer_node->append_node(color_node);
        view_node->append_node(layer_node);

        xml_node<>* zindex_node = doc->allocate_node(node_element, "z_index");
        zindex_node->value(doc->allocate_string(std::to_string(view->GetZIndex()).c_str()));
        view_node->append_node(zindex_node);

        xml_node<>* visible_node = doc->allocate_node(node_element, "visible");
        visible_node->value(doc->allocate_string(std::to_string(view->Visible).c_str()));
        view_node->append_node(visible_node);

        xml_node<>* corner_radius_node = doc->allocate_node(node_element, "corner_radius");
        corner_radius_node->value(doc->allocate_string(std::to_string(view->CornerRadius).c_str()));
        view_node->append_node(corner_radius_node);
    }

    void AddLabelPropertyNodes(Ref<xml_document<>>& doc, xml_node<>*& view_node, Ref<UILabel>& label)
    {
        xml_node<>* text_node = doc->allocate_node(node_element, "text");
        text_node->value(doc->allocate_string(label->Text.c_str()));
        view_node->append_node(text_node);

        xml_node<>* use_widestring_text_node = doc->allocate_node(node_element, "use_widestring");
        use_widestring_text_node->value(doc->allocate_string(std::to_string(label->UseWidestringText).c_str()));
        view_node->append_node(use_widestring_text_node);

        xml_node<>* color_node = doc->allocate_node(node_element, "color");
        color_node->append_attribute(doc->allocate_attribute("r", doc->allocate_string(std::to_string(label->color.r).c_str())));
        color_node->append_attribute(doc->allocate_attribute("g", doc->allocate_string(std::to_string(label->color.g).c_str())));
        color_node->append_attribute(doc->allocate_attribute("b", doc->allocate_string(std::to_string(label->color.b).c_str())));
        color_node->append_attribute(doc->allocate_attribute("a", doc->allocate_string(std::to_string(label->color.alpha).c_str())));
        view_node->append_node(color_node);

        xml_node<>* text_properties_node = doc->allocate_node(node_element, "text_properties");

        xml_node<>* font_node = doc->allocate_node(node_element, "font");
        font_node->value(doc->allocate_string(label->Properties.Font.c_str()));
        text_properties_node->append_node(font_node);

        xml_node<>* font_size_node = doc->allocate_node(node_element, "font_size");
        font_size_node->value(doc->allocate_string(std::to_string(label->Properties.FontSize).c_str()));
        text_properties_node->append_node(font_size_node);

        const auto alignment_to_string = [](TextAlignment alignment) -> std::string {
            switch (alignment)
            {
            case TextAlignment::CENTERED: return "mc::TextAlignment::CENTERED";
            case TextAlignment::LEADING: return "mc::TextAlignment::LEADING";
            case TextAlignment::TRAILING: return "mc::TextAlignment::TRAILING";
            default: return "Unknown";
            }
        };

        const auto wrapping_to_string = [](WordWrapping wrapping) -> std::string {
            switch (wrapping)
            {
            case WordWrapping::CHARACTER_WRAP: return "mc::WordWrapping::CHARACTER_WRAP";
            case WordWrapping::NORMAL_WRAP: return "mc::WordWrapping::NORMAL_WRAP";
            case WordWrapping::NO_WRAP: return "mc::WordWrapping::NO_WRAP";
            case WordWrapping::EMERGENCY_BREAK: return "mc::WordWrapping::EMERGENCY_BREAK";
            case WordWrapping::WORD_WRAP: return "mc::WordWrapping::WORD_WRAP";
            default: return "Unknown";
            }
        };

        const auto style_to_string = [](TextStyle style) -> std::string {
            switch (style)
            {
            case TextStyle::DEFAULT: return "mc::TextStyle::DEFAULT";
            case TextStyle::BOLD: return "mc::TextStyle::BOLD";
            case TextStyle::BOLD_ITALIC: return "mc::TextStyle::BOLD_ITALIC";
            case TextStyle::ITALIC: return "mc::TextStyle::ITALIC";
            case TextStyle::SEMIBOLD_ITALIC: return "mc::TextStyle::SEMIBOLD_ITALIC";
            case TextStyle::SEMIBOLD: return "mc::TextStyle::SEMIBOLD";
            case TextStyle::Light: return "mc::TextStyle::Light";
            default: return "Unknown";
            }
        };

        xml_node<>* alignment_node = doc->allocate_node(node_element, "alignment");
        alignment_node->value(doc->allocate_string(alignment_to_string(label->Properties.Allignment).c_str()));
        text_properties_node->append_node(alignment_node);

        xml_node<>* style_node = doc->allocate_node(node_element, "style");
        style_node->value(doc->allocate_string(style_to_string(label->Properties.Style).c_str()));
        text_properties_node->append_node(style_node);

        xml_node<>* wrapping_node = doc->allocate_node(node_element, "wrapping");
        wrapping_node->value(doc->allocate_string(wrapping_to_string(label->Properties.Wrapping).c_str()));
        text_properties_node->append_node(wrapping_node);

        view_node->append_node(text_properties_node);
    }

    void AddButtonPropertyNodes(Ref<xml_document<>>& doc, xml_node<>*& view_node, Ref<UIButton>& button)
    {
        xml_node<>* filled_node = doc->allocate_node(node_element, "filled");
        filled_node->value(doc->allocate_string(std::to_string(button->Filled).c_str()));
        view_node->append_node(filled_node);

        xml_node<>* stroke_node = doc->allocate_node(node_element, "stroke");
        stroke_node->value(doc->allocate_string(std::to_string(button->Filled).c_str()));
        view_node->append_node(stroke_node);

        xml_node<>* hover_on_node = doc->allocate_node(node_element, "hover_on_color");
        hover_on_node->append_attribute(doc->allocate_attribute("r", doc->allocate_string(std::to_string(button->HoverOnColor.r).c_str())));
        hover_on_node->append_attribute(doc->allocate_attribute("g", doc->allocate_string(std::to_string(button->HoverOnColor.g).c_str())));
        hover_on_node->append_attribute(doc->allocate_attribute("b", doc->allocate_string(std::to_string(button->HoverOnColor.b).c_str())));
        hover_on_node->append_attribute(doc->allocate_attribute("a", doc->allocate_string(std::to_string(button->HoverOnColor.alpha).c_str())));
        view_node->append_node(hover_on_node);

        xml_node<>* on_mouse_press_color = doc->allocate_node(node_element, "on_mouse_press_color");
        on_mouse_press_color->append_attribute(doc->allocate_attribute("r", doc->allocate_string(std::to_string(button->HoverOnColor.r).c_str())));
        on_mouse_press_color->append_attribute(doc->allocate_attribute("g", doc->allocate_string(std::to_string(button->HoverOnColor.g).c_str())));
        on_mouse_press_color->append_attribute(doc->allocate_attribute("b", doc->allocate_string(std::to_string(button->HoverOnColor.b).c_str())));
        on_mouse_press_color->append_attribute(doc->allocate_attribute("a", doc->allocate_string(std::to_string(button->HoverOnColor.alpha).c_str())));
        view_node->append_node(on_mouse_press_color);
    }

    void AddCheckboxPropertyNodes(Ref<xml_document<>>& doc, xml_node<>*& view_node, Ref<UICheckbox>& checkbox)
    {
        xml_node<>* checked_node = doc->allocate_node(node_element, "checked");
        checked_node->value(doc->allocate_string(std::to_string(checkbox->Checked).c_str()));
        view_node->append_node(checked_node);

        xml_node<>* box_size_node = doc->allocate_node(node_element, "box_size");
        box_size_node->value(doc->allocate_string(std::to_string(checkbox->BoxSize).c_str()));
        view_node->append_node(box_size_node);

        xml_node<>* label_margins_node = doc->allocate_node(node_element, "label_margins_size");
        label_margins_node->value(doc->allocate_string(std::to_string(checkbox->BoxSize).c_str()));
        view_node->append_node(label_margins_node);

        xml_node<>* checkmark_color = doc->allocate_node(node_element, "checkmark_color");
        checkmark_color->append_attribute(doc->allocate_attribute("r", doc->allocate_string(std::to_string(checkbox->CheckmarkColor.r).c_str())));
        checkmark_color->append_attribute(doc->allocate_attribute("g", doc->allocate_string(std::to_string(checkbox->CheckmarkColor.g).c_str())));
        checkmark_color->append_attribute(doc->allocate_attribute("b", doc->allocate_string(std::to_string(checkbox->CheckmarkColor.b).c_str())));
        checkmark_color->append_attribute(doc->allocate_attribute("a", doc->allocate_string(std::to_string(checkbox->CheckmarkColor.alpha).c_str())));
        view_node->append_node(checkmark_color);

        xml_node<>* checkedbox_color = doc->allocate_node(node_element, "checkedbox_color");
        checkedbox_color->append_attribute(doc->allocate_attribute("r", doc->allocate_string(std::to_string(checkbox->CheckedBoxColor.r).c_str())));
        checkedbox_color->append_attribute(doc->allocate_attribute("g", doc->allocate_string(std::to_string(checkbox->CheckedBoxColor.g).c_str())));
        checkedbox_color->append_attribute(doc->allocate_attribute("b", doc->allocate_string(std::to_string(checkbox->CheckedBoxColor.b).c_str())));
        checkedbox_color->append_attribute(doc->allocate_attribute("a", doc->allocate_string(std::to_string(checkbox->CheckedBoxColor.alpha).c_str())));
        view_node->append_node(checkedbox_color);
    }

    void AddSliderPropertyNodes(Ref<xml_document<>>& doc, xml_node<>*& view_node, Ref<UISlider>& slider)
    {
        std::string knob_shape_string = (slider->SliderKnobShape == Shape::Rectangle) ? "rectangle" : "circle";
        xml_node<>* knob_shape_node = doc->allocate_node(node_element, "knob_shape");
        knob_shape_node->value(doc->allocate_string(knob_shape_string.c_str()));
        view_node->append_node(knob_shape_node);

        xml_node<>* slider_bar_height_node = doc->allocate_node(node_element, "sliderbar_height");
        slider_bar_height_node->value(doc->allocate_string(std::to_string(slider->SliderBarHeight).c_str()));
        view_node->append_node(slider_bar_height_node);

        xml_node<>* max_val_node = doc->allocate_node(node_element, "max_value");
        max_val_node->value(doc->allocate_string(std::to_string(slider->MaxValue).c_str()));
        view_node->append_node(max_val_node);

        xml_node<>* min_val_node = doc->allocate_node(node_element, "min_value");
        min_val_node->value(doc->allocate_string(std::to_string(slider->MinValue).c_str()));
        view_node->append_node(min_val_node);

        xml_node<>* val_node = doc->allocate_node(node_element, "value");
        val_node->value(doc->allocate_string(std::to_string(slider->Value).c_str()));
        view_node->append_node(val_node);

        xml_node<>* intervals_node = doc->allocate_node(node_element, "intervals");
        intervals_node->value(doc->allocate_string(std::to_string(slider->Intervals).c_str()));
        view_node->append_node(intervals_node);

        xml_node<>* tickmark_visibility_node = doc->allocate_node(node_element, "visible_tickmarks");
        tickmark_visibility_node->value(doc->allocate_string(std::to_string(slider->VisibleTickmarks).c_str()));
        view_node->append_node(tickmark_visibility_node);

        xml_node<>* knob_color_node = doc->allocate_node(node_element, "knob_color");
        knob_color_node->append_attribute(doc->allocate_attribute("r", doc->allocate_string(std::to_string(slider->SliderKnobColor.r).c_str())));
        knob_color_node->append_attribute(doc->allocate_attribute("g", doc->allocate_string(std::to_string(slider->SliderKnobColor.g).c_str())));
        knob_color_node->append_attribute(doc->allocate_attribute("b", doc->allocate_string(std::to_string(slider->SliderKnobColor.b).c_str())));
        knob_color_node->append_attribute(doc->allocate_attribute("a", doc->allocate_string(std::to_string(slider->SliderKnobColor.alpha).c_str())));
        view_node->append_node(knob_color_node);

        xml_node<>* tickmarks_color_node = doc->allocate_node(node_element, "tickmarks_color");
        tickmarks_color_node->append_attribute(doc->allocate_attribute("r", doc->allocate_string(std::to_string(slider->SliderKnobColor.r).c_str())));
        tickmarks_color_node->append_attribute(doc->allocate_attribute("g", doc->allocate_string(std::to_string(slider->SliderKnobColor.g).c_str())));
        tickmarks_color_node->append_attribute(doc->allocate_attribute("b", doc->allocate_string(std::to_string(slider->SliderKnobColor.b).c_str())));
        tickmarks_color_node->append_attribute(doc->allocate_attribute("a", doc->allocate_string(std::to_string(slider->SliderKnobColor.alpha).c_str())));
        view_node->append_node(tickmarks_color_node);
    }

    void AddWindowSettingsNode(Ref<xml_document<>>& doc, xml_node<>*& root_node, WindowSettings& window_settings)
    {
        xml_node<>* width_node = doc->allocate_node(node_element, "width");
        width_node->value(doc->allocate_string(std::to_string(window_settings.width).c_str()));

        xml_node<>* height_node = doc->allocate_node(node_element, "height");
        height_node->value(doc->allocate_string(std::to_string(window_settings.height).c_str()));

        xml_node<>* title_node = doc->allocate_node(node_element, "title");
        title_node->value(doc->allocate_string(window_settings.title));

        xml_node<>* color_node = doc->allocate_node(node_element, "color");
        color_node->append_attribute(doc->allocate_attribute("r", doc->allocate_string(std::to_string(window_settings.color.r).c_str())));
        color_node->append_attribute(doc->allocate_attribute("g", doc->allocate_string(std::to_string(window_settings.color.g).c_str())));
        color_node->append_attribute(doc->allocate_attribute("b", doc->allocate_string(std::to_string(window_settings.color.b).c_str())));
        color_node->append_attribute(doc->allocate_attribute("a", doc->allocate_string(std::to_string(window_settings.color.alpha).c_str())));

        xml_node<>* window_node = doc->allocate_node(node_element, "mcwindow");
        window_node->append_node(width_node);
        window_node->append_node(height_node);
        window_node->append_node(title_node);
        window_node->append_node(color_node);

        root_node->append_node(window_node);
    }

    //============================================================================================================================================//
    //============================================================================================================================================//
    //============================================================================================================================================//
    //============================================================================================================================================//
    //============================================================================================================================================//

    void ReadWindowSettingsNode(xml_node<>* node, MCLayout& layout)
    {

    }

    MCLayout ProjectGenerator::LoadMCProject(const std::string& path)
    {
        MCLayout layout;

        std::ifstream file(path);
        Ref<xml_document<>> document = MakeRef<xml_document<>>();

        std::vector<char> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        buffer.push_back('\0');
        document->parse<0>(&buffer[0]);
        
        xml_node<>* root_node = document->first_node();
        ReadWindowSettingsNode(root_node, layout);

        file.close();
        return layout;
    }
}
