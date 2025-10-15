# HtmlToCppChar 使用指南

## 功能

**注释处理：**

-   `<!-- ... -->` HTML 注释
-   `//···` JavaScript 单行注释(不会误伤`http`的`//`)
-   `/*···*/` JavaScript/css 多行注释

**CSS 处理：**

-   将 CSS 中每个选择器的内容变成一行
-   保持 JavaScript 代码格式不变

**格式处理：**

-   去除行首缩进
-   去除所有空行
-   转义特殊字符（反斜杠和引号）
-   转换为 C++字符串格式

## 编译

在使用 HtmlToCppChar 之前，需要先编译 C++源文件为可执行文件：

```bash
g++ HtmlToCppChar.cpp -o HtmlToCppChar -std=c++17
```

## 基本用法

将 HtmlToCppChar 生成的 exe 文件放在与要处理的 HTML 文件相同目录下，或者将其添加到系统环境变量中。
以下示例假设 HtmlToCppChar 已添加到 HTML 文件相同目录下。（添加到环境变量中可去除.\）

```bash
.\HtmlToCppChar # 查看帮助
.\HtmlToCppChar 输入文件路径 [输出文件路径] # 处理文件
```

## 使用示例

### 1. 基本处理（默认输出）

```bash
.\HtmlToCppChar index.html
```

生成：index_processed.cpp

### 2. 基本处理（指定输出）

```bash
.\HtmlToCppChar index.html output.cpp
```

生成：output.html

## 处理流程

1. **读取原始 HTML 文件**
2. **移除 HTML 注释** (<!-- ... -->)
3. **移除 JavaScript 注释** (// 和 /_ ... _/)
4. **处理 CSS 内容**，将每个选择器的内容变成一行
5. **去除行首缩进和空行**
6. **转义特殊字符**（反斜杠和引号）
7. **转换为 C++字符串格式**
8. **生成最终文件**

## 输出格式

处理后的文件将以 C++字符串格式输出，格式如下：

```cpp
const char *page = "<!DOCTYPE html>"
"<html>"
"<head>"
"<style>"
"* {margin: 0; padding: 0; box-sizing: border-box;}"
"body {font-family: Arial;}"
"</style>"
"</head>"
"<body>"
"<!-- 内容 -->"
"</body>"
"</html>";
```

## 注意事项

-   程序会智能处理 URL 中的`//`，避免误删
-   CSS 内容会被单行化，但 JavaScript 代码保持原有格式
-   所有空行和行首缩进都会被移除
-   特殊字符（\和"）会被自动转义

## 示例对比

**原始 HTML:**

```html
<!DOCTYPE html>
<html>
    <head>
        <style>
            body {
                margin: 0;
                padding: 0;
            }
        </style>
        <!-- 这是一个注释 -->
        <script>
            // 这是一个JavaScript注释
            function test() {
                console.log("Hello");
            }
        </script>
    </head>
    <body>
        <h1>标题</h1>
    </body>
</html>
```

**处理后:**

```cpp
const char *page = "<!DOCTYPE html>"
"<html>"
"<head>"
"<style>"
"body {margin: 0; padding: 0;}"
"</style>"
"<script>"
"function test() {"
"    console.log(\"Hello\");"
"}"
"</script>"
"</head>"
"<body>"
"<h1>标题</h1>"
"</body>"
"</html>";
```
