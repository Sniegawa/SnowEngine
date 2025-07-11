#pragma once

#include <miniaudio.h>
#include <string>
#include <glm/glm.hpp>
namespace Snow
{

	enum class AttenuationModel
	{
		Inverse,
		Linear,
		Exponential
	};

	class Sound
	{
	public:
		Sound(const std::string& name,const std::string& filepath);
		ma_sound* GetSoundPointer() { return &m_Sound; }

		void Play();
		void Stop();

		float GetVolume();
		void SetVolume(const float volume);

		void SetPosition(const glm::vec2& position);
		const glm::vec2& GetPosition() const { return m_Position; }

		void SetPitch(const float pitch);
		float GetPitch();

		void SetNearRadius(const float NearRadius);
		float GetNearRadius();
		void SetFarRadius(const float FarRadius);
		float GetFarRadius();

		void SetAttenuationModel(AttenuationModel model);

		const std::string& GetName() const { return m_Name; }

		~Sound();

	private:
		ma_sound m_Sound;

		std::string m_Name;

		glm::vec2 m_Position;

		bool m_Loaded = false;
	};
};