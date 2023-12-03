#include <iomanip>

#include "../../aal/include/aal/aal.hpp"
#include "aurora/aurora.hpp"

int main(int argc, char* argv[])
{

	string cmd = "./movedlls.bat";

	cmd += "Debug ";
	cmd += "C:\\Users\\chris\\Downloads\\Aurora\\aal\\bin\\Debug";

	system(cmd.c_str());

	EntityMgr entityMgr;

	Ref<Entity> entity = entityMgr.CreateEntity("NewEntity");

	entity->Transform.position = glm::vec3{ 10,10,10 };

	entity->Transform.position *= glm::vec3{ 2 };

	TestComponent* testComponent = entity->AttachComponent<TestComponent>();

	AudioMgr* audioMgr = new AudioMgr();

	Sound* testSound = new Sound(new SoundCreateInfo{Sound::LoadSoundBuffer("testresources/trc.wav")});

	testSound->Play(audioMgr);

	bool close = false;

	while (!close)
	{
		testSound->Update();
		audioMgr->Update();
		entityMgr.UpdateAllEntities();
	}

	entity->Destroy();

	testSound->Destroy();

	audioMgr->Destroy();

	return 0;

}