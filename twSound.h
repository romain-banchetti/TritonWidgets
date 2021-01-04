#ifndef TWSOUND_H_INCLUDED
#define TWSOUND_H_INCLUDED

#include "AL/al.h"
#include "AL/alc.h"
#include "stb_vorbis/stb_vorbis.h"
#include "twString.h"

class twSound
{
	public:
		twSound(twString sound);
		virtual ~twSound();
		void play();
		void pause();
		void stop();
		void setLoop(bool loop);
		bool getLoop();
		void setVolume(int volume);
		int getVolume();
		int getDuration();
	private:
		twString m_path;
		stb_vorbis* m_sound;
		int m_duration;
		ALuint m_source;

};

#endif // TWSOUND_H_INCLUDED
