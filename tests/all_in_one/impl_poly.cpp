#include "test.hpp"

template <>
struct pimpl::impl_ptr<Base>::implementation
{
	implementation(int k) : base_int_(k), trace_("Base::implementation(int)") {}
	virtual ~implementation() = default;

	virtual string call_virtual() { return ("Base::call_virtual()"); }

	void* operator new(size_t sz)
	{
		return malloc(sz);
	};
	void operator delete(void* p, size_t)
	{
		if (p)
			free(p);
	}
	int base_int_;
	string trace_;
};

struct Derived1Impl : pimpl::impl_ptr<Base>::implementation
{
	using this_impl = Derived1Impl;
	using base_impl = pimpl::impl_ptr<Base>::implementation;

	Derived1Impl(int k, int l) : base_impl(k), derived_int_(l)
	{
		EXPECT_TRUE(trace_ == "Base::implementation(int)");
		trace_ = "Derived1::implementation(int, int)";
	}
	~Derived1Impl()
	{
		// printf("Derived1::~implementation()\n");
	}
	virtual string call_virtual() { return ("Derived1::call_virtual()"); }

	int derived_int_;
};

struct Derived2Impl : Derived1Impl
{
	using base_impl = Derived1Impl;
	using this_impl = Derived2Impl;

	Derived2Impl(int k, int l, int m) : base_impl(k, l), more_int_(m)
	{
		EXPECT_TRUE(trace_ == "Derived1::implementation(int, int)");
		trace_ = "Derived2::implementation(int, int, int)";
	}
	~Derived2Impl()
	{
		//        printf("Derived2::~implementation()\n");
	}
	virtual string call_virtual() { return ("Derived2::call_virtual()"); }

	int more_int_;
};

Base::Base(int k) : impl_ptr_type(in_place, k)
{
}

Derived1::Derived1(int k, int l) : Base(pimpl::impl_ptr<Base>::null())
{
	emplace<Derived1Impl>(k, l);
}

int Derived1::getDerived1Data() const
{
	auto ret = getImpl<Derived1Impl>()->derived_int_;
	return ret;
}

Derived2::Derived2(int k, int l, int m) : Derived1(pimpl::impl_ptr<Derived1>::null())
{
	emplace<Derived2Impl>(k, l, m);
}

string Base::trace() const
{
	return *this ? (*this)->trace_ : "null";
}

string Base::call_virtual()
{
	return (*this)->call_virtual();
}

template <>
struct pimpl::impl_ptr<Object>::implementation
{
	implementation() { flags = 33; }

	virtual ~implementation() = default;

	virtual std::string call_virtual() { return ("Base::call_virtual()"); }

	char flags;
	std::string trace_{"Base::implementation(int)"};
	int dwgType{123};
};
/**********Object class*************/
Object::Object() : impl_ptr_type(in_place)
{
}

Object::~Object()
{
}

std::string Object::trace() const
{
	return (*this)->trace_;
}

int Object::getDwgType() const
{
	return (*this)->dwgType;
}

char Object::getFlags() const
{
	return (*this)->flags;
}

struct EntityImpl : pimpl::impl_ptr<Object>::implementation
{
	EntityImpl() : implementation() {}

	virtual ~EntityImpl() = default;

	bool invisible{false};
};

Entity::Entity() : Object(pimpl::impl_ptr<Object>::null())
{
	emplace<EntityImpl>();
}

Entity::~Entity()
{
}

bool Entity::invisible() const
{
	return getImpl<EntityImpl>()->invisible;
}
