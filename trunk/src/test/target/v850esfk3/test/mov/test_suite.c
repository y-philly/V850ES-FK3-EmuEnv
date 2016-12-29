#include "test_check.h"

extern int do_test_mov1_1(void);

void test_suite(void)
{

	DO_TEST(do_test_mov1_1);
#if 0
	ret = do_test_mov1_1();
	ret = do_test_mov1_2();
	ret = do_test_mov1_3();
	ret = do_test_mov1_4();

	ret = do_test_mov2_1();
	ret = do_test_mov2_2();
	ret = do_test_mov2_3();
	ret = do_test_mov2_4();
	ret = do_test_mov2_5();
#endif
	return;
}
