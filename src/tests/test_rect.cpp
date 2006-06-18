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

void Test_Rect::testDoesNotOverlap() {
	// x1,y1 x2,y2
	Rect r(10.0f, 10.0f, 20.0f, 20.0f);

	Rect r1(0.0f, 1.0f, 5.0f, 2.0f);
	Rect r2(10.0f, 1.0f, 20.0f, 2.0f);
	Rect r3(21.0f, 1.0f, 30.0f, 2.0f);

	Rect r4(0.0f, 10.0f, 5.0f, 20.0f);
	// Rect r5(10.0f, 10.0f, 20.0f, 20.0f);
	Rect r6(21.0f, 10.0f, 30.0f, 20.0f);

	Rect r7(0.0f, 21.0f, 5.0f, 30.0f);
	Rect r8(10.0f, 21.0f, 20.0f, 30.0f);
	Rect r9(21.0f, 21.0f, 30.0f, 30.0f);
	
	CPPUNIT_ASSERT(!r.Overlaps(r1));
	CPPUNIT_ASSERT(!r.Overlaps(r2));
	CPPUNIT_ASSERT(!r.Overlaps(r3));
	CPPUNIT_ASSERT(!r.Overlaps(r4));
	CPPUNIT_ASSERT(!r.Overlaps(r6));
	CPPUNIT_ASSERT(!r.Overlaps(r7));
	CPPUNIT_ASSERT(!r.Overlaps(r8));
	CPPUNIT_ASSERT(!r.Overlaps(r9));
}


void Test_Rect::testOverlap() {
	Rect r(-1.0f, -1.0f, 1.0f, 1.0f);

	#define IN1  -0.5f
	#define OUT1 -1.5f
	#define IN2   0.5f
	#define OUT2  1.5f

	#define TEST1(y1, y2) OUT1, y1, OUT2, y2
	#define TEST2(y1, y2) IN1, y1, OUT2, y2
	#define TEST3(y1, y2) IN1, y1, IN2, y2
	#define TEST4(y1, y2) OUT1, y1, IN2, y2
	
	Rect r1(TEST1(OUT1, OUT2));
	Rect r2(TEST2(OUT1, OUT2));
	Rect r3(TEST3(OUT1, OUT2));
	Rect r4(TEST4(OUT1, OUT2));

	Rect r5(TEST1(OUT1, IN2));
	Rect r6(TEST2(OUT1, IN2));
	Rect r7(TEST3(OUT1, IN2));
	Rect r8(TEST4(OUT1, IN2));

	Rect r9(TEST1(IN1, IN2));
	Rect rA(TEST2(IN1, IN2));
	Rect rB(TEST3(IN1, IN2));
	Rect rC(TEST4(IN1, IN2));

	Rect rD(TEST1(IN1, OUT2));
	Rect rE(TEST2(IN1, OUT2));
	Rect rF(TEST3(IN1, OUT2));
	Rect rG(TEST4(IN1, OUT2));

	/*Rect r1(OUT1, OUT1, OUT2, OUT2);
  Rect r2(IN1,  OUT1, OUT2, OUT2);
  Rect r3(IN1,  OUT1, IN2,  OUT2);
  Rect r4(OUT1, OUT1, IN2,  OUT2);

  Rect r5(OUT1, OUT1, OUT2, IN2);
  Rect r6(IN1,  OUT1, OUT2, IN2);
  Rect r7(IN1,  OUT1, IN2,  IN2);
  Rect r8(OUT1, OUT1, IN2,  IN2);

  Rect r9(OUT1, IN1, OUT2, IN2);
  Rect rA(OUT1, IN1, OUT2, IN2);
  Rect rB(OUT1, IN1, OUT2, IN2);
  Rect rC(OUT1, IN1, OUT2, IN2);

  Rect rD(OUT1, IN1, OUT2, OUT2);
  Rect rE(OUT1, IN1, OUT2, OUT2);
  Rect rF(OUT1, IN1, OUT2, OUT2);
  Rect rG(OUT1, IN1, OUT2, OUT2);*/

	CPPUNIT_ASSERT(r.Overlaps(r1));
	CPPUNIT_ASSERT(r.Overlaps(r2));
	CPPUNIT_ASSERT(r.Overlaps(r3));
	CPPUNIT_ASSERT(r.Overlaps(r4));
	CPPUNIT_ASSERT(r.Overlaps(r5));
	CPPUNIT_ASSERT(r.Overlaps(r6));
	CPPUNIT_ASSERT(r.Overlaps(r7));
	CPPUNIT_ASSERT(r.Overlaps(r8));
	CPPUNIT_ASSERT(r.Overlaps(r9));
	CPPUNIT_ASSERT(r.Overlaps(rA));
	CPPUNIT_ASSERT(r.Overlaps(rB));
	CPPUNIT_ASSERT(r.Overlaps(rC));
	CPPUNIT_ASSERT(r.Overlaps(rD));
	CPPUNIT_ASSERT(r.Overlaps(rE));
	CPPUNIT_ASSERT(r.Overlaps(rF));
	CPPUNIT_ASSERT(r.Overlaps(rG));
}

//! INIT: Setup the tests
void Test_Rect::setUp() {
}

//! CLEANUP: Teardown the tests
void Test_Rect::tearDown() {
}
