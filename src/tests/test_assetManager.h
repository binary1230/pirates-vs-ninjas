#ifndef Test_AssetManager_H 
#define Test_AssetManager_H 

#include <cppunit/extensions/HelperMacros.h>
#include <stdio.h>
#include "assetManager.h"

class Test_AssetManager : public CppUnit::TestFixture {
				CPPUNIT_TEST_SUITE( Test_AssetManager ); 
				CPPUNIT_TEST( testConstructors );
				CPPUNIT_TEST_SUITE_END(); 
				
			public: 
				void setUp(); 
				void tearDown(); 
				
				void testConstructors();

}; 

#endif // Test_AssetManager_H 
