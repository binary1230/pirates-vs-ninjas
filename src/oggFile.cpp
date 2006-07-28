// OGG reading funtions based on alogg's example.c

#include <assert.h>
#include "oggFile.h"

int OGGFILE::Poll() {
	char *data;
  long len;

	assert(s!=NULL);
	assert(f!=NULL);

  data = (char *)alogg_get_oggstream_buffer(s);
  if (data) { 
    len = pack_fread(data, DATASZ, f);
    if (len < DATASZ)
      alogg_free_oggstream_buffer(s, len);
    else
      alogg_free_oggstream_buffer(s, -1);
  }
    
  return alogg_poll_oggstream(s);
}

bool OGGFILE::Open(const char* file) {
  char data[DATASZ];
  int len;

  if (!(f = pack_fopen(file, F_READ)))
    return false;

  if ((len = pack_fread(data, DATASZ, f)) <= 0)
		goto error;
	
	assert(f!=NULL);

  if (len < DATASZ) {
    if (!(s = alogg_create_oggstream(data, len, TRUE)))
     	goto error;
  } else {
    if (!(s = alogg_create_oggstream(data, DATASZ, FALSE)))
      goto error;
	}

	assert(s!=NULL);
	return true;

	// DAC: Yes - GOTO. Not evil when used _correctly_
	// Don't believe me? Look in the linux kernel.
	error:
		if (f) 
			pack_fclose(f);
		f = NULL;
		s = NULL;

	return false;
}

bool OGGFILE::Play(bool loop, int vol, int pan, int buflen) {
	assert(s!=NULL);
	assert(f!=NULL);

	alogg_play_oggstream(s, buflen, vol, pan);
	//if (alogg_play_ex_oggstream(s, buflen, vol, pan, loop) != ALOGG_OK) {
	//	return false;
	//}

	return true;
}

void OGGFILE::Close() {
	if (f)
		pack_fclose(f);

	if (s)
		alogg_destroy_oggstream(s);

	f = NULL;
	s = NULL;
}

void OGGFILE::Update() {
	if (Poll() != ALOGG_OK) 
		fprintf(stderr, " - MUSIC: ERR: Poll failed.\n");
}

OGGFILE::OGGFILE() {
	f = NULL;
	s = NULL;
}

OGGFILE::~OGGFILE() {
	Close();
}
