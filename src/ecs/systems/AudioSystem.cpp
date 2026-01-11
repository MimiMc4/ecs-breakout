#include <ecs/systems/AudioSystem.hpp>


void AudioSystem::Update(ComponentManager& cm) {
	auto& soundArray = cm.GetArray<Sound>();
	static bool end = false;
	if (cm.GetComponent<GameState>(ECS_Game) == GameState::GAME_WIN) {
		if (!end) {
			end = true;
			ma_sound* maSound = ResourceManager::GetSound("holo1");
			ma_sound_start(maSound);
		} else {
			return;
		}
	}
	for (auto& sound: soundArray) {
		ma_sound* maSound = ResourceManager::GetSound(sound.name);
		if (sound.pitchChanged) {
			sound.pitchChanged = false;
			ma_sound_stop(maSound);
			ma_sound_set_pitch(maSound, sound.pitch);
			ma_sound_seek_to_pcm_frame(maSound, 0);
		}
		if (sound.volumeChanged) {
			sound.volumeChanged = false;
			ma_sound_set_volume(maSound, sound.volume);
		}
		if (sound.play) {
			sound.play = false;
			ma_sound_stop(maSound);
			ma_sound_seek_to_pcm_frame(maSound, 0);
			ma_sound_start(maSound);
		}

	}
}