# pages

## 文件夹介绍

用于测试前端网页，并将其转换为 C++ 字符串。

## 使用指南

### 环境要求

-   开发环境：`node22.17.1`
-   运行环境不确定需不需要`22.17.1`

### 使用方法

1.  一键测试（有需要可以测试）

    ```bash
    cd ./z-index/back
    npm install
    npm start
    ```

    `ctrl+c` 退出

2.  更换地址
    在 colorpage.html 中找到

    ```javascript
    let deviceAddress = "localhost"; // 设备的mDNS地址
    ```

    改为

    ```javascript
    let deviceAddress = "magic-mini.local"; // 设备的mDNS地址
    ```

3.  把 html 改为 cpp 字符串（可以给其他 html 转 cpp 字符串复用）

    运行脚本即可

    ```bash
    cd ../before
    ./cpp/HtmlToCppChar.exe index.html main.cpp
    ```

4.  替换

    把 main.cpp 的内容替换到`./scr/web.cpp`中的`*pageColor`（不要带上变量名）

其他同理。
