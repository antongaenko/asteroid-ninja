//
//  Math2DTest.mm
//  Asteroid Ninja
//
//  Created by Anton Gaenko on 14.09.13.
//  Copyright (c) 2013 Anton Gaenko. All rights reserved.
//

#import "Math2DTest.h"
#import "Math2D.h"

using namespace math2d;

// TODO Write cross-platfrom tests
@implementation Math2DTest

Matrix2 *one;
Matrix2 *two;
Vector *v1;
Vector *v2;
Matrix *m1;
Matrix *m2;


- (void)setUp {
  [super setUp];
  one = new Matrix2();
  two = new Matrix2();
  v1 = new Vector(10, -10, 1);
  v2 = new Vector(5, -5, 1);
  m1 = new Matrix(true);
  m2 = new Matrix(false);
}

- (void)tearDown {
  [super tearDown];
  delete one;
  delete two;
  delete v1;
  delete v2;
  delete m1;
  delete m2;
}

// Empty matrix (with all 0)
- (void)testEmptyMatrix2 {
  STAssertEquals((*one)[0][0], 0.0f, nil);
  STAssertEquals((*one)[0][1], 0.0f, nil);
  STAssertEquals((*one)[1][0], 0.0f, nil);
  STAssertEquals((*one)[1][1], 0.0f, nil);
}

// Identity matrix should have 1 for diagonal
- (void)testIdentityMatrix2 {
  one->setIdentity();
  STAssertEquals((*one)[0][0], 1.0f, nil);
  STAssertEquals((*one)[0][1], 0.0f, nil);
  STAssertEquals((*one)[1][0], 0.0f, nil);
  STAssertEquals((*one)[1][1], 1.0f, nil);
}

// Nil (with all 0) * Identity = Nil
- (void)testMultNil {
  one->setIdentity();
  Matrix2 r = *one * *two;
  STAssertEquals(r[0][0], 0.0f, nil);
  STAssertEquals(r[0][1], 0.0f, nil);
  STAssertEquals(r[1][0], 0.0f, nil);
  STAssertEquals(r[1][1], 0.0f, nil);
}

// Identity * Identity = Identity
- (void)testMultIdentities {
  one->setIdentity();
  two->setIdentity();
  Matrix2 r = *one * *two;
  STAssertEquals(r[0][0], 1.0f, nil);
  STAssertEquals(r[0][1], 0.0f, nil);
  STAssertEquals(r[1][0], 0.0f, nil);
  STAssertEquals(r[1][1], 1.0f, nil);
}

// Identity * Not Nil = Not Nil
- (void)testMultIdentityAndNotNil {
  one->setIdentity();
  (*two)[0][0] = 1;
  (*two)[0][1] = 2;
  (*two)[1][0] = 3;
  (*two)[1][1] = 4;
  Matrix2 r = *one * *two;
  STAssertEquals(r[0][0], 1.0f, nil);
  STAssertEquals(r[0][1], 2.0f, nil);
  STAssertEquals(r[1][0], 3.0f, nil);
  STAssertEquals(r[1][1], 4.0f, nil);
}

// Not Nil  * Not Nil = Not Nil
- (void)testMultNotNilAndNotNil {
  (*one)[0][0] = 1;
  (*one)[0][1] = 2;
  (*one)[1][0] = 3;
  (*one)[1][1] = 4;
  (*two)[0][0] = 1;
  (*two)[0][1] = 2;
  (*two)[1][0] = 3;
  (*two)[1][1] = 4;
  Matrix2 r = *one * *two;
  STAssertEquals(r[0][0], 7.0f, nil);
  STAssertEquals(r[0][1], 10.0f, nil);
  STAssertEquals(r[1][0], 15.0f, nil);
  STAssertEquals(r[1][1], 22.0f, nil);
}

// Test Vector3
- (void)testVector3 {
  Vector v3 = *v1;
  STAssertEquals(v3[0], v1->getX(), nil);
  STAssertEquals(v3[1], v1->getY(), nil);
  STAssertEquals(v3[2], v1->getZ(), nil);
  Vector v4 (v2->getX(), v2->getY(), v2->getZ());
  STAssertEquals(v4[0], v2->getX(), nil);
  STAssertEquals(v4[1], v2->getY(), nil);
  STAssertEquals(v4[2], v2->getZ(), nil);
  v4 = v3;
  STAssertEquals(v4[0], v1->getX(), nil);
  STAssertEquals(v4[1], v1->getY(), nil);
  STAssertEquals(v4[2], v1->getZ(), nil);
}

// Test SquareMatrix initialization
- (void)testInitSquareMatrix {
  FlatMatrix m = m1->flat();
  STAssertEquals(m1->getSize(), 3, nil);
  STAssertEquals(m.getSize(), 9, nil);
  for (int i = 0; i < m.getSize(); i++) {
    if (i == 0 || i == 4 || i == 8) STAssertEquals(m[i], 1.0f, nil);
    else STAssertEquals(m[i], 0.0f, nil);
  }

  m = m2->flat();
  for (int i = 0; i < m.getSize(); i++) {
    STAssertEquals(m[i], 0.0f, nil);
  }
}

// Test SquareMatrix access
- (void)testSquareMatrixAccess {
  Matrix &m = *m2;
  FlatMatrix a = m.flat();
  for (int i = 0; i < a.getSize(); i++) {
    STAssertEquals(a[i], 0.0f, nil);
  }

  // test flat matrix internal array
  for (int i = 0; i < a.getSize(); i++) {
    STAssertEquals(a.getArrayC()[i], 0.0f, nil);
  }
}

// Nil (with all 0) * Any = Nil
- (void)testMatrixNilMultiplication {
  Matrix &m = *m2;
  STAssertEquals((RotateMatrix(10) * m).flat(), m.flat(), nil);
}

// Any * Identity = Any
- (void)testMatrixIdentityMultiplication {
  Matrix &m = *m1;
  STAssertEquals((ScaleMatrix(10) * m).flat(), ScaleMatrix(10).flat(), nil);
}

// Vector * R * T * M = Vector
- (void)testVectorMatrixMultiplication {
  Vector v(-10, -10, 1);
  Vector result = v * RotateMatrix(90, Degree) * TranslateMatrix(5, 5) * ScaleMatrix(5);
  STAssertEquals(isEqual(result, Vector(15, -5, 0.2)), true, nil);
  result = v * RotateMatrix(45, Degree) * TranslateMatrix(5, 5) * ScaleMatrix(1.0f / 50);
  STAssertEquals(isEqual(result, Vector(5, -9.1421356, 50)), true, nil);
}

// Geometry * R * T * M = Geometry
- (void)testGeometryMatrixMultiplication {
  Vector v(-10, -10, 1);
  Vector result = v * RotateMatrix(90, Degree) * TranslateMatrix(5, 5) * ScaleMatrix(5);
  STAssertEquals(isEqual(result, Vector(15, -5, 0.2)), true, nil);
  result = v * RotateMatrix(45, Degree) * TranslateMatrix(5, 5) * ScaleMatrix(1.0f / 50);
  STAssertEquals(isEqual(result, Vector(5, -9.1421356, 50)), true, nil);

  Geometry<float, 3> g = Geometry<float, 3>((Vector[]) {
      Vector(-10, -10, 1),
      Vector(10, -10, 1),
      Vector(0, 10, 1)}) * (RotateMatrix(45, Degree) * TranslateMatrix(5, 5) * ScaleMatrix(1.0f / 50));
  STAssertEquals(isEqual(g[0], Vector(5, -9.1421356, 50)), true, nil);
  STAssertEquals(isEqual(g[1], Vector(19.1421356, 5, 50)), true, nil);
  STAssertEquals(isEqual(g[2], Vector(-2.0710678, 12.0710678, 50)), true, nil);

}

// Test Array
- (void)testArrayCopy {
  FlatMatrix a;
  // test bigger
  Array<float, 10> donor = Array<float, 10>();
  for (int i = 0; i < donor.getSize(); i++) {donor[i] = 1;}
  a = donor;

  for (int i = 0; i < a.getSize(); i++) {
    STAssertEquals(a[i], donor[i], nil);
  }

  // test smaller
  Array<float, 5> donor2 = Array<float, 5>();
  for (int i = 0; i < donor2.getSize(); i++) {donor2[i] = 2;}
  a = donor2;
  for (int i = 0; i < a.getSize(); i++) {
    STAssertEquals(a[i], i < donor2.getSize() ? donor2[i] : 0.0f, nil);
  }
}

@end
