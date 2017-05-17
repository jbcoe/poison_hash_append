#pragma once
#include <type_traits>

namespace std
{
namespace experimental
{

template <class T>
struct is_contiguously_hashable : std::false_type{};

template <>
struct is_contiguously_hashable<size_t> : std::true_type
{
};

template <>
struct is_contiguously_hashable<int> : std::true_type
{
};

template <class HashAlgorithm, class T>
inline std::enable_if_t<is_contiguously_hashable<T>::value>
hash_append(HashAlgorithm& h, T const& t) noexcept
{
  h(addressof(t), sizeof(t));
}

template <class HashAlgorithm, class T, class Alloc>
inline std::enable_if_t<!is_contiguously_hashable<T>::value>
hash_append(HashAlgorithm& h, std::vector<T, Alloc> const& v) noexcept
{
  for (auto const& t : v)
    hash_append(h, t);
  hash_append(h, v.size());
}

template <class HashAlgorithm, class T, class Alloc>
inline std::enable_if_t<is_contiguously_hashable<T>::value>
hash_append(HashAlgorithm& h, std::vector<T, Alloc> const& v) noexcept
{
  h(v.data(), v.size() * sizeof(T));
  hash_append(h, v.size());
}

template <class HashAlgorithm, class T>
inline std::enable_if_t<std::is_floating_point<T>::value>
hash_append(HashAlgorithm& h, T t) noexcept
{
  if (t == 0)
    t = 0;
  h(&t, sizeof(t));
}

template <class HashAlgorithm, class T0, class T1, class... Ts>
inline void hash_append(HashAlgorithm& h, T0 const& t0, T1 const& t1,
                        Ts const&... ts) noexcept
{
  hash_append(h, t0);
  hash_append(h, t1, ts...);
}

class fnv1a
{
  std::size_t state_ = 14695981039346656037u;

public:
  using result_type = std::size_t;

  void operator()(void const* key, std::size_t len) noexcept
  {
    unsigned char const* p = static_cast<unsigned char const*>(key);
    unsigned char const* const e = p + len;
    for (; p < e; ++p)
      state_ = (state_ ^ *p) * 1099511628211u;
  }

  explicit operator result_type() noexcept
  {
    return state_;
  }
};

template <class HashAlgorithm = fnv1a>
struct uhash
{
  using result_type = typename HashAlgorithm::result_type;

  template <class T>
  result_type operator()(T const& t) const noexcept
  {
    HashAlgorithm h;
    using std::experimental::hash_append;

    hash_append(h, t);
    return static_cast<result_type>(h);
  }
};

} // namespace experimental
} // namespace std
