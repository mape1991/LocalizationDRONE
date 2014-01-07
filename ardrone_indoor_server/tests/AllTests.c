#include <stdio.h>

#include "../../ardrone_indoor_commons/cunit/CuTest.h"

extern void Test_full_retrieve_message_values(CuTest*);

void RunAllTests(void)
{
	CuString *output = CuStringNew();
	CuSuite* suite = CuSuiteNew();

   SUITE_ADD_TEST(suite, Test_full_retrieve_message_values);

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
