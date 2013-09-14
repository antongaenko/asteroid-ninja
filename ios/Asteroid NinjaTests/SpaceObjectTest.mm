//
//  SpaceObjectTest.m
//  Asteroid Ninja
//
//  Created by Anton Gaenko on 14.09.13.
//  Copyright (c) 2013 Anton Gaenko. All rights reserved.
//

#import "SpaceObjectTest.h"
#import "SpaceObject.h"

@implementation SpaceObjectTest

SpaceObject *spObj;
ColorRGB color = {0.8, 0.5, 0.1};
Point2D geom[3] = {
    {-10, 0},
    {10, 0},
    {0, 10}};

- (void)setUp {
    [super setUp];
    spObj = new SpaceObject(1,geom,3);
    spObj->setColor(2, color);
}

- (void)tearDown {
    [super tearDown];
    delete spObj;
}

- (void)testFlatGeom {
    float g[spObj->getFlatGeomSize()];
    spObj->getFlatGeometryXY(g);
    STAssertEquals(g[0], -10.0f, nil);
    STAssertEquals(g[1], 0.0f, nil);
    STAssertEquals(g[2], 10.0f, nil);
    STAssertEquals(g[3], 0.0f, nil);
    STAssertEquals(g[4], 0.0f, nil);
    STAssertEquals(g[5], 10.0f, nil);
}

- (void)testFlatGeomWithScale {
    float g[spObj->getFlatGeomSize()];
    spObj->getFlatGeometryWithScale(g, 0.5, 0.2);
    STAssertEquals(g[0], -5.0f, nil);
    STAssertEquals(g[1], 0.0f, nil);
    STAssertEquals(g[2], 5.0f, nil);
    STAssertEquals(g[3], 0.0f, nil);
    STAssertEquals(g[4], 0.0f, nil);
    STAssertEquals(g[5], 2.0f, nil);
}

- (void)testFlatColor {
    float c[spObj->getFlatColorSize()];
    spObj->getFlatColorArray(c);
    STAssertEquals(c[0], color.r, nil);
    STAssertEquals(c[1], color.g, nil);
    STAssertEquals(c[2], color.b, nil);
    STAssertEquals(c[3], color.r, nil);
    STAssertEquals(c[4], color.g, nil);
    STAssertEquals(c[5], color.b, nil);
    STAssertEquals(c[6], color.r, nil);
    STAssertEquals(c[7], color.g, nil);
    STAssertEquals(c[8], color.b, nil);
}

@end
