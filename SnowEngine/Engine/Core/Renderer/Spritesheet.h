#pragma once

#include "SnowEngineAPI.h"
#include "Texture.h"

namespace Snow
{
	class Spritesheet
	{
	public:
		Spritesheet(Ref<Texture2D>& texture, glm::vec2 cellSize, glm::ivec2 cellResolution);

		Ref<Texture2D> GetTexture() const { return m_Texture; }
		const glm::vec2& GetCellSize() const { return m_CellSize; }
		const glm::vec2& GetCellResolution() const { return m_CellResolution; }

		static Ref<Spritesheet> CreateFromTexture(Ref<Texture2D>& texture, glm::vec2 cellSize);
		static Ref<Spritesheet> CreateFromTexture(Ref<Texture2D>& texture, glm::ivec2 cellResolution);
		static Ref<Spritesheet> CreateFromPath(const std::string& path, glm::vec2 cellSize);
		static Ref<Spritesheet> CreateFromPath(const std::string& path, glm::ivec2 cellResolution);
	private:
		Ref<Texture2D> m_Texture;
		glm::vec2 m_CellSize = glm::vec2(0.0f);
		glm::vec2 m_CellResolution;
	};

	class Subtexture2D
	{
	public:
		Subtexture2D(const Ref<Spritesheet>& spriteSheet, const glm::vec2& min, const glm::vec2& max);

		const Ref<Spritesheet> GetSpritesheet() const { return m_SpriteSheet; }
		const Ref<Texture2D> GetTexture() const { return m_SpriteSheet->GetTexture(); }
		const glm::vec2* GetTexCoords() const { return m_TexCoords; }
		
		static Ref<Subtexture2D> CreateFromCoords(const Ref<Spritesheet>& spriteSheet,const glm::vec2& coords, const glm::ivec2& spriteSize = {1,1});
	private:
		Ref<Spritesheet> m_SpriteSheet;
		glm::vec2 m_TexCoords[4];
	};
};