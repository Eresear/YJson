#include "yjson.h"
#include <assert.h>  /* assert() */
#include <stdlib.h>  /* NULL, malloc(), realloc(), free(), strtod() */
#include <string.h>  /* memcpy() */
#include <errno.h>
#include <stddef.h> /* size_t */
#include <math.h>
#ifndef YJSON_PARSE_STACK_INIT_SIZE
#define YJSON_PARSE_STACK_INIT_SIZE 256
#endif
#define ISDIGIT(ch) ((ch) >='0' && (ch) <='9')
#define ISDIGIT1T09(ch)  ((ch) >= '1' && (ch) <= '9')
#define EXPECT(c,ch) do {assert(*c->json == (ch)) ; c->json++;} while(0)
#define PUTC(c, ch) do { *(char*)yjson_context_push(c, sizeof(char)) = (ch); } while(0)


typedef struct {
    const char* json;
    char* stack;
    size_t size,top;
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
    const char* p= c->json;
    if(*p =='-') p++;
    if(*p == '0') p++;
    else{
        if(!ISDIGIT1T09(*p)) return YJSON_PARSE_INVALID_VALUE;
        for(p++; ISDIGIT(*p) ;p++);
    }
    if(*p =='.'){
        p++;
        if(!ISDIGIT(*p)) return YJSON_PARSE_INVALID_VALUE;
        for(p++; ISDIGIT(*p) ;p++);
    }
    if(*p == 'e' || *p == 'E'){
        p++;
        if(*p == '+' || *p =='-') p++;
        if(!ISDIGIT(*p)) return YJSON_PARSE_INVALID_VALUE;
        for(p++; ISDIGIT(*p) ;p++);
    }


    errno = 0;
    v->u.n = strtod(c->json,NULL);
    if(errno ==  ERANGE && (v->u.n == HUGE_VAL || v->u.n == -HUGE_VAL))
        return YJSON_PARSE_NUMBER_TOO_BIG;
    c->json= p;
    v->type = YJSON_NUMBER;
    return YJSON_PARSE_OK;
        
}
static void* yjson_context_push(yjson_context* c,size_t size){
    void* ret;
    assert(size>0);
    if(c->top + size >=c->size){
        if(c->size == 0){
            c->size = YJSON_PARSE_STACK_INIT_SIZE;
        }
        while(c->top + size >= c->size)
            c->size += c->size >>1;
        c->stack = (char*) realloc(c->stack,c->size);
    }
    ret = c->stack + c->top;
    c->top +=size;
    return ret;
}
static void* yjson_context_pop(yjson_context*c , size_t size){
    assert(c->top >= size);
    return c->stack+ (c->top -=size);
}

static int yjson_parse_string(yjson_context* c, yjson_value* v) {
    size_t head = c->top, len;
    const char* p;
    EXPECT(c, '\"');
    p = c->json;
    for (;;) {
        char ch = *p++;
        switch (ch) {
            case '\\':
                switch (*p++) {
                    case '\"': PUTC(c, '\"'); break;
                    case '\\': PUTC(c, '\\'); break;
                    case '/':  PUTC(c, '/' ); break;
                    case 'b':  PUTC(c, '\b'); break;
                    case 'f':  PUTC(c, '\f'); break;
                    case 'n':  PUTC(c, '\n'); break;
                    case 'r':  PUTC(c, '\r'); break;
                    case 't':  PUTC(c, '\t'); break;
                    default:
                        c->top = head;
                        return YJSON_PARSE_INVALID_STRING_ESCAPE;
                }
                break;

            case '\"':
                len = c->top - head;
                yjson_set_string(v, (const char*)yjson_context_pop(c, len), len);
                c->json = p;
                return YJSON_PARSE_OK;
            case '\0':
                c->top = head;
                return YJSON_PARSE_MISS_QUOTATION_MARK;
            default:
                if((unsigned char) ch < 0x20){
                    c->top = head;
                    return YJSON_PARSE_INVALID_STRING_CHAR;
                }

                PUTC(c, ch);
        }
    }
}


void yjson_set_string(yjson_value* v, const char* s, size_t len){
    assert(v!=NULL && (s!=NULL ||len ==0));
    yjson_free(v);
    v->u.s.s = (char*)malloc(len+1);
    memcpy(v->u.s.s,s,len);
    v->u.s.s[len]='\0';
    v->u.s.len = len;
    v->type = YJSON_STRING;
    
}

void yjson_free(yjson_value* v){
    assert(v!= NULL);
    if(v->type == YJSON_STRING)
        free(v->u.s.s);
    v->type = YJSON_NULL;
}
int yjson_get_boolean(const yjson_value* v) {
    assert(v != NULL && (v->type == YJSON_TRUE || v->type == YJSON_FALSE));
    return v->type == YJSON_TRUE;
}

void yjson_set_boolean(yjson_value* v, int b) {
    yjson_free(v);
    if(b){
         v->type = YJSON_TRUE;
    }
    else{
        v->type = YJSON_FALSE;
    }
    
}

const char* yjson_get_string(const yjson_value* v) {
    assert(v != NULL && v->type == YJSON_STRING);
    return v->u.s.s;
}
size_t yjson_get_string_length(const yjson_value* v){
    assert(v != NULL && v->type == YJSON_STRING);
    return v->u.s.len;
}

static int yjson_parse_value(yjson_context *c , yjson_value *v ){
    switch(*c->json){
        case 'n': return yjson_parse_literal(c,v,"null",YJSON_NULL);
        case 't': return yjson_parse_literal(c,v,"true",YJSON_TRUE);
        case 'f': return yjson_parse_literal(c,v,"false",YJSON_FALSE);
        case '"': return yjson_parse_string(c,v);
        case '\0': return YJSON_PARSE_EXPECT_VALUE;
        default: return yjson_parse_number(c,v);
    }
}

int yjson_parse(yjson_value* v, const char* json) {
    yjson_context c;
    int ret;
    assert(v != NULL);
    c.json = json;
    c.stack = NULL;
    c.size = c.top = 0;
    v->type = YJSON_NULL;
    yjson_parse_whitespace(&c);
    if((ret = yjson_parse_value(&c,v))==YJSON_PARSE_OK){
        yjson_parse_whitespace(&c);
        if(*c.json !='\0'){
            v->type = YJSON_NULL;
            ret = YJSON_PARSE_ROOT_NOT_SINGULAR;
        }
            
    }

    assert(c.top ==0);
    free(c.stack);
    return ret;
    

}

yjson_type yjson_get_type(const yjson_value* v){
    assert(v !=NULL);
    return v->type;
}
double yjson_get_number(const yjson_value *v){
    assert(v !=NULL && v->type == YJSON_NUMBER);
    return v->u.n;
}
void yjson_set_number(yjson_value* v, double n) {
    yjson_free(v);
    v->u.n = n;
    v->type = YJSON_NUMBER;
}

