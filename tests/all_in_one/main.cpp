#include "test.hpp"

int main()
{
	testing::InitGoogleTest();
	return RUN_ALL_TESTS();
}

TEST(test_basics, basics)
{
	Book book("Dune", "Frank Herbert");

	EXPECT_TRUE(book.title() == "Dune");
	EXPECT_TRUE(book.author() == "Frank Herbert");
}

TEST(test_is_pimpl, is_pimpl)
{
	static_assert(false == pimpl::is_impl_ptr<Foo>::value, "");
	static_assert(false == pimpl::is_impl_ptr<int>::value, "");
	static_assert(false == pimpl::is_impl_ptr<int*>::value, "");
	static_assert(true == pimpl::is_impl_ptr<Shared>::value, "");
	static_assert(true == pimpl::is_impl_ptr<Shared const>::value, "");
	static_assert(false == pimpl::is_impl_ptr<Shared*>::value, "");
	static_assert(true == pimpl::is_impl_ptr<Copied>::value, "");
	static_assert(true == pimpl::is_impl_ptr<Base>::value, "");
	static_assert(true == pimpl::is_impl_ptr<Derived1>::value, "");
	static_assert(true == pimpl::is_impl_ptr<Derived1 const>::value, "");
	static_assert(true == pimpl::is_impl_ptr<Derived2>::value, "");
}

TEST(test_swap, swap)
{
	Shared s01(1);
	Shared s02(2);
	Copied c03(5);
	Copied c04(6);
	void const* p01 = &*s01;
	void const* p02 = &*s02;
	void const* p03 = &*c03;
	void const* p04 = &*c04;

	EXPECT_TRUE(s01.trace() == "Shared(int)");
	EXPECT_TRUE(s02.trace() == "Shared(int)");
	EXPECT_TRUE(s01 != s02);
	EXPECT_TRUE(&*s01 != &*s02);
	EXPECT_TRUE(c03.trace() == "Copied(int)");
	EXPECT_TRUE(c04.trace() == "Copied(int)");
	EXPECT_TRUE(&*c03 != &*c04);

	EXPECT_TRUE(s01.value() == 1);
	EXPECT_TRUE(s02.value() == 2);
	EXPECT_TRUE(&*s01 == p01);
	EXPECT_TRUE(&*s02 == p02);
	EXPECT_TRUE(c03.value() == 5);
	EXPECT_TRUE(c04.value() == 6);
	EXPECT_TRUE(&*c03 == p03);
	EXPECT_TRUE(&*c04 == p04);

	s01.swap(s02);
	c03.swap(c04);

	EXPECT_TRUE(s01.value() == 2);
	EXPECT_TRUE(s02.value() == 1);
	EXPECT_TRUE(&*s01 == p02);
	EXPECT_TRUE(&*s02 == p01);
	EXPECT_TRUE(c03.value() == 6);
	EXPECT_TRUE(c04.value() == 5);
	EXPECT_TRUE(&*c03 == p04);
	EXPECT_TRUE(&*c04 == p03);
}

TEST(test_runtime_polymorphic_behavior, runtime_polymorphic_behavior)
{
	Base base1(1);
	Derived1 derived1(1, 2);
	std::cout << derived1.getDerived1Data() << '\n';
	Derived2 derived2(1, 2, 3);
	Base base2(derived1);
	Base base3(derived2);
	Base base4(Derived2(2, 3, 4) /*const ref to temporary*/);
	Base* bs1 = &base1;
	Base* bs2 = &base2;
	Base* bp3 = &base3;
	Base* bp4 = &derived1;
	Base* bp5 = &derived2;
	Base bad1 = pimpl::impl_ptr<Base>::null();
	Base bad2 = pimpl::impl_ptr<Base>::null();
	Base bad3 = pimpl::impl_ptr<Derived1>::null();
	Base bad4 = pimpl::impl_ptr<Derived2>::null();
	Derived1 bad5(pimpl::impl_ptr<Derived1>::null());
	Derived2 bad6(pimpl::impl_ptr<Derived2>::null());
	std::cout << derived2.call_virtual();

	EXPECT_TRUE(derived1.trace() == "Derived1::implementation(int, int)");
	EXPECT_TRUE(derived2.trace() == "Derived2::implementation(int, int, int)");

	EXPECT_TRUE(base1 != derived1);
	EXPECT_TRUE(base1 != derived2);
	EXPECT_TRUE(base2 == derived1);
	EXPECT_TRUE(base3 == derived2);
	EXPECT_TRUE(derived1 != base1);
	EXPECT_TRUE(derived2 != base1);
	EXPECT_TRUE(derived1 == base2);
	EXPECT_TRUE(derived2 == base3);

	bool bad1_bool1 = bad5 ? true : false; // Shared conversion to bool
	bool bad1_bool2 = !!bad5;			   // Shared operator!()
	bool bad2_bool1 = bad6 ? true : false; // Shared conversion to bool
	bool bad2_bool2 = !!bad6;			   // Shared operator!()

	EXPECT_TRUE(!bad1_bool1);
	EXPECT_TRUE(!bad1_bool2);
	EXPECT_TRUE(!bad2_bool1);
	EXPECT_TRUE(!bad2_bool2);

	EXPECT_TRUE(base2.call_virtual() == "Derived1::call_virtual()");
	EXPECT_TRUE(base3.call_virtual() == "Derived2::call_virtual()");
	EXPECT_TRUE(bs1->call_virtual() == "Base::call_virtual()");
	EXPECT_TRUE(bs2->call_virtual() == "Derived1::call_virtual()");
	EXPECT_TRUE(bp3->call_virtual() == "Derived2::call_virtual()");
	EXPECT_TRUE(bp4->call_virtual() == "Derived1::call_virtual()");
	EXPECT_TRUE(bp5->call_virtual() == "Derived2::call_virtual()");

	const Object obj;
	std::cout << obj.trace() << '\n';
	std::cout << obj.getDwgType() << '\n';
	char ret = obj.getFlags();
	std::cout << ret;
	std::cout << '\n';

	Entity ent;
	std::cout << ent.getDwgType() << '\n';
	std::cout << ent.getFlags() << '\n';
	std::cout << std::boolalpha << ent.invisible();
}

TEST(test_null, null)
{
	Shared s01 = pimpl::impl_ptr<Shared>::null();
	EXPECT_TRUE(s01.trace() == "null");
	Shared s02(pimpl::impl_ptr<Shared>::null());
	EXPECT_TRUE(s02.trace() == "null");
	Unique u01 = pimpl::impl_ptr<Unique>::null();
	EXPECT_TRUE(u01.trace() == "null");
	Unique u02(pimpl::impl_ptr<Unique>::null());
	EXPECT_TRUE(u02.trace() == "null");
	Copied c01 = pimpl::impl_ptr<Copied>::null();
	EXPECT_TRUE(c01.trace() == "null");
	Copied c02(pimpl::impl_ptr<Copied>::null());
	EXPECT_TRUE(c02.trace() == "null");

	Base p03(pimpl::impl_ptr<Base>::null());
	EXPECT_TRUE(p03.trace() == "null");
	Derived1 p04(pimpl::impl_ptr<Derived1>::null());
	EXPECT_TRUE(p04.trace() == "null");
	Derived2 p05(pimpl::impl_ptr<Derived2>::null());
	EXPECT_TRUE(p05.trace() == "null");
	Base p06(p03);
	EXPECT_TRUE(p06.trace() == "null");
	Base p07(p04);
	EXPECT_TRUE(p07.trace() == "null");
	Base p08(p05);
	EXPECT_TRUE(p08.trace() == "null");
}

TEST(test_shared, shared)
{
	Foo foo;
	Foo const const_foo = foo;
	Foo& ref = foo;
	Foo const& const_ref = const_foo;
	Foo* ptr = &foo;
	Foo const* const_ptr = &const_foo;

	Shared s11;
	EXPECT_TRUE(s11.trace() == "Shared()");
	Shared s12(5);
	EXPECT_TRUE(s12.trace() == "Shared(int)");
	Shared s15(5, 6);
	EXPECT_TRUE(s15.trace() == "Shared(int, int)");
	Shared s13 = s12;
	EXPECT_TRUE(&*s13 == &*s12); // Implementation shared.
	Shared s14(s12);
	EXPECT_TRUE(&*s14 == &*s12);			   // Implementation shared.
	EXPECT_TRUE(s13.trace() == "Shared(int)"); // trace state is the same
	EXPECT_TRUE(s14.trace() == "Shared(int)"); // trace state is the same
	Shared s21(foo);
	EXPECT_TRUE(s21.trace() == "Shared(Foo&)");
	Shared s22(const_foo);
	EXPECT_TRUE(s22.trace() == "Shared(Foo const&)");
	Shared s23(ref);
	EXPECT_TRUE(s23.trace() == "Shared(Foo&)");
	Shared s24(const_ref);
	EXPECT_TRUE(s24.trace() == "Shared(Foo const&)");
	Shared s25(ptr);
	EXPECT_TRUE(s25.trace() == "Shared(Foo*)");
	Shared s26(const_ptr);
	EXPECT_TRUE(s26.trace() == "Shared(Foo const*)");

	Shared s16(test::singleton);
	EXPECT_TRUE(s16.trace() == "Shared()");
	Shared s17(test::singleton);
	EXPECT_TRUE(s17.trace() == "Shared()");
	EXPECT_TRUE(&*s17 == &*s16); // Implementation shared.
	Shared s31;
	Shared s32(5);
	Shared s33 = s32;

	EXPECT_TRUE(s32 != s31); // calls impl_ptr::op!=()
	EXPECT_TRUE(s32 == s33); // calls impl_ptr::op==()
}

TEST(test_copied, copied)
{
	Copied c11;
	EXPECT_TRUE(c11.trace() == "Copied()");
	Copied c12(5);
	EXPECT_TRUE(c12.trace() == "Copied(int)");
	Copied c13 = c12;
	EXPECT_TRUE(c13.trace() == "Copied(Copied const&)");
	Copied c14(c12);
	EXPECT_TRUE(c14.trace() == "Copied(Copied const&)");
	EXPECT_TRUE(&*c13 != &*c12); // Copied.
	EXPECT_TRUE(&*c14 != &*c12); // Copied.

	Copied c21(3);
	Copied c22(5);

	EXPECT_TRUE(&*c21 != &*c22);
	EXPECT_TRUE(c21 != c22);

	c21 = c22;

	EXPECT_TRUE(c21.trace() == "Copied::operator=(Copied const&)");
	EXPECT_TRUE(&*c21 != &*c22);
	EXPECT_TRUE(c21 == c22);

	Copied c31;
	Copied c32(5);
	Copied c33 = c32;

	EXPECT_TRUE(c32 == c33);
	EXPECT_TRUE(c32.trace() == "Copied::operator==(Copied const&)");
	EXPECT_TRUE(c32 != c31);
	EXPECT_TRUE(c32.trace() == "Copied::operator==(Copied const&)");
}

TEST(test_unique, unique)
{
	Unique u11(3);
	EXPECT_TRUE(u11.value() == 3);
	Unique u12(5);
	EXPECT_TRUE(u12.value() == 5);

	//  u11 = u12; // Properly does not compile
	u11 = Unique(6);
	EXPECT_TRUE(u11.value() == 6);
}

TEST(test_inplace, inplace)
{
	InPlace s11(3);
	EXPECT_TRUE(s11.value() == 3);
	InPlace s12(5);
	EXPECT_TRUE(s12.value() == 5);
	InPlace s13 = pimpl::impl_ptr<InPlace>::null();

	// Check that implementation is allocated on the stack.
	EXPECT_TRUE((void*)&s11 == (void*)&*s11);
	EXPECT_TRUE(!bool(s13)); // Test op bool()
	EXPECT_TRUE(!s13);		 // Test op!()

	s11 = s12;
	EXPECT_TRUE(s11.value() == 5);
	s11 = InPlace(6);
	EXPECT_TRUE(s11.value() == 6);
}

TEST(test_always_inplace, always_inplace)
{
	AlwaysInPlace s11(3);
	EXPECT_TRUE(s11.value() == 3);
	AlwaysInPlace s12(5);
	EXPECT_TRUE(s12.value() == 5);
	AlwaysInPlace s13;

	// Check that implementation is allocated on the stack.
	EXPECT_TRUE((void*)&s11 == (void*)&*s11);
	EXPECT_TRUE(bool(s13)); // Test op bool()
	EXPECT_TRUE(!!s13);		// Test op!()

	s11 = s12;
	EXPECT_TRUE(s11.value() == 5);
	s11 = AlwaysInPlace(6);
	EXPECT_TRUE(s11.value() == 6);
}

TEST(test_bool_conversions, bool_conversions)
{
	Shared s1;
	Copied c1;
	Shared s2 = pimpl::impl_ptr<Shared>::null();
	Copied c2 = pimpl::impl_ptr<Copied>::null();

	EXPECT_TRUE(s1.trace() == "Shared()");
	EXPECT_TRUE(c1.trace() == "Copied()");
	EXPECT_TRUE(s2.trace() == "null");
	EXPECT_TRUE(c2.trace() == "null");

	EXPECT_TRUE(bool(s1)); // Test op bool()
	EXPECT_TRUE(!!s1);	   // Test op!()
	EXPECT_TRUE(bool(c1));
	EXPECT_TRUE(!!c1);
	EXPECT_TRUE(!bool(s2));
	EXPECT_TRUE(!s2);
	EXPECT_TRUE(!bool(c2));
	EXPECT_TRUE(!c2);
}
