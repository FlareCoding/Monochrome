import sys
from xml.etree import ElementTree

class MCLayoutReader:
    def __init__(self, layout_file):
        self.__dom = ElementTree.parse(layout_file)
        self.__cppsource = ""
        self.__window_creation_source = ""
        self.__available_label_id = 0
        self.__available_button_id = 0
        self.__available_checkbox_id = 0
        self.__available_slider_id = 0
    
    def __get_label_id(self):
        self.__available_label_id += 1
        return "label" + str(self.__available_label_id)

    def __get_button_id(self):
        self.__available_button_id += 1
        return "button" + str(self.__available_button_id)

    def __get_checkbox_id(self):
        self.__available_checkbox_id += 1
        return "checkbox" + str(self.__available_checkbox_id)

    def __get_slider_id(self):
        self.__available_slider_id += 1
        return "slider" + str(self.__available_slider_id)

    def __should_parse_subviews(self, widget_type):
        if widget_type == 'UIButton':
            return False
        if widget_type == 'UICheckbox':
            return False

        return True

    def get_cpp_source_string(self) -> str:
        return self.__cppsource

    def get_window_creation_source_string(self) -> str:
        return self.__window_creation_source

    def __generate_window_creation_cpp_source(self):
        window_node = self.__dom.find('mcwindow')
        width = window_node.find('width').text
        height = window_node.find('height').text
        title = window_node.find('title').text
        color = self.__get_color_ctor(window_node, 'color')

        self.__window_creation_source += "m_Window = mc::UIWindow::Create(mc::WindowStyle::Modern, "
        self.__window_creation_source += width + ", "
        self.__window_creation_source += height + ", "
        self.__window_creation_source += "\"" + title + "\");\n"
        self.__window_creation_source += "m_Window->SetBackgroundColor(" + color + ");\n"
        self.__window_creation_source += "m_Window->SetModernWindowButtonsColor(" + color + ");\n"

    def generate_cpp_source(self):
        self.__generate_window_creation_cpp_source()

        views = self.__dom.findall('uiview')

        for view in views:
            self.__parse_uiview_node(view)

    def __get_color_ctor(self, view_node, path) -> str:
        color_node = view_node.find(path)
        color_rgba = color_node.attrib['r'] + ", " + color_node.attrib['g'] + ", " + color_node.attrib['b'] + ", " + (color_node.attrib['a'] + "f")
        return "mc::Color(" + color_rgba + ")"

    def __parse_uiview_node(self, view_node):
        widget_type = view_node.attrib['type']
        
        element_source, element_name = self.__get_UIView_cpp_source(widget_type, view_node)

        self.__cppsource += element_source

        if self.__should_parse_subviews(widget_type):
            for subview in view_node.findall('uiview'):
                self.__parse_uiview_node(subview)

        self.__cppsource += "m_Window->AddView(" + element_name + ");\n\n"

    def __get_UIView_cpp_source(self, widget_type, view_node):

        if widget_type == 'UILabel':
            source, name = self.__get_UILabel_cpp_source(view_node)
        elif widget_type == 'UIButton':
            source, name = self.__get_UIButton_cpp_source(view_node)
        elif widget_type == 'UICheckbox':
            source, name = self.__get_UICheckbox_cpp_source(view_node)
        elif widget_type == 'UISlider':
            source, name = self.__get_UISlider_cpp_source(view_node)
        
        return source, name

    def __get_Basic_UIView_cpp_source(self, view_node, name) :
        source = ""

        xpos = view_node.find('layer/frame/position/x').text
        ypos = view_node.find('layer/frame/position/y').text
        width = view_node.find('layer/frame/size/width').text
        height = view_node.find('layer/frame/size/height').text

        position_decl = "mc::Position{ " + str(int(float(xpos))) + ", " + str(int(float(ypos))) + " }"
        size_decl = "mc::Size{ " + str(int(float(width))) + ", " + str(int(float(height))) + " }"

        source += name + "->layer.frame = mc::Frame(" + position_decl + ", " + size_decl + ");" + "\n"

        source += name + "->layer.color = " + self.__get_color_ctor(view_node, 'layer/color') + ";" + "\n"
        source += name + "->SetZIndex(" + view_node.find('z_index').text + ");" + "\n"
        source += name + "->Visible = " + view_node.find('visible').text + ";" + "\n"
        source += name + "->CornerRadius = " + (view_node.find('corner_radius').text + "f") + ";" + "\n"

        return source

    def __get_UILabel_cpp_source(self, view_node):
        name = self.__get_label_id()
        source = "mc::Ref<mc::UILabel> " + name + " = mc::MakeRef<mc::UILabel>();\n"
        source += self.__get_Basic_UIView_cpp_source(view_node, name)

        # Text
        source += name + "->Text = \"" + view_node.find('text').text + "\";" + "\n"

        # Should Use Widestring Flag
        source += name + "->UseWidestringText = " + view_node.find('use_widestring').text + ";" + "\n"

        # Text Color
        source += name + "->color = " + self.__get_color_ctor(view_node, 'color') + ";" + "\n"

        return source, name

    def __get_UIButton_cpp_source(self, view_node):
        name = self.__get_button_id()
        source = "mc::Ref<mc::UIButton> " + name + " = mc::MakeRef<mc::UIButton>();\n"
        source += self.__get_Basic_UIView_cpp_source(view_node, name)

        # Filled Flag
        source += name + "->Filled = " + view_node.find('filled').text + ";" + "\n"

        # Stroke
        source += name + "->Stroke = " + view_node.find('stroke').text + ";" + "\n"

        # HoverOn Color
        source += name + "->HoverOnColor = " + self.__get_color_ctor(view_node, 'hover_on_color') + ";" + "\n"

        # OnMousePress Color
        source += name + "->OnMousePressColor = " + self.__get_color_ctor(view_node, 'on_mouse_press_color') + ";" + "\n"

        return source, name

    def __get_UICheckbox_cpp_source(self, view_node):
        name = self.__get_checkbox_id()
        source = "mc::Ref<mc::UICheckbox> " + name + " = mc::MakeRef<mc::UICheckbox>();\n"
        source += self.__get_Basic_UIView_cpp_source(view_node, name)

        # Checked Flag
        source += name + "->Checked = " + view_node.find('checked').text + ";" + "\n"

        # Box Size
        source += name + "->BoxSize = " + view_node.find('box_size').text + ";" + "\n"

        # Label Margins
        source += name + "->LabelMargins = " + view_node.find('label_margins_size').text + ";" + "\n"

        # Checkmark Color
        source += name + "->CheckmarkColor = " + self.__get_color_ctor(view_node, 'checkmark_color') + ";" + "\n"

        # CheckedBox Color
        source += name + "->CheckedBoxColor = " + self.__get_color_ctor(view_node, 'checkedbox_color') + ";" + "\n"

        return source, name

    def __get_UISlider_cpp_source(self, view_node):
        name = self.__get_slider_id()
        source = "mc::Ref<mc::UISlider> " + name + " = mc::MakeRef<mc::UISlider>();\n"
        source += self.__get_Basic_UIView_cpp_source(view_node, name)

        # Checked Flag
        knob_shape = "mc::Shape::Rectangle"
        if view_node.find('knob_shape').text == 'circle':
            knob_shape = 'mc::Shape::Circle'

        source += name + "->SliderKnobShape = " + knob_shape + ";" + "\n"

        # Slider Bar Height
        source += name + "->SliderBarHeight = " + view_node.find('sliderbar_height').text + ";" + "\n"

        # Max/Min/Def Value
        source += name + "->MaxValue = " + view_node.find('max_value').text + ";" + "\n"
        source += name + "->MinValue = " + view_node.find('min_value').text + ";" + "\n"
        source += name + "->Value = " + view_node.find('value').text + ";" + "\n"

        # Intervals
        source += name + "->Intervals = " + view_node.find('intervals').text + ";" + "\n"

        # Visible Tickmarks
        source += name + "->VisibleTickmarks = " + view_node.find('visible_tickmarks').text + ";" + "\n"

        # Knob Color
        source += name + "->SliderKnobColor  = " + self.__get_color_ctor(view_node, 'knob_color') + ";" + "\n"

        # Tickmarks Color
        source += name + "->TickmarksColor = " + self.__get_color_ctor(view_node, 'tickmarks_color') + ";" + "\n"

        return source, name
