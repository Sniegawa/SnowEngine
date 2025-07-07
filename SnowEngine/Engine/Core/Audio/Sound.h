#pragma once

#include <miniaudio.h>
#include <string>

namespace Snow
{
	class Sound
	{
	public:
		Sound(const std::string& name,const std::string& filepath);
		ma_sound* GetSoundPointer() { return &m_Sound; }

		const float& GetVolume() const { return m_Volume; }
		void SetVolume(float volume);

		const std::string& GetName() const { return m_Name; }

		~Sound();

	private:
		ma_sound m_Sound;

		std::string m_Name;

		float m_Volume = 1.0f;
		bool m_Loaded = false;
	};
};