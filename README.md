Observed issue in the implementation of `hash_append`
====================================================

*TL;DR* Implicit conversions can cause infinite recursion in `hash_append`. A
suitably defined function template will poison the overload set and push
run-time failures out to compile time.

Introduction
------------
Hinnant et al. propose the addition of `hash_append` to the C++ Standard Library [1].

The paper proposes the addition of a function template, `hash_append` as a
customization point so that for a user-defined type one defines an overload of
the function and hashing, with a yet-to-be-defined hash works as intended.  The
proposal is in that it seprates out what to hash and how to hash it.

```
class student {
  std::string name_;
  size_t id_;
};


template<class H>
void hash_append(H& h, const student& s) {
  hash_append(h, s.name_, s.id_);
}
```


Implicit conversion leads to infinite recursion
-----------------------------------------------

Having implemented `hash_append` as proposed in a real-world code base we can
strongly recommend the design but have encountered a small issue.

Suppose we have a user-defined type with an implicit conversion:

```
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
  hash_append(h, b.a_);
}
```

Failure to specify `hash_append` for `A` does not result in a compile error. Instead the implicit conversion of 
an `A` to a `B` is invoked and `hash_append(h, b.a_)` results in another call to `hash_append(H& h, const B& b)`.
This results in infinite recursion and a hard-to-diagnose problem at run-time.


Proposed amendment
------------------

Adding the overload below will prevent a conversion being selected as an
overload unless `hash_append` has been explicitly specified for a value-type.

```
template <class H, class T>
inline void hash_append(H&, T) = delete;
```

We propose that this overload is added to the functions proposed by Hinnant et al.                                     


Building example code
---------------------

Example code can be built with macOS, Linux and Windows (Visual Studio 2015) using the CMake driver script:

`python ./scripts/build.py -t`

Note that this will crash as the tests are intedned to demonstrate the infinite recursion in action.


References
----------

[1] _Types don't know #_ <http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n3980.html>

