//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <Containers\ForwardList.hpp>

// iterator erase_after(const_iterator p);

#include <Containers\ForwardList.hpp>
#include <cassert>



int main()
{
    {
        typedef int T;
        typedef Yupei::forward_list<T> C;
        const T t[] = {0, 1, 2, 3, 4};
        C c(std::begin(t), std::end(t));

        C::iterator i = c.erase_after(std::next(c.cbefore_begin(), 4));
        assert(i == c.end());
        assert(std::distance(c.begin(), c.end()) == 4);
        assert(*std::next(c.begin(), 0) == 0);
        assert(*std::next(c.begin(), 1) == 1);
        assert(*std::next(c.begin(), 2) == 2);
        assert(*std::next(c.begin(), 3) == 3);

        i = c.erase_after(std::next(c.cbefore_begin(), 0));
        assert(i == c.begin());
        assert(std::distance(c.begin(), c.end()) == 3);
        assert(*std::next(c.begin(), 0) == 1);
        assert(*std::next(c.begin(), 1) == 2);
        assert(*std::next(c.begin(), 2) == 3);

        i = c.erase_after(std::next(c.cbefore_begin(), 1));
        assert(i == std::next(c.begin()));
        assert(std::distance(c.begin(), c.end()) == 2);
        assert(*std::next(c.begin(), 0) == 1);
        assert(*std::next(c.begin(), 1) == 3);

        i = c.erase_after(std::next(c.cbefore_begin(), 1));
        assert(i == c.end());
        assert(std::distance(c.begin(), c.end()) == 1);
        assert(*std::next(c.begin(), 0) == 1);

        i = c.erase_after(std::next(c.cbefore_begin(), 0));
        assert(i == c.begin());
        assert(i == c.end());
        assert(std::distance(c.begin(), c.end()) == 0);
    }
    assert(_CrtDumpMemoryLeaks() == 0);
}
