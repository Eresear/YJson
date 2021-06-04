#ifndef YJSON_H_
#define YJSON_H_
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
     double n;
    yjson_type type;
} yjson_value;


enum {
    YJSON_PARSE_OK = 0,
    YJSON_PARSE_EXPECT_VALUE, // JSON 只含有空白 ,没有值
    YJSON_PARSE_INVALID_VALUE,// 无效字符
    YJSON_PARSE_ROOT_NOT_SINGULAR // 一个值之后 ，除了空白字符，还有其他字符
};


int yjson_parse(yjson_value* v, const char* json);

yjson_type yjson_get_type(const yjson_value* v);


#endif