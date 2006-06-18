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
	CPPUNIT_ASSERT(r2 == r1);
	CPPUNIT_ASSERT(	r2.getx1() == 1.1f && r2.gety1() == 2.2f && 
									r2.getx2() == 3.3f && r2.gety2() == 4.4f );

	CPPUNIT_ASSERT(	r1.getx1() == 1.1f && r1.gety1() == 2.2f && 
									r1.getx2() == 3.3f && r1.gety2() == 4.4f );

	// test the swapping via Rect::Fix()
	float d1 = 0.0f, d2 =  -20.0f, d3 = -1.0f, d4 = 20.0f;
	Rect r3(d1, d4, d3, d2);
	Rect r4 = r3;
	CPPUNIT_ASSERT(r4 == r3);
	CPPUNIT_ASSERT(r3 == r4);
	CPPUNIT_ASSERT(	r4.getx1() == d3 && r4.gety1() == d2 && 
									r4.getx2() == d1 && r4.gety2() == d4 );

	CPPUNIT_ASSERT(	r3.getx1() == d3 && r3.gety1() == d2 && 
									r3.getx2() == d1 && r3.gety2() == d4 );

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
	
	// test rectangle swap behavior
	// remember that Fix() in Rect::set_whatever() will make sure
	// that the rectangle always has its values as follows:
	// x1<x2 and y2<y1
	
	float d1 = 5.0f, d2 = 10.0f, d3 = 20.0f, d4 = 30.0f;
	r.Clear();

	// test where no swap should take place.
	r.setx1(d1); r.setx2(d2); r.Fix();
	CPPUNIT_ASSERT_EQUAL_MESSAGE("x1 should NOT have swapped", r.getx1(), d1);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("x2 should NOT have swapped", r.getx2(), d2);
	
	r.Clear();
	r.sety1(d3); r.sety2(d4); r.Fix();
	CPPUNIT_ASSERT_EQUAL_MESSAGE("y1 should NOT have swapped", r.gety1(), d3);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("y2 should NOT have swapped", r.gety2(), d4);

	r.setx1(d1); r.setx2(d2); r.sety1(d3); r.sety2(d4); r.Fix();
	CPPUNIT_ASSERT_EQUAL_MESSAGE("x1 should NOT have swapped", r.getx1(), d1);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("x2 should NOT have swapped", r.getx2(), d2);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("y1 should NOT have swapped", r.gety1(), d3);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("y2 should NOT have swapped", r.gety2(), d4);

	// test where X's should swap
	r.setx1(d2); r.setx2(d1); r.sety1(d3); r.sety2(d4); r.Fix();
	CPPUNIT_ASSERT_EQUAL_MESSAGE("x1 SHOULD have swapped", r.getx1(), d1);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("x2 SHOULD have swapped", r.getx2(), d2);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("y1 should NOT have swapped", r.gety1(), d3);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("y2 should NOT have swapped", r.gety2(), d4);
	
	// test where Y's should swap
	r.setx1(d1); r.setx2(d2); r.sety1(d4); r.sety2(d3); r.Fix();
	CPPUNIT_ASSERT_EQUAL_MESSAGE("x1 should NOT have swapped", r.getx1(), d1);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("x2 should NOT have swapped", r.getx2(), d2);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("y1 SHOULD have swapped", r.gety1(), d3);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("y2 SHOULD have swapped", r.gety2(), d4);
	
	// test where both X's and Y's should swap
	r.setx1(d2); r.setx2(d1); r.sety1(d4); r.sety2(d3); r.Fix();
	CPPUNIT_ASSERT_EQUAL_MESSAGE("x1 SHOULD have swapped", r.getx1(), d1);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("x2 SHOULD have swapped", r.getx2(), d2);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("y1 SHOULD have swapped", r.gety1(), d3);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("y2 SHOULD have swapped", r.gety2(), d4);
}

//! INIT: Setup the tests
void Test_Rect::setUp() {
}

//! CLEANUP: Teardown the tests
void Test_Rect::tearDown() {
}
