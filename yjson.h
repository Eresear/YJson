#ifndef YJSON_H_
#define YJSON_H_

#include <stddef.h> /* size_t */

#define yjson_set_null(v) yjson_free(v)

typedef enum{ 
    YJSON_NULL,
    YJSON_FALSE,
    YJSON_TRUE,
    YJSON_NUMBER,
    YJSON_STRING,
    YJSON_ARRAY,
    YJSON_OBJECT

}   yjson_type;

typedef struct {
    union 
    {
        struct{ char*s ; size_t len;}s;
        double n;
    } u;
    
    yjson_type type;
} yjson_value;


enum {
    YJSON_PARSE_OK = 0,
    YJSON_PARSE_EXPECT_VALUE, // JSON 只含有空白 ,没有值
    YJSON_PARSE_INVALID_VALUE,// 无效字符
    YJSON_PARSE_ROOT_NOT_SINGULAR, // 一个值之后 ，除了空白字符，还有其他字符
    YJSON_PARSE_NUMBER_TOO_BIG,
    YJSON_PARSE_MISS_QUOTATION_MARK,
    YJSON_PARSE_INVALID_STRING_ESCAPE,
    YJSON_PARSE_INVALID_STRING_CHAR
};


#define yjson_init(v) do { (v)->type = YJSON_NULL; } while(0)


int yjson_parse(yjson_value* v, const char* json);

void yjson_free(yjson_value* v);

yjson_type yjson_get_type(const yjson_value* v);

#define yjson_set_null(v) yjson_free(v)
double yjson_get_number(const yjson_value* v);

int yjson_get_boolean(const yjson_value* v);
void yjson_set_boolean(yjson_value* v, int b);

double yjson_get_number(const yjson_value* v);
void yjson_set_number(yjson_value* v, double n);

const char* yjson_get_string(const yjson_value* v);
size_t yjson_get_string_length(const yjson_value* v);
void yjson_set_string(yjson_value* v, const char* s, size_t len);


#endif