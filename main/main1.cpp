
#include <memory>
#include <iostream>

struct foo_t
{
    int id = 111;
};

/**
 * define a struct and assign its address to a pointer,
 *
 * struct will be released when scope leaving, foo_pointer will not be released.
 * foo_ptr will be released after scope, causes segment fault.
 *
 * http://stackoverflow.com/questions/30011603/how-to-enable-rust-ownership-paradigm-in-c
 *
 * a std::shared_ptr/std::unique_ptr should be initialized by make_shared/make_unique,
 * or exception will happened
 */
int main(int argc, char** argv)
{
    {
        std::shared_ptr<struct foo_t> foo_ptr;

        {
            struct foo_t foo;

            std::shared_ptr<struct foo_t> foo_pointer(&foo); // foo_pointer->ref == 1, SHOULD NOT TAKE OWNERSHIP HERE
            foo_ptr = foo_pointer; // foo_ptr->ref == 2 & foo_pointer->ref == 2

        } // foo_pointer->ref = 1, will not free, foo_ptr->ref = 1, will not free

        //std::cout << "i am iostream" << std::endl;

        std::cout << foo_ptr->id << std::endl;
    } // foo_ptr->ref = 0, will free, exception happens

    return 0;
}
