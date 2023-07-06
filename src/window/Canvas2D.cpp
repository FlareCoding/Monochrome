#include "Canvas2D.h"
#include <rendering/RenderTarget.h>

namespace mc {
	void Canvas2D::__render() {
		d_commandBufferMutex.lock();

		for (auto it = d_commandBuffer.begin(); it != d_commandBuffer.end(); ++it) {
			auto& cmd = *it;

			switch (cmd->getOpType()) {
			case DrawRectangle: {
				auto drCmd = std::static_pointer_cast<CanvasDrawRectangleCommand>(cmd);

				d_renderTarget->drawRectangle(
					drCmd->position.x, drCmd->position.y,
					drCmd->size.width, drCmd->size.height,
					drCmd->color,
					drCmd->radius,
					drCmd->filled,
					drCmd->stroke
				);
				break;
			}
			case DrawCircle: {
				auto dcCmd = std::static_pointer_cast<CanvasDrawCircleCommand>(cmd);

				d_renderTarget->drawCircle(
					dcCmd->position.x, dcCmd->position.y,
					dcCmd->radius,
					dcCmd->color,
					dcCmd->filled,
					dcCmd->stroke
				);
				break;
			}
			case DrawText: {
				auto dtCmd = std::static_pointer_cast<CanvasDrawTextCommand>(cmd);

				d_renderTarget->drawText(
					dtCmd->position.x, dtCmd->position.y,
					dtCmd->size.width, dtCmd->size.height,
					dtCmd->color,
					dtCmd->text,
					dtCmd->font,
					dtCmd->fontSize,
					dtCmd->fontStyle,
					dtCmd->alignment
				);
				break;
			}
			default: break;
			}
		}

		d_commandBufferMutex.unlock();
	}

	uint32_t Canvas2D::getWidth() const {
		return d_renderTarget->getWidth();
	}

	uint32_t Canvas2D::getHeight() const {
		return d_renderTarget->getHeight();
	}

	Size Canvas2D::getSize() const {
		return Size(d_renderTarget->getWidth(), d_renderTarget->getHeight());
	}

	void Canvas2D::clear() {
		d_commandBufferMutex.lock();
		d_commandBuffer.clear();
		d_commandBufferMutex.unlock();
	}

	void Canvas2D::strokeRectangle(
		const Position& position,
		const Size& size,
		const Color& color,
		uint32_t thickness,
		uint32_t radius
	) {
		auto cmd = MakeRef<CanvasDrawRectangleCommand>();
		cmd->position = position;
		cmd->size = size;
		cmd->color = color;
		cmd->filled = false;
		cmd->stroke = thickness;
		cmd->radius = radius;

		d_commandBuffer.push_back(cmd);
	}

	void Canvas2D::fillRectangle(
		const Position& position,
		const Size& size,
		const Color& color,
		uint32_t radius
	) {
		auto cmd = MakeRef<CanvasDrawRectangleCommand>();
		cmd->position = position;
		cmd->size = size;
		cmd->color = color;
		cmd->filled = true;
		cmd->radius = radius;

		d_commandBuffer.push_back(cmd);
	}
	
	void Canvas2D::strokeCircle(
		const Position& position,
		uint32_t radius,
		const Color& color,
		uint32_t thickness
	) {
		auto cmd = MakeRef<CanvasDrawCircleCommand>();
		cmd->position = position;
		cmd->radius = radius;
		cmd->color = color;
		cmd->filled = false;
		cmd->stroke = thickness;

		d_commandBuffer.push_back(cmd);
	}
	
	void Canvas2D::fillCircle(
		const Position& position,
		uint32_t radius,
		const Color& color
	) {
		auto cmd = MakeRef<CanvasDrawCircleCommand>();
		cmd->position = position;
		cmd->radius = radius;
		cmd->color = color;
		cmd->filled = true;

		d_commandBuffer.push_back(cmd);
	}

	void Canvas2D::fillText(
		const Position& position,
		const Size& size,
		const Color& color,
		const std::string& text,
		const std::string& font,
		uint32_t fontSize,
		const std::string& fontStyle,
		const std::string& alignment
	) {
		auto cmd = MakeRef<CanvasDrawTextCommand>();
		cmd->position = position;
		cmd->size = size;
		cmd->color = color;
		cmd->text = text;
		cmd->font = font;
		cmd->fontSize = fontSize;
		cmd->fontStyle = fontStyle;
		cmd->alignment = alignment;

		d_commandBuffer.push_back(cmd);
	}
} // namespace mc
