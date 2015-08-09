//
//  main.cpp
//  UnitTests
//
//  Created by Philip Cooper-King on 19/03/2014.
//  Copyright (c) 2014 negneg. All rights reserved.
//

#include <iostream>

#define CAFF_UNIT_TEST

//#include <Caffeine/Tests/EntityTest.hpp>
//#include <Caffeine/Tests/MathTest.hpp>
#include <Caffeine/Tests/StateTest.hpp>


int main(int argc, const char * argv[])
{
	Test::RunTests();

    return 0;
}

