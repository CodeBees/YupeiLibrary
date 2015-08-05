#include "..\..\Source\Stl\memory.h"
#include <iostream>

struct Foo { int i = 0; };

//LWG 2228
template <class T>
class deleter
{
	// count the number of times this deleter is used
	unsigned count_ = 0;
public:
	// special members
	~deleter() { count_ = std::numeric_limits<unsigned>::max(); }
	deleter() = default;
	deleter(const deleter&) {}
	deleter& operator=(const deleter&) { return *this; }
	deleter(deleter&& d)
		: count_(d.count_)
	{
		d.count_ = 0;
	}
	deleter& operator=(deleter&& d)
	{
		count_ = d.count_;
		d.count_ = 0;
		return *this;
	}

	// converting constructors
	template <class U,
	class = std::enable_if_t<
		std::is_convertible<U*, T*>::value >>
		deleter(const deleter<U>& d)
	{}

	template <class U,
	class = std::enable_if_t<std::is_convertible<U*, T*>::value >>
		deleter(deleter<U>&& d)
		: count_(d.count_)
	{
		d.count_ = 0;
	}

	// deletion operator
	void operator()(T* t)
	{
		delete t;
		++count_;
	}

	// observers
	unsigned count() const { return count_; }

	friend
		std::ostream&
		operator<<(std::ostream& os, const deleter& d)
	{
		return os << d.count_;
	}

	template <class> friend class deleter;
};

struct do_nothing
{
	template <class T>
	void operator()(T*) {}
};

template <class T>
using Ptr = Yupei::unique_ptr<T, deleter<T>&>;

struct base { int i = 0; };

struct derived : base { int j = 0; };
int main()
{
	Yupei::compress_pair<
		Yupei::allocator<int>,
		int> p( Yupei::compress_value_initialize_first_arg,2);
	{
		auto ptr = Yupei::make_unique<int>(3);
		std::cout << *ptr;
		auto rptr = ptr.release();
		Yupei::default_delete<int>{}(rptr);
	}
	{
		auto ptr = Yupei::make_unique<int[]>(3);
		std::cout << ptr[1];
	}
	{
		//Yupei::unique_ptr<Foo const * const[]> ptr1(new Foo*[10]);
	}
	{
		int i = 0;
		Yupei::unique_ptr<int, do_nothing> p1(&i);
		Yupei::unique_ptr<int> p2;
		p1 != p2;
		// This mistakenly compiles:(Yupei's is OK!!!!!)
		//static_assert(Yupei::is_assignable<decltype(p2), decltype(p1)>::value, "hahah");

		 //But this correctly does not compile:
		//p2 = std::move(p1);

	}
	{
		

		deleter<base> db;
		deleter<derived> dd;
		std::cout << "db = " << db << '\n';
		std::cout << "dd = " << dd << '\n';
		{
			using Yupei::swap;
			Ptr<derived> pd(new derived, dd);
			pd.reset(new derived);
			Ptr<base> pb(nullptr, db);
			pb = Yupei::move(pd);  // The converting move assignment!
			std::cout << "pb.get_deleter() = " << pb.get_deleter() << '\n';
			std::cout << "pd.get_deleter() = " << pd.get_deleter() << '\n';
			swap(pd, pd);
		}
		std::cout << "db = " << db << '\n';
		std::cout << "dd = " << dd << '\n';

	}
	getchar();
	_CrtDumpMemoryLeaks();
	return 0;
}