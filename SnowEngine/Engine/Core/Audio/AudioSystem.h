#pragma once

#include <miniaudio.h>
#include <SnowEngineAPI.h>

#include "Sound.h"

struct ma_engine;

namespace Snow
{
	class AudioSystem
	{
	public:
		static void Init();
		static void Shutdown();
		static void Play(Ref<Sound>& sound);
		static ma_engine& GetEngine() { return s_Engine; }
	private:
		static ma_engine s_Engine;
	};
};