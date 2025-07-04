#include "Spritesheet.h"

namespace Snow
{
	Spritesheet::Spritesheet(Ref<Texture2D>& texture, glm::vec2 cellSize, glm::ivec2 cellResolution)
		:m_Texture(texture),m_CellSize(cellSize),m_CellResolution(cellResolution)
	{}

	Ref<Spritesheet> Spritesheet::CreateFromTexture(Ref<Texture2D>& texture, glm::vec2 cellSize)
	{
		glm::ivec2 CellResolution = { (int)(texture->GetWidth() / cellSize.x),(int)(texture->GetHeight() / cellSize.y) };
		return CreateRef<Spritesheet>(texture, cellSize, CellResolution);
	}
	Ref<Spritesheet> Spritesheet::CreateFromTexture(Ref<Texture2D>& texture, glm::ivec2 cellResolution)
	{
		glm::vec2 CellSize = { texture->GetWidth() / cellResolution.x,texture->GetHeight() / cellResolution.y };
		return CreateRef<Spritesheet>(texture, CellSize, cellResolution);
	}
	Ref<Spritesheet> Spritesheet::CreateFromPath(const std::string& path, glm::vec2 cellSize)
	{
		Ref<Texture2D> Texture = Texture2D::Create(path); // Can add texture parameters
		glm::ivec2 CellResolution = { (int)(Texture->GetWidth() / cellSize.x),(int)(Texture->GetHeight() / cellSize.y) };
		return CreateRef<Spritesheet>(Texture, cellSize, CellResolution);
	}
	Ref<Spritesheet> Spritesheet::CreateFromPath(const std::string& path, glm::ivec2 cellResolution)
	{
		Ref<Texture2D> Texture = Texture2D::Create(path); // Can add texture parameters
		glm::vec2 CellSize = { Texture->GetWidth() / cellResolution.x,Texture->GetHeight() / cellResolution.y };
		return CreateRef<Spritesheet>(Texture, CellSize, cellResolution);
	}


	Subtexture2D::Subtexture2D(const Ref<Spritesheet>& spriteSheet, const glm::vec2& min, const glm::vec2& max)
		: m_SpriteSheet(spriteSheet)
	{
		m_TexCoords[0] = { min.x,min.y };
		m_TexCoords[1] = { max.x,min.y };
		m_TexCoords[2] = { max.x,max.y };
		m_TexCoords[3] = { min.x,max.y };
	}
	Ref<Subtexture2D> Subtexture2D::CreateFromCoords(const Ref<Spritesheet>& SpriteSheet, const glm::vec2& coords, const glm::ivec2& spriteSize)
	{
		
		glm::vec2 sheetSize = SpriteSheet->GetTexture()->GetSize();
		glm::vec2 sheetResolution = SpriteSheet->GetCellSize();
		glm::vec2 min = { (coords.x * sheetResolution.x) / sheetSize.x,(coords.y * sheetResolution.y) / sheetSize.y };
		glm::vec2 max = { ((coords.x + spriteSize.x) * sheetResolution.x) / sheetSize.x,((coords.y + spriteSize.y) * sheetResolution.y) / sheetSize.y };

		return CreateRef<Subtexture2D>(SpriteSheet, min, max);
	}

};