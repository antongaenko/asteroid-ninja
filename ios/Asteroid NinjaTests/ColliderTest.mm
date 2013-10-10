//
//  ColliderTest.m
//  Asteroid Ninja
//
//  Created by Anton Gaenko on 07.10.13.
//  Copyright (c) 2013 Anton Gaenko. All rights reserved.
//

#import <SenTestingKit/SenTestingKit.h>
#import "LAlgebra.h"
#import "../../common/Collider.cpp"

using namespace lalgebra;

@interface ColliderTest : SenTestCase

@end

@implementation ColliderTest

- (void)setUp
{
    [super setUp];
    // Put setup code here; it will be run once, before the first test case.
}

- (void)tearDown
{
    // Put teardown code here; it will be run once, after the last test case.
    [super tearDown];
}

- (void)testCollisionDetection {
  Geometry g({Vector(5,3.5), Vector(6,5), Vector(7,4), Vector(7, 2), Vector(6, 1), Vector(4, 0), Vector(1, 1), Vector(0, 3), Vector(1,5), Vector(3,4), Vector(4,6)});
  // all them should be outside
  for (auto v : {Vector(0,4), Vector(3.5,6), Vector(5,4), Vector(1.58,0.54), Vector(7.35,3), Vector(-3,2.5)}) {
    STAssertEquals(Collider::isCollision(v, g), false, nil);
  }
  STAssertEquals(Collider::isCollision(Vector(0,4), g), false, nil);
  STAssertEquals(Collider::isCollision(Vector(3.5,6), g), false, nil);
  STAssertEquals(Collider::isCollision(Vector(5,4), g), false, nil);
  
  STAssertEquals(Collider::isCollision(Vector(1.0005,4), g), true, nil);
  
  STAssertEquals(Collider::isCollision(Vector(1,2), g), true, nil);
  
  
  STAssertEquals(Collider::isCollision(Vector(3.95,4.5), g), true, nil);
  
  STAssertEquals(Collider::isCollision(Vector(1.58,0.54), g), false, nil);
  
  STAssertEquals(Collider::isCollision(Vector(7.35,3), g), false, nil);
  
  STAssertEquals(Collider::isCollision(Vector(-3,2.5), g), false, nil);
  
  // at the edge
  STAssertEquals(Collider::isCollision(Vector(0.5, 2), g), true, nil);
  
  // at the vertex
  STAssertEquals(Collider::isCollision(Vector(6, 1), g), true, nil);

  
}

@end
