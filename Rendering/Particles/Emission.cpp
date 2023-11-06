#include "Emission.h"

#include "../../Time.h"

using namespace Jauntlet;

Emission::Emission() {
    
}

void Emission::draw(SpriteBatch* spriteBatch, Camera2D* camera, GLuint texture) {
    spriteBatch->draw(glm::vec4(position.x,position.y,size.x,size.y), texture, 0, Jauntlet::Color(255,255,255,255));
}

void Emission::update() {
    position += movement * Time::getDeltaTime();
    age += Time::getDeltaTime();
}