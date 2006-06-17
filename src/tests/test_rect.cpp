#include "test_rect.h"

//! Unit tests for Rect class

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( Test_Rect );

//! Test constructors
void Test_Rect::testConstructors() {
	Rect r;
	CPPUNIT_ASSERT(	r.getx1() == 0.0f && r.gety1() == 0.0f && 
									r.getx2() == 0.0f && r.gety2() == 0.0f );

	Rect r1(1.1f, 2.2f, 3.3f, 4.4f);
	CPPUNIT_ASSERT(	r1.getx1() == 1.1f && r1.gety1() == 2.2f && 
									r1.getx2() == 3.3f && r1.gety2() == 4.4f );

	Rect r2 = r1;
	CPPUNIT_ASSERT(r1 == r2);
	CPPUNIT_ASSERT(	r2.getx1() == 1.1f && r2.gety1() == 2.2f && 
									r2.getx2() == 3.3f && r2.gety2() == 4.4f );

	CPPUNIT_ASSERT(	r1.getx1() == 1.1f && r1.gety1() == 2.2f && 
									r1.getx2() == 3.3f && r1.gety2() == 4.4f );
}

//! Test Clear Function
void Test_Rect::testClear() {
	Rect r;
	r.setx1(3.0f); r.setx2(3.0f);
	r.sety1(3.0f); r.sety2(3.0f);
	r.Clear();

	CPPUNIT_ASSERT(	r.getx1() == 0.0f && r.gety1() == 0.0f && 
									r.getx2() == 0.0f && r.gety2() == 0.0f );
}

//! Test strict equality 
void Test_Rect::testOperatorEquals() {
	Rect r1, r2;
	int i;

	for (i = 0; i < count; i++) {
			r1.setx1(testing_set[i]);	
			r1.setx2(testing_set[i]);	
			r1.sety1(testing_set[i]);	
			r1.sety2(testing_set[i]);	

			r2.setx1(testing_set[i]);
			r2.setx2(testing_set[i]);	
			r2.sety1(testing_set[i]);	
			r2.sety2(testing_set[i]);	

			CPPUNIT_ASSERT(r1 == r2);
			CPPUNIT_ASSERT(r2 == r1);
	}
}

//! Test equality operator with a small offset
void Test_Rect::testOperatorEqualsFuzzy() {
	Rect r1, r2;
	int i;

	for (i = 0; i < count; i++) {
			r1.setx1(testing_set[i] + 0.00000001f);	
			r1.setx2(testing_set[i] + 0.00000001f);	
			r1.sety1(testing_set[i] + 0.00000001f);	
			r1.sety2(testing_set[i] + 0.00000001f);	

			r2.setx1(testing_set[i]);
			r2.setx2(testing_set[i]);	
			r2.sety1(testing_set[i]);	
			r2.sety2(testing_set[i]);	
			
			CPPUNIT_ASSERT(r1 == r2);
			CPPUNIT_ASSERT(r2 == r1);
	}
}

//! Test basic methods 
void Test_Rect::testGetSet() {
	Rect r;
	int i;
	float x1, y1, x2, y2;
	
	for (i = 0; i < count; i++) {
		x1 = testing_set[i];	r.setx1(x1);
		CPPUNIT_ASSERT_EQUAL(x1, r.getx1());
		
		y1 = testing_set[i];	r.sety1(y1);
		CPPUNIT_ASSERT_EQUAL(y1, r.gety1());
		
		x2 = testing_set[i];	r.setx2(x2);
		CPPUNIT_ASSERT_EQUAL(x2, r.getx2());
		
		y2 = testing_set[i];	r.sety2(y2);
		CPPUNIT_ASSERT_EQUAL(y2, r.gety2());
	}
}

//! INIT: Setup the tests
void Test_Rect::setUp() {

	static float set[] = {	0.0f, -1.0f, 1.0f, 5.0f, -5.0f, 0.00001f, 
									-0.00001f, 100000.0f, -100000.0f };

	testing_set = (float*)malloc(sizeof(set));
	memcpy(testing_set, set, sizeof(set));
	count = sizeof(set) / sizeof(float);
}

//! CLEANUP: Teardown the tests
void Test_Rect::tearDown() {
	free(testing_set);
}
