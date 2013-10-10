//
//  Math2DTest.mm
//  Asteroid Ninja
//
//  Created by Anton Gaenko on 14.09.13.
//  Copyright (c) 2013 Anton Gaenko. All rights reserved.
//


#import <SenTestingKit/SenTestingKit.h>
#import "LAlgebra.h"

using namespace lalgebra;

@interface Math2DTest : SenTestCase

@end


// TODO Write cross-platfrom tests
@implementation Math2DTest

Vector *v1;
Vector *v2;
Matrix *m1;
Matrix *m2;


- (void)setUp {
  [super setUp];
  v1 = new Vector(10, -10, 1);
  v2 = new Vector(5, -5, 1);
  m1 = new Matrix(true);
  m2 = new Matrix(false);
}

- (void)tearDown {
  [super tearDown];
  delete v1;
  delete v2;
  delete m1;
  delete m2;
}

// Test SquareMatrix initialization
- (void)testInitSquareMatrix {
  std::vector<float> m = m1->flat();
  STAssertEquals(m1->getSize(), 3, nil);
  STAssertEquals((int) m.size(), 9, nil);
  for (int i = 0; i < m.size(); i++) {
    if (i == 0 || i == 4 || i == 8) STAssertEquals(m[i], 1.0f, nil);
    else STAssertEquals(m[i], 0.0f, nil);
  }

  m = m2->flat();
  for (int i = 0; i < m.size(); i++) {
    STAssertEquals(m[i], 0.0f, nil);
  }
}

// Test SquareMatrix access
- (void)testSquareMatrixAccess {
  Matrix &m = *m2;
  std::vector<float> a = m.flat();
  for (int i = 0; i < a.size(); i++) {
    STAssertEquals(a[i], 0.0f, nil);
  }
}

// Nil (with all 0) * Any = Nil
- (void)testMatrixNilMultiplication {
  Matrix &m = *m2;
  auto f = m.flat();
  auto f2 = (RotateMatrix(10) * m).flat();
  for (int i = 0; i < f.size(); i++) {
    STAssertEquals(f[i], f2[i], nil);
  }
}

// Any * Identity = Any
- (void)testMatrixIdentityMultiplication {
  Matrix &m = *m1;
  auto f = ScaleMatrix(10).flat();
  auto f2 = (ScaleMatrix(10) * m).flat();
  for (int i = 0; i < f.size(); i++) {
    STAssertEquals(f[i], f2[i], nil);
  }
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

  Geometry g = Geometry({{-10, -10, 1}, {10, -10, 1}, {0, 10, 1}}) *
      RotateMatrix(45, Degree) *
      TranslateMatrix(5, 5) *
      ScaleMatrix(1.0f / 50);

  STAssertEquals(isEqual(g[0], Vector(5, -9.1421356, 50)), true, nil);
  STAssertEquals(isEqual(g[1], Vector(19.1421356, 5, 50)), true, nil);
  STAssertEquals(isEqual(g[2], Vector(-2.0710678, 12.0710678, 50)), true, nil);

  g = Geometry({
      Vector(-10, -10, 1),
      Vector(10, -10, 1),
      Vector(0, 10, 1)}).rotate(45, Degree).translate(5,5).scale(1.0f / 50);

  STAssertEquals(isEqual(g[0], Vector(5, -9.1421356, 50)), true, nil);
  STAssertEquals(isEqual(g[1], Vector(19.1421356, 5, 50)), true, nil);
  STAssertEquals(isEqual(g[2], Vector(-2.0710678, 12.0710678, 50)), true, nil);
}

// test rectangle-point intersection
- (void)testRectangleInsideOutside {
  Rectangle r(Vector(-10, 10), Vector(10,-10));
  STAssertEquals(r.isInside(Vector(0,0)), true, nil);
  STAssertEquals(r.isInside(Vector(5,5)), true, nil);
  STAssertEquals(r.isInside(Vector(-5,-5)), true, nil);
  STAssertEquals(r.isOutside(Vector(-11,0)), true, nil);
  STAssertEquals(r.isOutside(Vector(0,11)), true, nil);
  STAssertEquals(r.isOutside(Vector(11,0)), true, nil);
  STAssertEquals(r.isOutside(Vector(0,-11)), true, nil);
  // check the edges
  STAssertEquals(r.isInside(Vector(10,10)), true, nil);
  STAssertEquals(r.isInside(Vector(-10,10)), true, nil);
  STAssertEquals(r.isInside(Vector(-10,-10)), true, nil);
  STAssertEquals(r.isInside(Vector(10,-10)), true, nil);
}

// test rectangle-rectangle intersection
- (void)testRectanglesIntersection {
  Rectangle r(Vector(-10, 10), Vector(10,-10));
  Rectangle r2(Vector(-5,5), Vector(15,-15));
  STAssertEquals(r.isIntersected(r2), true, nil);
  // and on the contrary
  STAssertEquals(r2.isIntersected(r), true, nil);

  Rectangle r3(Vector(11,10), Vector(31,-15));
  STAssertEquals(r.isIntersected(r3), false, nil);
  // and on the contrary
  STAssertEquals(r3.isIntersected(r), false, nil);

  // check an intersection by a vertex
  Rectangle r4(Vector(10,10), Vector(30,-15));
  STAssertEquals(r.isIntersected(r4), true, nil);
  // and on the contrary
  STAssertEquals(r4.isIntersected(r), true, nil);
}

// test rectangle widht and height
- (void)testRectangleDimensions {
  Rectangle r(Vector(-10, 10), Vector(10,-10));
  STAssertEquals(r.getWidth(), 20, nil);
  STAssertEquals(r.getHeight(), 20, nil);
  
  Rectangle r2(Vector(-10, 10), Vector(-5,5));
  STAssertEquals(r2.getWidth(), 5, nil);
  STAssertEquals(r2.getHeight(), 5, nil);
}

@end
