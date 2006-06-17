#include "test_vector2D.h"

//! Unit tests for Vector2D class

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( Test_Vector2D );

//! Test constructors
void Test_Vector2D::testConstructors() {
	Vector2D v;
	CPPUNIT_ASSERT(v.GetX() == 0.0f && v.GetY() == 0.0f);
	Vector2D v1(1.1f, 2.2f);
	CPPUNIT_ASSERT(v1.GetX() == 1.1f && v1.GetY() == 2.2f);
	Vector2D v2 = v1;
	CPPUNIT_ASSERT(v1 == v2);
}

//! Test Clear Function
void Test_Vector2D::testClear() {
	Vector2D v;
	v.SetX(3.0f);
	v.SetY(3.0f);
	v.Clear();
	CPPUNIT_ASSERT(v.GetX() == 0.0f && v.GetY() == 0.0f);
}

//! Test += operator
void Test_Vector2D::testOperatorPlusEquals() {
	Vector2D v1, v2, v_expected;
	
	v1.SetX(-1.0f);	v1.SetY(2.0f);
	v2.SetX(-1.0f);	v2.SetY(2.0f);
	v_expected.SetX(-2.0f);	v_expected.SetY(4.0f);

	v2 += v1;

	CPPUNIT_ASSERT(v2 == v_expected);
	CPPUNIT_ASSERT(v2.GetX() == -2.0f && v2.GetY() == 4.0f);
}

//! Test -= operator
void Test_Vector2D::testOperatorMinusEquals() {
	Vector2D v1, v2, v_expected;
	
	v1.SetX(2.0f);	v1.SetY(-2.0f);
	v2.SetX(1.0f);	v2.SetY(2.0f);
	v_expected.SetX(-1.0f);	v_expected.SetY(4.0f);

	v2 -= v1;

	CPPUNIT_ASSERT(v2 == v_expected);
	CPPUNIT_ASSERT(v2.GetX() == -1.0f && v2.GetY() == 4.0f);
}

//! Test addition
void Test_Vector2D::testOperatorPlus() {
	Vector2D v1, v2, v_result, v_expected;
	
	v1.SetX(1.0f);	v1.SetY(2.0f);
	v2.SetX(1.0f);	v2.SetY(2.0f);
	v_expected.SetX(2.0f);	v_expected.SetY(4.0f);

	v_result = v1 + v2;

	CPPUNIT_ASSERT(v_result == v_expected);
	CPPUNIT_ASSERT(v_result.GetX() == 2.0f && v_result.GetY() == 4.0f);
}

//! Test subtraction
void Test_Vector2D::testOperatorMinus() {
	Vector2D v1, v2, v_result, v_expected;
	
	v1.SetX(3.0f);	v1.SetY(4.0f);
	v2.SetX(1.0f);	v2.SetY(-4.0f);
	v_expected.SetX(2.0f);	v_expected.SetY(8.0f);

	v_result = v1 - v2;

	CPPUNIT_ASSERT(v_result == v_expected);
	CPPUNIT_ASSERT(v_result.GetX() == 2.0f && v_result.GetY() == 8.0f);
}

//! Test multiplication
void Test_Vector2D::testOperatorMultiply() {
	Vector2D v1, v_result, v_expected;
	
	v1.SetX(1.0f);	v1.SetY(2.0f);
	float x = 2.0f;
	v_expected.SetX(2.0f);	v_expected.SetY(4.0f);

	v_result = v1 * x;

	CPPUNIT_ASSERT(v_result == v_expected);
	CPPUNIT_ASSERT(v_result.GetX() == 2.0f && v_result.GetY() == 4.0f);
}

//! Test division
void Test_Vector2D::testOperatorDivide() {
	Vector2D v1, v_result, v_expected;
	
	v1.SetX(2.0f);	v1.SetY(4.0f);
	float x = 2.0f;
	v_expected.SetX(1.0f);	v_expected.SetY(2.0f);

	v_result = v1 / x;

	CPPUNIT_ASSERT(v_result == v_expected);
	CPPUNIT_ASSERT(v_result.GetX() == 1.0f && v_result.GetY() == 2.0f);
}

//! Test strict equality 
void Test_Vector2D::testOperatorEquals() {
	Vector2D v1, v2;
	int i;

	for (i = 0; i < count; i++) {
			v1.SetX(testing_set[i]);	
			v1.SetY(testing_set[i]);
			
			v2.SetX(testing_set[i]);	
			v2.SetY(testing_set[i]);

			CPPUNIT_ASSERT(v1 == v2);
			CPPUNIT_ASSERT(v2 == v1);
	}
}

//! Test equality operator with a small offset
void Test_Vector2D::testOperatorEqualsFuzzy() {
	Vector2D v1, v2;
	int i;

	for (i = 0; i < count; i++) {
		v1.SetX(testing_set[i]);	
		v1.SetY(testing_set[i]);
		
		v2.SetX(testing_set[i] + 0.00000001f); 
		v2.SetY(testing_set[i] + 0.00000001f);
		
		CPPUNIT_ASSERT(v1 == v2);
		CPPUNIT_ASSERT(v2 == v1);
	}
}

//! Test basic methods 
void Test_Vector2D::testGetSet() {
	Vector2D v;
	int i;
	float x, y;
	
	for (i = 0; i < count; i++) {
		x = testing_set[i];
		v.SetX(x);
		CPPUNIT_ASSERT_EQUAL(x, v.GetX());
		
		y = testing_set[i];
		v.SetY(y);
		CPPUNIT_ASSERT_EQUAL(y, v.GetY());
	}
}

//! Test the dot product
void Test_Vector2D::testDotProduct() {
	Vector2D v1(12.0f, 11.0f);
	Vector2D v2(14.0f, 13.0f);
	float dot_expected = (v1.GetX() * v2.GetX()) + (v1.GetY() * v2.GetY());
	float dot_result = v1.Dot(v2);
	CPPUNIT_ASSERT_EQUAL( dot_expected, dot_result );
}

//! INIT: Setup the tests
void Test_Vector2D::setUp() {
	static float set[] = {	0.0f, -1.0f, 1.0f, 5.0f, -5.0f, 0.00001f, 
									-0.00001f, 100000.0f, -100000.0f };

	testing_set = (float*)malloc(sizeof(set));
	memcpy(testing_set, set, sizeof(set));
	count = sizeof(set) / sizeof(float);
}

//! CLEANUP: Teardown the tests
void Test_Vector2D::tearDown() {
	free(testing_set);
}
