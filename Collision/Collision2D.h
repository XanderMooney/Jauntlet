/* Purpose:
 * Main Contributer(s):
 */
#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "Collider2D.h"

namespace Jauntlet
{
	class Collision2D //the returned class for collision data
	{
	public:
		Collision2D();

		//not functional yet
		//std::vector<glm::vec2> GetCollisionPoints();

		//returns a vector2  regarding a direction (0,0 would be dead center) with a magnitude of 1.
		glm::vec2 GetNormal();

		float GetOverlap();

		bool getCollision(CircleCollider2D* parent, CircleCollider2D* other);
		bool getCollision(CircleCollider2D* parent, BoxCollider2D* other);
		bool getCollision(BoxCollider2D* parent, CircleCollider2D* other);
		bool getCollision(BoxCollider2D* parent, BoxCollider2D* other);

		bool getCollision(CircleCollider2D* parent, glm::vec2 other); //Circle-Position collision
		bool getCollision(BoxCollider2D* parent, glm::vec2 other); //Box-Position collision
	private:
		//still not functional
		//std::vector<std::vector<float>> _collisionPoints; 
		//Main collider in calculation
		Collider2D* _parent = nullptr;
		//the collider that collided with during the check
		Collider2D* _other = nullptr;
		//normal at the point of collision
		glm::vec2 _normal = glm::vec2();
		//overlap at point of collision
		float _overlap = 0;
	};
}