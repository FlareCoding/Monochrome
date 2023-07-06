#include "WidgetTreeController.h"

namespace mc::mcstudio {
	WidgetTreeController::WidgetTreeController() {
		d_widgetTreeView = getWidgetById<TreeView>("widgetTreeView");

		d_widgetTreeView->on("itemSelected", [this](Shared<Event> e) {
			if (!d_nodeSelectedCallback) {
				return;
			}

			const auto key = e->get<std::string>("key");
			const uuid_t selectedWidgetId = static_cast<uuid_t>(std::stoull(key));

			d_nodeSelectedCallback(selectedWidgetId);
		});
	}

	void WidgetTreeController::clearTree() {
		d_widgetTreeView->setRootNode(nullptr);
	}
	
	void WidgetTreeController::setRootWidget(Shared<BaseContainerWidget> root) {
		const auto rootNode = d_widgetTreeView->getRootNode();
		const auto rootWidgetNode =
			MakeRef<TreeViewNode>(root->getWidgetName(), std::to_string(root->getID()));

		rootWidgetNode->expanded = true;
		rootNode->addChild(rootWidgetNode);

		d_rootWidget = root;
	}
	
	void WidgetTreeController::selectWidget(Shared<BaseWidget> widget) {
		if (!widget) {
			d_widgetTreeView->selectNode(nullptr);
			return;
		}

		const auto widgetNodeKey = std::to_string(widget->getID());
		d_widgetTreeView->selectNodeByKey(widgetNodeKey);
	}

	void WidgetTreeController::insertWidgetNode(Shared<BaseWidget> widget) {
		const auto widgetNodeKey = std::to_string(widget->getID());
		const auto parentNodeKey = std::to_string(widget->getParent()->getID());

		const auto parentNode = d_widgetTreeView->findNodeWithKey(parentNodeKey);
		parentNode->addChild(widget->getWidgetName(), widgetNodeKey);

		// Trigger parent node expansion
		parentNode->expanded = true;
	}
	
	void WidgetTreeController::removeWidgetNode(Shared<BaseWidget> widget) {
		const auto widgetNodeKey = std::to_string(widget->getID());
		d_widgetTreeView->removeNodeWithKey(widgetNodeKey);
	}
} // namespace mc::mcstudio
