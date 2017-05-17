#define CATCH_CONFIG_MAIN 
#include "catch.hpp"
#include "hash_append.h"

using std::experimental::uhash;

TEST_CASE("hash double", "[hash_append]")
{
  uhash<> h;

  // We just want to exercise the hash.
  REQUIRE(h(3.14159) != h(2.782818));
}

TEST_CASE("hash vector<int>", "[hash_append]")
{
  uhash<> h;
  
  std::vector<int> xs = {0,1,2,3,4,5,6,7,8};

  // We just want to exercise the hash.
  REQUIRE(h(xs) == h(xs));
}

struct A {};

struct B
{
  B(const A&)
  {
  }

  B()
  {
  }

  A a_;
};

template <class H>
inline void hash_append(H& h, const B& b) noexcept
{
  hash_append(h, b.a_); // hash member as required.
}

TEST_CASE("hash user defined type with implicit constructor from member type", "[hash_append]")
{
  uhash<> h;

  B b;

  // We just want to exercise the hash.
  REQUIRE(h(b) == h(b));
}

