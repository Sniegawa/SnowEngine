#pragma once

#include <miniaudio.h>
#include <string>

namespace Snow
{
	class Sound
	{
	public:
		Sound(const std::string& filepath);
		ma_sound* GetSoundPointer() { return &m_Sound; }
		~Sound();

	private:
		ma_sound m_Sound;
		bool m_Loaded = false;
	};
};