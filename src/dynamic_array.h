#pragma once


template <typename T> class DynamicArray
{
	T* m_data = nullptr;
	std::size_t m_size = 0;
	std::size_t m_allocated = 0;

	public:
	constexpr DynamicArray ()
	{
		m_allocated = 10;
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
	DynamicArray (DynamicArray const& obj) = delete;
	DynamicArray& operator= (DynamicArray const& obj) = delete;

	DynamicArray (DynamicArray&& obj)
	: m_data (obj.m_data), m_size (obj.m_size), m_allocated (obj.m_allocated)
	{
		obj.m_data = nullptr;
	}
	DynamicArray& operator= (DynamicArray&& obj)
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
			T* temp = new float[m_allocated * 2];
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

	constexpr T& set (std::size_t index, T value)
	{
		if (index < m_size)
		{
			m_data[index] = value;
			return m_data[index];
		}
	}

	constexpr void push_back (T value)
	{
		m_data[m_size] = value;
		m_size++;
	}

	constexpr void erase (std::size_t index) {}

	constexpr void reset () { m_size = 0; }

	constexpr T const& at (std::size_t index) const { return m_data[index]; }
	constexpr std::size_t size () const { return m_size; }
	constexpr T* data () const { return m_data; }
};