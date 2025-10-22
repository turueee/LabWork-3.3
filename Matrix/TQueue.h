#pragma once

#include <iostream>
#include <fstream>
#include "TError.h"
#define ERROR(err,...) throw TError(err, __func__, __FILE__, __LINE__)


template<class T>
class TQueue {
protected:
    size_t size;
    size_t capacity;
    T* memory;
public:
    TQueue();
    TQueue(size_t capacity_);
    TQueue(const TQueue& other);
    TQueue(TQueue&& other) noexcept;
    ~TQueue();

    TQueue& operator=(const TQueue& other);
    TQueue& operator=(TQueue&& other) noexcept;
    bool operator==(const TQueue& other);
    bool operator!=(const TQueue& other);

    bool IsEmpty();
    bool IsFull();

    T operator[] (size_t index) const;

    T* begin() const;
    const T* cbegin() const;
    T* end() const;
    const T* cend() const;

    void Put(T elem);
    T Get();
    T Head();
    T Tail();
    T Min();
    size_t Size();

    void WriteToFile(const TString& filename) const;
    void ReadFromFile(const TString& filename);

    template<class U>
    friend std::ostream& operator<<(std::ostream& os, const TQueue<U>& stack);
    template<class O>
    friend std::istream& operator>>(std::istream& is, TQueue<O>& stack);
};

template<class T>
inline TQueue<T>::TQueue() : size(0), capacity(0)
{
    memory = nullptr;
}

template<class T>
inline TQueue<T>::TQueue(size_t capacity_) : size(0), capacity(capacity_)
{
    if (capacity)
        memory = new T[capacity];
    else
        memory == nullptr;
}

template<class T>
inline TQueue<T>::TQueue(const TQueue& other) : size(other.size), capacity(other.capacity)
{
    if (other.memory)
    {
        memory = new T[capacity];
        for (size_t i = 0; i < size; ++i)
            memory[i] = other.memory[i];
    }
    else
    {
        memory = nullptr;
    }
}

template<class T>
inline TQueue<T>::TQueue(TQueue&& other) noexcept
{
    size = other.size;
    capacity = other.capacity;
    memory = other.memory;
    other.memory = nullptr;
    other.size = 0;
    other.capacity = 0;
}

template<class T>
inline TQueue<T>::~TQueue()
{
    if (memory != nullptr)
        delete[] memory;
    size = 0;
    capacity = 0;
}

template<class T>
inline TQueue<T>& TQueue<T>::operator=(const TQueue<T>& other)
{
    if (*this != other)
    {
        size = other.size;
        capacity = other.capacity;
        if (memory)
            delete[] memory;
        if (other.memory)
        {
            memory = new T[capacity];
            for (size_t i = 0; i < size; ++i)
                memory[i] = other.memory[i];
        }
        else
            memory = nullptr;
    }
    return *this;
}

template<class T>
inline TQueue<T>& TQueue<T>::operator=(TQueue<T>&& other) noexcept
{
    if (*this != other)
    {
        size = other.size;
        capacity = other.capacity;
        memory = other.memory;
        other.memory = nullptr;
        other.size = 0;
        other.capacity = 0;
    }
    return *this;
}

template<class T>
inline bool TQueue<T>::operator==(const TQueue& other)
{
    if (size != other.size || capacity != other.capacity)
        return false;
    for (size_t i = 0; i < size; ++i)
        if (memory[i] != other.memory[i])
            return false;
    return true;
}

template<class T>
inline bool TQueue<T>::operator!=(const TQueue& other)
{
    if (size != other.size || capacity != other.capacity)
        return true;
    for (size_t i = 0; i < size; ++i)
        if (memory[i] != other.memory[i])
            return true;
    return false;
}

template<class T>
inline bool TQueue<T>::IsEmpty()
{
    return size == 0;
}

template<class T>
inline bool TQueue<T>::IsFull()
{
    return size == capacity;
}

template<class T>
inline T TQueue<T>:: operator[] (size_t index) const
{
    if (index >= size)
        ERROR("size_error");
    return memory[index];
}

template<class T>
inline T* TQueue<T>:: begin() const
{
    return memory;
}

template<class T>
inline const T* TQueue<T>::cbegin() const
{
    return memory;
}

template<class T>
inline T* TQueue<T>::end() const
{
    return memory+size;
}

template<class T>
inline const T* TQueue<T>::cend() const
{
    return memory+size;
}

template<class T>
inline void TQueue<T>::Put(T elem)
{
    if (size != capacity)
    {
        memory[size++] = elem;
    }
    else
        ERROR("full_queue");
}

template<class T>
inline T TQueue<T>::Get()
{
    if (size != 0)
    {
        T elem(std::move(memory[0]));
        for (size_t i = 0; i < size;++i)
            memory[i] = std::move(memory[i+1]);
        size--;
        return elem;
    }
    else
        ERROR("empty_stack");
}

template<class T>
inline T TQueue<T>::Head()
{
    if(size!=0)
        return memory[0];
    else
        ERROR("empty_stack");
}

template<class T>
inline T TQueue<T>::Tail()
{
    if (size != 0)
        return memory[size-1];
    else
        ERROR("empty_stack");
}

template<class T>
inline T TQueue<T>::Min()
{
    if (size != 0)
    {
        T min = memory[0];
        for (size_t i = 1; i < size; ++i)
            if (memory[i] < min)
                min = memory[i];
        return min;
    }
    else
        ERROR("empty_stack");
}

template<class T>
inline size_t TQueue<T>::Size()
{
    return size;
}

template<class T>
void TQueue<T>::WriteToFile(const TString& filename) const
{
    std::ofstream file(filename.CStr());
    if (!file.is_open())
        ERROR("file_open_error");

    file << size << std::endl;

    for (size_t i = 0; i < size; ++i)
    {
        file << memory[i] << std::endl;
    }

    file.close();
}

template<class T>
void TQueue<T>::ReadFromFile(const TString& filename)
{
    std::ifstream file(filename.CStr());
    if (!file.is_open())
        ERROR("file_open_error");

    size_t count;
    file >> count;

    if (memory != nullptr)
        delete[] memory;

    capacity = count;
    size = 0;
    memory = new T[capacity];

    T element;
    for (size_t i = 0; i < count; ++i)
    {
        file >> element;
        memory[size++] = element;
    }

    file.close();
}

template<class T>
std::ostream& operator<<(std::ostream& os, const TQueue<T>& queue)
{
    os << "[";
    for (size_t i = 0; i < queue.size; ++i) {
        os << queue.memory[i];
        if (i < queue.size - 1)
            os << ", ";
    }
    os << "]";
    return os;
}

template<class T>
std::istream& operator>>(std::istream& is, TQueue<T>& queue)
{
    size_t count;
    is >> count;

    if (queue.memory != nullptr)
        delete[] queue.memory;

    queue.capacity = count;
    queue.size = 0;
    queue.memory = new T[queue.capacity];

    T element;
    for (size_t i = 0; i < count; ++i)
    {
        is >> element;
        queue.memory[queue.size++] = element;
    }

    return is;
}
