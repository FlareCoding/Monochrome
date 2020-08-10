import sys
from xml.etree import ElementTree

class MCLayoutReader:
    def __init__(self, layout_file, class_name):
        self.__dom = ElementTree.parse(layout_file)
        self.__class_name = class_name
        self.__cppsource = "\n"
        self.__window_creation_source = "\n"
        self.__public_data_members_source = "\n"
        self.__private_data_members_source = "\n"
        self.__public_member_function_declarations_source = "\n"
        self.__private_member_function_declarations_source = "\n"
        self.__public_member_function_definitions_source = "\n"
        self.__private_member_function_definitions_source = "\n"
        self.__available_uiview_id = 0
        self.__available_label_id = 0
        self.__available_button_id = 0
        self.__available_checkbox_id = 0
        self.__available_slider_id = 0
    
    def __str2bool(self, v):
        return str(v).lower() in ("true", "1")
    
    def __get_uiview_id(self):
        self.__available_uiview_id += 1
        return "view" + str(self.__available_uiview_id)

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

    def __get_color_ctor(self, view_node, path) -> str:
        color_node = view_node.find(path)
        color_rgba = color_node.attrib['r'] + ", " + color_node.attrib['g'] + ", " + color_node.attrib['b'] + ", " + (color_node.attrib['a'] + "f")
        return "mc::Color(" + color_rgba + ")"

    def __read_UISlider_properties(self, node):
        props = {}

        knob_shape = "mc::Shape::Rectangle"
        if node.find('knob_shape').text == 'circle':
            knob_shape = "mc::Shape::Circle"

        props['knob_shape'] = knob_shape
        props['sliderbar_height']   = str(int(float(node.find('sliderbar_height').text)))
        props['max_value']          = str(int(float(node.find('max_value').text)))
        props['min_value']          = str(int(float(node.find('min_value').text)))
        props['value']              = str(int(float(node.find('value').text)))
        props['intervals']          = str(int(float(node.find('intervals').text)))
        
        props['knob_color'] = self.__get_color_ctor(node, 'knob_color')
        props['tickmarks_color'] = self.__get_color_ctor(node, 'tickmarks_color')

        props['visible_tickmarks'] = node.find('visible_tickmarks').text

        return props  

    def __generate_cpp_source_UISlider_properties(self, source, name, props, base_props):
        if base_props is not None:
            source += self.__generate_cpp_source_Basic_UIView_properties(name, base_props)

        source += "{}->SliderKnobShape = {};\n".format(name, props['knob_shape'])
        source += "{}->SliderBarHeight = {};\n".format(name, props['sliderbar_height'])
        source += "{}->MaxValue = {};\n".format(name, props['max_value'])
        source += "{}->MinValue = {};\n".format(name, props['min_value'])
        source += "{}->Value = {};\n".format(name, props['value'])
        source += "{}->Intervals = {};\n".format(name, props['intervals'])
        source += "{}->VisibleTickmarks = {};\n".format(name, props['visible_tickmarks'])
        source += "{}->SliderKnobColor = {};\n".format(name, props['knob_color'])
        source += "{}->TickmarksColor = {};\n".format(name, props['tickmarks_color'])

        return source

    def __read_UICheckbox_properties(self, node):
        props = {}
        props['checked'] = node.find('checked').text
        props['box_size'] = str(int(float(node.find('box_size').text)))
        props['label_margins'] = str(int(float(node.find('label_margins_size').text)))
        props['checkmark_color'] = self.__get_color_ctor(node, 'checkmark_color')
        props['checkedbox_color'] = self.__get_color_ctor(node, 'checkedbox_color')

        inner_label_node = node.find('uiview')
        props['label_props'] = self.__read_UILabel_properties(inner_label_node)

        return props

    def __generate_cpp_source_UICheckbox_properties(self, source, name, props, base_props):
        if base_props is not None:
            source += self.__generate_cpp_source_Basic_UIView_properties(name, base_props)

        source += "{}->Checked = {};\n".format(name, props['checked'])
        source += "{}->BoxSize = {};\n".format(name, props['box_size'])
        source += "{}->LabelMargins = {};\n".format(name, props['label_margins'])
        source += "{}->CheckmarkColor = {};\n".format(name, props['checkmark_color'])
        source += "{}->CheckedBoxColor = {};\n".format(name, props['checkedbox_color'])

        source += self.__generate_cpp_source_UILabel_properties("", "{}->Label".format(name), props['label_props'], None)
        return source

    def __read_UIButton_properties(self, node):
        props = {}
        props['filled'] = node.find('filled').text
        props['stroke'] = node.find('stroke').text
        props['hover_on_color'] = self.__get_color_ctor(node, 'hover_on_color')
        props['on_mouse_press_color'] = self.__get_color_ctor(node, 'on_mouse_press_color')

        inner_label_node = node.find('uiview')
        props['label_props'] = self.__read_UILabel_properties(inner_label_node)

        return props

    def __generate_cpp_source_UIButton_properties(self, source, name, props, base_props):
        if base_props is not None:
            source += self.__generate_cpp_source_Basic_UIView_properties(name, base_props)

        source += "{}->Filled = {};\n".format(name, props['filled'])
        source += "{}->Stroke = {};\n".format(name, props['stroke'])
        source += "{}->HoverOnColor = {};\n".format(name, props['hover_on_color'])
        source += "{}->OnMousePressColor = {};\n".format(name, props['on_mouse_press_color'])

        source += self.__generate_cpp_source_UILabel_properties("", "{}->Label".format(name), props['label_props'], None)
        return source

    def __read_UILabel_properties(self, node):
        props = {}
        props['text'] = node.find('text').text
        props['color'] = self.__get_color_ctor(node, 'color')
        props['use_widestring'] = node.find('use_widestring').text
        props['text_properties'] = {}
        props['text_properties']['font'] = node.find('text_properties/font').text
        props['text_properties']['font_size'] = node.find('text_properties/font_size').text
        props['text_properties']['alignment'] = node.find('text_properties/alignment').text
        props['text_properties']['style'] = node.find('text_properties/style').text
        props['text_properties']['wrapping'] = node.find('text_properties/wrapping').text

        return props

    def __generate_cpp_source_UILabel_properties(self, source, name, props, base_props):
        if base_props is not None:
            source += self.__generate_cpp_source_Basic_UIView_properties(name, base_props)

        source += "{}->Text = \"{}\";\n".format(name, props['text'])
        source += "{}->UseWidestringText = {};\n".format(name, props['use_widestring'])
        source += "{}->color = {};\n".format(name, props['color'])
        source += "{}->Properties.Font = \"{}\";\n".format(name, props['text_properties']['font'])
        source += "{}->Properties.FontSize = {};\n".format(name, props['text_properties']['font_size'])
        source += "{}->Properties.Allignment = {};\n".format(name, props['text_properties']['alignment'])
        source += "{}->Properties.Style = {};\n".format(name, props['text_properties']['style'])
        source += "{}->Properties.Wrapping = {};\n".format(name, props['text_properties']['wrapping'])

        return source

    def __read_Basic_UIView_properties(self, node):
        props = {}
        props['type'] = node.attrib['type']
        props['layer'] = {}
        props['layer']['color'] = self.__get_color_ctor(node, 'layer/color')
        props['layer']['frame'] = {}
        props['layer']['frame']['position'] = {}
        props['layer']['frame']['position']['x'] = str(int(float(node.find('layer/frame/position/x').text)))
        props['layer']['frame']['position']['y'] = str(int(float(node.find('layer/frame/position/y').text)))
        props['layer']['frame']['size'] = {}
        props['layer']['frame']['size']['width'] = str(int(float(node.find('layer/frame/size/width').text)))
        props['layer']['frame']['size']['height'] = str(int(float(node.find('layer/frame/size/height').text)))
        props['z_index'] = node.find('z_index').text
        props['corner_radius'] =  str(int(float(node.find('corner_radius').text)))
        props['visible'] = node.find('visible').text

        # Parse event handlers data
        event_handlers_node = node.find('event_handlers')
        if event_handlers_node is not None:
            props['event_handlers'] = []
            for handler_node in event_handlers_node.findall('handler'):
                handler = {}
                handler['type'] = handler_node.attrib['type']
                handler['generate_member_fn'] = self.__str2bool(handler_node.attrib['generate_member_fn'])
                if handler['generate_member_fn'] is True:
                    handler['member_fn'] = {}
                    handler['member_fn']['name'] = handler_node.find('member_fn/name').text
                    handler['member_fn']['visibility'] = handler_node.find('member_fn/visibility').text
                
                handler['return_status'] = handler_node.find('return_status').text
                props['event_handlers'].append(handler)

        return props

    def __generate_cpp_source_Basic_UIView_properties(self, name, props):
        source = ""
        xpos = props['layer']['frame']['position']['x']
        ypos = props['layer']['frame']['position']['y']
        width = props['layer']['frame']['size']['width']
        height = props['layer']['frame']['size']['height']
        
        source += "{}->layer.frame = mc::Frame(mc::Position{{ {}, {} }}, mc::Size{{ {}, {} }});\n".format(name, xpos, ypos, width, height)
        source += "{}->layer.color = {};\n".format(name, props['layer']['color'])
        source += "{}->SetZIndex({});\n".format(name, props['z_index'])
        source += "{}->CornerRadius = {};\n".format(name, props['corner_radius'])
        source += "{}->Visible = {};\n".format(name, props['visible'])

        if 'event_handlers' in props:
            for handler in props['event_handlers']:
                should_generate_member_fn = handler['generate_member_fn']

                source += "{}->AddEventHandler<mc::EventType::{}>([this](mc::Event& e, mc::UIView* sender) -> bool {{\n".format(name, handler['type'])
                if should_generate_member_fn is True:
                    parameters = "reinterpret_cast<mc::{0}Event&>(e), dynamic_cast<mc::{1}*>(sender)".format(handler['type'], props['type'])
                    source += "\t{}({});\n".format(handler['member_fn']['name'], parameters)
                else:
                    source += "\n"
                
                source += "\treturn {};\n}});\n".format(handler['return_status'])

                # Generation of member functions
                if should_generate_member_fn is True:
                    function_name = handler['member_fn']['name']
                    visibility = handler['member_fn']['visibility']
                    parameters = "mc::{}Event& e, mc::{}* sender".format(handler['type'], props['type'])
                    if visibility == 'public':
                        self.__public_member_function_declarations_source += "void {}({});\n".format(function_name, parameters)
                        self.__public_member_function_definitions_source += "void {}::{}({})\n{{\n\n}}\n\n".format(self.__class_name, function_name, parameters)
                    else:
                        self.__private_member_function_declarations_source += "void {}({});\n".format(function_name, parameters)
                        self.__private_member_function_definitions_source += "void {}::{}({})\n{{\n\n}}\n\n".format(self.__class_name, function_name, parameters)

        return source

    def __get_element_props_and_name(self, view_node, widget_type):
        props = {}
        name = ""

        # If a variable is public or private, use the given name
        # and add the variable declaration as a data member.
        if 'visibility' in view_node.attrib:
            visibility = view_node.attrib['visibility']
            if visibility == 'public':
                name = view_node.attrib['name']
                self.__public_data_members_source += "mc::Ref<mc::{0}> {1};\n".format(widget_type, name)
            elif visibility == 'private':
                name = view_node.attrib['name']
                self.__private_data_members_source += "mc::Ref<mc::{0}> {1};\n".format(widget_type, name)

        if widget_type == 'UIView':
            if not name:
                name = self.__get_uiview_id()
        elif widget_type == 'UILabel':
            props = self.__read_UILabel_properties(view_node)
            if not name:
                name = self.__get_label_id()
        elif widget_type == 'UIButton':
            props = self.__read_UIButton_properties(view_node)
            if not name:
                name = self.__get_button_id()
        elif widget_type == 'UICheckbox':
            props = self.__read_UICheckbox_properties(view_node)
            if not name:
                name = self.__get_checkbox_id()
        elif widget_type == 'UISlider':
            props = self.__read_UISlider_properties(view_node)
            if not name:
                name = self.__get_slider_id()

        return props, name

    def __parse_uiview_node(self, view_node, parent_name):
        widget_type = view_node.attrib['type']
        name = ""
        base_props = self.__read_Basic_UIView_properties(view_node)

        props, name = self.__get_element_props_and_name(view_node, widget_type)

        decl = ""
        source = ""

        if 'visibility' in view_node.attrib:
            if view_node.attrib['visibility'] == 'local':
                decl = "mc::Ref<mc::{0}> {1} = mc::MakeRef<mc::{0}>();\n".format(widget_type, name)
            else:
                decl = "{1} = mc::MakeRef<mc::{0}>();\n".format(widget_type, name)

        if widget_type == 'UIView':
            source += decl
            source += self.__generate_cpp_source_Basic_UIView_properties(name, base_props)
        elif widget_type == 'UILabel':
            source += self.__generate_cpp_source_UILabel_properties(decl, name, props, base_props)
        elif widget_type == 'UIButton':
            source += self.__generate_cpp_source_UIButton_properties(decl, name, props, base_props)
        elif widget_type == 'UICheckbox':
            source += self.__generate_cpp_source_UICheckbox_properties(decl, name, props, base_props)
        elif widget_type == 'UISlider':
            source += self.__generate_cpp_source_UISlider_properties(decl, name, props, base_props)

        self.__cppsource += source
       
        if not parent_name:
            self.__cppsource += "m_Window->AddView({});\n".format(name)
        else:
            self.__cppsource += "{}->AddSubview({});\n".format(parent_name, name)

        self.__cppsource += "\n"
        
        if self.__should_parse_subviews(widget_type):
            for subview in view_node.findall('uiview'):
                self.__parse_uiview_node(subview, name)

    def get_cpp_source_string(self) -> str:
        return self.__cppsource

    def get_window_creation_source_string(self) -> str:
        return self.__window_creation_source

    def get_public_data_members_source_string(self) -> str:
        return self.__public_data_members_source

    def get_private_data_members_source_string(self) -> str:
        return self.__private_data_members_source

    def get_public_member_function_declarations_source_string(self) -> str:
        return self.__public_member_function_declarations_source

    def get_private_member_function_declarations_source_string(self) -> str:
        return self.__private_member_function_declarations_source

    def get_public_member_function_definitions_source_string(self) -> str:
        return self.__public_member_function_definitions_source

    def get_private_member_function_definitions_source_string(self) -> str:
        return self.__private_member_function_definitions_source

    def generate_cpp_source(self):
        self.__generate_window_creation_cpp_source()

        views = self.__dom.findall('uiview')

        for view in views:
            self.__parse_uiview_node(view, None)

        self.__cppsource = self.__cppsource.replace('\n', '\n\t')
        self.__window_creation_source = self.__window_creation_source.replace('\n', '\n\t')
        self.__public_data_members_source = self.__public_data_members_source.replace('\n', '\n\t')
        self.__private_data_members_source = self.__private_data_members_source.replace('\n', '\n\t')
        self.__public_member_function_declarations_source = self.__public_member_function_declarations_source.replace('\n', '\n\t')
        self.__private_member_function_declarations_source = self.__private_member_function_declarations_source.replace('\n', '\n\t')
