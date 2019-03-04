#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

const int ERR_EOF = 1; /* End of file */
const int ERR_UT = 2; /* Unexpected token */

const int TYPE_INTEGER = 1;
const int TYPE_STRING = 2;
const int TYPE_OBJECT = 3;
const int TYPE_ARRAY = 4;

/* 错误号 */
int error = 0;

void printError(int err) {
    if (err == 0) {
        /* do noting */
    } else if (err == ERR_EOF) {
        printf("Unexpected end of file\n");
    } else if (err == ERR_UT) {
        printf("Unexpected token \"%c\"\n", getchar());
    } else {
        printf("Unkown error code: %d\n", error);
    }
}

/* 跳过多个空白符 */
void skipSpace() {
    int c;
    for (;;) {
        c = getchar();
        if (c == -1) {
            error = ERR_EOF;
            break;
        } else if (!isspace(c)) { /* isspace判断是否是空白符，在ctype.h中 */
            ungetc(c, stdin);
            break;
        }
    }
}

/* 预读取下一个字符 */
int peek() {
    int c = getchar();
    ungetc(c, stdin); /* 向输入流返向插入一个字符，在stdio.h中 */
    return c;
}

/* 根据下一个字符预测数据类型 */
int predictType() {
    int c = peek();
    int result = 0;
    if (c == EOF) {
        error = ERR_EOF;
    } else if (c == '-' || (c >= '0' && c <= '9')) {
        result = TYPE_INTEGER;
    } else if (c == '\"') {
        result = TYPE_STRING;
    } else if (c == '{') {
        result = TYPE_OBJECT;
    } else if (c == '[') {
        result = TYPE_ARRAY;
    } else {
        error = ERR_UT;
    }
    return result;
}

/* 强制期望特定字符 */
int expected(char target) {
    int c = getchar();
    if (c == EOF) {
        error = ERR_EOF;
    } else if (c != target) {
        error = ERR_UT;
    }
    return error;
}

/* 测试下一个字符是否是目标字符 */
int tryChar(char target) {
    int c = peek();
    int result = 0;
    if (c == EOF) {
        error = ERR_EOF;
        result = 0;
    } else if (c == target) {
        result = 1;
    }
    return result;
}

/* 解析字符串 */
void parseRawString(char* p) {
    int c;
    if (expected('\"')) {
        return;
    }
    for (;;) {
        c = getchar();
        if (c == EOF) {
            error = EOF;
            break;
        } else if (c == '\"') {
            *p = '\0';
            break;
        }
        *p = (char) c;
        p++;
    }
}

/* 解析整数 */
void parseInteger(int inObject, char* key) {
    int n;
    int flag = scanf("%d", &n);
    if (flag == EOF) {
        error = ERR_EOF;
    } else {
        if (inObject) {
            printf("INTEGER %s %d\n", key, n);
        } else {
            printf("INTEGER %d\n", n);
        }
    }
}

/* 解析字符串 */
void parseString(int inObject, char* key) {
    char str[120];
    parseRawString(str);
    if (!error) {
        if (inObject) {
            printf("STRING %s %s\n", key, str);
        } else {
            printf("STRING %s\n", str);
        }
    }
}

/* 解析对象 */
void parseObject(int inObject, char* key);

/* 解析数组 */
void parseArray(int inObject, char* key);

/* 解析值 */
void parseValue(int inObject, char* key) {
    int type = predictType();
    if (error) {
        return;
    }
    if (type == 0) {
        error = ERR_UT;
    } else if (type == TYPE_INTEGER) {
        parseInteger(inObject, key);
    } else if (type == TYPE_STRING) {
        parseString(inObject, key);
    } else if (type == TYPE_OBJECT) {
        parseObject(inObject, key);
    } else if (type == TYPE_ARRAY) {
        parseArray(inObject, key);
    } else {
        error = ERR_UT;
    }
}

/* 解析对象 */
void parseObject(int inObject, char* pkey) {
    int status = 0;
    int c;
    char key[120];
    if (inObject) {
        printf("OBJECT_BEGIN %s\n", pkey);
    } else {
        printf("OBJECT_BEGIN\n");
    }
    while (status >= 0) {
        if (error) {
            status = -1;
        }
        switch (status) {
            case 0:
                expected('{');
                status = 1;
                break;
            case 1: /* 解析key_1 */
                skipSpace();
                status = 2;
                break;
            case 2: /* 对象是否结束 */
                if (tryChar('}')) {
                    status = 17; /* 对象结束 */
                } else {
                    status = 3;
                }
                break;
            case 3: /* 解析key_2 */
                parseRawString(key);
                status = 5;
                break;
            case 5: /* 解析冒号_1 */
                skipSpace();
                status = 6;
                break;
            case 6: /* 解析冒号_2 */
                expected(':');
                status = 10;
                break;
            case 10: /* 解析值_1 */
                skipSpace();
                status = 11;
                break;
            case 11: /* 解析值_2 */
                parseValue(1, key);
                status = 15;
                break;
            case 15: /* 解析逗号_1 */
                skipSpace();
                status = 16;
                break;
            case 16: /* 解析逗号或结尾_2 */
                if (tryChar(',')) {
                    getchar();
                    status = 1; /* 继续解析key */
                } else {
                    status = 17;
                }
                break;
            case 17: /* 解析结尾_3 */
                expected('}');
                status = 18;
                break;
            case 18: /* 解析结束 */
                printf("OBJECT_END\n");
                status = -1;
                break;
            default:
                status = -1;
                break;
        }
    }
}

/* 解析数组 */
void parseArray(int inObject, char* pkey) {
    int status = 0;
    int c;
    if (inObject) {
        printf("ARRAY_BEGIN %s\n", pkey);
    } else {
        printf("ARRAY_BEGIN\n");
    }
    while (status >= 0) {
        if (error) {
            status = -1;
        }
        switch (status) {
            case 0:
                expected('[');
                status = 1;
                break;
            case 1: /* 解析元素_1 */
                skipSpace();
                status = 2;
                break;
            case 2: /* 数组是否结束 */
                if (tryChar(']')) {
                    status = 11; /* 数组结束 */
                } else {
                    status = 5;
                }
                break;
            case 5: /* 解析元素_2 */
                parseValue(0, NULL);
                status = 6;
                break;
            case 6: /* 解析逗号_1 */
                skipSpace();
                status = 10;
                break;
            case 10: /* 解析逗号或结尾_2 */
                if (tryChar(',')) {
                    getchar();
                    status = 1; /* 继续解析元素 */
                } else {
                    status = 11;
                }
                break;
            case 11: /* 解析结尾_3 */
                expected(']');
                status = 12;
                break;
            case 12: /* 解析结束 */
                printf("ARRAY_END\n");
                status = -1;
                break;
            default:
                status = -1;
                break;
        }
    }
}

int main(int argc, char const *argv[]) {
    int type;
    /* 以下为调试用命令行参数，可以无视掉 */
    if (argc >= 1) {
        freopen(argv[1], "r", stdin);
    }
    if (argc >= 2) {
        freopen(argv[2], "w", stdout);
    }
    skipSpace();
    parseValue(0, NULL);
    printError(error);
    return 0;
}
