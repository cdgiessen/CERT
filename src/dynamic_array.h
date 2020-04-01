#pragma once

namespace cert
{
template <typename T> class DynamicArray
{
	T* m_data = nullptr;
	std::size_t m_size = 0;
	std::size_t m_allocated = 0;

	public:
	constexpr DynamicArray ()
	{
		m_allocated = 100;
		m_data = new T[m_allocated];
		m_size = 0;
	}
	constexpr DynamicArray (std::size_t size)
	{
		m_data = new T[size];
		m_size = size;
		m_allocated = size;
	}
	constexpr ~DynamicArray ()
	{
		if (m_data != nullptr) delete[] m_data;
	}
	constexpr DynamicArray (DynamicArray const& obj) = delete;
	constexpr DynamicArray& operator= (DynamicArray const& obj) = delete;

	constexpr DynamicArray (DynamicArray&& obj)
	: m_data (obj.m_data), m_size (obj.m_size), m_allocated (obj.m_allocated)
	{
		obj.m_data = nullptr;
	}
	constexpr DynamicArray& operator= (DynamicArray&& obj)
	{
		m_data = obj.m_data;
		m_size = obj.m_size;
		m_allocated = obj.m_allocated;
		obj.m_data = nullptr;
		return *this;
	}

	constexpr void resize (std::size_t new_size)
	{
		if (new_size > m_allocated)
		{
			T* temp = new T[m_allocated * 2];
			for (int i = 0; i < m_size; i++)
			{
				temp[i] = m_data[i];
			}
			delete[] m_data;
			m_data = temp;
			m_size = new_size;
			m_allocated = m_allocated * 2;
		}
		else
		{
			m_size = new_size;
		}
	}

	constexpr void reserve (std::size_t new_reserve)
	{
		if (new_reserve > m_allocated)
		{
			T* temp = new T[new_reserve];
			for (int i = 0; i < m_size; i++)
			{
				temp[i] = m_data[i];
			}
			delete[] m_data;
			m_data = temp;
			m_allocated = new_reserve;
		}
	}

	constexpr void push_back (T value)
	{
		int new_val_index = m_size;
		if (m_size >= m_allocated)
		{
			reserve (2 * m_allocated);
		}
		m_data[new_val_index] = value;
		m_size++;
	}

	constexpr void erase (std::size_t index)
	{
		// TODO: make shrinking vector
	}

	constexpr void reset () { m_size = 0; }

	constexpr T const& at (std::size_t index) const { return m_data[index]; }
	constexpr T& at (std::size_t index) { return m_data[index]; }
	constexpr std::size_t size () const { return m_size; }
	constexpr T* data () const { return m_data; }
};
} // namespace cert