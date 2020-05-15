#include "static_hash.h"
#include "dynamic.h"
class test_tuple
{
private:
public:
    int other;
    int key;
    test_tuple(void);
    test_tuple(int _key, int _other);
    ~test_tuple();
};
test_tuple::test_tuple(void)
{
}
test_tuple::test_tuple(int _key, int _other)
{
    key = _key;
    other = _other;
}

test_tuple::~test_tuple()
{
}

int main()
{
    test_tuple *test = new test_tuple(4, 4);
    test_tuple *test2 = new test_tuple(5, 7);
    test_tuple *test3 = new test_tuple(6, 7);
    test_tuple *test4 = new test_tuple(7, 8);
    test_tuple *tester = new test_tuple();
    dynamic_hash<test_tuple> d_hash;
    d_hash.insert_to_hash((*test));
    d_hash.insert_to_hash((*test2));
    d_hash.insert_to_hash((*test3));
    d_hash.insert_to_hash((*test4));
    std::cout << d_hash.search_in_hash(5,tester);

    
    
}