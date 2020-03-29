#pragma once

namespace cert
{

template <typename T> class UniquePtr
{
	T* ptr;

	public:
	constexpr UniquePtr (T* ptr) : ptr (ptr) {}
	constexpr ~UniquePtr () { delete ptr; }
	constexpr UniquePtr (UniquePtr<T> const& other) = delete;
	constexpr UniquePtr& operator= (UniquePtr<T> const& other) = delete;
	constexpr UniquePtr (UniquePtr<T>&& other) : ptr (other.ptr) { other.ptr = nullptr; };
	constexpr UniquePtr& operator= (UniquePtr<T>&& other)
	{
		ptr = other.ptr;
		other.ptr = nullptr;
	};
	T* get () const { return ptr; }
};

} // namespace cert