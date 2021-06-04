#pragma once

#include "impl_ptr.h"
#include <string>
#include "gtest/gtest.h"

using string = std::string;
namespace policy = pimpl::impl_ptr_policy;

//struct Book :pimpl::impl_ptr<Book>::shared
struct Book : pimpl::impl_ptr<Book, policy::shared>
{
	Book(string const& title, string const& author);

	string const& title() const;
	string const& author() const;
};

struct test
{
	struct singleton_type
	{
	};

	static constexpr singleton_type singleton{};
};

struct Foo
{
};

struct Shared : pimpl::impl_ptr<Shared>::shared // Pure interface.
{
	Shared();
	Shared(int);
	Shared(int, int);
	Shared(Foo&);
	Shared(Foo const&);
	Shared(Foo const&, Foo const&);
	Shared(Foo&, Foo const&);
	Shared(Foo const&, Foo&);
	Shared(Foo&, Foo&);
	Shared(Foo*);
	Shared(Foo const*);
	Shared(test::singleton_type);

	string trace() const;
	int value() const;
};

struct Unique : pimpl::impl_ptr<Unique>::unique // Pure interface.
{
	Unique();
	Unique(int);

	string trace() const;
	int value() const;
};

struct Copied : pimpl::impl_ptr<Copied>::copied // Pure interface.
{
	Copied();
	Copied(int);

	// Value-semantics Pimpl must explicitly define comparison operators
	// if it wants to be comparable. The same as normal classes do.
	bool operator==(Copied const& o) const;
	bool operator!=(Copied const& o) const { return !operator==(o); }

	string trace() const;
	int value() const;
};

//struct InPlace :pimpl::impl_ptr<InPlace>::onstack<int[16]>
struct InPlace : pimpl::impl_ptr<InPlace, policy::inplace, policy::storage<64>>
{
	InPlace();
	InPlace(int);

	// Value-semantics Pimpl must explicitly define comparison operators
	// if it wants to be comparable. The same as normal classes do.
	bool operator==(InPlace const& o) const;
	bool operator!=(InPlace const& o) const { return !operator==(o); }

	string trace() const;
	int value() const;
};

struct AlwaysInPlace : pimpl::impl_ptr<AlwaysInPlace, policy::always_inplace, policy::storage<sizeof(void*) * 2, alignof(void*)>>
{
	AlwaysInPlace();
	AlwaysInPlace(int);

	// Value-semantics Pimpl must explicitly define comparison operators
	// if it wants to be comparable. The same as normal classes do.
	bool operator==(AlwaysInPlace const& o) const;
	bool operator!=(AlwaysInPlace const& o) const { return !operator==(o); }

	string trace() const;
	int value() const;
};

struct Base : pimpl::impl_ptr<Base>::shared
{
	Base(int);

	string call_virtual(); // Non-virtual.
	string trace() const;
};

struct Derived1 : Base
{
	Derived1(int, int);

	int getDerived1Data() const;
};

struct Derived2 : Derived1
{
	Derived2(int, int, int);
};


class  Object : public pimpl::impl_ptr<Object>::shared
{
public:
	Object();

	~Object();

	std::string trace() const;

	int getDwgType() const;

	char getFlags() const;
};

class  Entity : public Object
{
public:
	Entity();
	~Entity();

	bool invisible() const;
};
