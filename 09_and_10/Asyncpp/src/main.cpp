import asyncpp.core;
import asyncpp.task;
import asyncpp.io;
import asyncpp.fs;

#include <iostream>

using asyncpp::task::asyncify;
using asyncpp::task::AsyncTaskLoop;
using asyncpp::task::Coroutine;

using asyncpp::fs::createDirectories;
using asyncpp::fs::exists;
using asyncpp::fs::removeAll;
using asyncpp::fs::voidFsFunction;

using asyncpp::io::AsyncIoLoop;

Coroutine asyncFs()
{
    std::string dirPath = "dir1/a/b/c";

    // ����Ŀ¼
    std::string cmd = "createDirectories";
    std::cout << "[AWAIT] Before: " << cmd << std::endl;
    auto createResult = co_await createDirectories(dirPath);
    std::cout << "[AWAIT] After: " << cmd << ": " << std::boolalpha << createResult << std::endl;

    // �ж�·���Ƿ����
    cmd = "exists1";
    std::cout << "[AWAIT] Before: " << cmd << std::endl;
    auto existsResult1 = co_await exists(dirPath);
    std::cout << "[AWAIT] After: " << cmd << ": " << std::boolalpha << existsResult1 << std::endl;

    // ɾ��Ŀ¼
    cmd = "removeAll";
    std::cout << "[AWAIT] Before: " << cmd << std::endl;
    auto removeResult = co_await removeAll(dirPath);
    std::cout << "[AWAIT] After: " << cmd << ": " << std::boolalpha << removeResult << std::endl;

    // �ж�·���Ƿ����
    cmd = "exists2";
    std::cout << "[AWAIT] Before: " << cmd << std::endl;
    auto existsResult2 = co_await exists(dirPath);
    std::cout << "[AWAIT] After: " << cmd << ": " << std::boolalpha << existsResult2 << std::endl;

    cmd = "voidFs";
    std::cout << "[AWAIT] Before: " << cmd << std::endl;
    co_await voidFsFunction(dirPath);
    std::cout << "[AWAIT] After: " << cmd << std::endl;
}

void hello() {
    std::cout << "<HELLO>" << std::endl;
}

auto asyncHello() {
    return asyncify(hello);
}

asyncpp::task::Coroutine testVoid() {
    // void������װʾ��
    co_await asyncHello();
}

int main() {
    // �����첽I/O�����߳�
    AsyncIoLoop::start();

    // ����Э�̣�Э�̻Ტ��ִ�У�
    asyncFs();
    testVoid();

    // �������߳�����ѭ����һ��Ҫ�����ã������������
    AsyncTaskLoop::start();

    return 0;
}