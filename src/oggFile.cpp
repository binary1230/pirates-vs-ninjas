#include "stdafx.h"

/*#include "oggFile.h"

int OGGFILE::Poll() {
	if (!initialized)
		return -1;

	char *data;
	long len;

	assert(s!=NULL);
	assert(f!=NULL);

	data = (char *)alogg_get_oggstream_buffer(s);
	if (data) { 
		len = pack_fread(data, DEFAULT_MUSIC_DATA_SIZE, f);
		if (len < DEFAULT_MUSIC_DATA_SIZE)
			alogg_free_oggstream_buffer(s, len);
		else
			alogg_free_oggstream_buffer(s, -1);
	}
    
	return alogg_poll_oggstream(s);
}

bool OGGFILE::Init(const char* file) {
	if (initialized) {
		TRACE("WARNING: Ogg File already loaded, "
										"and we're trying to load another one!\n");
		return false;
	}

	initialized = false;
  char data[DEFAULT_MUSIC_DATA_SIZE];
  int len;

	f = NULL;
	s = NULL;

  if (!(f = pack_fopen(file, F_READ)))
    return false;

  if ((len = pack_fread(data, DEFAULT_MUSIC_DATA_SIZE, f)) <= 0)
		goto error;
	
	assert(f!=NULL);

  if (len < DEFAULT_MUSIC_DATA_SIZE) {
    if (!(s = alogg_create_oggstream(data, len, TRUE)))
     	goto error;
  } else {
    if (!(s = alogg_create_oggstream(data, DEFAULT_MUSIC_DATA_SIZE, FALSE)))
      goto error;
	}

	assert(s!=NULL);
	initialized = true;
	return true;

	// DAC: Yes - GOTO. Not evil when used _correctly_
	// Don't believe me? Look in the linux kernel.
	error:
		if (f) {
			pack_fclose(f);
		}
		f = NULL;
		s = NULL;

	initialized = false;
	return false;
}

bool OGGFILE::Play(bool loop, int vol, int pan, int buflen) {
	if (!initialized) {
		return false;
	}

	assert(s!=NULL);
	assert(f!=NULL);

	alogg_play_oggstream(s, buflen, vol, pan);
	//if (alogg_play_ex_oggstream(s, buflen, vol, pan, loop) != ALOGG_OK) {
	//	return false;
	//}

	return true;
}

void OGGFILE::Shutdown() {	
	if (!initialized)
		return;

	if (f)
		pack_fclose(f);

	if (s) 
		alogg_destroy_oggstream(s);

	f = NULL;
	s = NULL;
	initialized = false;
}

void OGGFILE::Update() {
	if (!initialized)
		return;

	if (Poll() != ALOGG_OK) 
		TRACE(" - MUSIC: ERR: Poll failed.\n");
}

OGGFILE::OGGFILE() {
	initialized = false;
	f = NULL;
	s = NULL;
}

OGGFILE::~OGGFILE() {
	Shutdown();
}
*/