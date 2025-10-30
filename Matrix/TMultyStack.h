#pragma once

#include <iostream>
#include <fstream>
#include "TError.h"
#define ERROR(err,...) throw TError(err, __func__, __FILE__, __LINE__)


template<class T>
class TMultyStack
{
protected:
	size_t capacity;
	size_t count;
	T* data;
	size_t* stacksBegin;
	size_t* starts;
	void Repack(size_t stackpos);
public:
	TMultyStack();
	TMultyStack(size_t count_, size_t size);
	TMultyStack(const TMultyStack& other);
	TMultyStack(TMultyStack&& other) noexcept;
	~TMultyStack();

	bool operator==(const TMultyStack& other);
	bool operator!=(const TMultyStack& other);
	T operator()(size_t stackpos, size_t pos)  const;
	TMultyStack& operator=(const TMultyStack& other);
	TMultyStack& operator=(TMultyStack&& other) noexcept;

	size_t Capacity() const;
	size_t Count() const;
	size_t Size(size_t stackpos) const;
	bool IsFull(size_t stackpos) const;
	bool IsEmpty(size_t stackpos) const;
	void Push(size_t stackpos, const T& elem);
	T Pop(size_t stackpos);
	T FindMin() const;
	void SaveToFile(const std::string& filename) const;
	void LoadFromFile(const std::string& filename);

	template<class O>
	friend std::ostream& operator<<(std::ostream& os, const TMultyStack<O>& stack);
	template<class I>
	friend std::istream& operator>>(std::istream& is, TMultyStack<I>& stack);
};

template<class T>
inline void TMultyStack<T>::Repack(size_t stackpos)
{
	if (stackpos >= count)
		ERROR("stack_error");
	size_t posFirstNoFull(count);
	for (size_t i = 0; i < count; ++i)
		if (!this->IsFull(i))
		{
			posFirstNoFull = i;
			break;
		}
	if (posFirstNoFull == count)
		ERROR("no_empty_stacks");
	if (posFirstNoFull < stackpos)
	{
		for (size_t i = stacksBegin[posFirstNoFull + 1]; i < starts[stackpos]; ++i)
			data[i - 1] = std::move(data[i]);
		for (size_t i = posFirstNoFull + 1; i <= stackpos; ++i)
		{
			stacksBegin[i]-=1;
			starts[i]-=1;
		}
	}
	else
	{
		for (size_t i = starts[posFirstNoFull]-1; i >= stacksBegin[stackpos + 1]; --i)
			data[i+1] = std::move(data[i]);
		for (size_t i = stackpos + 1; i <= posFirstNoFull; ++i)
		{
			stacksBegin[i]+=1;
			starts[i]+=1;
		}
	}
}

template<class T>
inline TMultyStack<T>::TMultyStack():capacity(0),count(0)
{
	data = nullptr;
	stacksBegin = nullptr;
	starts = nullptr;
}

template<class T>
inline TMultyStack<T>::TMultyStack(size_t count_, size_t size):capacity(count_*size),count(count_)
{
	if (capacity == 0 || count == 0)
	{
		capacity = 0;
		count = 0;
		data = nullptr;
		stacksBegin = nullptr;
		starts = nullptr;
	}
	data = new T[capacity];
	stacksBegin = new size_t[count];
	starts = new size_t[count];
	for (size_t i = 0; i < count; ++i)
	{
		stacksBegin[i] = i * size;
		starts[i] = i * size;
	}
}

template<class T>
inline TMultyStack<T>::TMultyStack(const TMultyStack& other):capacity(other.capacity),count(other.count)
{
	data = new T[capacity];
	stacksBegin = new size_t[count];
	starts = new size_t[count];
	for (size_t i = 0; i < capacity; ++i)
		data[i] = other.data[i];
	for (size_t i = 0; i < count; ++i)
	{
		stacksBegin[i] = other.stacksBegin[i];
		starts[i] = other.starts[i];
	}
}

template<class T>
inline TMultyStack<T>::TMultyStack(TMultyStack&& other) noexcept
{
	data = other.data;
	stacksBegin = other.stacksBegin;
	starts = other.starts;
	capacity = other.capacity;
	count = other.count;
	other.data = nullptr;
	other.stacksBegin = nullptr;
	other.starts = nullptr;
	other.capacity = 0;
	other.count = 0;
}

template<class T>
inline TMultyStack<T>::~TMultyStack()
{
	if (capacity)
	{
		delete[] data;
		delete[] starts;
		delete[] stacksBegin;
	}
	capacity = 0;
	count = 0;
}

template<class T>
inline bool TMultyStack<T>::operator==(const TMultyStack& other)
{
	if (count != other.count || capacity != other.capacity)
		return false;
	for (size_t i = 0; i < count; ++i)
	{
		if (starts[i] != other.starts[i] || stacksBegin[i] != other.stacksBegin[i])
			return false;
		for (size_t j = stacksBegin[i]; j < starts[i]; ++j)
			if (data[j] != other.data[j])
				return false;
	}
	return true;
}

template<class T>
inline bool TMultyStack<T>::operator!=(const TMultyStack& other)
{
	if (count != other.count || capacity != other.capacity)
		return true;
	for (size_t i = 0; i < count; ++i)
	{
		if (starts[i] != other.starts[i] || stacksBegin[i] != other.stacksBegin[i])
			return true;
		for (size_t j = stacksBegin[i]; j < starts[i]; ++j)
			if (data[j] != other.data[j])
				return true;
	}
	return false;
}

template<class T>
inline T TMultyStack<T>::operator()(size_t stackpos, size_t pos) const
{
	if (stackpos >= count)
		ERROR("stacks_error");
	if (pos >= this->Size(stackpos))
		ERROR("size_error");
	return data[stacksBegin[stackpos] + pos];
}

template<class T>
inline TMultyStack<T>& TMultyStack<T>::operator=(const TMultyStack<T>& other)
{
	if (this != &other)
	{
		if (data) delete[] data;
		if (stacksBegin) delete[] stacksBegin;
		if (starts) delete[] starts;
		capacity = other.capacity;
		count = other.count;
		data = new T[capacity];
		stacksBegin = new size_t[count];
		starts = new size_t[count];
		for (size_t i = 0; i < count; ++i)
		{
			stacksBegin[i] = other.stacksBegin[i];
			starts[i] = other.starts[i];
		}
		for (size_t i = 0; i < capacity; ++i)
		{
			data[i] = other.data[i];
		}
	}
	return *this;
}

template<class T>
inline TMultyStack<T>& TMultyStack<T>::operator=(TMultyStack<T>&& other) noexcept
{
	if (*this != other)
	{
		data = other.data;
		stacksBegin = other.stacksBegin;
		starts = other.starts;
		count = other.count;
		capacity = other.capacity;
		other.starts = nullptr;
		other.data = nullptr;
		other.stacksBegin = nullptr;
		other.capacity = 0;
		other.count = 0;
	}
	return *this;
}

template<class T>
inline size_t TMultyStack<T>::Capacity() const
{
	return capacity;
}

template<class T>
inline size_t TMultyStack<T>::Count() const
{
	return count;
}

template<class T>
inline size_t TMultyStack<T>::Size(size_t stackpos) const
{
	if (stackpos >= count)
		ERROR("stack_error");
	return starts[stackpos] - stacksBegin[stackpos];
}

template<class T>
inline bool TMultyStack<T>::IsFull(size_t stackpos) const
{
	if (stackpos >= count)
		ERROR("stack_error");
	return stackpos<count-1 && starts[stackpos]==stacksBegin[stackpos+1] || stackpos == count - 1 && capacity==starts[count-1];
}

template<class T>
inline bool TMultyStack<T>::IsEmpty(size_t stackpos) const
{
	if (stackpos >= count)
		ERROR("stack_error");
	return starts[stackpos]==stacksBegin[stackpos];
}

template<class T>
inline void TMultyStack<T>::Push(size_t stackpos, const T& elem)
{
	if (stackpos >= count)
		ERROR("stack_error");
	if (this->IsFull(stackpos))
		this->Repack(stackpos);
	data[starts[stackpos]] = elem;
	starts[stackpos]++;
}

template<class T>
inline T TMultyStack<T>::Pop(size_t stackpos)
{
	if (stackpos >= count)
		ERROR("stack_error");
	if (this->IsEmpty(stackpos))
		ERROR("empty_stack");
	starts[stackpos]--;
	return data[starts[stackpos]];
}

template<class O>
std::ostream& operator<<(std::ostream& os, const TMultyStack<O>& stack)
{
	os << "{";
	for (size_t i = 0; i < stack.Count(); ++i) {
		os << "[";
		for (size_t j = 0; j < stack.Size(i); ++j) {
			os << stack(i, j);
			if (j < stack.Size(i) - 1) {
				os << ",";
			}
		}
		os << "]";
		if (i < stack.Count() - 1) {
			os << ",";
		}
	}
	os << "}\n";
	return os;
}

template<class I>
std::istream& operator>>(std::istream& is, TMultyStack<I>& stack)
{
	size_t stack_count, stack_size;
	is >> stack_count >> stack_size;
	TMultyStack<I> temp(stack_count, stack_size);
	for (size_t i = 0; i < stack_count; ++i) {
		size_t element_count;
		is >> element_count;

		for (size_t j = 0; j < element_count; ++j) {
			I element;
			is >> element;
			temp.Push(i, element);
		}
	}
	stack = std::move(temp);
	return is;
}

template<class T>
inline T TMultyStack<T>::FindMin() const
{
	if (capacity == 0 || count == 0)
		ERROR("empty_stack");

	bool found = false;
	T minElem;
	for (size_t i = 0; i < count; ++i)
	{
		if (!this->IsEmpty(i))
		{
			minElem = data[stacksBegin[i]];
			found = true;
			break;
		}
	}

	if (!found)
		ERROR("all_stacks_empty");
	for (size_t i = 0; i < count; ++i)
	{
		if (!this->IsEmpty(i))
		{
			for (size_t j = stacksBegin[i]; j < starts[i]; ++j)
			{
				if (data[j] < minElem)
					minElem = data[j];
			}
		}
	}

	return minElem;
}

template<class T>
inline void TMultyStack<T>::SaveToFile(const std::string& filename) const
{
	std::ofstream file(filename, std::ios::binary);
	if (!file.is_open())
		ERROR("cannot_open_file");
	file.write(reinterpret_cast<const char*>(&capacity), sizeof(capacity));
	file.write(reinterpret_cast<const char*>(&count), sizeof(count));
	if (capacity > 0)
	{
		for (size_t i = 0; i < capacity; ++i)
		{
			file.write(reinterpret_cast<const char*>(&data[i]), sizeof(T));
		}
	}
	if (count > 0)
	{
		for (size_t i = 0; i < count; ++i)
		{
			file.write(reinterpret_cast<const char*>(&stacksBegin[i]), sizeof(size_t));
		}
	}
	if (count > 0)
	{
		for (size_t i = 0; i < count; ++i)
		{
			file.write(reinterpret_cast<const char*>(&starts[i]), sizeof(size_t));
		}
	}

	file.close();
}

template<class T>
inline void TMultyStack<T>::LoadFromFile(const std::string& filename)
{
	std::ifstream file(filename, std::ios::binary);
	if (!file.is_open())
		ERROR("cannot_open_file");
	if (data) delete[] data;
	if (stacksBegin) delete[] stacksBegin;
	if (starts) delete[] starts;
	file.read(reinterpret_cast<char*>(&capacity), sizeof(capacity));
	file.read(reinterpret_cast<char*>(&count), sizeof(count));
	if (capacity > 0)
	{
		data = new T[capacity];
		for (size_t i = 0; i < capacity; ++i)
		{
			file.read(reinterpret_cast<char*>(&data[i]), sizeof(T));
		}
	}
	else
	{
		data = nullptr;
	}

	if (count > 0)
	{
		stacksBegin = new size_t[count];
		starts = new size_t[count];
		for (size_t i = 0; i < count; ++i)
		{
			file.read(reinterpret_cast<char*>(&stacksBegin[i]), sizeof(size_t));
		}

		for (size_t i = 0; i < count; ++i)
		{
			file.read(reinterpret_cast<char*>(&starts[i]), sizeof(size_t));
		}
	}
	else
	{
		stacksBegin = nullptr;
		starts = nullptr;
	}

	file.close();
}