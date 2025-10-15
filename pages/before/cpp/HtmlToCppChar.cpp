#include <iostream>   // 输入输出流库，用于控制台输入输出
#include <fstream>    // 文件流库，用于文件读写操作
#include <string>     // 字符串处理库，提供字符串操作功能
#include <regex>      // 正则表达式库，用于模式匹配和替换
#include <sys/stat.h> // 文件状态检查库，用于检查文件是否存在
#include <cstdlib>    // 标准库函数，提供系统相关功能
#include <sstream>    // 字符串流库，用于字符串流操作

// 函数声明
std::string removeHtmlComments(const std::string &content);     // 移除HTML注释的函数声明
std::string removeOtherComments(const std::string &content);    // 移除JavaScript注释的函数声明
std::string convertHtmlToCppString(const std::string &content); // 转换HTML为C++字符串的函数声明
std::string processCssContent(const std::string &content);      // 处理CSS内容的函数声明
std::string processFile(const std::string &filePath);           // 处理文件的函数声明
void printUsage(const std::string &programName);                // 打印使用说明的函数声明
bool fileExists(const std::string &filePath);                   // 检查文件是否存在的函数声明
std::string getOutputFileName(const std::string &filePath);     // 获取输出文件名的函数声明
void setupConsole();                                            // 设置控制台的函数声明

int main(int argc, char *argv[])
{ // 主函数，程序入口点
    // 设置控制台为UTF-8并清屏
    setupConsole(); // 调用设置控制台函数

    // 检查命令行参数数量是否正确
    if (argc < 2 || argc > 3)
    {                        // 如果参数数量少于2个或多于3个
        printUsage(argv[0]); // 打印使用说明
        return 1;            // 返回错误代码1
    }

    std::string filePath = argv[1]; // 获取第一个参数作为输入文件路径
    std::string outputPath;         // 声明输出文件路径变量

    // 解析命令行参数
    if (argc == 2)
    { // 如果只有2个参数（程序名和输入文件）
        // 只有输入文件，自动生成输出文件名
        outputPath = getOutputFileName(filePath); // 调用函数生成输出文件名
    }
    else if (argc == 3)
    { // 如果有3个参数（程序名、输入文件和输出文件）
        // 输入文件和输出文件都由命令行指定
        outputPath = argv[2]; // 获取第三个参数作为输出文件路径
    }

    // 检查输入文件是否存在
    if (!fileExists(filePath))
    {                                                                        // 如果文件不存在
        std::cerr << "错误: 文件 '" << filePath << "' 不存在!" << std::endl; // 输出错误信息
        return 1;                                                            // 返回错误代码1
    }

    // 处理文件，转换为C++字符串格式
    std::string processedContent = processFile(filePath); // 调用处理文件函数

    // 检查处理结果是否为空
    if (processedContent.empty())
    {                                                                        // 如果处理结果为空
        std::cerr << "错误: 无法处理文件 '" << filePath << "'" << std::endl; // 输出错误信息
        return 1;                                                            // 返回错误代码1
    }

    // 创建并打开输出文件
    std::ofstream outputFile(outputPath); // 创建输出文件流对象
    if (outputFile.is_open())
    {                                                                           // 如果文件成功打开
        outputFile << processedContent;                                         // 将处理后的内容写入文件
        outputFile.close();                                                     // 关闭文件
        std::cout << "成功! 处理后的文件已保存为: " << outputPath << std::endl; // 输出成功信息
    }
    else
    {                                                                              // 如果文件打开失败
        std::cerr << "错误: 无法创建输出文件 '" << outputPath << "'" << std::endl; // 输出错误信息
        return 1;                                                                  // 返回错误代码1
    }

    return 0; // 程序正常结束，返回0
}

// 设置控制台为UTF-8并清屏
void setupConsole()
{
    system("chcp 65001 && cls"); // 执行系统命令，设置控制台为UTF-8编码并清屏
}

// 检查文件是否存在
bool fileExists(const std::string &filePath)
{
    struct stat buffer;                            // 定义文件状态结构体
    return (stat(filePath.c_str(), &buffer) == 0); // 使用stat函数检查文件是否存在，返回0表示存在
}

// 获取输出文件名
std::string getOutputFileName(const std::string &filePath)
{
    // 查找最后一个目录分隔符的位置
    size_t lastSlash = filePath.find_last_of("/\\");                                                     // 查找最后一个/或\的位置
    std::string directory = (lastSlash == std::string::npos) ? "" : filePath.substr(0, lastSlash + 1);   // 提取目录路径
    std::string fileName = (lastSlash == std::string::npos) ? filePath : filePath.substr(lastSlash + 1); // 提取文件名

    // 查找文件扩展名的位置
    size_t lastDot = fileName.find_last_of(".");                                                    // 查找最后一个.的位置
    std::string baseName = (lastDot == std::string::npos) ? fileName : fileName.substr(0, lastDot); // 提取不含扩展名的文件名

    return directory + baseName + "_processed.cpp"; // 组合成新的文件名，添加_processed.cpp后缀
}

// 打印使用说明
void printUsage(const std::string &programName)
{
    std::cout << "用法: " << programName << " <输入文件路径> [输出文件名]" << std::endl;                                   // 打印基本用法
    std::cout << "功能:" << std::endl;                                                                                     // 打印功能标题
    std::cout << "  1. 移除HTML注释 (<!-- ... -->)" << std::endl;                                                          // 打印功能1
    std::cout << "  2. 移除JavaScript注释 (// 和 /* ... */)" << std::endl;                                                 // 打印功能2
    std::cout << "  3. 将CSS中每个选择器的内容变成一行" << std::endl;                                                      // 打印功能3
    std::cout << "  4. 去除行首缩进和空行" << std::endl;                                                                   // 打印功能4
    std::cout << "  5. 转换为C++字符串格式" << std::endl;                                                                  // 打印功能5
    std::cout << "示例:" << std::endl;                                                                                     // 打印示例标题
    std::cout << "  " << programName << " index.html                          // 输出到 index_processed.cpp" << std::endl; // 打印示例1
    std::cout << "  " << programName << " index.html main.cpp                // 输出到 main.cpp" << std::endl;             // 打印示例2
}

// 处理文件，将HTML转换为C++字符串格式
std::string processFile(const std::string &filePath)
{
    std::ifstream inputFile(filePath); // 创建输入文件流对象
    if (!inputFile.is_open())
    {              // 如果文件打开失败
        return ""; // 返回空字符串
    }

    // 读取整个文件内容
    std::string content((std::istreambuf_iterator<char>(inputFile)), // 使用迭代器读取整个文件内容
                        std::istreambuf_iterator<char>());           // 结束迭代器
    inputFile.close();                                               // 关闭文件

    // 移除HTML注释
    content = removeHtmlComments(content); // 调用移除HTML注释函数

    // 移除JavaScript注释
    content = removeOtherComments(content); // 调用移除JavaScript注释函数

    // 处理CSS内容，将每个选择器的内容变成一行
    content = processCssContent(content); // 调用处理CSS内容函数

    // 转换HTML为C++字符串格式
    return convertHtmlToCppString(content); // 调用转换函数并返回结果
}

// 移除HTML注释
std::string removeHtmlComments(const std::string &content)
{
    // 定义匹配HTML注释的正则表达式 <!-- ... -->
    std::regex htmlCommentRegex("<!--.*?-->", std::regex::icase); // 创建正则表达式对象，icase表示不区分大小写
    return std::regex_replace(content, htmlCommentRegex, "");     // 使用正则表达式替换所有HTML注释为空字符串
}

// 移除JavaScript注释
std::string removeOtherComments(const std::string &content)
{
    // 先处理多行注释 /* ... */
    std::regex jsMultiLineCommentRegex("/\\*.*?\\*/", std::regex::icase);          // 创建匹配多行注释的正则表达式
    std::string result = std::regex_replace(content, jsMultiLineCommentRegex, ""); // 替换所有多行注释为空字符串

    // 移除JavaScript单行注释，但要避免误删URL中的http://或https://
    size_t pos = 0; // 初始化位置变量
    while ((pos = result.find("//", pos)) != std::string::npos)
    { // 查找所有//出现的位置
        // 检查是否是URL的一部分
        bool isUrlPart = false; // 初始化URL部分标志

        // 检查前面是否有http:或https
        if (pos >= 5)
        {                                                   // 确保有足够的空间来检查http:
            std::string prefix = result.substr(pos - 5, 7); // 获取http://或https://的一部分
            if (prefix.find("http://") != std::string::npos || prefix.find("https:/") != std::string::npos)
            {                     // 检查是否包含URL前缀
                isUrlPart = true; // 设置URL部分标志为真
            }
        }

        // 检查是否在字符串中（在反引号或引号内）
        if (!isUrlPart && pos > 0)
        {                                                      // 如果不是URL部分且不在字符串开头
            size_t startQuote = result.rfind('`', pos);        // 查找最近的反引号
            size_t startSingleQuote = result.rfind('\'', pos); // 查找最近的单引号
            size_t startDoubleQuote = result.rfind('"', pos);  // 查找最近的双引号

            size_t lastQuote = std::max({startQuote, startSingleQuote, startDoubleQuote}); // 找到最后一个引号位置
            if (lastQuote != std::string::npos)
            {                                        // 如果找到引号
                size_t endQuote = std::string::npos; // 初始化结束引号位置
                if (result[lastQuote] == '`')
                {                                               // 如果是反引号
                    endQuote = result.find('`', lastQuote + 1); // 查找匹配的反引号
                }
                else if (result[lastQuote] == '\'')
                {                                                // 如果是单引号
                    endQuote = result.find('\'', lastQuote + 1); // 查找匹配的单引号
                }
                else if (result[lastQuote] == '"')
                {                                               // 如果是双引号
                    endQuote = result.find('"', lastQuote + 1); // 查找匹配的双引号
                }

                if (endQuote != std::string::npos && pos < endQuote)
                {                     // 如果在引号内
                    isUrlPart = true; // 设置URL部分标志为真
                }
            }
        }

        if (!isUrlPart)
        { // 如果不是URL部分
            // 找到行尾
            size_t endLine = result.find('\n', pos); // 查找换行符
            if (endLine != std::string::npos)
            {                                     // 如果找到换行符
                result.erase(pos, endLine - pos); // 删除从//到行尾的内容
            }
            else
            {                      // 如果是最后一行
                result.erase(pos); // 删除从//到文件末尾的内容
            }
        }
        else
        {             // 如果是URL部分
            pos += 2; // 跳过这两个字符，继续查找
        }
    }

    return result; // 返回处理后的结果
}

// 处理CSS内容，将每个选择器的内容变成一行
std::string processCssContent(const std::string &content)
{
    std::string result;                 // 存储处理结果的字符串
    std::istringstream stream(content); // 创建字符串流，用于逐行处理
    std::string line;                   // 存储当前行的字符串
    bool inStyleTag = false;            // 标记是否在<style>标签内
    bool inCssBlock = false;            // 标记是否在CSS块内
    std::string currentSelector;        // 存储当前选择器的字符串
    std::string currentRules;           // 存储当前规则的字符串

    // 逐行处理内容
    while (std::getline(stream, line))
    { // 逐行读取内容
        // 去除行首缩进（空格和制表符）
        size_t start = 0; // 初始化起始位置
        while (start < line.length() && (line[start] == ' ' || line[start] == '\t'))
        {            // 跳过行首的空格和制表符
            start++; // 移动到下一个字符
        }
        if (start > 0)
        {                              // 如果有缩进
            line = line.substr(start); // 去除缩进部分
        }

        // 移除行尾的回车符（Windows系统中的换行符是\r\n）
        if (!line.empty() && line.back() == '\r')
        {                    // 如果行尾有回车符
            line.pop_back(); // 移除回车符
        }

        // 跳过空行
        if (line.empty())
        {             // 如果是空行
            continue; // 跳过当前行
        }

        // 检查是否是<style>标签开始
        if (line.find("<style>") != std::string::npos)
        {                          // 如果找到<style>标签
            inStyleTag = true;     // 设置在style标签内的标志
            result += line + "\n"; // 将当前行添加到结果中
            continue;              // 继续处理下一行
        }

        // 检查是否是</style>标签结束
        if (line.find("</style>") != std::string::npos)
        { // 如果找到</style>标签
            // 处理最后一个选择器（如果有）
            if (!currentSelector.empty())
            {                                                           // 如果有未处理的选择器
                result += currentSelector + "{" + currentRules + "}\n"; // 将选择器和规则添加到结果中
                currentSelector = "";                                   // 清空当前选择器
                currentRules = "";                                      // 清空当前规则
            }
            inStyleTag = false;    // 重置在style标签内的标志
            inCssBlock = false;    // 重置在CSS块内的标志
            result += line + "\n"; // 将当前行添加到结果中
            continue;              // 继续处理下一行
        }

        // 如果在<style>标签内
        if (inStyleTag)
        { // 如果在style标签内
            // 检查是否是CSS选择器行（包含{）
            if (line.find('{') != std::string::npos)
            { // 如果找到{
                // 如果之前有选择器，先处理它
                if (!currentSelector.empty())
                {                                                           // 如果有未处理的选择器
                    result += currentSelector + "{" + currentRules + "}\n"; // 将选择器和规则添加到结果中
                }

                // 开始新的选择器
                size_t bracePos = line.find('{');           // 查找{的位置
                currentSelector = line.substr(0, bracePos); // 提取选择器部分
                currentRules = "";                          // 清空当前规则
                inCssBlock = true;                          // 设置在CSS块内的标志
            }
            // 检查是否是CSS结束行（包含}）
            else if (line.find('}') != std::string::npos && inCssBlock)
            { // 如果找到}且在CSS块内
                // 结束当前选择器
                size_t bracePos = line.find('}'); // 查找}的位置
                if (bracePos > 0)
                { // 如果}不在行首
                    // 如果}前面有内容，添加到规则中
                    std::string lastRule = line.substr(0, bracePos); // 提取}前面的内容
                    if (!lastRule.empty())
                    { // 如果有内容
                        if (!currentRules.empty())
                        {                        // 如果已有规则
                            currentRules += " "; // 添加空格分隔
                        }
                        currentRules += lastRule; // 将新规则添加到当前规则中
                    }
                }

                result += currentSelector + "{" + currentRules + "}\n"; // 将选择器和规则添加到结果中
                currentSelector = "";                                   // 清空当前选择器
                currentRules = "";                                      // 清空当前规则
                inCssBlock = false;                                     // 重置在CSS块内的标志
            }
            // 如果在CSS块内，添加到规则中
            else if (inCssBlock && !line.empty())
            { // 如果在CSS块内且当前行不为空
                if (!currentRules.empty())
                {                        // 如果已有规则
                    currentRules += " "; // 添加空格分隔
                }
                currentRules += line; // 将当前行添加到规则中
            }
            // 如果在<style>标签内但不在CSS块内，直接添加到结果
            else if (!inCssBlock)
            {                          // 如果不在CSS块内
                result += line + "\n"; // 将当前行添加到结果中
            }
        }
        // 如果不在<style>标签内，直接添加到结果
        else
        {                          // 如果不在style标签内
            result += line + "\n"; // 将当前行添加到结果中
        }
    }

    // 处理最后一个选择器（如果有）
    if (!currentSelector.empty())
    {                                                           // 如果有未处理的选择器
        result += currentSelector + "{" + currentRules + "}\n"; // 将选择器和规则添加到结果中
    }

    return result; // 返回处理后的结果
}

// 转换HTML为C++字符串格式
std::string convertHtmlToCppString(const std::string &content)
{
    std::string result = "const char *page = "; // C++字符串变量声明
    std::string line;                           // 存储当前行的字符串
    bool firstLine = true;                      // 标记是否是第一行

    // 分割内容为行
    std::istringstream stream(content); // 创建字符串流，用于逐行处理

    // 逐行处理内容
    while (std::getline(stream, line))
    { // 逐行读取内容
        // 处理每一行
        std::string processedLine = line; // 复制当前行用于处理

        // 去除行首缩进（空格和制表符）
        size_t start = 0; // 初始化起始位置
        while (start < processedLine.length() && (processedLine[start] == ' ' || processedLine[start] == '\t'))
        {            // 跳过行首的空格和制表符
            start++; // 移动到下一个字符
        }
        if (start > 0)
        {                                                // 如果有缩进
            processedLine = processedLine.substr(start); // 去除缩进部分
        }

        // 转义反斜杠字符
        size_t pos = 0; // 初始化位置变量
        while ((pos = processedLine.find("\\", pos)) != std::string::npos)
        {                                          // 查找所有反斜杠的位置
            processedLine.replace(pos, 1, "\\\\"); // 将单个反斜杠替换为双反斜杠
            pos += 2;                              // 跳过替换后的字符
        }

        // 转义双引号字符
        pos = 0; // 重置位置变量
        while ((pos = processedLine.find("\"", pos)) != std::string::npos)
        {                                          // 查找所有双引号的位置
            processedLine.replace(pos, 1, "\\\""); // 将双引号替换为转义的双引号
            pos += 2;                              // 跳过替换后的字符
        }

        // 移除行尾的回车符（Windows系统中的换行符是\r\n）
        if (!processedLine.empty() && processedLine.back() == '\r')
        {                             // 如果行尾有回车符
            processedLine.pop_back(); // 移除回车符
        }

        // 跳过空行
        if (processedLine.empty())
        {             // 如果是空行
            continue; // 跳过当前行
        }

        // 如果不是第一行，添加换行符
        if (!firstLine)
        {                   // 如果不是第一行
            result += "\n"; // 添加换行符
        }
        else
        {                      // 如果是第一行
            firstLine = false; // 设置第一行标志为false
        }

        // 添加处理后的行，用双引号包围
        result += "\"" + processedLine + "\""; // 将处理后的行添加到结果中
    }

    result += ";"; // 添加C++语句结束符

    return result; // 返回处理后的结果
}