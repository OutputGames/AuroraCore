#if !defined(AUDIO_HPP)
#define AUDIO_HPP
#include "component.hpp"
#include "aal/aal.hpp"



class AURORA_API AudioPlayer : public Component
{
	CLASS_DECLARATION(AudioPlayer)

public:
	AudioPlayer(std::string&& initialValue) : Component(move(initialValue))
	{
	}

	AudioPlayer();

	void Update() override;

	void SetAudio(Sound* audio);

	nlohmann::json Serialize() override;
	void Load(nlohmann::json j) override;

	float Pitch = 1.f, Volume = 1.f;
	bool Loop=false;

	void Play();
	void Pause();
	void Stop();

	inline static AudioMgr* audioMgr = nullptr;

private:

	Sound* audio;

};

#endif // AUDIO_HPP
