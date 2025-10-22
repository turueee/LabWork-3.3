#pragma once

#include <iostream>
#include <fstream>
#include "TError.h"
#define ERROR(err,...) throw TError(err, __func__, __FILE__, __LINE__)

#include <iostream>

template<class T>
class TStack {
protected:
    size_t start;
    size_t capacity;
    T* memory;
public:
    TStack();
    TStack(size_t size_);
    TStack(const TStack& other);
    TStack(TStack&& other) noexcept;
    ~TStack();

    TStack& operator=(const TStack& other);
    TStack& operator=(TStack&& other) noexcept;
    bool operator==(const TStack& other);
    bool operator!=(const TStack& other);

    bool IsEmpty();
    bool IsFull();

    T operator[] (size_t index) const;

    T* begin() const;
    const T* cbegin() const;
    T* end() const;
    const T* cend() const;

    void Put(T elem);
    T Get();
    T Top();
    T Min();
    size_t Size();

    void WriteToFile(const TString& filename) const;
    void ReadFromFile(const TString& filename);

    template<class U>
    friend std::ostream& operator<<(std::ostream& os, const TStack<U>& stack);
    template<class O>
    friend std::istream& operator>>(std::istream& is, TStack<O>& stack);
};

template<class T>
inline TStack<T>::TStack() : start(0), capacity(0)
{
    memory = nullptr;
}

template<class T>
inline TStack<T>::TStack(size_t size_) : start(0), capacity(size_)
{
    if (capacity)
        memory = new T[capacity];
    else
        memory = nullptr;
}

template<class T>
inline TStack<T>::TStack(const TStack& other) : start(other.start), capacity(other.capacity)
{
    memory = new T[capacity];
    for (size_t i = 0; i < start; ++i)
        memory[i] = other.memory[i];
}

template<class T>
inline TStack<T>::TStack(TStack&& other) noexcept
{
    start = other.start;
    capacity = other.capacity;
    memory = other.memory;
    other.memory = nullptr;
    other.start = 0;
    other.capacity = 0;
}

template<class T>
inline TStack<T>::~TStack()
{
    if (memory != 0)
        delete[] memory;
    start = 0;
    capacity = 0;
}

template<class T>
inline TStack<T>& TStack<T>::operator=(const TStack<T>& other)
{
    if (*this != other)
    {
        start = other.start;
        capacity = other.capacity;
        memory = new T[capacity];
        for (size_t i = 0; i < start; ++i)
            memory[i] = other.memory[i];
    }
    return *this;
}

template<class T>
inline TStack<T>& TStack<T>::operator=(TStack<T>&& other) noexcept
{
    if (*this != other)
    {
        start = other.start;
        capacity = other.capacity;
        memory = other.memory;
        other.memory = nullptr;
        other.start = 0;
        other.capacity = 0;
    }
    return *this;
}

template<class T>
inline bool TStack<T>::operator==(const TStack& other)
{
    if (start != other.start || capacity != other.capacity)
        return false;
    for (size_t i = 0; i < start; ++i)
        if (memory[i] != other.memory[i])
            return false;
    return true;
}

template<class T>
inline bool TStack<T>::operator!=(const TStack& other)
{
    if (start != other.start || capacity != other.capacity)
        return true;
    for (size_t i = 0; i < start; ++i)
        if (memory[i] != other.memory[i])
            return true;
    return false;
}

template<class T>
inline bool TStack<T>::IsEmpty()
{
    return start == 0;
}

template<class T>
inline bool TStack<T>::IsFull()
{
    return start == capacity;
}

template<class T>
inline T TStack<T>:: operator[] (size_t index) const
{
    if (index >= start)
        ERROR("size_error");
    return memory[index];
}

template<class T>
inline T* TStack<T>:: begin() const
{
    return memory;
}

template<class T>
inline const T* TStack<T>::cbegin() const
{
    return memory;
}

template<class T>
inline T* TStack<T>::end() const
{
    return memory+start;
}

template<class T>
inline const T* TStack<T>::cend() const
{
    return memory+start;
}

template<class T>
inline void TStack<T>::Put(T elem)
{
    if (start != capacity)
    {
        memory[start] = elem;
        start++;
    }
    else
        ERROR("full_stack");
}

template<class T>
inline T TStack<T>::Get()
{
    if (start != 0)
    {
        start--;
        return memory[start];
    }
    else
        ERROR("empty_stack");
}

template<class T>
inline T TStack<T>::Top()
{
    if(start!=0)
        return memory[start - 1];
    else
        ERROR("empty_stack");
}

template<class T>
inline T TStack<T>::Min()
{
    if (start != 0)
    {
        T min = memory[0];
        for (size_t i = 1; i < start; ++i)
            if (memory[i] < min)
                min = memory[i];
        return min;
    }
    else
        ERROR("empty_stack");
}

template<class T>
inline size_t TStack<T>::Size()
{
    return start;
}

template<class T>
void TStack<T>::WriteToFile(const TString& filename) const
{
    std::ofstream file(filename.CStr());
    if (!file.is_open())
        ERROR("file_open_error");

    file << start << std::endl;

    for (size_t i = 0; i < start; ++i)
    {
        file << memory[i] << std::endl;
    }

    file.close();
}

template<class T>
void TStack<T>::ReadFromFile(const TString& filename)
{
    std::ifstream file(filename.CStr());
    if (!file.is_open())
        ERROR("file_open_error");

    size_t count;
    file >> count;

    if (memory != nullptr)
        delete[] memory;

    capacity = count;
    start = 0;
    memory = new T[capacity];

    T element;
    for (size_t i = 0; i < count; ++i)
    {
        file >> element;
        memory[start++] = element;
    }

    file.close();
}

template<class T>
std::ostream& operator<<(std::ostream& os, const TStack<T>& stack)
{
    os << "[";
    for (size_t i = 0; i < stack.start; ++i) {
        os << stack.memory[i];
        if (i < stack.start - 1)
            os << ", ";
    }
    os << "]";
    return os;
}

template<class O>
std::istream& operator>>(std::istream& is, TStack<O>& stack)
{
    size_t count;
    is >> count;

    if (stack.memory != nullptr)
        delete[] stack.memory;

    stack.capacity = count;
    stack.start = 0;
    stack.memory = new O[stack.capacity];

    O element;
    for (size_t i = 0; i < count; ++i)
    {
        is >> element;
        stack.memory[stack.start++] = element;
    }

    return is;
}
