#ifndef Test_ResourceLoader_H 
#define Test_ResourceLoader_H 

#include <cppunit/extensions/HelperMacros.h>
#include <stdio.h>
#include "resourceLoader.h"

class Test_ResourceLoader : public CppUnit::TestFixture {
				CPPUNIT_TEST_SUITE( Test_ResourceLoader ); 
				CPPUNIT_TEST( testConstructors );
				CPPUNIT_TEST_SUITE_END(); 
				
			public: 
				void setUp(); 
				void tearDown(); 
				
				void testConstructors();

}; 

#endif // Test_ResourceLoader_H 
