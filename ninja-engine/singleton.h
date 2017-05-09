#ifndef SINGLETON_H
#define SINGLETON_H

// Implements a singleton design pattern
// 
// Why we need this:
// Typically everything needs access to the global-ish
// classes like Game, Physics, Sound, and AssetManager
//
// But what a _pain_ it is to keep a pointer to these in all
// of our objects!  For instance, all the Objects need access to
// Sound, but we don't like storing a Sound* in every object because
// it's redundant.
//
// The other option is to make a global variable called Sound*, but
// GLOBAL VARIABLES ARE EVIL.
//
// The singleton design pattern, implemented here via a macro,
// allows you to ensure only one instance is created of a class 
// that you want everything to have.
//
// Furthermore, singleton's memory and things are handled by static
// members of the class itself, not by some global thing which is
// hard to maintain.
//
// Summary:
// - Class that wants to be single now can control its single instance
// - No global variables, but global access.
//
// To use:
// At the beginning of a class, use the macro DECLARE_SINGLETON.

// Include in the .cpp file
#define DECLARE_SINGLETON(CLASS) CLASS* CLASS::instance = NULL;

// Include in the .h file
#define DECLARE_SINGLETON_CLASS(CLASS)	\
																				\
	public: 															\
		static CLASS *GetInstance() { 			\
			return CLASS::instance; 					\
		};																	\
																				\
		static void SetInstance(CLASS * c) {\
			CLASS::instance = c;							\
		};																	\
																				\
		static void CreateInstance() {			\
			if (!CLASS::instance)							\
				CLASS::instance = new CLASS;		\
		};																	\
																				\
		static void FreeInstance() {				\
			SAFE_DELETE(CLASS::instance);			\
		};																	\
																				\
	private:															\
		static CLASS* instance;							\
																				\
	protected:														\
		CLASS();														\


#endif // SINGLETON_H
