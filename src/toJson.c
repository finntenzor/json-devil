#include <stdio.h>
#include <string.h>

const int TYPE_INTEGER = 1; /* 1 表示整数 */
const int TYPE_STRING = 2; /* 2 表示字符串 */
const int TYPE_OBJECT = 3; /* 3 表示对象 */
const int TYPE_ARRAY = 4; /* 4 表示数组 */
const int TYPE_END = 5; /* 4 表示对象结束或者数组结束 */

/* 获取下一个单词，根据单词来判断类型，并返回类型 */
int getType() {
    char type[120];
    int result = 0;
    scanf("%s", type);
    if (strcmp(type, "INTEGER") == 0) {
        result = TYPE_INTEGER;
    } else if (strcmp(type, "STRING") == 0) {
        result = TYPE_STRING;
    } else if (strcmp(type, "OBJECT_BEGIN") == 0) {
        result = TYPE_OBJECT;
    } else if (strcmp(type, "ARRAY_BEGIN") == 0) {
        result = TYPE_ARRAY;
    } else if (strcmp(type, "OBJECT_END") == 0) {
        result = TYPE_END;
    } else if (strcmp(type, "ARRAY_END") == 0) {
        result = TYPE_END;
    }
    return result;
}

/**
 * 根据类型来解析数据
 * inObject为0或者1
 * 0表示这个数据不在对象中，处于根元素，或者在另一个数组中，这一行输入没有键名
 * 1表示这个数据在对象中，这一行输入有键名
 * type表示已经确定了当前数据的类型
 * 需要先读取这一行第一个单词，判断完类型后，再调用
 */
void parseValue(int inObject, int type);

/**
 * 解析为一个整数
 * inObject为0或者1
 * 0表示这个整数不在对象中，处于根元素，或者在数组中，这一行输入没有键名
 * 1表示这个整数在对象中，这一行输入有键名
 * 需要在读取了表示类型的单词后调用此函数
 */
void parseInteger(int inObject) {
    char key[120];
    int value;
    if (inObject) {
        scanf("%s %d", key, &value);
        printf("\"%s\":%d", key, value);
    } else {
        scanf("%d", &value);
        printf("%d", value);
    }
}

/**
 * 解析为一个字符串
 * inObject为0或者1
 * 0表示这个字符串不在对象中，处于根元素，或者在数组中，这一行输入没有键名
 * 1表示这个字符串在对象中，这一行输入有键名
 * 需要在读取了表示类型的单词后调用此函数
 */
void parseString(int inObject) {
    char key[120];
    char value[120];
    if (inObject) {
        scanf("%s %s", key, value);
        printf("\"%s\":\"%s\"", key, value);
    } else {
        scanf("%s", value);
        printf("\"%s\"", value);
    }
}

/**
 * 解析为一个对象
 * inObject为0或者1
 * 0表示这个对象不在另一个对象中，处于根元素，或者在数组中，这一行输入没有键名
 * 1表示这个对象在另一个对象中，这一行输入有键名
 * 需要在读取了表示类型的单词后调用此函数
 */
void parseObject(int inObject) {
    char key[120];
    int type;
    int i;
    /**
     * 这里的inObject表示当前要解析的对象，它所处的上一层是否是对象
     * 如果是，那么这一行输入必定为 OBJECT_BEGIN 键名 的形式
     * 由于已经读取了“OBJECT_BEGIN”，因此需要再读取键名
     * 然后从下一行开始读取属性
     * 如果不是，那么这一样输入只有 OBJECT_BEGIN，则不能读取键名
     */
    if (inObject) {
        scanf("%s", key);
        printf("\"%s\":", key);
    }
    printf("{");
    for (i = 0; ;i++) {
        type = getType(); /* 读取这一行的第一个单词，判断类型 */
        if (type != TYPE_END && i != 0) { /* 在各个键值对之间加逗号 */
            printf(",");
        }
        /**
         * 根据刚刚获取的类型解析数据
         * 由于这里是在parseObject函数中
         * 运行到这里说明这一层的元素全都是 类型 键名 值 这样类似结构的形式
         * 因此给parseValue传入 inObject=1，让这一层所有输入都要多读取一个key
         */
        parseValue(1, type);
        if (type == TYPE_END) {
            break;
        }
    }
    printf("}");
}

/**
 * 解析为一个数组
 * inObject为0或者1
 * 0表示这个数组不在对象中，处于根元素，或者在另一个数组中，这一行输入没有键名
 * 1表示这个数组在对象中，这一行输入有键名
 * 需要在读取了表示类型的单词后调用此函数
 */
void parseArray(int inObject) {
    char key[120];
    int type;
    int i;
    /**
     * 这里的inObject表示当前要解析的数组，它所处的上一层是否是对象
     * 如果是，那么这一行输入必定为 ARRAY_BEGIN 键名 的形式
     * 由于已经读取了“ARRAY_BEGIN”，因此需要再读取键名
     * 然后从下一行开始读取元素
     * 如果不是，那么这一样输入只有 ARRAY_BEGIN，则不能读取键名
     */
    if (inObject) {
        scanf("%s", key);
        printf("\"%s\":", key);
    }
    printf("[");
    for (i = 0; ;i++) {
        type = getType(); /* 读取这一行的第一个单词，判断类型 */
        if (type != TYPE_END && i != 0) {
            printf(",");
        }
        /**
         * 根据刚刚获取的类型解析数据
         * 由于这里是在parseArray函数中
         * 运行到这里说明这一层的元素全都是 类型 值 这样类似结构的形式
         * 因此给parseValue传入 inObject=0，让这一层所有输入都不读取key
         */
        parseValue(0, type);
        if (type == TYPE_END) {
            break;
        }
    }
    printf("]");
}

/**
 * 根据类型来解析数据
 * inObject为0或者1
 * 0表示这个数据不在对象中，处于根元素，或者在另一个数组中，这一行输入没有键名
 * 1表示这个数据在对象中，这一行输入有键名
 * type表示已经确定了当前数据的类型
 * 需要先读取这一行第一个单词，判断完类型后，再调用
 */
void parseValue(int inObject, int type) {
    char key[120];
    if (type == TYPE_INTEGER) {
        parseInteger(inObject);
    } else if (type == TYPE_STRING) {
        parseString(inObject);
    } else if (type == TYPE_OBJECT) {
        parseObject(inObject);
    } else if (type == TYPE_ARRAY) {
        parseArray(inObject);
    }
}

int main(int argc, char* argv[]) {
    /**
     * argc为命令行参数个数
     * argv为命令行参数字符串列表
     * 有兴趣可以百度查一下，没兴趣可以将这一段忽略
     */
    if (argc >= 1) {
        /* 输入重定向，从文件读取输入而不是键盘 */
        freopen(argv[1], "r", stdin);
    }
    if (argc >= 2) {
        /* 输出重定向，向文件写入输出而不是屏幕 */
        freopen(argv[2], "w", stdout);
    }

    /* 输入保证根元素只有一个，先获取其类型 */
    int type = getType();
    /* 根元素必然不在对象中，inObject=0，根据类型解析 */
    parseValue(0, type);
    return 0;
}
