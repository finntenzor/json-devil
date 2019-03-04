# 输入
本题目的输入将是一段标记语言（也就是题目中所说的魔物语，这并不是公认的一种语言，而是纯粹为了出题而发明的语言），用标记语言来描述一个JSON数据，你需要以JSON的压缩格式输出。
保证输入：
1. 所有数字在int表达范围内（有负数）。
2. 所有字符串只均由：大写字母、小写字母、数字、下划线、中横线组成。（中间没有空白符，你可以使用%s扫描）。
3. 所有的键名、字符串类型的键值，字符串都不超过100个字符。
4. 所有的输入标记均能组成一个合法JSON，不会存在不匹配或结构错误的情况。
5. 每个样例输入都是恰好一个JSON数据。
6. JSON数据内部嵌套不会超过50层（你可以放心不会爆调用栈）。
7. 输入输出数据大小均在200KB以内。

# 标记语言
## 1. 字符串
字符串使用“STRING”表达，结构为“STRING 数据”，保证数据中只包含大写字母、小写字母、下划线、数字。例如：  
输入：
```txt
STRING Hello
```
输出：
```json
"Hello"
```
## 2. 数字
本题中所有的数字均为整数，结构“INTEGER 数据”，保证数据中只包含数字、负号，并且在int表达范围内。例如：  
输入：
```txt
INTEGER 123
```
输出：
```json
123
```
## 3. 对象
对象使用多行表示，以“OBJECT_BEGIN”开始，以“OBJECT_END”结束。例如：
输入：  
```txt
OBJECT_BEGIN
OBJECT_END
```
输出：
```json
{}
```
## 4. 数组
对象使用多行表示，以“ARRAY_BEGIN”开始，以“ARRAY_END”结束。例如：
输入：  
```txt
ARRAY_BEGIN
ARRAY_END
```
输出：
```json
[]
```
### 4.1 数组中的元素没有键名
数组中的元素没有键名，所有元素均为“类型 值”的形式。例如：  
输入：  
```txt
ARRAY_BEGIN
INTEGER 123
INTEGER 456
ARRAY_END
```
输出：
```json
[123,456]
```
### 4.2 数组中的元素类型可以不同
在数组中你需要根据这一行的第一个标识符来判断元素类型，例如：
输入：  
```txt
ARRAY_BEGIN
INTEGER 123
STRING 456
ARRAY_END
```
输出：
```json
[123,"456"]
```
## 5. 对象中的属性
对象中属性将包含键名，使用“类型 键名 键值”的形式表示，保证键名由大写字母、小写字母、下划线、数字组成，键值类型保证形式上文所述。
### 5.1 对象中的字符串
对象中的字符串，结构为“STRING 键名 键值”。例如：  
输入：  
```txt
OBJECT_BEGIN
STRING name Finn
OBJECT_END
```
输出：
```json
{"name":"Finn"}
```
### 5.2 对象中的数字
对象中的数字（保证为整数），结构为“INTEGER 键名 键值”。例如：  
输入：  
```txt
OBJECT_BEGIN
INTEGER age 20
OBJECT_END
```
输出：
```json
{"age":"20"}
```
### 5.3 对象中的对象
对象中的对象使用较为复杂的多行结构表示，第一行为“OBJECT_BEGIN 键名”，然后是若干行对象属性，最后跟一行“OBJECT_END”。例如：    
输入：  
```txt
OBJECT_BEGIN
OBJECT_BEGIN favorite
STRING type Tsundere
STRING name Misaka_Mikoto
OBJECT_END
OBJECT_END
```
输出：
```json
{"favorite":{"type":"Tsundere","name":"Misaka_Mikoto"}}
```
(字符串中不会出现空格)
### 5.4 对象中的数组
对象中的对象使用较为复杂的多行结构表示，第一行为“ARRAY_BEGIN 键名”，然后是若干行元素，最后跟一行“ARRAY_END”。例如：  
输入：  
```txt
OBJECT_BEGIN
ARRAY_BEGIN favorite
STRING Misaka_Mikoto
STRING Gokou_Ruri
ARRAY_END
OBJECT_END
```
输出：
```json
{"favorite":["Misaka_Mikoto","Gokou_Ruri"]}
```
## 6. 数组中的对象、数组
### 6.1 数组中的对象
数组中没有键名，因此在“OBJECT_BEGIN”后没有键名，下一行直接开始描述元素，例如：  
输入：
```txt
ARRAY_BEGIN
OBJECT_BEGIN
STRING name Misaka_Mikoto
INTEGER age 14
OBJECT_END
OBJECT_BEGIN
STRING name Gokou_Ruri
INTEGER age 16
OBJECT_END
ARRAY_END
```
输出：
```json
[{"name":"Misaka_Mikoto","age":14},{"name":"Gokou_Ruri","age":16}]
```
### 6.2 数组中的数组
数组中没有键名，因此在“ARRAY_BEGIN”后没有键名，下一行直接开始描述元素，例如：  
输入：
```txt
ARRAY_BEGIN
ARRAY_BEGIN
INTEGER 1
INTEGER 2
INTEGER 3
ARRAY_END
ARRAY_BEGIN
INTEGER 4
INTEGER 5
INTEGER 6
ARRAY_END
ARRAY_END
```
输出：
```json
[[1,2,3],[4,5,6]]
```

# 7. 综合样例
再次特别说明：  
__在此问题中，为了简化逻辑，所有的数字均为int范围内的整数，所以的字符串均仅由大写字母、小写字母、下划线和数字组成（没有空格）__
## 样例1
输入：
```txt
OBJECT_BEGIN
STRING name Finn
INTEGER age 20
OBJECT_BEGIN favorite
STRING type Tsundere
STRING name Misaka_Mikoto
OBJECT_BEGIN cp
STRING name KamijouTouma
STRING gender male
OBJECT_END
OBJECT_END
OBJECT_END
```
输出：
```json
{"name":"Finn","age":20,"favorite":{"type":"Tsundere","name":"Misaka_Mikoto","cp":{"name":"KamijouTouma","gender":"male"}}}
```
## 样例2
输入：
```txt
OBJECT_BEGIN
INTEGER success 1
ARRAY_BEGIN data
OBJECT_BEGIN
INTEGER id 1
INTEGER place_id 1
INTEGER start 8
INTEGER end 22
INTEGER device 0
INTEGER capacity 1000
OBJECT_END
OBJECT_BEGIN
INTEGER id 2
INTEGER place_id 1
INTEGER start 8
INTEGER end 22
INTEGER device 0
INTEGER capacity 1000
OBJECT_END
OBJECT_BEGIN
INTEGER id 3
INTEGER place_id 1
INTEGER start 8
INTEGER end 22
INTEGER device 0
INTEGER capacity 1000
OBJECT_END
OBJECT_BEGIN
INTEGER id 4
INTEGER place_id 1
INTEGER start 8
INTEGER end 22
INTEGER device 1
INTEGER capacity 0
OBJECT_END
ARRAY_END
OBJECT_END
```
输出：
```json
{"success":1,"data":[{"id":1,"place_id":1,"start":8,"end":22,"device":0,"capacity":1000},{"id":2,"place_id":1,"start":8,"end":22,"device":0,"capacity":1000},{"id":3,"place_id":1,"start":8,"end":22,"device":0,"capacity":1000},{"id":4,"place_id":1,"start":8,"end":22,"device":1,"capacity":0}]}
```
## 样例3
输入：
```txt
ARRAY_BEGIN
ARRAY_BEGIN
ARRAY_BEGIN
INTEGER 1
INTEGER 2
INTEGER 3
ARRAY_END
ARRAY_BEGIN
INTEGER 4
INTEGER 5
INTEGER 6
ARRAY_END
ARRAY_BEGIN
INTEGER 7
INTEGER 8
INTEGER 9
ARRAY_END
ARRAY_END
ARRAY_BEGIN
ARRAY_BEGIN
STRING 1
STRING 2
STRING 3
ARRAY_END
ARRAY_BEGIN
STRING 4
STRING 5
STRING 6
ARRAY_END
ARRAY_BEGIN
STRING 7
STRING 8
STRING 9
ARRAY_END
ARRAY_END
ARRAY_END
```
输出：
```json
[[[1,2,3],[4,5,6],[7,8,9]],[["1","2","3"],["4","5","6"],["7","8","9"]]]
```
