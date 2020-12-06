#include "WidgetProperties.h"
#include <sstream>

WidgetProperties::PropertiesList_t WidgetProperties::WidgetPropertiesList;

#define UIVIEW_BASE_PROPERTIES	VisibleFieldProperty, ZIndexProperty, \
								PosXProperty, PosYProperty, \
								WidthProperty, HeightProperty, \
								CornerRadiusProperty, LayerColorProperty

namespace WidgetProperties
{
	void CreatePropertiesList()
	{
#pragma region Base Properties

		Property VisibleFieldProperty;
		VisibleFieldProperty.type = Flags::TOGGLABLE;
		VisibleFieldProperty.name = "Visible";
		VisibleFieldProperty.applier_fn = [](UIView* target, const std::string& value) {
			target->Visible = (bool)std::stoi(value);
		};
		VisibleFieldProperty.getter_fn = [](UIView* target) {
			return std::to_string((uint32_t)target->Visible);
		};

		Property ZIndexProperty;
		ZIndexProperty.type = Flags::DATA_ENTRY;
		ZIndexProperty.name = "Z-Index";
		ZIndexProperty.applier_fn = [](UIView* target, const std::string& value) {
			try {
				target->SetZIndex((uint32_t)std::stoi(value));
			}
			catch (...) {}
		};
		ZIndexProperty.getter_fn = [](UIView* target) {
			return std::to_string(target->GetZIndex());
		};

		Property PosXProperty;
		PosXProperty.type = Flags::DATA_ENTRY;
		PosXProperty.name = "Position.x";
		PosXProperty.applier_fn = [](UIView* target, const std::string& value) {
			try {
				target->layer.frame.position.x = std::stof(value);
			}
			catch (...) {}
		};
		PosXProperty.getter_fn = [](UIView* target) {
			return std::to_string((uint32_t)target->layer.frame.position.x);
		};

		Property PosYProperty;
		PosYProperty.type = Flags::DATA_ENTRY;
		PosYProperty.name = "Position.y";
		PosYProperty.applier_fn = [](UIView* target, const std::string& value) {
			try {
				target->layer.frame.position.y = std::stof(value);
			}
			catch (...) {}
		};
		PosYProperty.getter_fn = [](UIView* target) {
			return std::to_string((uint32_t)target->layer.frame.position.y);
		};

		Property WidthProperty;
		WidthProperty.type = Flags::DATA_ENTRY;
		WidthProperty.name = "Width";
		WidthProperty.applier_fn = [](UIView* target, const std::string& value) {
			try {
				target->layer.frame.size.width = std::stof(value);
			}
			catch (...) {}
		};
		WidthProperty.getter_fn = [](UIView* target) {
			return std::to_string((uint32_t)target->layer.frame.size.width);
		};

		Property HeightProperty;
		HeightProperty.type = Flags::DATA_ENTRY;
		HeightProperty.name = "Height";
		HeightProperty.applier_fn = [](UIView* target, const std::string& value) {
			try {
				target->layer.frame.size.height = std::stof(value);
			}
			catch (...) {}
		};
		HeightProperty.getter_fn = [](UIView* target) {
			return std::to_string((uint32_t)target->layer.frame.size.height);
		};

		Property CornerRadiusProperty;
		CornerRadiusProperty.type = Flags::DATA_ENTRY;
		CornerRadiusProperty.name = "Corner Radius";
		CornerRadiusProperty.applier_fn = [](UIView* target, const std::string& value) {
			try {
				target->CornerRadius = std::stof(value);
			}
			catch (...) {}
		};
		CornerRadiusProperty.getter_fn = [](UIView* target) {
			return std::to_string((uint32_t)target->CornerRadius);
		};

		Property LayerColorProperty;
		LayerColorProperty.type = Flags::DATA_ENTRY;
		LayerColorProperty.name = "Color";
		LayerColorProperty.applier_fn = [](UIView* target, const std::string& value) {
			try {
				target->layer.color = Color::FromRGBString(value.c_str());
			}
			catch (...) {}
		};
		LayerColorProperty.getter_fn = [](UIView* target) {
			return target->layer.color.ToString();
		};

#pragma endregion

#pragma region Label Properties

		Property LabelTextProperty;
		LabelTextProperty.type = Flags::DATA_ENTRY;
		LabelTextProperty.name = "Text";
		LabelTextProperty.applier_fn = [](UIView* target, const std::string& value) {
			try {
				reinterpret_cast<UILabel*>(target)->Text = value;
			}
			catch (...) {}
		};
		LabelTextProperty.getter_fn = [](UIView* target) {
			return reinterpret_cast<UILabel*>(target)->Text;
		};

		Property LabelTextColorProperty;
		LabelTextColorProperty.type = Flags::DATA_ENTRY;
		LabelTextColorProperty.name = "Text Color";
		LabelTextColorProperty.applier_fn = [](UIView* target, const std::string& value) {
			try {
				reinterpret_cast<UILabel*>(target)->color = Color::FromRGBString(value.c_str());
			}
			catch (...) {}
		};
		LabelTextColorProperty.getter_fn = [](UIView* target) {
			return reinterpret_cast<UILabel*>(target)->color.ToString();
		};

		Property LabelFontProperty;
		LabelFontProperty.type = Flags::DATA_ENTRY;
		LabelFontProperty.name = "Font";
		LabelFontProperty.applier_fn = [](UIView* target, const std::string& value) {
			try {
				reinterpret_cast<UILabel*>(target)->Properties.Font = value;
			}
			catch (...) {}
		};
		LabelFontProperty.getter_fn = [](UIView* target) {
			return reinterpret_cast<UILabel*>(target)->Properties.Font;
		};

		Property LabelFontSizeProperty;
		LabelFontSizeProperty.type = Flags::DATA_ENTRY;
		LabelFontSizeProperty.name = "Font Size";
		LabelFontSizeProperty.applier_fn = [](UIView* target, const std::string& value) {
			try {
				reinterpret_cast<UILabel*>(target)->Properties.FontSize = std::stoi(value);
			}
			catch (...) {}
		};
		LabelFontSizeProperty.getter_fn = [](UIView* target) {
			return std::to_string(reinterpret_cast<UILabel*>(target)->Properties.FontSize);
		};

		Property LabelTextAlignmentProperty;
		LabelTextAlignmentProperty.type = Flags::SELECTABLE;
		LabelTextAlignmentProperty.name = "Alignment";
		LabelTextAlignmentProperty.items = { "Leading", "Centered", "Trailing" };
		LabelTextAlignmentProperty.applier_fn = [](UIView* target, const std::string& value) {
			reinterpret_cast<UILabel*>(target)->Properties.Alignment = (TextAlignment)std::stoi(value);
		};
		LabelTextAlignmentProperty.getter_fn = [](UIView* target) {
			return std::to_string((uint32_t)reinterpret_cast<UILabel*>(target)->Properties.Alignment);
		};

		Property LabelTextVerticalAlignmentProperty;
		LabelTextVerticalAlignmentProperty.type = Flags::SELECTABLE;
		LabelTextVerticalAlignmentProperty.name = "Vertical Alignment";
		LabelTextVerticalAlignmentProperty.items = { "Leading", "Centered", "Trailing" };
		LabelTextVerticalAlignmentProperty.applier_fn = [](UIView* target, const std::string& value) {
			reinterpret_cast<UILabel*>(target)->Properties.VerticalAlignment = (TextAlignment)std::stoi(value);
		};
		LabelTextVerticalAlignmentProperty.getter_fn = [](UIView* target) {
			return std::to_string((uint32_t)reinterpret_cast<UILabel*>(target)->Properties.VerticalAlignment);
		};

		Property LabelTextStyleProperty;
		LabelTextStyleProperty.type = Flags::SELECTABLE;
		LabelTextStyleProperty.name = "Text Style";
		LabelTextStyleProperty.items = { "Default", "Light", "Italic", "Bold", "Bold Italic", "Semibold", "Semibold Italic" };
		LabelTextStyleProperty.applier_fn = [](UIView* target, const std::string& value) {
			reinterpret_cast<UILabel*>(target)->Properties.Style = (TextStyle)std::stoi(value);
		};
		LabelTextStyleProperty.getter_fn = [](UIView* target) {
			return std::to_string((uint32_t)reinterpret_cast<UILabel*>(target)->Properties.Style);
		};

		Property LabelWordWrappingProperty;
		LabelWordWrappingProperty.type = Flags::SELECTABLE;
		LabelWordWrappingProperty.name = "Word Wrapping";
		LabelWordWrappingProperty.items = { "Normal", "No Wrap", "Char Wrap", "Word Wrap", "Emergency Break" };
		LabelWordWrappingProperty.applier_fn = [](UIView* target, const std::string& value) {
			reinterpret_cast<UILabel*>(target)->Properties.Wrapping = (WordWrapping)std::stoi(value);
		};
		LabelWordWrappingProperty.getter_fn = [](UIView* target) {
			return std::to_string((uint32_t)reinterpret_cast<UILabel*>(target)->Properties.Wrapping);
		};


#pragma endregion

#pragma region Button Properties

		Property ButtonFilledProperty;
		ButtonFilledProperty.type = Flags::TOGGLABLE;
		ButtonFilledProperty.name = "Filled";
		ButtonFilledProperty.applier_fn = [](UIView* target, const std::string& value) {
			reinterpret_cast<UIButton*>(target)->Filled = (bool)std::stoi(value);
		};
		ButtonFilledProperty.getter_fn = [](UIView* target) {
			return std::to_string((uint32_t)reinterpret_cast<UIButton*>(target)->Filled);
		};

		Property ButtonStrokeProperty;
		ButtonStrokeProperty.type = Flags::DATA_ENTRY;
		ButtonStrokeProperty.name = "Stroke";
		ButtonStrokeProperty.applier_fn = [](UIView* target, const std::string& value) {
			try {
				reinterpret_cast<UIButton*>(target)->Stroke = std::stof(value);
			}
			catch (...) {}
		};
		ButtonStrokeProperty.getter_fn = [](UIView* target) {
			return std::to_string((uint32_t)reinterpret_cast<UIButton*>(target)->Stroke);
		};

		Property HoverOnColorProperty;
		HoverOnColorProperty.type = Flags::DATA_ENTRY;
		HoverOnColorProperty.name = "Hover Color";
		HoverOnColorProperty.applier_fn = [](UIView* target, const std::string& value) {
			try {
				reinterpret_cast<UIButton*>(target)->HoverOnColor = Color::FromRGBString(value.c_str());
			}
			catch (...) {}
		};
		HoverOnColorProperty.getter_fn = [](UIView* target) {
			return reinterpret_cast<UIButton*>(target)->HoverOnColor.ToString();
		};

		Property OnPressColorProperty;
		OnPressColorProperty.type = Flags::DATA_ENTRY;
		OnPressColorProperty.name = "OnPress Color";
		OnPressColorProperty.applier_fn = [](UIView* target, const std::string& value) {
			try {
				reinterpret_cast<UIButton*>(target)->OnMousePressColor = Color::FromRGBString(value.c_str());
			}
			catch (...) {}
		};
		OnPressColorProperty.getter_fn = [](UIView* target) {
			return reinterpret_cast<UIButton*>(target)->OnMousePressColor.ToString();
		};

		Property ButtonTextProperty;
		ButtonTextProperty.type = Flags::DATA_ENTRY;
		ButtonTextProperty.name = "Text";
		ButtonTextProperty.applier_fn = [](UIView* target, const std::string& value) {
			try {
				reinterpret_cast<UIButton*>(target)->Label->Text = value;
			}
			catch (...) {}
		};
		ButtonTextProperty.getter_fn = [](UIView* target) {
			return reinterpret_cast<UIButton*>(target)->Label->Text;
		};

		Property ButtonTextColorProperty;
		ButtonTextColorProperty.type = Flags::DATA_ENTRY;
		ButtonTextColorProperty.name = "Text Color";
		ButtonTextColorProperty.applier_fn = [](UIView* target, const std::string& value) {
			try {
				reinterpret_cast<UIButton*>(target)->Label->color = Color::FromRGBString(value.c_str());
			}
			catch (...) {}
		};
		ButtonTextColorProperty.getter_fn = [](UIView* target) {
			return reinterpret_cast<UIButton*>(target)->Label->color.ToString();
		};

		Property ButtonFontProperty;
		ButtonFontProperty.type = Flags::DATA_ENTRY;
		ButtonFontProperty.name = "Font";
		ButtonFontProperty.applier_fn = [](UIView* target, const std::string& value) {
			try {
				reinterpret_cast<UIButton*>(target)->Label->Properties.Font = value;
			}
			catch (...) {}
		};
		ButtonFontProperty.getter_fn = [](UIView* target) {
			return reinterpret_cast<UIButton*>(target)->Label->Properties.Font;
		};

		Property ButtonFontSizeProperty;
		ButtonFontSizeProperty.type = Flags::DATA_ENTRY;
		ButtonFontSizeProperty.name = "Font Size";
		ButtonFontSizeProperty.applier_fn = [](UIView* target, const std::string& value) {
			try {
				reinterpret_cast<UIButton*>(target)->Label->Properties.FontSize = std::stoi(value);
			}
			catch (...) {}
		};
		ButtonFontSizeProperty.getter_fn = [](UIView* target) {
			return std::to_string(reinterpret_cast<UIButton*>(target)->Label->Properties.FontSize);
		};

		Property ButtonTextAlignmentProperty;
		ButtonTextAlignmentProperty.type = Flags::SELECTABLE;
		ButtonTextAlignmentProperty.name = "Alignment";
		ButtonTextAlignmentProperty.items = { "Leading", "Centered", "Trailing" };
		ButtonTextAlignmentProperty.applier_fn = [](UIView* target, const std::string& value) {
			reinterpret_cast<UIButton*>(target)->Label->Properties.Alignment = (TextAlignment)std::stoi(value);
		};
		ButtonTextAlignmentProperty.getter_fn = [](UIView* target) {
			return std::to_string((uint32_t)reinterpret_cast<UIButton*>(target)->Label->Properties.Alignment);
		};

		Property ButtonTextVerticalAlignmentProperty;
		ButtonTextVerticalAlignmentProperty.type = Flags::SELECTABLE;
		ButtonTextVerticalAlignmentProperty.name = "Vertical Alignment";
		ButtonTextVerticalAlignmentProperty.items = { "Leading", "Centered", "Trailing" };
		ButtonTextVerticalAlignmentProperty.applier_fn = [](UIView* target, const std::string& value) {
			reinterpret_cast<UIButton*>(target)->Label->Properties.VerticalAlignment = (TextAlignment)std::stoi(value);
		};
		ButtonTextVerticalAlignmentProperty.getter_fn = [](UIView* target) {
			return std::to_string((uint32_t)reinterpret_cast<UIButton*>(target)->Label->Properties.VerticalAlignment);
		};

		Property ButtonTextStyleProperty;
		ButtonTextStyleProperty.type = Flags::SELECTABLE;
		ButtonTextStyleProperty.name = "Text Style";
		ButtonTextStyleProperty.items = { "Default", "Light", "Italic", "Bold", "Bold Italic", "Semibold", "Semibold Italic" };
		ButtonTextStyleProperty.applier_fn = [](UIView* target, const std::string& value) {
			reinterpret_cast<UIButton*>(target)->Label->Properties.Style = (TextStyle)std::stoi(value);
		};
		ButtonTextStyleProperty.getter_fn = [](UIView* target) {
			return std::to_string((uint32_t)reinterpret_cast<UIButton*>(target)->Label->Properties.Style);
		};

		Property ButtonWordWrappingProperty;
		ButtonWordWrappingProperty.type = Flags::SELECTABLE;
		ButtonWordWrappingProperty.name = "Word Wrapping";
		ButtonWordWrappingProperty.items = { "Normal", "No Wrap", "Char Wrap", "Word Wrap", "Emergency Break" };
		ButtonWordWrappingProperty.applier_fn = [](UIView* target, const std::string& value) {
			reinterpret_cast<UIButton*>(target)->Label->Properties.Wrapping = (WordWrapping)std::stoi(value);
		};
		ButtonWordWrappingProperty.getter_fn = [](UIView* target) {
			return std::to_string((uint32_t)reinterpret_cast<UIButton*>(target)->Label->Properties.Wrapping);
		};

#pragma endregion

#pragma region Checkbox Properties

		Property CheckboxBoxSizeProperty;
		CheckboxBoxSizeProperty.type = Flags::DATA_ENTRY;
		CheckboxBoxSizeProperty.name = "Box Size";
		CheckboxBoxSizeProperty.applier_fn = [](UIView* target, const std::string& value) {
			try {
				reinterpret_cast<UICheckbox*>(target)->BoxSize = std::stof(value);
			}
			catch (...) {}
		};
		CheckboxBoxSizeProperty.getter_fn = [](UIView* target) {
			return std::to_string((uint32_t)reinterpret_cast<UICheckbox*>(target)->BoxSize);
		};

		Property CheckboxLabelMarginsProperty;
		CheckboxLabelMarginsProperty.type = Flags::DATA_ENTRY;
		CheckboxLabelMarginsProperty.name = "Label Margins";
		CheckboxLabelMarginsProperty.applier_fn = [](UIView* target, const std::string& value) {
			try {
				reinterpret_cast<UICheckbox*>(target)->LabelMargins = std::stof(value);
			}
			catch (...) {}
		};
		CheckboxLabelMarginsProperty.getter_fn = [](UIView* target) {
			return std::to_string((uint32_t)reinterpret_cast<UICheckbox*>(target)->LabelMargins);
		};

		Property CheckboxCheckmarkColorProperty;
		CheckboxCheckmarkColorProperty.type = Flags::DATA_ENTRY;
		CheckboxCheckmarkColorProperty.name = "Checkmark Color";
		CheckboxCheckmarkColorProperty.applier_fn = [](UIView* target, const std::string& value) {
			try {
				reinterpret_cast<UICheckbox*>(target)->CheckmarkColor = Color::FromRGBString(value.c_str());
			}
			catch (...) {}
		};
		CheckboxCheckmarkColorProperty.getter_fn = [](UIView* target) {
			return reinterpret_cast<UICheckbox*>(target)->CheckmarkColor.ToString();
		};

		Property CheckboxCheckedBoxColorProperty;
		CheckboxCheckedBoxColorProperty.type = Flags::DATA_ENTRY;
		CheckboxCheckedBoxColorProperty.name = "Checked Color";
		CheckboxCheckedBoxColorProperty.applier_fn = [](UIView* target, const std::string& value) {
			try {
				reinterpret_cast<UICheckbox*>(target)->CheckedBoxColor = Color::FromRGBString(value.c_str());
			}
			catch (...) {}
		};
		CheckboxCheckedBoxColorProperty.getter_fn = [](UIView* target) {
			return reinterpret_cast<UICheckbox*>(target)->CheckedBoxColor.ToString();
		};

		Property CheckboxCheckedProperty;
		CheckboxCheckedProperty.type = Flags::TOGGLABLE;
		CheckboxCheckedProperty.name = "Checked";
		CheckboxCheckedProperty.applier_fn = [](UIView* target, const std::string& value) {
			reinterpret_cast<UICheckbox*>(target)->Checked = (bool)std::stoi(value);
		};
		CheckboxCheckedProperty.getter_fn = [](UIView* target) {
			return std::to_string((uint32_t)reinterpret_cast<UICheckbox*>(target)->Checked);
		};

		Property CheckboxTextProperty;
		CheckboxTextProperty.type = Flags::DATA_ENTRY;
		CheckboxTextProperty.name = "Text";
		CheckboxTextProperty.applier_fn = [](UIView* target, const std::string& value) {
			try {
				reinterpret_cast<UICheckbox*>(target)->Label->Text = value;
			}
			catch (...) {}
		};
		CheckboxTextProperty.getter_fn = [](UIView* target) {
			return reinterpret_cast<UICheckbox*>(target)->Label->Text;
		};

		Property CheckboxTextColorProperty;
		CheckboxTextColorProperty.type = Flags::DATA_ENTRY;
		CheckboxTextColorProperty.name = "Text Color";
		CheckboxTextColorProperty.applier_fn = [](UIView* target, const std::string& value) {
			try {
				reinterpret_cast<UICheckbox*>(target)->Label->color = Color::FromRGBString(value.c_str());
			}
			catch (...) {}
		};
		CheckboxTextColorProperty.getter_fn = [](UIView* target) {
			return reinterpret_cast<UICheckbox*>(target)->Label->color.ToString();
		};

		Property CheckboxFontProperty;
		CheckboxFontProperty.type = Flags::DATA_ENTRY;
		CheckboxFontProperty.name = "Font";
		CheckboxFontProperty.applier_fn = [](UIView* target, const std::string& value) {
			try {
				reinterpret_cast<UICheckbox*>(target)->Label->Properties.Font = value;
			}
			catch (...) {}
		};
		CheckboxFontProperty.getter_fn = [](UIView* target) {
			return reinterpret_cast<UICheckbox*>(target)->Label->Properties.Font;
		};

		Property CheckboxFontSizeProperty;
		CheckboxFontSizeProperty.type = Flags::DATA_ENTRY;
		CheckboxFontSizeProperty.name = "Font Size";
		CheckboxFontSizeProperty.applier_fn = [](UIView* target, const std::string& value) {
			try {
				reinterpret_cast<UICheckbox*>(target)->Label->Properties.FontSize = std::stoi(value);
			}
			catch (...) {}
		};
		CheckboxFontSizeProperty.getter_fn = [](UIView* target) {
			return std::to_string(reinterpret_cast<UICheckbox*>(target)->Label->Properties.FontSize);
		};

		Property CheckboxTextAlignmentProperty;
		CheckboxTextAlignmentProperty.type = Flags::SELECTABLE;
		CheckboxTextAlignmentProperty.name = "Alignment";
		CheckboxTextAlignmentProperty.items = { "Leading", "Centered", "Trailing" };
		CheckboxTextAlignmentProperty.applier_fn = [](UIView* target, const std::string& value) {
			reinterpret_cast<UICheckbox*>(target)->Label->Properties.Alignment = (TextAlignment)std::stoi(value);
		};
		CheckboxTextAlignmentProperty.getter_fn = [](UIView* target) {
			return std::to_string((uint32_t)reinterpret_cast<UICheckbox*>(target)->Label->Properties.Alignment);
		};

		Property CheckboxTextVerticalAlignmentProperty;
		CheckboxTextVerticalAlignmentProperty.type = Flags::SELECTABLE;
		CheckboxTextVerticalAlignmentProperty.name = "Vertical Alignment";
		CheckboxTextVerticalAlignmentProperty.items = { "Leading", "Centered", "Trailing" };
		CheckboxTextVerticalAlignmentProperty.applier_fn = [](UIView* target, const std::string& value) {
			reinterpret_cast<UICheckbox*>(target)->Label->Properties.VerticalAlignment = (TextAlignment)std::stoi(value);
		};
		CheckboxTextVerticalAlignmentProperty.getter_fn = [](UIView* target) {
			return std::to_string((uint32_t)reinterpret_cast<UICheckbox*>(target)->Label->Properties.VerticalAlignment);
		};

		Property CheckboxTextStyleProperty;
		CheckboxTextStyleProperty.type = Flags::SELECTABLE;
		CheckboxTextStyleProperty.name = "Text Style";
		CheckboxTextStyleProperty.items = { "Default", "Light", "Italic", "Bold", "Bold Italic", "Semibold", "Semibold Italic" };
		CheckboxTextStyleProperty.applier_fn = [](UIView* target, const std::string& value) {
			reinterpret_cast<UICheckbox*>(target)->Label->Properties.Style = (TextStyle)std::stoi(value);
		};
		CheckboxTextStyleProperty.getter_fn = [](UIView* target) {
			return std::to_string((uint32_t)reinterpret_cast<UICheckbox*>(target)->Label->Properties.Style);
		};

		Property CheckboxWordWrappingProperty;
		CheckboxWordWrappingProperty.type = Flags::SELECTABLE;
		CheckboxWordWrappingProperty.name = "Word Wrapping";
		CheckboxWordWrappingProperty.items = { "Normal", "No Wrap", "Char Wrap", "Word Wrap", "Emergency Break" };
		CheckboxWordWrappingProperty.applier_fn = [](UIView* target, const std::string& value) {
			reinterpret_cast<UICheckbox*>(target)->Label->Properties.Wrapping = (WordWrapping)std::stoi(value);
		};
		CheckboxWordWrappingProperty.getter_fn = [](UIView* target) {
			return std::to_string((uint32_t)reinterpret_cast<UICheckbox*>(target)->Label->Properties.Wrapping);
		};

#pragma endregion

#pragma region Slider Properties

		Property SliderKnobShapeProperty;
		SliderKnobShapeProperty.type = Flags::SELECTABLE;
		SliderKnobShapeProperty.name = "Knob Shape";
		SliderKnobShapeProperty.items = { "Rectangle", "Circle" };
		SliderKnobShapeProperty.applier_fn = [](UIView* target, const std::string& value) {
			reinterpret_cast<UISlider*>(target)->SliderKnobShape = (Shape)std::stoi(value);
		};
		SliderKnobShapeProperty.getter_fn = [](UIView* target) {
			return std::to_string((uint32_t)reinterpret_cast<UISlider*>(target)->SliderKnobShape);
		};

		Property SliderBarHeightProperty;
		SliderBarHeightProperty.type = Flags::DATA_ENTRY;
		SliderBarHeightProperty.name = "Bar Height";
		SliderBarHeightProperty.applier_fn = [](UIView* target, const std::string& value) {
			try {
				reinterpret_cast<UISlider*>(target)->SliderBarHeight = std::stof(value);
			}
			catch (...) {}
		};
		SliderBarHeightProperty.getter_fn = [](UIView* target) {
			return std::to_string((uint32_t)reinterpret_cast<UISlider*>(target)->SliderBarHeight);
		};

		Property SliderKnobColorProperty;
		SliderKnobColorProperty.type = Flags::DATA_ENTRY;
		SliderKnobColorProperty.name = "Knob Color";
		SliderKnobColorProperty.applier_fn = [](UIView* target, const std::string& value) {
			try {
				reinterpret_cast<UISlider*>(target)->SliderKnobColor = Color::FromRGBString(value.c_str());
			}
			catch (...) {}
		};
		SliderKnobColorProperty.getter_fn = [](UIView* target) {
			return reinterpret_cast<UISlider*>(target)->SliderKnobColor.ToString();
		};

		Property SliderTickmarkColorProperty;
		SliderTickmarkColorProperty.type = Flags::DATA_ENTRY;
		SliderTickmarkColorProperty.name = "Tickmark Color";
		SliderTickmarkColorProperty.applier_fn = [](UIView* target, const std::string& value) {
			try {
				reinterpret_cast<UISlider*>(target)->TickmarksColor = Color::FromRGBString(value.c_str());
			}
			catch (...) {}
		};
		SliderTickmarkColorProperty.getter_fn = [](UIView* target) {
			return reinterpret_cast<UISlider*>(target)->TickmarksColor.ToString();
		};

		Property SliderMaxValueProperty;
		SliderMaxValueProperty.type = Flags::DATA_ENTRY;
		SliderMaxValueProperty.name = "Max Value";
		SliderMaxValueProperty.applier_fn = [](UIView* target, const std::string& value) {
			try {
				reinterpret_cast<UISlider*>(target)->MaxValue = std::stof(value);
			}
			catch (...) {}
		};
		SliderMaxValueProperty.getter_fn = [](UIView* target) {
			std::stringstream ss;
			ss.precision(3);
			ss << reinterpret_cast<UISlider*>(target)->MaxValue;

			return ss.str();
		};

		Property SliderMinValueProperty;
		SliderMinValueProperty.type = Flags::DATA_ENTRY;
		SliderMinValueProperty.name = "Min Value";
		SliderMinValueProperty.applier_fn = [](UIView* target, const std::string& value) {
			try {
				reinterpret_cast<UISlider*>(target)->MinValue = std::stof(value);
			}
			catch (...) {}
		};
		SliderMinValueProperty.getter_fn = [](UIView* target) {
			std::stringstream ss;
			ss.precision(3);
			ss << reinterpret_cast<UISlider*>(target)->MinValue;

			return ss.str();
		};

		Property SliderValueProperty;
		SliderValueProperty.type = Flags::DATA_ENTRY;
		SliderValueProperty.name = "Value";
		SliderValueProperty.applier_fn = [](UIView* target, const std::string& value) {
			try {
				reinterpret_cast<UISlider*>(target)->Value = std::stof(value);
			}
			catch (...) {}
		};
		SliderValueProperty.getter_fn = [](UIView* target) {
			std::stringstream ss;
			ss.precision(3);
			ss << reinterpret_cast<UISlider*>(target)->Value;

			return ss.str();
		};

		Property SliderIntervalsProperty;
		SliderIntervalsProperty.type = Flags::DATA_ENTRY;
		SliderIntervalsProperty.name = "Intervals";
		SliderIntervalsProperty.applier_fn = [](UIView* target, const std::string& value) {
			try {
				reinterpret_cast<UISlider*>(target)->Intervals = std::stof(value);
			}
			catch (...) {}
		};
		SliderIntervalsProperty.getter_fn = [](UIView* target) {
			std::stringstream ss;
			ss.precision(3);
			ss << reinterpret_cast<UISlider*>(target)->Intervals;

			return ss.str();
		};

		Property SliderShowTickmarksProperty;
		SliderShowTickmarksProperty.type = Flags::TOGGLABLE;
		SliderShowTickmarksProperty.name = "Show Tickmarks";
		SliderShowTickmarksProperty.applier_fn = [](UIView* target, const std::string& value) {
			reinterpret_cast<UISlider*>(target)->VisibleTickmarks = (bool)std::stoi(value);
		};
		SliderShowTickmarksProperty.getter_fn = [](UIView* target) {
			return std::to_string((uint32_t)reinterpret_cast<UISlider*>(target)->VisibleTickmarks);
		};

#pragma endregion

#pragma region Textbox Properties

		Property TextboxStrokeProperty;
		TextboxStrokeProperty.type = Flags::DATA_ENTRY;
		TextboxStrokeProperty.name = "Stroke";
		TextboxStrokeProperty.applier_fn = [](UIView* target, const std::string& value) {
			try {
				reinterpret_cast<UITextbox*>(target)->Stroke = std::stof(value);
			}
			catch (...) {}
		};
		TextboxStrokeProperty.getter_fn = [](UIView* target) {
			return std::to_string((uint32_t)reinterpret_cast<UITextbox*>(target)->Stroke);
		};

		Property TextboxFocusedHighlightColorProperty;
		TextboxFocusedHighlightColorProperty.type = Flags::DATA_ENTRY;
		TextboxFocusedHighlightColorProperty.name = "Focused Color";
		TextboxFocusedHighlightColorProperty.applier_fn = [](UIView* target, const std::string& value) {
			try {
				reinterpret_cast<UITextbox*>(target)->FocusedHighlightColor = Color::FromRGBString(value.c_str());
			}
			catch (...) {}
		};
		TextboxFocusedHighlightColorProperty.getter_fn = [](UIView* target) {
			return reinterpret_cast<UITextbox*>(target)->FocusedHighlightColor.ToString();
		};

		Property TextboxTextProperty;
		TextboxTextProperty.type = Flags::DATA_ENTRY;
		TextboxTextProperty.name = "Text";
		TextboxTextProperty.applier_fn = [](UIView* target, const std::string& value) {
			try {
				reinterpret_cast<UITextbox*>(target)->Text = value;
			}
			catch (...) {}
		};
		TextboxTextProperty.getter_fn = [](UIView* target) {
			return reinterpret_cast<UITextbox*>(target)->Text;
		};

		Property TextboxPlaceholderProperty;
		TextboxPlaceholderProperty.type = Flags::DATA_ENTRY;
		TextboxPlaceholderProperty.name = "Placeholder";
		TextboxPlaceholderProperty.applier_fn = [](UIView* target, const std::string& value) {
			try {
				reinterpret_cast<UITextbox*>(target)->Placeholder = value;
			}
			catch (...) {}
		};
		TextboxPlaceholderProperty.getter_fn = [](UIView* target) {
			return reinterpret_cast<UITextbox*>(target)->Placeholder;
		};

		Property TextboxTextColorProperty;
		TextboxTextColorProperty.type = Flags::DATA_ENTRY;
		TextboxTextColorProperty.name = "Text Color";
		TextboxTextColorProperty.applier_fn = [](UIView* target, const std::string& value) {
			try {
				reinterpret_cast<UITextbox*>(target)->TextColor = Color::FromRGBString(value.c_str());
			}
			catch (...) {}
		};
		TextboxTextColorProperty.getter_fn = [](UIView* target) {
			return reinterpret_cast<UITextbox*>(target)->TextColor.ToString();
		};

		Property TextboxReadOnlyProperty;
		TextboxReadOnlyProperty.type = Flags::TOGGLABLE;
		TextboxReadOnlyProperty.name = "Read Only";
		TextboxReadOnlyProperty.applier_fn = [](UIView* target, const std::string& value) {
			reinterpret_cast<UITextbox*>(target)->ReadOnly = (bool)std::stoi(value);
		};
		TextboxReadOnlyProperty.getter_fn = [](UIView* target) {
			return std::to_string((uint32_t)reinterpret_cast<UITextbox*>(target)->ReadOnly);
		};

		Property TextboxFontProperty;
		TextboxFontProperty.type = Flags::DATA_ENTRY;
		TextboxFontProperty.name = "Font";
		TextboxFontProperty.applier_fn = [](UIView* target, const std::string& value) {
			try {
				reinterpret_cast<UITextbox*>(target)->textProperties.Font = value;
			}
			catch (...) {}
		};
		TextboxFontProperty.getter_fn = [](UIView* target) {
			return reinterpret_cast<UITextbox*>(target)->textProperties.Font;
		};

		Property TextboxFontSizeProperty;
		TextboxFontSizeProperty.type = Flags::DATA_ENTRY;
		TextboxFontSizeProperty.name = "Font Size";
		TextboxFontSizeProperty.applier_fn = [](UIView* target, const std::string& value) {
			try {
				reinterpret_cast<UITextbox*>(target)->textProperties.FontSize = std::stoi(value);
			}
			catch (...) {}
		};
		TextboxFontSizeProperty.getter_fn = [](UIView* target) {
			return std::to_string(reinterpret_cast<UITextbox*>(target)->textProperties.FontSize);
		};

		Property TextboxTextAlignmentProperty;
		TextboxTextAlignmentProperty.type = Flags::SELECTABLE;
		TextboxTextAlignmentProperty.name = "Alignment";
		TextboxTextAlignmentProperty.items = { "Leading", "Centered", "Trailing" };
		TextboxTextAlignmentProperty.applier_fn = [](UIView* target, const std::string& value) {
			reinterpret_cast<UITextbox*>(target)->textProperties.Alignment = (TextAlignment)std::stoi(value);
		};
		TextboxTextAlignmentProperty.getter_fn = [](UIView* target) {
			return std::to_string((uint32_t)reinterpret_cast<UITextbox*>(target)->textProperties.Alignment);
		};

		Property TextboxTextVerticalAlignmentProperty;
		TextboxTextVerticalAlignmentProperty.type = Flags::SELECTABLE;
		TextboxTextVerticalAlignmentProperty.name = "Vertical Alignment";
		TextboxTextVerticalAlignmentProperty.items = { "Leading", "Centered", "Trailing" };
		TextboxTextVerticalAlignmentProperty.applier_fn = [](UIView* target, const std::string& value) {
			reinterpret_cast<UITextbox*>(target)->textProperties.VerticalAlignment = (TextAlignment)std::stoi(value);
		};
		TextboxTextVerticalAlignmentProperty.getter_fn = [](UIView* target) {
			return std::to_string((uint32_t)reinterpret_cast<UITextbox*>(target)->textProperties.VerticalAlignment);
		};

		Property TextboxTextStyleProperty;
		TextboxTextStyleProperty.type = Flags::SELECTABLE;
		TextboxTextStyleProperty.name = "Text Style";
		TextboxTextStyleProperty.items = { "Default", "Light", "Italic", "Bold", "Bold Italic", "Semibold", "Semibold Italic" };
		TextboxTextStyleProperty.applier_fn = [](UIView* target, const std::string& value) {
			reinterpret_cast<UITextbox*>(target)->textProperties.Style = (TextStyle)std::stoi(value);
		};
		TextboxTextStyleProperty.getter_fn = [](UIView* target) {
			return std::to_string((uint32_t)reinterpret_cast<UITextbox*>(target)->textProperties.Style);
		};

		Property TextboxWordWrappingProperty;
		TextboxWordWrappingProperty.type = Flags::SELECTABLE;
		TextboxWordWrappingProperty.name = "Word Wrapping";
		TextboxWordWrappingProperty.items = { "Normal", "No Wrap", "Char Wrap", "Word Wrap", "Emergency Break" };
		TextboxWordWrappingProperty.applier_fn = [](UIView* target, const std::string& value) {
			reinterpret_cast<UITextbox*>(target)->textProperties.Wrapping = (WordWrapping)std::stoi(value);
		};
		TextboxWordWrappingProperty.getter_fn = [](UIView* target) {
			return std::to_string((uint32_t)reinterpret_cast<UITextbox*>(target)->textProperties.Wrapping);
		};

#pragma endregion

#pragma region Properties of Each Widget

		WidgetPropertiesList[WidgetType::View] =
		{
			UIVIEW_BASE_PROPERTIES
		};

		WidgetPropertiesList[WidgetType::Label] =
		{
			UIVIEW_BASE_PROPERTIES,
			LabelTextProperty,
			LabelTextColorProperty,
			LabelFontProperty,
			LabelFontSizeProperty,
			LabelTextAlignmentProperty,
			LabelTextVerticalAlignmentProperty,
			LabelTextStyleProperty,
			LabelWordWrappingProperty
		};

		WidgetPropertiesList[WidgetType::Button] =
		{
			UIVIEW_BASE_PROPERTIES,
			ButtonFilledProperty,
			ButtonStrokeProperty,
			HoverOnColorProperty,
			OnPressColorProperty,
			ButtonTextProperty,
			ButtonTextColorProperty,
			ButtonFontProperty,
			ButtonFontSizeProperty,
			ButtonTextAlignmentProperty,
			ButtonTextVerticalAlignmentProperty,
			ButtonTextStyleProperty,
			ButtonWordWrappingProperty
		};

		WidgetPropertiesList[WidgetType::Checkbox] =
		{
			UIVIEW_BASE_PROPERTIES,
			CheckboxBoxSizeProperty,
			CheckboxLabelMarginsProperty,
			CheckboxCheckmarkColorProperty,
			CheckboxCheckedBoxColorProperty,
			CheckboxCheckedProperty,
			CheckboxTextProperty,
			CheckboxTextColorProperty,
			CheckboxFontProperty,
			CheckboxFontSizeProperty,
			CheckboxTextAlignmentProperty,
			CheckboxTextVerticalAlignmentProperty,
			CheckboxTextStyleProperty,
			CheckboxWordWrappingProperty
		};
		
		WidgetPropertiesList[WidgetType::Slider] =
		{
			UIVIEW_BASE_PROPERTIES,
			SliderKnobShapeProperty,
			SliderBarHeightProperty,
			SliderKnobColorProperty,
			SliderTickmarkColorProperty,
			SliderMaxValueProperty,
			SliderMinValueProperty,
			SliderValueProperty,
			SliderIntervalsProperty,
			SliderShowTickmarksProperty
		};

		WidgetPropertiesList[WidgetType::Textbox] =
		{
			UIVIEW_BASE_PROPERTIES,
			TextboxStrokeProperty,
			TextboxFocusedHighlightColorProperty,
			TextboxTextProperty,
			TextboxPlaceholderProperty,
			TextboxTextColorProperty,
			TextboxReadOnlyProperty,
			TextboxFontProperty,
			TextboxFontSizeProperty,
			TextboxTextAlignmentProperty,
			TextboxTextVerticalAlignmentProperty,
			TextboxTextStyleProperty,
			TextboxWordWrappingProperty,
		};

#pragma endregion

	}
}
