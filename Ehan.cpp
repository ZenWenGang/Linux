#include <iostream>

class Test
{
private:
    // 1.私有化构造函数，禁止外部直接创建对象
    Test(int a, int b)
        : _a(a), _b(b)
    {
        std::cout << "Test instance created" << std::endl;
    }
    // 2.禁用拷贝构造和赋值运算符，防止通过拷贝创建实例
    Test(const Test &) = delete;
    Test &operator=(const Test &) = delete;
    // 3.静态成员变量：保存唯一实例(饿汉模式核心)
    static Test _instance;

    // 成员变量
    int _a;
    int _b;

public:
    // 4.全局访问点：静态成员函数，返回唯一实例的引用
    static Test &getInstance()
    {
        return _instance;
    }

    void printfInfo()
    {
        std::cout << "a:" << _a << ",b:" << _b << std::endl;
    }
};

// 饿汉模式关键：类外初始化静态成员变量，程序启动时自动创建实例
Test Test::_instance(1, 2);

int main()
{
    // 通过静态函数获取唯一实例
    Test &t = Test::getInstance();
    t.printfInfo();
    return 0;
}