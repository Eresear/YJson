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
        EXPECT_EQ_INT(error, yjson_parse(&v, json));   \
        EXPECT_EQ_INT(YJSON_NULL, yjson_get_type(&v)); \
    } while (0)

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
}
static void test_parse_invalid_value()
{
    /* ... */
    /* invalid number */
    TEST_ERROR(YJSON_PARSE_INVALID_VALUE, "+0");
    TEST_ERROR(YJSON_PARSE_INVALID_VALUE, "+1");
    TEST_ERROR(YJSON_PARSE_INVALID_VALUE, ".123"); /* at least one digit before '.' */
    TEST_ERROR(YJSON_PARSE_INVALID_VALUE, "1.");   /* at least one digit after '.' */
    TEST_ERROR(YJSON_PARSE_INVALID_VALUE, "INF");
    TEST_ERROR(YJSON_PARSE_INVALID_VALUE, "inf");
    TEST_ERROR(YJSON_PARSE_INVALID_VALUE, "NAN");
    TEST_ERROR(YJSON_PARSE_INVALID_VALUE, "nan");
}

#define TEST_RETURN(return_type, json)                        \
    do                                                        \
    {                                                         \
        yjson_value v;                                        \
        EXPECT_EQ_INT(YJSON_PARSE_OK, yjson_parse(&v, json)); \
        EXPECT_EQ_INT(return_type, yjson_get_type(&v));       \
    } while (0)

static void test_parse_null()
{
    TEST_RETURN(YJSON_NULL, lept_parse(&v, "null"));
}
static void test_parse_true()
{
    TEST_RETURN(YJSON_TRUE, lept_parse(&v, "true"));
}
static void test_parse_false()
{
    TEST_RETURN(YJSON_FALSE, yjson_parse(&v, "false"));
}
static void test_parse_expect_value()
{
    TEST_ERROR(YJSON_PARSE_EXPECT_VALUE, "");
    TEST_ERROR(YJSON_PARSE_EXPECT_VALUE, " ");
}

static void test_parse_invalid_value()
{
    TEST_ERROR(YJSON_PARSE_INVALID_VALUE, "nul");
    TEST_ERROR(YJSON_PARSE_INVALID_VALUE, "?");
}
static void test_parse_root_not_sigular()
{
    TEST_ERROR(YJSON_PARSE_ROOT_NOT_SINGULAR, yjson_parse(&v, "null x"));
}

static void test_parse()
{
    test_parse_null();
    test_parse_true();
    test_parse_false();
    test_parse_expect_value();
    test_parse_invalid_value();
    test_parse_root_not_sigular();
}

int main()
{
    test_parse();
    printf("%d/%d (%3.2F%%)passed \n", test_pass, test_count, test_pass * 100.0 / test_count);
    return main_ret;
}