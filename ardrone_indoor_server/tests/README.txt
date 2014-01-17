PURPOSES
This folder contains all unit tests related to the src source folder.
It uses the CuTest source code in the ardrone_indoor_commons/cunit directory.

RULES
The main directory tree applies.

HOWTO ?
To create a new test, create a C source file with a filename starting with Test.
i.e. If I need to test a "print" function, the filename is TestPrint.c
Add it to the suite in the AllTests.c and define the test function as extern above AllTests.c