#ifndef GLOBALDEFINES_H
#define GLOBALDEFINES_H

/* This class is best illustrated by example:
 * Say you do this: 
 *
 * if (!GLOBALS->bool Value("gravity", myGravity)) {
 * 	myGravity = -9.81f;
 * }
 *
 * GLOBALS will look in params.xml for the tag <gravity>
 * and throw the value back into myGravity.  If that tag
 * doesn't exist, it will return false.
 *
 * This is a quick n dirty way to expose game params in XML
 * without having to do extra work.
 */

//! Implements a lookup table for generic global parameters
class GlobalDefines {
	DECLARE_SINGLETON_CLASS(GlobalDefines)

	protected:
		XMLNode xParams;		// contains the XML info

		bool initialized;

	public:
		bool Value(char* varName, int &out);
		bool Value(char* varName, float &out);
		bool Value(char* varName, bool &out);
		bool Value(char* varName, CString &out);

		bool Init(XMLNode &_xParams);
		void Shutdown();

		virtual ~GlobalDefines();
};

#define GLOBALS (GlobalDefines::GetInstance())

#endif // GLOBALDEFINES_H
