
#include <list>
#include <string>
#include <memory>
#include <iostream>

struct foo_t
{
    int id = 10;
};

/**
 * C++ 有以下几种所有权管理方式：
 * 1. 独有，通过unique_ptr<T>管理，常用于资源维护，如std::vector<unique_ptr<File>> _files;
 * 2. 共有，通过shared_ptr<T>管理，如共享一个公共服务类，shared_ptr<ResourceManager> _resource_manager;
 * 3. 临时，临时对象通过const T *, const T & 传递，临时对象不应该获得所有权
 *
 * C++ 对象管理，在过往中还使用以下方法，但无法达到所有权管理的效果
 * 1. pointer 指针无法体现所有权，无法引用计数，这是最原始的用法，出于兼容C的考虑被大量使用
 * 2. ref 引用虽然体现了所有权的逻辑，但无法引用计数，在如内存池这样的应用场景，分配出去的内存块，在没有合适归还时，会出现无法管理的错误
 *
 * 在过往的代码中，充满了上述的旧有代码，新的实现需要认为旧有代码是不可靠{unsafe}的，需要使用检查工具隔离的，参考以下线索，以便迁移到安全的实现
 *
 * https://github.com/isocpp/CppCoreGuidelines
 * https://msdn.microsoft.com/en-us/library/d3bbz7tz.aspx, https://blogs.msdn.microsoft.com/vcblog/2015/12/03/c-core-guidelines-checkers-available-for-vs-2015-update-1/
 * http://clang.llvm.org/extra/clang-tidy/
 */
int main(int argc, char** argv)
{
    std::unique_ptr<std::string> s(new std::string("hello"));
    std::string & s2 = *s.get(); // should not allow
    std::cout << s2 << std::endl;

    typedef std::reference_wrapper<struct foo_t> foo_ref;
    std::list<foo_ref> foos;

    {
        std::shared_ptr<struct foo_t> foo_ptr;

        {
            struct foo_t value;

            std::shared_ptr<struct foo_t> foo_pointer(&value); // take ownership, should not allow
            //struct foo_t& foo = *foo_pointer; // ref ownership, should not allow
            struct foo_t foo = *foo_pointer; // copy is ok

            std::shared_ptr<struct foo_t> temp(&foo); // take ownership
            foo_ptr = temp; // share ownership, it is ok, but failed for temp

            std::cout << foo_ptr->id << std::endl;
        }

        std::cout << foo_ptr->id << std::endl;
    }

    return 0;
}
