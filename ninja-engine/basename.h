#ifndef BASENAME_H
#define BASENAME_H

//! Returns a pointer to the last component of name
//! Behavior is undefined if the pathname ends in a directory separator.
char* basename (const char* name);

#endif // BASENAME_H
