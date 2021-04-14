#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "barryjson.h"

static int main_ret = 0;
static int test_count = 0;
static int test_pass = 0;

#define EXPECT_EQ_BASE(equality, expect, actual, format)	\
	do {\
		test_count++; \
		if (equality)\
			test_pass++; \
		else {\
			fprintf(stderr, "%s:%d: expect " format " actual: " format "\n", __FILE__, __LINE__, expect, actual); \
			main_ret = 1;\
		}\
	} while (0)

#define EXPECT_EQ_INT(expect, actual) EXPECT_EQ_BASE((expect) == (actual), expect, actual, "%d")
#define EXPECT_EQ_DOUBLE(expect, actual) EXPECT_EQ_BASE((expect) == (actual), expect, actual, "%f")

#define TEST_ERROR(error, json)\
	do{\
		bary_value v;\
		v.type = BARY_FALSE;\
		EXPECT_EQ_INT(error, bary_parse(&v, json));\
		EXPECT_EQ_INT(BARY_NULL, bary_get_type(&v));\
	} while (0)

#define TEST_NUMBER(expect, json)\
	do{\
		bary_value v;\
		EXPECT_EQ_INT(BARY_PARSE_OK, bary_parse(&v, json));\
		EXPECT_EQ_INT(BARY_NUMBER, bary_get_type(&v));\
		EXPECT_EQ_DOUBLE(expect, bary_get_number(&v));\
	} while (0)

static void test_parse_null(){
	bary_value v;
	v.type = BARY_FALSE;
	EXPECT_EQ_INT(BARY_PARSE_OK, bary_parse(&v, " null"));
	EXPECT_EQ_INT(BARY_NULL, bary_get_type(&v));
}

static void test_parse_true(){
	bary_value v;
	v.type = BARY_FALSE;
	EXPECT_EQ_INT(BARY_PARSE_OK, bary_parse(&v, " true "));
	EXPECT_EQ_INT(BARY_TRUE, bary_get_type(&v));
}

static void test_parse_false(){
	bary_value v;
	v.type = BARY_TRUE;
	EXPECT_EQ_INT(BARY_PARSE_OK, bary_parse(&v, "  false "));
	EXPECT_EQ_INT(BARY_FALSE, bary_get_type(&v));
}

static void test(){
	test_parse_null();
	test_parse_true();
	test_parse_false();
}

int main(){
	char* str = "0.55556";
	char* endChar;
	double num = strtod(&str, &endChar);
	printf("%s", str);

	//test();
	//printf("%d/%d (%3.2f%%) passed\n", test_pass, test_count, test_pass * 100.0 / test_count);
	return 0;
}	