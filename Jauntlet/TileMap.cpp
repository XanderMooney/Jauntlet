#include "TileMap.h"

#include <fstream>

#include "Errors.h"
#include "ResourceManager.h"
#include "ImageLoader.h"
#include "TileSet.h"

using namespace Jauntlet;

TileMap::TileMap(TextureCache& textureCache, int tileSize) : _tileSize(tileSize), _textureCache(textureCache) {
}

void TileMap::registerTile(char identifier, std::string filePath, TileCollision collisionType /*= TileCollision::SQUARE*/) {
	tile tmp = tile(filePath, collisionType);
	
	_tiles.insert(std::make_pair(identifier, tmp));
}

void TileMap::registerTileSet(char identifier, TileSet& tileSet, TileCollision collisionType /*= TileCollision::SQUARE*/) {
	tile tmp = tile(&tileSet, collisionType);

	_tiles.insert(std::make_pair(identifier, tmp));
}
void TileMap::registerFunction(char identifier, std::function<void(int, int)> customFunction) {
	tile tmp = tile(customFunction);

	_tiles.insert(std::make_pair(identifier, tmp));
}

void TileMap::loadTileMap(std::string filePath, float offsetX /*= 0*/, float offsetY /*= 0*/) {
	std::ifstream file;
	file.open(filePath);

	if (file.fail()) {
		fatalError("Failed to open TileMap " + filePath);
	}

	std::string tmp;

	while (std::getline(file, tmp)) {
		_levelData.push_back(tmp);
	}

	_spriteBatch.init();

	_spriteBatch.begin();

	glm::vec4 uvRect(0, 0, 1, 1);
	Jauntlet::Color whiteColor;
	whiteColor.setColor(255, 255, 255, 255);

	// Rendering all tiles into the sprite batch
	for (int y = 0; y < _levelData.size(); y++) {
		for (int x = 0; x < _levelData[y].size(); x++) {
			char tile = _levelData[y][x];
			// Create the location and size of the tile
			glm::vec4 destRect(x * _tileSize + offsetX, -y * _tileSize + offsetY, _tileSize, _tileSize);
			// Find and Process the tile
			auto mapIterator = _tiles.find(tile);

			if (mapIterator == _tiles.end()) {
				continue;
			}
			
			if (mapIterator->second.tileSet != nullptr) {
				unsigned int tileData = 0;

				if (testTileSetRules(*mapIterator->second.tileSet, x + 1, y)) {
					tileData |= TileSet::TileSides::RIGHT;
				}
				if (testTileSetRules(*mapIterator->second.tileSet, x - 1, y)) {
					tileData |= TileSet::TileSides::LEFT;
				}
				if (testTileSetRules(*mapIterator->second.tileSet, x, y + 1)) {
					tileData |= TileSet::TileSides::BOTTOM;
				}
				if (testTileSetRules(*mapIterator->second.tileSet, x, y - 1)) {
					tileData |= TileSet::TileSides::TOP;
				}

				_spriteBatch.draw(destRect, uvRect, _textureCache.getTexture(mapIterator->second.tileSet->tileSetToTile(tileData)).id, 0, whiteColor);
			}
			else if (mapIterator->second.tileFunc != nullptr) {
				mapIterator->second.tileFunc(x * _tileSize + offsetX, -y * _tileSize + offsetY);
			}
			else {
				_spriteBatch.draw(destRect, uvRect, _textureCache.getTexture(mapIterator->second.texture).id, 0, whiteColor);
			}
		}
	}
	_spriteBatch.end();
}

void TileMap::draw() {
	_spriteBatch.renderBatch();
}

bool TileMap::testTileSetRules(TileSet tile, int x, int y) {
	
	// make sure the position is within the level range
	if (y < 0 || y >= _levelData.size() || x >= _levelData[y].size() || x < 0) {
		return (tile.connectionRules & TileSet::ConnectionRules::EMPTY) ? true : false;
	}
	
	auto iterator = _tiles.find(_levelData[y][x]);
	
	if (iterator == _tiles.end()) { // must always check if the result is empty first
		return (tile.connectionRules & TileSet::ConnectionRules::EMPTY) ? true : false;
	}

	if (iterator->second.tileSet != nullptr && iterator->second.tileSet->getID() == tile.getID()) { // check if the tile is the same tileset
		return true;
	}
	if (tile.connectionRules & TileSet::ConnectionRules::NONE) { // test if the tileset wants to connect to nothing
		return false;
	}
	if (iterator->second.tileFunc != nullptr) { // check if the tile is a function. For now we treat it as if the space is empty.
		return (tile.connectionRules & TileSet::ConnectionRules::EMPTY) ? true : false;
	}
	if (iterator->second.tileSet != nullptr) { // check if the tile is a tileset
		return (tile.connectionRules & TileSet::ConnectionRules::TILESETS) ? true : false;
	}
	// The only remaining condition is if the tile is a regular tile.
	return (tile.connectionRules & TileSet::ConnectionRules::TILES) ? true : false;
}