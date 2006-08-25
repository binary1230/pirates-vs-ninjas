// Weird fix mostly for win32

#ifndef STL_WRAPPER_H
#define STL_WRAPPER_H

// putting 'using namespace std' results in 
// MSVC namespace collision with allegro
// so we can't do it.

#include <vector>
#include <map>
#include <list>
#include <algorithm>

/*typedef std::list list;
typedef std::map map;
typedef std::vector vector;*/

#endif // STL_WRAPPER_H