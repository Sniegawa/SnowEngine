#include "MusicInstance.h"
#include "AudioSystem.h"

namespace Snow
{

	MusicInstance::MusicInstance(Ref<MusicAsset> asset, bool loop)
		: m_Asset(asset), m_Loop(loop)
	{
		auto Result = ma_sound_init_from_file(
			&AudioSystem::GetEngine(),
			asset->filePath.c_str(),
			MA_SOUND_FLAG_STREAM,
			nullptr,
			nullptr,
			&m_Music
		);
		SNOW_ASSERT(Result == MA_SUCCESS, "Couldn't load music from file");
	}

	MusicInstance::~MusicInstance()
	{
		ma_sound_uninit(&m_Music);
	}

	void MusicInstance::OnMusicEnd(void* pUserData, ma_sound* pMusic)
	{
		auto* self = static_cast<MusicInstance*>(pUserData);
		if (self->m_Loop)
		{
			//Restart streamed audio
			ma_sound_seek_to_pcm_frame(pMusic, 0);
			ma_sound_start(pMusic);
		}
		else
		{
			self->m_Finished = true;
		}
	}

	void MusicInstance::Play()
	{
		auto Result = ma_sound_start(&m_Music);
		SNOW_ASSERT(Result == MA_SUCCESS, "Couldn't play music");
		ma_sound_set_pitch(&m_Music, 2.0f);
		ma_sound_set_end_callback(&m_Music, &MusicInstance::OnMusicEnd, this);
	}

};