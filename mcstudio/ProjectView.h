#pragma once
#include "StudioSettings.h"
#include "WidgetProperties.h"

class ProjectView : public UIScrollPanel
{
public:
	ProjectView(Ref<UIScrollPanel> PropertiesPanel);
	void Initialize();

	void SetProjectWidth(float width);
	void SetProjectHeight(float height);

	void AddWidget(WidgetType type);

private:
	void SetupEventHandlers();
	void UpdateSize();

private:
	Ref<UIView> m_BackgroundView;
	Ref<UIScrollPanel> m_PropertiesPanel;

	void OpenWidgetProperties(WidgetType widget_type);

	float AddTogglableProperty(WidgetProperties::Property prop, float y_offset);
	float AddDataEntryProperty(WidgetProperties::Property prop, float y_offset);
	float AddSelectableProperty(WidgetProperties::Property prop, float y_offset);

private:
	float m_ProjectWidth = 900;
	float m_ProjectHeight = 800;

	UIView* m_TargetView = nullptr;
	void DragView();

	bool m_MouseButtonReleasedFromView = true;
	Point m_ViewClickedLocationOffset = { 0, 0 };
};
