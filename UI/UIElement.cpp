#include <string>
#include "../Errors.h"
#include "UIElement.h"

using namespace Jauntlet;

UIElement::UIElement() {
	// empty
}

void UIElement::draw(Camera2D* camera, SpriteBatch* spriteBatch, float scale) {
	error("UIElement.draw() was called! This is the wrong method! dont use a generic update statement like this");
}

void UIElement::resolvePosition(Camera2D* camera, glm::vec2* resolvedPins, float scale) {
	_resolvedPosition = camera->convertScreenToWorld(*_position);
}