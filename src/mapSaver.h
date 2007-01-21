/* Experimental map editor : Object Saver
 *
 * This class will be responsible for knowing how to save all
 * everything in the game to XML format.  This stuff _should_
 * go into the object classes themselves, but I don't want to
 * clutter them with tons of XML crap.
 *
 */

#ifndef MAPSAVER_H
#define MAPSAVER_H

#include "precompiled.h"

class GameWorld;
class ObjectLayer;
class Object;

class MapSaver {

	public:

		// Save everything in this physics simulation
		// to the already named XML file.
		bool SaveEverything(	const GameWorld* p, 
													const CString file, 
													XMLNode& xObjDefs);

		MapSaver();
		~MapSaver();

	protected:
		const GameWorld* simulation;

		void OutputLayers(XMLNode &xMap);
		void OutputLayer(const ObjectLayer* layer, XMLNode &xLayer);
		void OutputObject(const Object* obj, XMLNode &xObj);
};

#endif // MAPSAVER_H
