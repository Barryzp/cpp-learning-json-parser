#include "barryjson.h"
#include "string.h"
#include "assert.h"
#include "stdlib.h"

#define EXPECT(c, ch) do { assert(*c->json == (ch)); c->json++; } while (0)

typedef struct{
	const char* json;
}bary_context;

static void bary_parse_whitespace(bary_context* c){
	const char *p = c->json;
	while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r'){
		p++;
	}
	c->json = p;
}

static int bary_parse_literal(bary_context *c, bary_value *v, const char* right_match, int parse_succ_type){
	size_t str_len = strlen(right_match);
	for (int i = 0; i < str_len; i++){
		if (c->json[i] != right_match[i]){
			return BARY_PARSE_INVALID_VALUE;
		}
	}

	c->json += str_len;
	v->type = parse_succ_type;
	return BARY_PARSE_OK;
}

static int bary_parse_null(bary_context* c, bary_value* v){
	EXPECT(c, 'n');
	if (c->json[0] != 'u' || c->json[1] != 'l' || c->json[2] != 'l'){
		return BARY_PARSE_INVALID_VALUE;
	}

	c->json += 3;
	v->type = BARY_NULL;
	return BARY_PARSE_OK;
}

static int bary_parse_true(bary_context* c, bary_value* v){
	EXPECT(c, 't');
	if (c->json[0] != 'r' || c->json[1] != 'u' || c->json[2] != 'e'){
		return BARY_PARSE_INVALID_VALUE;
	}

	c->json += 3;
	v->type = BARY_TRUE;
	return BARY_PARSE_OK;
}

static int bary_parse_false(bary_context* c, bary_value* v){
	EXPECT(c, 'f');
	if (c->json[0] != 'a' || c->json[1] != 'l' || c->json[2] != 's' || c->json[3] != 'e'){
		return BARY_PARSE_INVALID_VALUE;
	}

	c->json += 4;
	v->type = BARY_FALSE;
	return BARY_PARSE_OK;
}

static int bary_parse_number(bary_context* c, bary_value* v){
	char* end;
	/* 负数 */
	/* 整数 */
	/* 小数 */
	/* 指针 */
	v->n = strtod(c->json, &end);
	if (c->json == end){
		return BARY_PARSE_INVALID_VALUE;
	}
	c->json = end;
	v->type = BARY_NUMBER;
	return BARY_PARSE_OK;
}

int bary_parse_value(bary_context* c, bary_value* v){
	switch (*c->json)
	{
	case 'n':
		return bary_parse_literal(c, v, "null", BARY_NULL);
	case 't':
		return bary_parse_literal(c, v, "true", BARY_TRUE);
	case 'f':
		return bary_parse_literal(c, v, "false", BARY_FALSE);
	case '\0':
		return BARY_PARSE_EXPECT_VALUE;
	default:
		return bary_parse_number(c, v);
	}
}

int bary_parse(bary_value* v, const char* json){
	bary_context c;
	int ret;
	assert(v != NULL);
	c.json = json;
	v->type = BARY_NULL;
	bary_parse_whitespace(&c);
	if ((ret = bary_parse_value(&c, v)) == BARY_PARSE_OK){
		// 防止正确匹配之后后面还有其它未结束的字符
		bary_parse_whitespace(&c);
		if (*c.json != '\0'){
			ret = BARY_PARSE_ROOT_SINGULAR;
		}
	}
	return ret;
}

bary_type bary_get_type(const bary_value* v){
	assert(v != NULL);
	return v->type;
}

double bary_get_number(const bary_value* v){
	assert(v != NULL && v->type == BARY_NUMBER);
	return v->n;
}