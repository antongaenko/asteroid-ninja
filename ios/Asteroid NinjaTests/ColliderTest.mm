//
//  ColliderTest.m
//  Asteroid Ninja
//
//  Created by Anton Gaenko on 07.10.13.
//  Copyright (c) 2013 Anton Gaenko. All rights reserved.
//

#import <SenTestingKit/SenTestingKit.h>
#import "Math2d.h"
#import "Collider.h"

using namespace math2d;

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
  Vector fvs[] =  {Vector(5,3.5), Vector(6,5), Vector(7,4), Vector(7, 2), Vector(6, 1), Vector(4, 0), Vector(1, 1), Vector(0, 3), Vector(1,5), Vector(3,4), Vector(4,6)};
  Geometry<float, 11> first(fvs);
  STAssertEquals(Collider::isCollision(Vector(0,4), first), false, nil);
  STAssertEquals(Collider::isCollision(Vector(3.5,6), first), false, nil); 
  STAssertEquals(Collider::isCollision(Vector(5,4), first), false, nil);
  
  STAssertEquals(Collider::isCollision(Vector(1.0005,4), first), true, nil);
  
  STAssertEquals(Collider::isCollision(Vector(1,2), first), true, nil);
  
  
  STAssertEquals(Collider::isCollision(Vector(3.95,4.5), first), true, nil);
  
  STAssertEquals(Collider::isCollision(Vector(1.58,0.54), first), false, nil);
  
  STAssertEquals(Collider::isCollision(Vector(7.35,3), first), false, nil);
  
  STAssertEquals(Collider::isCollision(Vector(-3,2.5), first), false, nil);
  
  // at the edge
  STAssertEquals(Collider::isCollision(Vector(0.5, 2), first), true, nil);
  
  // at the vertex
  STAssertEquals(Collider::isCollision(Vector(6, 1), first), true, nil);

  
}

@end
