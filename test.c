#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "yjson.h"

static int main_ret = 0;
static int test_count = 0;
static int test_pass = 0;

#define EXPECT_EQ_BASE(equality, expect, actual, format)                                                           \
    do                                                                                                             \
    {                                                                                                              \
        test_count++;                                                                                              \
        if (equality)                                                                                              \
            test_pass++;                                                                                           \
        else                                                                                                       \
        {                                                                                                          \
            fprintf(stderr, "%s:%d: expect: " format " actual: " format "\n", __FILE__, __LINE__, expect, actual); \
            main_ret = 1;                                                                                          \
        }                                                                                                          \
    } while (0)

#define EXPECT_EQ_INT(expect, actual) EXPECT_EQ_BASE((expect) == (actual), expect, actual, "%d")
#define EXPECT_EQ_DOUBLE(expect, actual) EXPECT_EQ_BASE((expect) == (actual), expect, actual, "%.17g")
#define EXPECT_EQ_STRING(expect, actual, alength) \
    EXPECT_EQ_BASE(sizeof(expect) - 1 == alength && memcmp(expect, actual, alength) == 0, expect, actual, "%s")



#define TEST_NUMBER(expect, json)                             \
    do                                                        \
    {                                                         \
        yjson_value v;                                        \
        EXPECT_EQ_INT(YJSON_PARSE_OK, yjson_parse(&v, json)); \
        EXPECT_EQ_INT(YJSON_NUMBER, yjson_get_type(&v));      \
        EXPECT_EQ_DOUBLE(expect, yjson_get_number(&v));       \
    } while (0)

#define TEST_ERROR(error, json)                        \
    do                                                 \
    {                                                  \
        yjson_value v;                                 \
        v.type = YJSON_FALSE;                           \
        EXPECT_EQ_INT(error, yjson_parse(&v, json));   \
        EXPECT_EQ_INT(YJSON_NULL, yjson_get_type(&v)); \
    } while (0)


static void test_parse_null() {
    yjson_value v;
    v.type = YJSON_FALSE;
    EXPECT_EQ_INT(YJSON_PARSE_OK, yjson_parse(&v, "null"));
    EXPECT_EQ_INT(YJSON_NULL, yjson_get_type(&v));
}

static void test_parse_true() {
    yjson_value v;
    v.type = YJSON_FALSE;
    EXPECT_EQ_INT(YJSON_PARSE_OK, yjson_parse(&v, "true"));
    EXPECT_EQ_INT(YJSON_TRUE, yjson_get_type(&v));
}

static void test_parse_false() {
    yjson_value v;
    v.type = YJSON_TRUE;
    EXPECT_EQ_INT(YJSON_PARSE_OK, yjson_parse(&v, "false"));
    EXPECT_EQ_INT(YJSON_FALSE, yjson_get_type(&v));
}

static void test_parse_number()
{
    TEST_NUMBER(0.0, "0");
    TEST_NUMBER(0.0, "-0");
    TEST_NUMBER(0.0, "-0.0");
    TEST_NUMBER(1.0, "1");
    TEST_NUMBER(-1.0, "-1");
    TEST_NUMBER(1.5, "1.5");
    TEST_NUMBER(-1.5, "-1.5");
    TEST_NUMBER(3.1416, "3.1416");
    TEST_NUMBER(1E10, "1E10");
    TEST_NUMBER(1e10, "1e10");
    TEST_NUMBER(1E+10, "1E+10");
    TEST_NUMBER(1E-10, "1E-10");
    TEST_NUMBER(-1E10, "-1E10");
    TEST_NUMBER(-1e10, "-1e10");
    TEST_NUMBER(-1E+10, "-1E+10");
    TEST_NUMBER(-1E-10, "-1E-10");
    TEST_NUMBER(1.234E+10, "1.234E+10");
    TEST_NUMBER(1.234E-10, "1.234E-10");
    TEST_NUMBER(0.0, "1e-10000"); /* must underflow */
    TEST_NUMBER(1.0000000000000002, "1.0000000000000002");
    /* minimum denormal */
    TEST_NUMBER( 4.9406564584124654e-324, "4.9406564584124654e-324");
    TEST_NUMBER(-4.9406564584124654e-324, "-4.9406564584124654e-324");
    /* Max subnormal double */
    TEST_NUMBER( 2.2250738585072009e-308, "2.2250738585072009e-308");
    TEST_NUMBER(-2.2250738585072009e-308, "-2.2250738585072009e-308");
    /* Min normal positive double */
    TEST_NUMBER( 2.2250738585072014e-308, "2.2250738585072014e-308");
    TEST_NUMBER(-2.2250738585072014e-308, "-2.2250738585072014e-308");
    /* Max double */
    TEST_NUMBER( 1.7976931348623157e+308, "1.7976931348623157e+308");
    TEST_NUMBER(-1.7976931348623157e+308, "-1.7976931348623157e+308");

}

#define TEST_STRING(expect, json)\
    do {\
        yjson_value v;\
        yjson_init(&v);\
        EXPECT_EQ_INT(YJSON_PARSE_OK, yjson_parse(&v, json));\
        EXPECT_EQ_INT(YJSON_STRING, yjson_get_type(&v));\
        EXPECT_EQ_STRING(expect, yjson_get_string(&v), yjson_get_string_length(&v));\
        yjson_free(&v);\
    } while(0)
    


static void test_parse_invalid_value()
{
    /* ... */
    /* invalid number */
    TEST_ERROR(YJSON_PARSE_INVALID_VALUE, "nul");
    TEST_ERROR(YJSON_PARSE_INVALID_VALUE, "?");
  
    TEST_ERROR(YJSON_PARSE_INVALID_VALUE, "+0");
    TEST_ERROR(YJSON_PARSE_INVALID_VALUE, "+1");
    TEST_ERROR(YJSON_PARSE_INVALID_VALUE, ".123"); /* at least one digit before '.' */
    TEST_ERROR(YJSON_PARSE_INVALID_VALUE, "1.");   /* at least one digit after '.' */
    TEST_ERROR(YJSON_PARSE_INVALID_VALUE, "INF");
    TEST_ERROR(YJSON_PARSE_INVALID_VALUE, "inf");
    TEST_ERROR(YJSON_PARSE_INVALID_VALUE, "NAN");
    TEST_ERROR(YJSON_PARSE_INVALID_VALUE, "nan");
  
}


static void test_parse_expect_value()
{
    TEST_ERROR(YJSON_PARSE_EXPECT_VALUE, "");
    TEST_ERROR(YJSON_PARSE_EXPECT_VALUE, " ");
}

static void test_parse_root_not_sigular()
{
    TEST_ERROR(YJSON_PARSE_ROOT_NOT_SINGULAR,  "null x");
    TEST_ERROR(YJSON_PARSE_ROOT_NOT_SINGULAR, "0123"); /* after zero should be '.' or nothing */
    TEST_ERROR(YJSON_PARSE_ROOT_NOT_SINGULAR, "0x0");
    TEST_ERROR(YJSON_PARSE_ROOT_NOT_SINGULAR, "0x123");
}

static void test_parse_number_too_big() {
    TEST_ERROR(YJSON_PARSE_NUMBER_TOO_BIG, "1e309");
    TEST_ERROR(YJSON_PARSE_NUMBER_TOO_BIG, "-1e309");
}
static void test_access_null() {
    yjson_value v;
    yjson_init(&v);
    yjson_set_string(&v, "a", 1);
    yjson_set_null(&v);
    EXPECT_EQ_INT(YJSON_NULL, yjson_get_type(&v));
    yjson_free(&v);
}
static void test_parse_missing_quotation_mark() {
    TEST_ERROR(YJSON_PARSE_MISS_QUOTATION_MARK, "\"");
    TEST_ERROR(YJSON_PARSE_MISS_QUOTATION_MARK, "\"abc");
}

static void test_parse_string() {
    TEST_STRING("", "\"\"");
    TEST_STRING("Hello", "\"Hello\"");

    TEST_STRING("Hello\nWorld", "\"Hello\\nWorld\"");
    TEST_STRING("\" \\ / \b \f \n \r \t", "\"\\\" \\\\ \\/ \\b \\f \\n \\r \\t\"");

}

static void test_access_string() {
    yjson_value v;
    yjson_init(&v);
    yjson_set_string(&v, "", 0);
    EXPECT_EQ_STRING("", yjson_get_string(&v), yjson_get_string_length(&v));
    yjson_set_string(&v, "Hello", 5);
    EXPECT_EQ_STRING("Hello", yjson_get_string(&v), yjson_get_string_length(&v));
    yjson_free(&v);
}


static void test_access_boolean() {
    yjson_value v;
    yjson_init(&v);
    yjson_set_boolean(&v, 1);
    EXPECT_EQ_INT(1, yjson_get_boolean(&v));
    yjson_set_boolean(&v, 0);
    EXPECT_EQ_INT(0, yjson_get_boolean(&v));
    yjson_free(&v);
}

static void test_access_number() {
    /* \TODO */
}

static void test_parse()
{
    test_parse_null();
    test_parse_true();
    test_parse_false();
    test_parse_expect_value();
    test_parse_number();
    test_parse_invalid_value();
    test_parse_root_not_sigular();
    test_parse_number_too_big;
    // test_parse_missing_quotation_mark();
        test_access_null();
    test_access_boolean();
    test_access_number();
    test_access_string();
    test_parse_string();
}

int main()
{
    test_parse();
    printf("%d/%d (%3.2f%%)passed \n", test_pass, test_count, test_pass * 100.0 / test_count);
    return main_ret;
}