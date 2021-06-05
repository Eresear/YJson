#include "yjson.h"
#include <assert.h>  /* assert() */
#include <stdlib.h>  /* NULL */

#define ISDIGIT(ch) ((ch) >='0' && (ch) <='9')
#define ISDIGIT1T09(ch)  ((ch) >= '1' && (ch) <= '9')
#define EXPECT(c,ch) do {assert(*c->json == (ch)) ; c->json++;} while(0)

typedef struct {
    const char* json;
}yjson_context;





static void yjson_parse_whitespace(yjson_context* c){
    const char *p = c->json;
    while(*p == ' '|| *p == '\t' || *p == '\n' || *p =='\r')
        p++;
    c->json = p;
}
static int yjson_parse_literal(yjson_context*c,yjson_value* v,const char* literal,yjson_type type){
    size_t i;
    EXPECT(c,literal[0]);
    for(i=0; literal[i+1]; i++){
        if(c->json[i] !=literal[i+1])
            return YJSON_PARSE_INVALID_VALUE;
    }
    c->json +=i;
    v->type = type;

    return YJSON_PARSE_OK;
}

static int yjson_parse_number(yjson_context*c ,yjson_value*v){
    char* end;
    v->n = strtod(c->json,&end);
    if(c->json == end)
        return YJSON_PARSE_INVALID_VALUE;
    c->json= end;
    v->type = YJSON_NUMBER;
    return YJSON_PARSE_OK;
        
}
static int yjson_parse_value(yjson_context *c , yjson_value *v ){
    switch(*c->json){
        case 'n': return yjson_parse_literal(c,v,"null",YJSON_NULL);
        case 't': return yjson_parse_literal(c,v,"true",YJSON_TRUE);
        case 'f': return yjson_parse_literal(c,v,"false",YJSON_FALSE);
        case '\0': return YJSON_PARSE_EXPECT_VALUE;
        default: return yjson_parse_number(c,v);
    }
}

/* 提示：这里应该是 JSON-text = ws value ws，*/
/* 以下实现没处理最后的 ws 和 YJSON_PARSE_ROOT_NOT_SINGULAR */
int yjson_parse(yjson_value* v, const char* json) {
    yjson_context c;
    int ret;
    assert(v != NULL);
    c.json = json;
    v->type = YJSON_NULL;
    yjson_parse_whitespace(&c);
    if((ret = yjson_parse_value(&c,v))==YJSON_PARSE_OK){
        yjson_parse_whitespace(&c);
        if(*c.json !='\0')
            ret = YJSON_PARSE_ROOT_NOT_SINGULAR;
    }
    return ret;
    

}

yjson_type yjson_get_type(const yjson_value* v){
    assert(v !=NULL);
    return v->type;
}
double yjson_get_number(const yjson_value *v){
    assert(v !=NULL && v->type == YJSON_NUMBER);
    return v->n;
}

