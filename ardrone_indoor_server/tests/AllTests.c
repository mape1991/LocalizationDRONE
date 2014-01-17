#include <stdio.h>

#include "../../ardrone_indoor_commons/cunit/CuTest.h"

#include "tests/Test_test_full.h"

void RunAllTests(void)
{
	CuString *output = CuStringNew();
	CuSuite* suite = CuSuiteNew();

   SUITE_ADD_TEST(suite, Test_full_retrieve_message_values2);

   CuSuiteRun(suite);
   CuSuiteSummary(suite, output);
   CuSuiteDetails(suite, output);
   printf("%s\n", output->buffer);
   CuStringDelete(output);
   CuSuiteDelete(suite);
}

int main(void)
{
	RunAllTests();
}
