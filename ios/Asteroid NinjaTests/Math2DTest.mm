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
      Vector(0, 10, 1)}) * RotateMatrix(45, Degree) * TranslateMatrix(5, 5) * ScaleMatrix(1.0f / 50);

  STAssertEquals(isEqual(g[0], Vector(5, -9.1421356, 50)), true, nil);
  STAssertEquals(isEqual(g[1], Vector(19.1421356, 5, 50)), true, nil);
  STAssertEquals(isEqual(g[2], Vector(-2.0710678, 12.0710678, 50)), true, nil);

  g = Geometry<float, 3>((Vector[]) {
      Vector(-10, -10, 1),
      Vector(10, -10, 1),
      Vector(0, 10, 1)}).rotate(45, Degree).translate(5,5).scale(1.0f / 50);

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

// test bounds of free geometry
-(void)testRectangleBoundsOfGeometry {
  Vector geomArr[] = { Vector(-10, 10), Vector(-12, 0), Vector(-5, -5), Vector(0, -7), Vector(10, -2), Vector(15, 7) };
  auto g = Geometry<float, 6>(geomArr);
  auto b = getBounds<6>(g);
  STAssertEquals(b.getTopLeft().getX(), -12.0f, nil);
  STAssertEquals(b.getTopLeft().getY(), 10.0f, nil);
    STAssertEquals(b.getBottomRight().getX(), 15.0f, nil);
    STAssertEquals(b.getBottomRight().getY(), -7.0f, nil);
}

// test getting center of geometry
-(void)testCenterOfGeometry {
  // test for one point
  Vector oneGeomArr[] = { Vector(10.5, -1.2) };
  auto onePointGeom = Geometry<float, 1>(oneGeomArr);
  STAssertEquals(isEqual(Vector(10.5, -1.2), getCentroid(onePointGeom)), true, nil);
  
  // test for free geometry
  Vector geomArr[] = { Vector(-10, 10), Vector(-12, 0), Vector(-5, -5), Vector(0, -7), Vector(10, -2), Vector(15, 7) };
  auto g = Geometry<float, 6>(geomArr);
  auto c = getCentroid(g);
  STAssertEquals(isEqual(Vector(1.5, 1.5),c), true, nil);
}

// test algorithm to check if point is inside free polygon. It uses point orientation relatively each polygon side
-(void)testInsideOutsideWithFreeGeometry {
  const int Size = 6;
  Vector geomArr[] = { Vector(-10, 10), Vector(-12, 0), Vector(-5, -5), Vector(0, -7), Vector(10, -2), Vector(15, 7) };
  auto g = Geometry<float, Size>(geomArr);
  int orientationCenter, orientationInside, orientationOutside;
  
  // then check point which is inside geometry
  for (int i = 0; i < Size; i++) {
    orientationCenter = crossProduct2D(g[i % Size], g[(i+1) % Size], getCentroid(g));
    orientationInside = crossProduct2D(g[i % Size], g[(i+1) % Size], Vector(-9, 8));
    // orientation should have equal signs
    STAssertEquals(orientationCenter * orientationInside > 0, true, nil);
  }
  
  bool isDifferent = false;
  // then check point which is outside geometry
  for (int i = 0; i < Size; i++) {
    orientationCenter = crossProduct2D(g[i % Size], g[(i+1) % Size], getCentroid(g));
    orientationOutside = crossProduct2D(g[i % Size], g[(i+1) % Size], Vector(-10, -5));
    if (orientationCenter * orientationOutside < 0) {
      isDifferent = true;
      break;
    }
  }
  // orientation should be different for some sides
  STAssertEquals(isDifferent, true, nil);
  
  // then check point which is part of geometry
  for (int i = 0; i < Size; i++) {
    orientationCenter = crossProduct2D(g[i % Size], g[(i+1) % Size], getCentroid(g));
    orientationOutside = crossProduct2D(g[i % Size], g[(i+1) % Size], Vector(-5, -5));
    // orientation should have equal signs or be zero
    STAssertEquals(orientationCenter * orientationInside > 0, true, nil);
  }
}

@end
