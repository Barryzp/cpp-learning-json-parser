#ifndef BARRYJSON_H__
#define BARRYJSON_H__

typedef enum{
	BARY_NULL,
	BARY_TRUE,
	BARY_FALSE,
	BARY_NUMBER,
	BARY_STRING,
	BARY_ARRAY,
	BARY_OBJECT
}bary_type;

typedef struct{
	double n;
	bary_type type;
}bary_value;

enum{
	BARY_PARSE_OK = 0,
	BARY_PARSE_EXPECT_VALUE,
	BARY_PARSE_INVALID_VALUE,
	BARY_PARSE_ROOT_SINGULAR,
};

int bary_parse(bary_value* v, const char* json);

double bary_get_number(const bary_value* v);
bary_type bary_get_type(const bary_value* v);
#endif // !BARRYJSON_H__
