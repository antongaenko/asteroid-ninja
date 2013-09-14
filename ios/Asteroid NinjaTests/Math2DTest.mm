//
//  Math2DTest.mm
//  Asteroid Ninja
//
//  Created by Anton Gaenko on 14.09.13.
//  Copyright (c) 2013 Anton Gaenko. All rights reserved.
//

#import "Math2DTest.h"
#import "Math2D.h"

// TODO Write cross-platfrom tests
@implementation Math2DTest

Matrix2 *one;
Matrix2 *two;


- (void)setUp {
    [super setUp];
    one = new Matrix2();
    two = new Matrix2();
}

- (void)tearDown {
    [super tearDown];
    delete one;
    delete two;
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

@end
