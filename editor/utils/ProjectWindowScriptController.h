#pragma once
#include "EditorUtils.h"
#include <future>

class ProjectWindowScriptController
{
public:
	void Initialize(Ref<UIWindow>* ProjectWindow, std::vector<Ref<UIView>>* ProjectUIElements) { m_ProjectWindow = ProjectWindow; m_ProjectUIElements = ProjectUIElements; }

	void Widget_OnMousePressed(Ref<UIView> view);
	void Widget_OnMouseReleased();

	void FindInnerMostView(Ref<UIView> view, Ref<UIView>& target);

private:
	Ref<UIWindow>* m_ProjectWindow = nullptr;
	std::vector<Ref<UIView>>* m_ProjectUIElements = nullptr;

private:
	Ref<UIView> m_TargetView = nullptr;
	Size m_ViewClickedLocationOffset = { 0, 0 };
	bool m_MouseButtonReleasedFromView = true;

private:
	void DragView();
	void CheckEmbeddingStatus();
	
private:
	bool IsViewEmbeddable(UIView* view);
	bool CheckIfViewNeedsEmbedding(Ref<UIView>& view_being_checked, std::vector<Ref<UIView>>& elements, Ref<UIView>& embeddable);
	void EmbedView(Ref<UIView> src, Ref<UIView> dest);
	bool UnembedViewIfNeeded(Ref<UIView> view);
};
