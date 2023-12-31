#include "audio.hpp"
#include "entity.hpp"

CLASS_DEFINITION(Component, AudioPlayer)

AudioPlayer::AudioPlayer()
{
	audio = nullptr;
	if (!audioMgr) {
		audioMgr = new AudioMgr;
	}
}

void AudioPlayer::Update()
{

	static bool UpdatedAudioMgr = false;

	if (!UpdatedAudioMgr)
	{
		audioMgr->Update();
		UpdatedAudioMgr = true;
	}

	audio->Update();
	audio->position = Entity->Transform.position;
	audio->pitch = Pitch;
	audio->gain = Volume;
	audio->loop = Loop;
}

void AudioPlayer::SetAudio(Sound* audio)
{
	this->audio = audio;
}

nlohmann::json AudioPlayer::Serialize()
{
	nlohmann::json j = Component::Serialize();

	j["Pitch"] = Pitch;
	j["Volume"] = Volume;
	j["Loop"] = Loop;

	return j;
}

void AudioPlayer::Load(nlohmann::json j)
{
	Pitch = j["Pitch"];
	Volume = j["Volume"];
	Loop = j["Loop"];
}

void AudioPlayer::Play()
{
	audio->Play(audioMgr);
}

void AudioPlayer::Pause()
{
}

void AudioPlayer::Stop()
{
}
