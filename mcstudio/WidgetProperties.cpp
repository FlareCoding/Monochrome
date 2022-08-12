#include "WidgetProperties.h"
#include <sstream>

WidgetProperties::PropertiesList_t WidgetProperties::WidgetPropertiesList;

#define UIVIEW_BASE_PROPERTIES	VisibleFieldProperty, ZIndexProperty, \
								PosXProperty, PosYProperty, \
								WidthProperty, HeightProperty, \
								CornerRadiusProperty, LayerColorProperty, AnchorProperty

namespace WidgetProperties
{
	static Property VisibleFieldProperty;
	static Property ZIndexProperty;
	static Property PosXProperty;
	static Property PosYProperty;
	static Property WidthProperty;
	static Property HeightProperty;
	static Property CornerRadiusProperty;
	static Property LayerColorProperty;
	static Property AnchorProperty;

	static Property LabelTextProperty;
	static Property LabelTextColorProperty;
	static Property LabelFontProperty;
	static Property LabelFontSizeProperty;
	static Property LabelTextAlignmentProperty;
	static Property LabelTextVerticalAlignmentProperty;
	static Property LabelTextStyleProperty;
	static Property LabelWordWrappingProperty;

	static Property ButtonFilledProperty;
	static Property ButtonStrokeProperty;
	static Property HoverOnColorProperty;
	static Property OnPressColorProperty;
	static Property ButtonTextProperty;
	static Property ButtonTextColorProperty;
	static Property ButtonFontProperty;
	static Property ButtonFontSizeProperty;
	static Property ButtonTextAlignmentProperty;
	static Property ButtonTextVerticalAlignmentProperty;
	static Property ButtonTextStyleProperty;
	static Property ButtonWordWrappingProperty;

	static Property CheckboxBoxSizeProperty;
	static Property CheckboxLabelMarginsProperty;
	static Property CheckboxCheckmarkColorProperty;
	static Property CheckboxCheckedBoxColorProperty;
	static Property CheckboxCheckedProperty;
	static Property CheckboxTextProperty;
	static Property CheckboxTextColorProperty;
	static Property CheckboxFontProperty;
	static Property CheckboxFontSizeProperty;
	static Property CheckboxTextAlignmentProperty;
	static Property CheckboxTextVerticalAlignmentProperty;
	static Property CheckboxTextStyleProperty;
	static Property CheckboxWordWrappingProperty;

	static Property SliderKnobShapeProperty;
	static Property SliderBarHeightProperty;
	static Property SliderKnobColorProperty;
	static Property SliderTickmarkColorProperty;
	static Property SliderMaxValueProperty;
	static Property SliderMinValueProperty;
	static Property SliderValueProperty;
	static Property SliderIntervalsProperty;
	static Property SliderShowTickmarksProperty;

	static Property TextboxStrokeProperty;
	static Property TextboxFocusedHighlightColorProperty;
	static Property TextboxTextProperty;
	static Property TextboxPlaceholderProperty;
	static Property TextboxTextColorProperty;
	static Property TextboxReadOnlyProperty;
	static Property TextboxFontProperty;
	static Property TextboxFontSizeProperty;
	static Property TextboxTextAlignmentProperty;
	static Property TextboxTextVerticalAlignmentProperty;
	static Property TextboxTextStyleProperty;
	static Property TextboxWordWrappingProperty;

	static Property ComboboxSlotSizeProperty;
	static Property ComboboxItemBackgroundColorProperty;
	static Property ComboboxItemTextColorProperty;
	static Property ComboboxDropdownArrowColorProperty;
	static Property ComboboxAddItemProperty;
	static Property ComboboxRemoveItemProperty;

	static Property TextAreaTextProperty;
	static Property TextAreaTextColorProperty;
	static Property TextAreaTopMarginsProperty;
	static Property TextAreaBottomMarginsProperty;
	static Property TextAreaLeftMarginsProperty;
	static Property TextAreaRightMarginsProperty;
	static Property TextAreaAutoScrollProperty;
	static Property TextAreaFontProperty;
	static Property TextAreaFontSizeProperty;
	static Property TextAreaTextAlignmentProperty;
	static Property TextAreaTextVerticalAlignmentProperty;
	static Property TextAreaTextStyleProperty;
	static Property TextAreaWordWrappingProperty;

	static Property ImageOpacityProperty;
	static Property ImageLocalPathProperty;
	static Property ImageWebURLProperty;

	static Property ScrollPanelScrollbarColorProperty;

	static Property DockPanelFilledProperty;
	static Property DockPanelStrokeProperty;

	static Property TabViewTabAreaHeightProperty;
	static Property TabViewTabWidthProperty;
	static Property TabViewUnderlineTabsProperty;
	static Property TabViewUnderlineColorProperty;
	static Property TabViewSelectedColorProperty;
	static Property TabViewAddTabProperty;
	static Property TabViewRemoveTabProperty;

	void CreatePropertiesList()
	{
#pragma region Base Properties

		VisibleFieldProperty.type = Flags::TOGGLABLE;
		VisibleFieldProperty.name = "Visible";
		VisibleFieldProperty.applier_fn = [](UIView* target, const std::string& value) {
			target->Visible = (bool)std::stoi(value);
		};
		VisibleFieldProperty.getter_fn = [](UIView* target) {
			return std::to_string((uint32_t)target->Visible);
		};

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

		AnchorProperty.type = Flags::SELECTABLE;
		AnchorProperty.name = "Anchor";
		AnchorProperty.items = { "Left", "Right", "Top", "Bottom", "Center", "None" };
		AnchorProperty.applier_fn = [](UIView* target, const std::string& value) {
			target->anchor = (Anchor)std::stoi(value);
		};
		AnchorProperty.getter_fn = [](UIView* target) {
			return std::to_string((uint32_t)target->anchor);
		};

#pragma endregion

#pragma region Label Properties

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

		LabelTextAlignmentProperty.type = Flags::SELECTABLE;
		LabelTextAlignmentProperty.name = "Alignment";
		LabelTextAlignmentProperty.items = { "Leading", "Centered", "Trailing" };
		LabelTextAlignmentProperty.applier_fn = [](UIView* target, const std::string& value) {
			reinterpret_cast<UILabel*>(target)->Properties.Alignment = (TextAlignment)std::stoi(value);
		};
		LabelTextAlignmentProperty.getter_fn = [](UIView* target) {
			return std::to_string((uint32_t)reinterpret_cast<UILabel*>(target)->Properties.Alignment);
		};

		LabelTextVerticalAlignmentProperty.type = Flags::SELECTABLE;
		LabelTextVerticalAlignmentProperty.name = "Vertical Alignment";
		LabelTextVerticalAlignmentProperty.items = { "Leading", "Centered", "Trailing" };
		LabelTextVerticalAlignmentProperty.applier_fn = [](UIView* target, const std::string& value) {
			reinterpret_cast<UILabel*>(target)->Properties.VerticalAlignment = (TextAlignment)std::stoi(value);
		};
		LabelTextVerticalAlignmentProperty.getter_fn = [](UIView* target) {
			return std::to_string((uint32_t)reinterpret_cast<UILabel*>(target)->Properties.VerticalAlignment);
		};

		LabelTextStyleProperty.type = Flags::SELECTABLE;
		LabelTextStyleProperty.name = "Text Style";
		LabelTextStyleProperty.items = { "Default", "Light", "Italic", "Bold", "Bold Italic", "Semibold", "Semibold Italic" };
		LabelTextStyleProperty.applier_fn = [](UIView* target, const std::string& value) {
			reinterpret_cast<UILabel*>(target)->Properties.Style = (TextStyle)std::stoi(value);
		};
		LabelTextStyleProperty.getter_fn = [](UIView* target) {
			return std::to_string((uint32_t)reinterpret_cast<UILabel*>(target)->Properties.Style);
		};

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

		ButtonFilledProperty.type = Flags::TOGGLABLE;
		ButtonFilledProperty.name = "Filled";
		ButtonFilledProperty.applier_fn = [](UIView* target, const std::string& value) {
			reinterpret_cast<UIButton*>(target)->Filled = (bool)std::stoi(value);
		};
		ButtonFilledProperty.getter_fn = [](UIView* target) {
			return std::to_string((uint32_t)reinterpret_cast<UIButton*>(target)->Filled);
		};

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

		ButtonTextAlignmentProperty.type = Flags::SELECTABLE;
		ButtonTextAlignmentProperty.name = "Alignment";
		ButtonTextAlignmentProperty.items = { "Leading", "Centered", "Trailing" };
		ButtonTextAlignmentProperty.applier_fn = [](UIView* target, const std::string& value) {
			reinterpret_cast<UIButton*>(target)->Label->Properties.Alignment = (TextAlignment)std::stoi(value);
		};
		ButtonTextAlignmentProperty.getter_fn = [](UIView* target) {
			return std::to_string((uint32_t)reinterpret_cast<UIButton*>(target)->Label->Properties.Alignment);
		};

		ButtonTextVerticalAlignmentProperty.type = Flags::SELECTABLE;
		ButtonTextVerticalAlignmentProperty.name = "Vertical Alignment";
		ButtonTextVerticalAlignmentProperty.items = { "Leading", "Centered", "Trailing" };
		ButtonTextVerticalAlignmentProperty.applier_fn = [](UIView* target, const std::string& value) {
			reinterpret_cast<UIButton*>(target)->Label->Properties.VerticalAlignment = (TextAlignment)std::stoi(value);
		};
		ButtonTextVerticalAlignmentProperty.getter_fn = [](UIView* target) {
			return std::to_string((uint32_t)reinterpret_cast<UIButton*>(target)->Label->Properties.VerticalAlignment);
		};

		ButtonTextStyleProperty.type = Flags::SELECTABLE;
		ButtonTextStyleProperty.name = "Text Style";
		ButtonTextStyleProperty.items = { "Default", "Light", "Italic", "Bold", "Bold Italic", "Semibold", "Semibold Italic" };
		ButtonTextStyleProperty.applier_fn = [](UIView* target, const std::string& value) {
			reinterpret_cast<UIButton*>(target)->Label->Properties.Style = (TextStyle)std::stoi(value);
		};
		ButtonTextStyleProperty.getter_fn = [](UIView* target) {
			return std::to_string((uint32_t)reinterpret_cast<UIButton*>(target)->Label->Properties.Style);
		};

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

		CheckboxCheckedProperty.type = Flags::TOGGLABLE;
		CheckboxCheckedProperty.name = "Checked";
		CheckboxCheckedProperty.applier_fn = [](UIView* target, const std::string& value) {
			reinterpret_cast<UICheckbox*>(target)->Checked = (bool)std::stoi(value);
		};
		CheckboxCheckedProperty.getter_fn = [](UIView* target) {
			return std::to_string((uint32_t)reinterpret_cast<UICheckbox*>(target)->Checked);
		};

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

		CheckboxTextAlignmentProperty.type = Flags::SELECTABLE;
		CheckboxTextAlignmentProperty.name = "Alignment";
		CheckboxTextAlignmentProperty.items = { "Leading", "Centered", "Trailing" };
		CheckboxTextAlignmentProperty.applier_fn = [](UIView* target, const std::string& value) {
			reinterpret_cast<UICheckbox*>(target)->Label->Properties.Alignment = (TextAlignment)std::stoi(value);
		};
		CheckboxTextAlignmentProperty.getter_fn = [](UIView* target) {
			return std::to_string((uint32_t)reinterpret_cast<UICheckbox*>(target)->Label->Properties.Alignment);
		};

		CheckboxTextVerticalAlignmentProperty.type = Flags::SELECTABLE;
		CheckboxTextVerticalAlignmentProperty.name = "Vertical Alignment";
		CheckboxTextVerticalAlignmentProperty.items = { "Leading", "Centered", "Trailing" };
		CheckboxTextVerticalAlignmentProperty.applier_fn = [](UIView* target, const std::string& value) {
			reinterpret_cast<UICheckbox*>(target)->Label->Properties.VerticalAlignment = (TextAlignment)std::stoi(value);
		};
		CheckboxTextVerticalAlignmentProperty.getter_fn = [](UIView* target) {
			return std::to_string((uint32_t)reinterpret_cast<UICheckbox*>(target)->Label->Properties.VerticalAlignment);
		};

		CheckboxTextStyleProperty.type = Flags::SELECTABLE;
		CheckboxTextStyleProperty.name = "Text Style";
		CheckboxTextStyleProperty.items = { "Default", "Light", "Italic", "Bold", "Bold Italic", "Semibold", "Semibold Italic" };
		CheckboxTextStyleProperty.applier_fn = [](UIView* target, const std::string& value) {
			reinterpret_cast<UICheckbox*>(target)->Label->Properties.Style = (TextStyle)std::stoi(value);
		};
		CheckboxTextStyleProperty.getter_fn = [](UIView* target) {
			return std::to_string((uint32_t)reinterpret_cast<UICheckbox*>(target)->Label->Properties.Style);
		};

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

		SliderKnobShapeProperty.type = Flags::SELECTABLE;
		SliderKnobShapeProperty.name = "Knob Shape";
		SliderKnobShapeProperty.items = { "Rectangle", "Circle" };
		SliderKnobShapeProperty.applier_fn = [](UIView* target, const std::string& value) {
			reinterpret_cast<UISlider*>(target)->SliderKnobShape = (Shape)std::stoi(value);
		};
		SliderKnobShapeProperty.getter_fn = [](UIView* target) {
			return std::to_string((uint32_t)reinterpret_cast<UISlider*>(target)->SliderKnobShape);
		};

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

		TextboxReadOnlyProperty.type = Flags::TOGGLABLE;
		TextboxReadOnlyProperty.name = "Read Only";
		TextboxReadOnlyProperty.applier_fn = [](UIView* target, const std::string& value) {
			reinterpret_cast<UITextbox*>(target)->ReadOnly = (bool)std::stoi(value);
		};
		TextboxReadOnlyProperty.getter_fn = [](UIView* target) {
			return std::to_string((uint32_t)reinterpret_cast<UITextbox*>(target)->ReadOnly);
		};

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

		TextboxTextAlignmentProperty.type = Flags::SELECTABLE;
		TextboxTextAlignmentProperty.name = "Alignment";
		TextboxTextAlignmentProperty.items = { "Leading", "Centered", "Trailing" };
		TextboxTextAlignmentProperty.applier_fn = [](UIView* target, const std::string& value) {
			reinterpret_cast<UITextbox*>(target)->textProperties.Alignment = (TextAlignment)std::stoi(value);
		};
		TextboxTextAlignmentProperty.getter_fn = [](UIView* target) {
			return std::to_string((uint32_t)reinterpret_cast<UITextbox*>(target)->textProperties.Alignment);
		};

		TextboxTextVerticalAlignmentProperty.type = Flags::SELECTABLE;
		TextboxTextVerticalAlignmentProperty.name = "Vertical Alignment";
		TextboxTextVerticalAlignmentProperty.items = { "Leading", "Centered", "Trailing" };
		TextboxTextVerticalAlignmentProperty.applier_fn = [](UIView* target, const std::string& value) {
			reinterpret_cast<UITextbox*>(target)->textProperties.VerticalAlignment = (TextAlignment)std::stoi(value);
		};
		TextboxTextVerticalAlignmentProperty.getter_fn = [](UIView* target) {
			return std::to_string((uint32_t)reinterpret_cast<UITextbox*>(target)->textProperties.VerticalAlignment);
		};

		TextboxTextStyleProperty.type = Flags::SELECTABLE;
		TextboxTextStyleProperty.name = "Text Style";
		TextboxTextStyleProperty.items = { "Default", "Light", "Italic", "Bold", "Bold Italic", "Semibold", "Semibold Italic" };
		TextboxTextStyleProperty.applier_fn = [](UIView* target, const std::string& value) {
			reinterpret_cast<UITextbox*>(target)->textProperties.Style = (TextStyle)std::stoi(value);
		};
		TextboxTextStyleProperty.getter_fn = [](UIView* target) {
			return std::to_string((uint32_t)reinterpret_cast<UITextbox*>(target)->textProperties.Style);
		};

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

#pragma region Combobox Properties

		ComboboxSlotSizeProperty.type = Flags::DATA_ENTRY;
		ComboboxSlotSizeProperty.name = "Slot Size";
		ComboboxSlotSizeProperty.applier_fn = [](UIView* target, const std::string& value) {
			try {
				reinterpret_cast<UICombobox*>(target)->SlotSize = std::stof(value);
			}
			catch (...) {}
		};
		ComboboxSlotSizeProperty.getter_fn = [](UIView* target) {
			return std::to_string((uint32_t)reinterpret_cast<UICombobox*>(target)->SlotSize);
		};

		ComboboxItemBackgroundColorProperty.type = Flags::DATA_ENTRY;
		ComboboxItemBackgroundColorProperty.name = "Background Color";
		ComboboxItemBackgroundColorProperty.applier_fn = [](UIView* target, const std::string& value) {
			try {
				reinterpret_cast<UICombobox*>(target)->SetItemBackgroundColor(Color::FromRGBString(value.c_str()));
			}
			catch (...) {}
		};
		ComboboxItemBackgroundColorProperty.getter_fn = [](UIView* target) {
			return reinterpret_cast<UICombobox*>(target)->GetItemBackgroundColor().ToString();
		};

		ComboboxItemTextColorProperty.type = Flags::DATA_ENTRY;
		ComboboxItemTextColorProperty.name = "Text Color";
		ComboboxItemTextColorProperty.applier_fn = [](UIView* target, const std::string& value) {
			try {
				reinterpret_cast<UICombobox*>(target)->SetItemTextColor(Color::FromRGBString(value.c_str()));
			}
			catch (...) {}
		};
		ComboboxItemTextColorProperty.getter_fn = [](UIView* target) {
			return reinterpret_cast<UICombobox*>(target)->GetItemTextColor().ToString();
		};

		ComboboxDropdownArrowColorProperty.type = Flags::DATA_ENTRY;
		ComboboxDropdownArrowColorProperty.name = "Arrow Color";
		ComboboxDropdownArrowColorProperty.applier_fn = [](UIView* target, const std::string& value) {
			try {
				reinterpret_cast<UICombobox*>(target)->SetDropdownArrowColor(Color::FromRGBString(value.c_str()));
			}
			catch (...) {}
		};
		ComboboxDropdownArrowColorProperty.getter_fn = [](UIView* target) {
			return reinterpret_cast<UICombobox*>(target)->GetDropdownArrowColor().ToString();
		};

		ComboboxAddItemProperty.type = Flags::DATA_ENTRY;
		ComboboxAddItemProperty.name = "Add Item";
		ComboboxAddItemProperty.applier_fn = [](UIView* target, const std::string& value) {
			try {
				reinterpret_cast<UICombobox*>(target)->AddItem(value);
			}
			catch (...) {}
		};
		ComboboxAddItemProperty.getter_fn = [](UIView* target) {
			return "";
		};

		ComboboxRemoveItemProperty.type = Flags::DATA_ENTRY;
		ComboboxRemoveItemProperty.name = "Remove Item";
		ComboboxRemoveItemProperty.applier_fn = [](UIView* target, const std::string& value) {
			try {
				reinterpret_cast<UICombobox*>(target)->RemoveItem(value);
			}
			catch (...) {}
		};
		ComboboxRemoveItemProperty.getter_fn = [](UIView* target) {
			return "";
		};

#pragma endregion

#pragma region TextArea Properties

		TextAreaTextProperty.type = Flags::DATA_ENTRY;
		TextAreaTextProperty.name = "Text";
		TextAreaTextProperty.applier_fn = [](UIView* target, const std::string& value) {
			try {
				reinterpret_cast<UITextArea*>(target)->Text = value;
			}
			catch (...) {}
		};
		TextAreaTextProperty.getter_fn = [](UIView* target) {
			return reinterpret_cast<UITextArea*>(target)->Text;
		};

		TextAreaTextColorProperty.type = Flags::DATA_ENTRY;
		TextAreaTextColorProperty.name = "Text Color";
		TextAreaTextColorProperty.applier_fn = [](UIView* target, const std::string& value) {
			try {
				reinterpret_cast<UITextArea*>(target)->TextColor = Color::FromRGBString(value.c_str());
			}
			catch (...) {}
		};
		TextAreaTextColorProperty.getter_fn = [](UIView* target) {
			return reinterpret_cast<UITextArea*>(target)->TextColor.ToString();
		};

		TextAreaTopMarginsProperty.type = Flags::DATA_ENTRY;
		TextAreaTopMarginsProperty.name = "Top Margins";
		TextAreaTopMarginsProperty.applier_fn = [](UIView* target, const std::string& value) {
			try {
				reinterpret_cast<UITextArea*>(target)->TopMargins = std::stof(value);
			}
			catch (...) {}
		};
		TextAreaTopMarginsProperty.getter_fn = [](UIView* target) {
			return std::to_string((uint32_t)reinterpret_cast<UITextArea*>(target)->TopMargins);
		};

		TextAreaBottomMarginsProperty.type = Flags::DATA_ENTRY;
		TextAreaBottomMarginsProperty.name = "Bottom Margins";
		TextAreaBottomMarginsProperty.applier_fn = [](UIView* target, const std::string& value) {
			try {
				reinterpret_cast<UITextArea*>(target)->BottomMargins = std::stof(value);
			}
			catch (...) {}
		};
		TextAreaBottomMarginsProperty.getter_fn = [](UIView* target) {
			return std::to_string((uint32_t)reinterpret_cast<UITextArea*>(target)->BottomMargins);
		};

		TextAreaLeftMarginsProperty.type = Flags::DATA_ENTRY;
		TextAreaLeftMarginsProperty.name = "Left Margins";
		TextAreaLeftMarginsProperty.applier_fn = [](UIView* target, const std::string& value) {
			try {
				reinterpret_cast<UITextArea*>(target)->LeftMargins = std::stof(value);
			}
			catch (...) {}
		};
		TextAreaLeftMarginsProperty.getter_fn = [](UIView* target) {
			return std::to_string((uint32_t)reinterpret_cast<UITextArea*>(target)->LeftMargins);
		};

		TextAreaRightMarginsProperty.type = Flags::DATA_ENTRY;
		TextAreaRightMarginsProperty.name = "Right Margins";
		TextAreaRightMarginsProperty.applier_fn = [](UIView* target, const std::string& value) {
			try {
				reinterpret_cast<UITextArea*>(target)->RightMargins = std::stof(value);
			}
			catch (...) {}
		};
		TextAreaRightMarginsProperty.getter_fn = [](UIView* target) {
			return std::to_string((uint32_t)reinterpret_cast<UITextArea*>(target)->RightMargins);
		};

		TextAreaAutoScrollProperty.type = Flags::TOGGLABLE;
		TextAreaAutoScrollProperty.name = "Autoscroll";
		TextAreaAutoScrollProperty.applier_fn = [](UIView* target, const std::string& value) {
			reinterpret_cast<UITextArea*>(target)->AutoScroll = (bool)std::stoi(value);
		};
		TextAreaAutoScrollProperty.getter_fn = [](UIView* target) {
			return std::to_string((uint32_t)reinterpret_cast<UITextArea*>(target)->AutoScroll);
		};

		TextAreaFontProperty.type = Flags::DATA_ENTRY;
		TextAreaFontProperty.name = "Font";
		TextAreaFontProperty.applier_fn = [](UIView* target, const std::string& value) {
			try {
				reinterpret_cast<UITextArea*>(target)->Properties.Font = value;
			}
			catch (...) {}
		};
		TextAreaFontProperty.getter_fn = [](UIView* target) {
			return reinterpret_cast<UITextArea*>(target)->Properties.Font;
		};

		TextAreaFontSizeProperty.type = Flags::DATA_ENTRY;
		TextAreaFontSizeProperty.name = "Font Size";
		TextAreaFontSizeProperty.applier_fn = [](UIView* target, const std::string& value) {
			try {
				reinterpret_cast<UITextArea*>(target)->Properties.FontSize = std::stoi(value);
			}
			catch (...) {}
		};
		TextAreaFontSizeProperty.getter_fn = [](UIView* target) {
			return std::to_string(reinterpret_cast<UITextArea*>(target)->Properties.FontSize);
		};

		TextAreaTextAlignmentProperty.type = Flags::SELECTABLE;
		TextAreaTextAlignmentProperty.name = "Alignment";
		TextAreaTextAlignmentProperty.items = { "Leading", "Centered", "Trailing" };
		TextAreaTextAlignmentProperty.applier_fn = [](UIView* target, const std::string& value) {
			reinterpret_cast<UITextArea*>(target)->Properties.Alignment = (TextAlignment)std::stoi(value);
		};
		TextAreaTextAlignmentProperty.getter_fn = [](UIView* target) {
			return std::to_string((uint32_t)reinterpret_cast<UITextArea*>(target)->Properties.Alignment);
		};

		TextAreaTextVerticalAlignmentProperty.type = Flags::SELECTABLE;
		TextAreaTextVerticalAlignmentProperty.name = "Vertical Alignment";
		TextAreaTextVerticalAlignmentProperty.items = { "Leading", "Centered", "Trailing" };
		TextAreaTextVerticalAlignmentProperty.applier_fn = [](UIView* target, const std::string& value) {
			reinterpret_cast<UITextArea*>(target)->Properties.VerticalAlignment = (TextAlignment)std::stoi(value);
		};
		TextAreaTextVerticalAlignmentProperty.getter_fn = [](UIView* target) {
			return std::to_string((uint32_t)reinterpret_cast<UITextArea*>(target)->Properties.VerticalAlignment);
		};

		TextAreaTextStyleProperty.type = Flags::SELECTABLE;
		TextAreaTextStyleProperty.name = "Text Style";
		TextAreaTextStyleProperty.items = { "Default", "Light", "Italic", "Bold", "Bold Italic", "Semibold", "Semibold Italic" };
		TextAreaTextStyleProperty.applier_fn = [](UIView* target, const std::string& value) {
			reinterpret_cast<UITextArea*>(target)->Properties.Style = (TextStyle)std::stoi(value);
		};
		TextAreaTextStyleProperty.getter_fn = [](UIView* target) {
			return std::to_string((uint32_t)reinterpret_cast<UITextArea*>(target)->Properties.Style);
		};

		TextAreaWordWrappingProperty.type = Flags::SELECTABLE;
		TextAreaWordWrappingProperty.name = "Word Wrapping";
		TextAreaWordWrappingProperty.items = { "Normal", "No Wrap", "Char Wrap", "Word Wrap", "Emergency Break" };
		TextAreaWordWrappingProperty.applier_fn = [](UIView* target, const std::string& value) {
			reinterpret_cast<UITextArea*>(target)->Properties.Wrapping = (WordWrapping)std::stoi(value);
		};
		TextAreaWordWrappingProperty.getter_fn = [](UIView* target) {
			return std::to_string((uint32_t)reinterpret_cast<UITextArea*>(target)->Properties.Wrapping);
		};

#pragma endregion

#pragma region Image Properties

		ImageOpacityProperty.type = Flags::DATA_ENTRY;
		ImageOpacityProperty.name = "Opacity";
		ImageOpacityProperty.applier_fn = [](UIView* target, const std::string& value) {
			try {
				reinterpret_cast<UIImage*>(target)->Opacity = std::stof(value);
			}
			catch (...) {}
		};
		ImageOpacityProperty.getter_fn = [](UIView* target) {
			std::stringstream ss;
			ss.precision(3);
			ss << reinterpret_cast<UIImage*>(target)->Opacity;

			return ss.str();
		};

		ImageLocalPathProperty.type = Flags::DATA_ENTRY;
		ImageLocalPathProperty.name = "Local Path";
		ImageLocalPathProperty.applier_fn = [](UIView* target, const std::string& value) {
			try {
				reinterpret_cast<UIImage*>(target)->LoadImageFromFile(value);
			}
			catch (...) {}
		};
		ImageLocalPathProperty.getter_fn = [](UIView* target) {
			return reinterpret_cast<UIImage*>(target)->IsImageLoaded() ? "[Loaded]" : "[Not Loaded]";
		};

		ImageWebURLProperty.type = Flags::DATA_ENTRY;
		ImageWebURLProperty.name = "Web URL";
		ImageWebURLProperty.applier_fn = [](UIView* target, const std::string& value) {
			try {
				reinterpret_cast<UIImage*>(target)->LoadWebImage(value);
			}
			catch (...) {}
		};
		ImageWebURLProperty.getter_fn = [](UIView* target) {
			return reinterpret_cast<UIImage*>(target)->IsImageLoaded() ? "[Loaded]" : "[Not Loaded]";
		};

#pragma endregion

#pragma region ScrollPanel Properties

		ScrollPanelScrollbarColorProperty.type = Flags::DATA_ENTRY;
		ScrollPanelScrollbarColorProperty.name = "Scrollbar Color";
		ScrollPanelScrollbarColorProperty.applier_fn = [](UIView* target, const std::string& value) {
			try {
				reinterpret_cast<UIScrollPanel*>(target)->ScrollbarColor = Color::FromRGBString(value.c_str());
			}
			catch (...) {}
		};
		ScrollPanelScrollbarColorProperty.getter_fn = [](UIView* target) {
			return reinterpret_cast<UIScrollPanel*>(target)->ScrollbarColor.ToString();
		};

#pragma endregion

#pragma region DockPanel Properties

		DockPanelFilledProperty.type = Flags::TOGGLABLE;
		DockPanelFilledProperty.name = "Filled";
		DockPanelFilledProperty.applier_fn = [](UIView* target, const std::string& value) {
			reinterpret_cast<UIDockPanel*>(target)->Filled = (bool)std::stoi(value);
		};
		DockPanelFilledProperty.getter_fn = [](UIView* target) {
			return std::to_string((uint32_t)reinterpret_cast<UIDockPanel*>(target)->Filled);
		};

		DockPanelStrokeProperty.type = Flags::DATA_ENTRY;
		DockPanelStrokeProperty.name = "Stroke";
		DockPanelStrokeProperty.applier_fn = [](UIView* target, const std::string& value) {
			try {
				reinterpret_cast<UIDockPanel*>(target)->Stroke = std::stof(value);
			}
			catch (...) {}
		};
		DockPanelStrokeProperty.getter_fn = [](UIView* target) {
			return std::to_string((uint32_t)reinterpret_cast<UIDockPanel*>(target)->Stroke);
		};

#pragma endregion

#pragma region TabView Properties

		TabViewTabAreaHeightProperty.type = Flags::DATA_ENTRY;
		TabViewTabAreaHeightProperty.name = "Tab Height";
		TabViewTabAreaHeightProperty.applier_fn = [](UIView* target, const std::string& value) {
			try {
				reinterpret_cast<UITabView*>(target)->TabAreaHeight = std::stof(value);
			}
			catch (...) {}
		};
		TabViewTabAreaHeightProperty.getter_fn = [](UIView* target) {
			return std::to_string((uint32_t)reinterpret_cast<UITabView*>(target)->TabAreaHeight);
		};

		TabViewTabWidthProperty.type = Flags::DATA_ENTRY;
		TabViewTabWidthProperty.name = "Tab Width";
		TabViewTabWidthProperty.applier_fn = [](UIView* target, const std::string& value) {
			try {
				reinterpret_cast<UITabView*>(target)->TabWidth = std::stof(value);
			}
			catch (...) {}
		};
		TabViewTabWidthProperty.getter_fn = [](UIView* target) {
			return std::to_string((uint32_t)reinterpret_cast<UITabView*>(target)->TabWidth);
		};

		TabViewUnderlineTabsProperty.type = Flags::TOGGLABLE;
		TabViewUnderlineTabsProperty.name = "Underline Tabs";
		TabViewUnderlineTabsProperty.applier_fn = [](UIView* target, const std::string& value) {
			reinterpret_cast<UITabView*>(target)->UnderlineTabs = (bool)std::stoi(value);
		};
		TabViewUnderlineTabsProperty.getter_fn = [](UIView* target) {
			return std::to_string((uint32_t)reinterpret_cast<UITabView*>(target)->UnderlineTabs);
		};

		TabViewUnderlineColorProperty.type = Flags::DATA_ENTRY;
		TabViewUnderlineColorProperty.name = "Underline Color";
		TabViewUnderlineColorProperty.applier_fn = [](UIView* target, const std::string& value) {
			try {
				reinterpret_cast<UITabView*>(target)->TabUnderlineColor = Color::FromRGBString(value.c_str());
			}
			catch (...) {}
		};
		TabViewUnderlineColorProperty.getter_fn = [](UIView* target) {
			return reinterpret_cast<UITabView*>(target)->TabUnderlineColor.ToString();
		};

		TabViewSelectedColorProperty.type = Flags::DATA_ENTRY;
		TabViewSelectedColorProperty.name = "Selected Color";
		TabViewSelectedColorProperty.applier_fn = [](UIView* target, const std::string& value) {
			try {
				reinterpret_cast<UITabView*>(target)->SelectedTabColor = Color::FromRGBString(value.c_str());
			}
			catch (...) {}
		};
		TabViewSelectedColorProperty.getter_fn = [](UIView* target) {
			return reinterpret_cast<UITabView*>(target)->SelectedTabColor.ToString();
		};

		TabViewAddTabProperty.type = Flags::DATA_ENTRY;
		TabViewAddTabProperty.name = "Add Tab";
		TabViewAddTabProperty.applier_fn = [](UIView* target, const std::string& value) {
			try {
				reinterpret_cast<UITabView*>(target)->AddTab(value);
			}
			catch (...) {}
		};
		TabViewAddTabProperty.getter_fn = [](UIView* target) {
			return "";
		};

		TabViewRemoveTabProperty.type = Flags::DATA_ENTRY;
		TabViewRemoveTabProperty.name = "Remove Tab";
		TabViewRemoveTabProperty.applier_fn = [](UIView* target, const std::string& value) {
			try {
				reinterpret_cast<UITabView*>(target)->RemoveTab(value);
			}
			catch (...) {}
		};
		TabViewRemoveTabProperty.getter_fn = [](UIView* target) {
			return "";
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
			TextboxWordWrappingProperty
		};

		WidgetPropertiesList[WidgetType::Combobox] =
		{
			UIVIEW_BASE_PROPERTIES,
			ComboboxSlotSizeProperty,
			ComboboxItemBackgroundColorProperty,
			ComboboxItemTextColorProperty,
			ComboboxDropdownArrowColorProperty,
			ComboboxAddItemProperty,
			ComboboxRemoveItemProperty
		};

		WidgetPropertiesList[WidgetType::TextArea] =
		{
			UIVIEW_BASE_PROPERTIES,
			TextAreaTextProperty,
			TextAreaTextColorProperty,
			TextAreaTopMarginsProperty,
			TextAreaBottomMarginsProperty,
			TextAreaLeftMarginsProperty,
			TextAreaRightMarginsProperty,
			TextAreaAutoScrollProperty,
			TextAreaFontProperty,
			TextAreaFontSizeProperty,
			TextAreaTextAlignmentProperty,
			TextAreaTextVerticalAlignmentProperty,
			TextAreaTextStyleProperty,
			TextAreaWordWrappingProperty
		};
		
		WidgetPropertiesList[WidgetType::Image] =
		{
			UIVIEW_BASE_PROPERTIES,
			ImageOpacityProperty,
			ImageLocalPathProperty,
			ImageWebURLProperty
		};
		
		WidgetPropertiesList[WidgetType::ScrollPanel] =
		{
			UIVIEW_BASE_PROPERTIES,
			ScrollPanelScrollbarColorProperty
		};

		WidgetPropertiesList[WidgetType::DockingPanel] =
		{
			UIVIEW_BASE_PROPERTIES,
			DockPanelFilledProperty,
			DockPanelStrokeProperty
		};

		WidgetPropertiesList[WidgetType::TabView] =
		{
			UIVIEW_BASE_PROPERTIES,
			TabViewTabAreaHeightProperty,
			TabViewTabWidthProperty,
			TabViewUnderlineTabsProperty,
			TabViewUnderlineColorProperty,
			TabViewSelectedColorProperty,
			TabViewAddTabProperty,
			TabViewRemoveTabProperty
		};

#pragma endregion

	}
}
