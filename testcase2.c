
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>

int myputs(const char* x){
	printf("%s\n",x);
	return 0;
}

int mynewputs(const char* x){
	printf("%s\n",x);
	return 0;
}

int myprintf(const char *format, ...){

	va_list args;
	va_start (args, format);
	int result = (int)write(1,"", 9);
	result += vprintf (format, args);
	va_end (args);
	return result;
}


#include "local_hook.h"
//LOCAL_HOOK(puts, myputs)

#define WRITE_TEST_OUTPUT(x) "\n[Testcheck]You should watch:\n"x"\n"
#define WRITE_TEST(x) write(1,WRITE_TEST_OUTPUT(x), sizeof(WRITE_TEST_OUTPUT(x)))

static void test_func1(){
	typeof(&puts) y = &puts;

	(*y)("1234567890");
	puts("0987654321");
	WRITE_TEST("1234567890\n0987654321\n");
}

static void test_func2(){
	//LOCAL_HOOK_START(puts, &mynewputs);
	typeof(&puts) z = &puts;

	(*z)("1234567890");
	puts("0987654321");
	WRITE_TEST("1234567890\n0987654321\n");
}
LOCAL_HOOK_INIT(puts)
LOCAL_HOOK_INIT(printf)
static void test_func3(){
	//LOCAL_HOOK_START(printf, &myprintf);
	typeof(&puts) z = &puts;

	(*z)("1234567890");
	puts("0987654321");
	printf("this is a pure printf:but gcc will implement it as puts\n");
	printf("%s%d\n","this is a pure printf",1);
	WRITE_TEST("1234567890\n0987654321\nthis is a pure printf:but gcc will implement it as puts\nthis is a pure printf1\n");
}

int main1(){
    LOCAL_HOOK_FUNCTIONPOINTER_SYNC(puts);
    LOCAL_HOOK_FUNCTIONPOINTER_SYNC(printf);
   // get_printf_plt_address ();
	test_func1();
	test_func2();
	test_func3();
   return 0;
}

