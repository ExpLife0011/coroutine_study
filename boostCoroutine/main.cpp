/*
* Э����Э��֮����ڵ����뱻���õĹ�ϵ����Ҫ����pull_type��push_type�Ĺ�ϵ
* ���ֲ�ƽ�ȵĹ�ϵ��֤�˱������߻��������ߣ���ɷ�����һ������
* �����в������ڣ�
* �������ߣ�void functionname(boost::coroutines2::coroutine<int>::pull_type &) 
* ������  ��boost::coroutines2::coroutine<int>::push_type source(������)
*/
/*
#include <iostream>
#include <boost/coroutine2/all.hpp>

constexpr int N = 10;

// ����һ���й��о�
void foo(boost::coroutines2::coroutine<int>::pull_type & sink) {
    using coIter = boost::coroutines2::coroutine<int>::pull_type::iterator;
    for (coIter start = begin(sink); start != end(sink); ++start) {
        std::cout << "retrieve " << *start << "\n";
    }
}
// ��������auto�Զ��Ƶ�
void foo2(boost::coroutines2::coroutine<int>::pull_type & sink) {
    for (auto val : sink) {
        std::cout << "retrieve " << val << "\n";
    }
}
// ���������ؾ�
void foo3(boost::coroutines2::coroutine<int>::pull_type & sink) {
    for (int i = 0; i < N; i++) {
        std::cout << "retrieve " << sink.get() << "\n";
        sink();
    }
}

int main() {
    boost::coroutines2::coroutine<int>::push_type source(foo);
    for (int i = 0; i < N; i++) {
        source(i);
    }
    return 0;
}
*/
/*
#include <boost/coroutine2/all.hpp>
#include <tuple>
#include <string>
#include <iostream>


void cooperative(boost::coroutines2::coroutine<std::tuple<int, std::string>>::pull_type &source)
{
    auto args = source.get();
    std::cout << std::get<0>(args) << " " << std::get<1>(args) << '\n';
    source();
    args = source.get();
    std::cout << std::get<0>(args) << " " << std::get<1>(args) << '\n';
}

int main()
{
    boost::coroutines2::coroutine<std::tuple<int, std::string>>::push_type sink{ cooperative };
    sink(std::make_tuple(0, "aaa"));
    sink(std::make_tuple(1, "bbb"));
    std::cout << "end\n";
}
*/
#include<boost/thread/thread.hpp>
#include<boost/coroutine2/all.hpp>
#include<iostream>
#include<thread>
typedef boost::coroutines2::coroutine<int> boost_coroutine;

void function() {
    std::cout << "in function" << std::endl;
}
int main() 
{
    /*thread *t = new thread(function);
    thread::id id= t->get_id();
    
    std::cout << id << std::endl;*/
}