#ifndef VECTOR_H
#define VECTOR_H

#include <algorithm> // std::random_access_iterator_tag
#include <cstddef> // size_t
#include <stdexcept> // std::out_of_range
#include <type_traits> // std::is_same

template <class T>
class Vector {
public:
    class iterator;
    using difference_type   = ptrdiff_t;
private:
    T* array;
    size_t _capacity, _size;

    // You may want to write a function that grows the vector
    void grow() {
    if (_capacity == 0)
    {
        _capacity++;
    }
    else
    {
        _capacity *= 2;
    }
    
    T* new_array = new T[_capacity];
    for (size_t i = 0; i <_size;i++)
    {
        new_array[i] = std::move(array[i]);
    }
    delete [] array;
    this->array = new_array;
    }

public:
    Vector() noexcept {_capacity = 0; _size = 0; array = nullptr;}
    Vector(size_t count, const T& value) {_capacity = count; _size = count; array = new T[_capacity];
    for (T* point = array; point < (array+count);point++) //point points to first element of array and then iterates through array
    {
        (*point) = value;
    }
    }
    explicit Vector(size_t count) {_capacity = count; _size = count; array = new T[_size];
    for (T* point = array; point < (array + count); point++)
    {
        (*point) = T();
    }
    }
    Vector(const Vector& other) { array = new T[other._capacity]; _size = other._size; _capacity = other._capacity;
    for (size_t i = 0; i < other._size;i++)
    {
        array[i] = other[i];
    }
    }
    Vector(Vector&& other) noexcept {
    _capacity = other._capacity; _size = other._size;  array = other.array;
    other._capacity = 0;
    other._size = 0;
    other.array = nullptr;
    }

    ~Vector() {_size = 0; _capacity = 0; delete [] array; array = nullptr;}

    Vector& operator=(const Vector& other) {
    //check for self-assignment
    if (&other == this)
    {
        return *this;
    }

    T* temp_array = new T[other._capacity];
    this->_size = other._size; this->_capacity = other._capacity;
    for (size_t i = 0; i < other._size;i++)
    {
        temp_array[i] = other[i];
    }
    delete [] array;
    array = temp_array;
    return *this;
    }
    Vector& operator=(Vector&& other) noexcept {
    //check for self-assignment
    if (&other == this)
    {
        return *this;
    }
    delete [] this->array; 
    this->_size = other._size; this->_capacity = other._capacity;
    this->array = other.array;
    other._size = 0;
    other._capacity = 0;
    other.array = nullptr;
    return *this;
    }

    iterator begin() noexcept {iterator point(*this);
    return point;}
    iterator end() noexcept {
    iterator point(array + _size);
    return point;}

    [[nodiscard]] bool empty() const noexcept {
    if (_size == 0)
    {
        return true;
    }
    return false;
    } 
    size_t size() const noexcept { return _size; }
    size_t capacity() const noexcept {return _capacity;}

    T& at(size_t pos) {
    if (pos >= _size || pos < 0)
    {
        throw std::out_of_range("out of bounds");
    }
    return array[pos];
    }
    const T& at(size_t pos) const { 
    if (pos >= _size || pos < 0)
    {
        throw std::out_of_range("out of bounds");
    }
    return array[pos];
    }
    T& operator[](size_t pos) { return array[pos];}
    const T& operator[](size_t pos) const { return array[pos]; }
    T& front() { return array[0];}
    const T& front() const { return array[0];}
    T& back() { return array[_size-1]; }
    const T& back() const { return array[_size-1]; }

    void push_back(const T& value) {
    if (_size >= _capacity)
    {
        this->grow();
    }
    array[_size] = value;
    this->_size++;
    }

    void push_back(T&& value) {
    if (_size >= _capacity)
    {
        this->grow();
    }
    array[_size] = std::move(value);
    this->_size++;
    }

    void pop_back() { _size--;
    }
    //shift elements right to make space for insert
    iterator insert(iterator pos, const T& value) { /* TODO */
    difference_type insert_idx = pos - begin();
    if (_size == _capacity) grow();
    for (difference_type index = _size; index > insert_idx;--index)
    {
        array[index] = std::move(array[index - 1]);
    }
    array[insert_idx] = value;
    _size++;
    return begin() + insert_idx;

    }
    iterator insert(iterator pos, T&& value) { /* TODO */ 
    difference_type insert_idx = pos - begin();
    if (_size == _capacity) grow();
    for (difference_type index = _size; index > insert_idx;index--)
    {
        array[index] = std::move(array[index - 1]);
    }
    array[insert_idx] = std::move(value);
    _size++;
    return begin() + insert_idx;}

    iterator insert(iterator pos, size_t count, const T& value) { /* TODO */ 
    if (count == 0) return pos;
    difference_type insert_idx = pos - begin();
    while (_size + count > _capacity) grow();
    //iterate from current last element to position of new last element and set each value equal to the one COUNT to the left of the new value(shift right)
    //count - 1 because indexing starts at 0
    for (difference_type index = (_size + count -1); index > (insert_idx + count - 1); --index)
    {
        array[index] = std::move(array[index - count]);
    }
    for (size_t index = insert_idx; index < insert_idx + count; index++)
    {
        array[index] = value;
    }
    _size += count;
    return begin() + insert_idx;
}
//shift elements left to erase
    iterator erase(iterator pos) { /* TODO */ 
    if (pos == (end() - 1)) 
    {
        _size--;
        return end();
    }
    difference_type erase_idx = pos - begin();
    //iterate from erase_idx to _size and set each value equal to the one next to it
    for (difference_type index = erase_idx; index < _size - 1; index++)
    {
        array[index] = std::move(array[index + 1]);
    }
    //decrement size
    _size--;
    //return erase_idx + 1
    return begin() + erase_idx;
    }
    
    iterator erase(iterator first, iterator last) { /* TODO */ 
    if (last == end() - 1)
    {
        _size--;
        return end();
    }
    difference_type firstErase = first - begin();
    //difference_type lastErase = last - begin();
    difference_type count = last - first;
    for (difference_type index = firstErase; index < _size - count; index++)
    {
        array[index] = std::move(array[index + count]);
    }
    _size-=count;
    return begin()  + firstErase; // or first
    }

    class iterator {
    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type        = T;
        using difference_type   = ptrdiff_t;
        using pointer           = T*;
        using reference         = T&;
    private:
        pointer point;
        // HINT: For random_access_iterator, the data member is a pointer 99.9% of the time
    public:
        iterator() {point = nullptr;}
        iterator(Vector& other) {point = other.array;}
        //iterator(iterator& other) {point = other.point;}
        iterator(pointer other) {point = other;}
        iterator(int& num) {point = array + num;}
        // Add any constructors that you may need

        // This assignment operator is done for you, please do not add more
        iterator& operator=(const iterator&) noexcept = default;

        [[nodiscard]] reference operator*() const noexcept { return *point; }
        [[nodiscard]] pointer operator->() const noexcept { return point;}

        // Prefix Increment: ++a
        iterator& operator++() noexcept { point++;
        return *this;}
        // Postfix Increment: a++
        iterator operator++(int) noexcept {iterator temp(point); point++; return temp;}
        // Prefix Decrement: --a
        iterator& operator--() noexcept {point--; return *this; }
        // Postfix Decrement: a--
        iterator operator--(int) noexcept {iterator temp(point); point--; return temp; }

        iterator& operator+=(difference_type offset) noexcept {point = point + offset; return *this;}
        [[nodiscard]] iterator operator+(difference_type offset) const noexcept {iterator temp(point); temp.point = temp.point + offset; return temp;}
        
        iterator& operator-=(difference_type offset) noexcept {point = point - offset; return *this;}
        [[nodiscard]] iterator operator-(difference_type offset) const noexcept {iterator diff(point); diff.point = diff.point - offset; return diff;}
        [[nodiscard]] difference_type operator-(const iterator& rhs) const noexcept { return (this->point - rhs.point);}

        [[nodiscard]] reference operator[](difference_type offset) const noexcept { pointer diff(point + offset);
        return *diff;}

        [[nodiscard]] bool operator==(const iterator& rhs) const noexcept {if (this->point == rhs.point) {return true;} return false;}
        [[nodiscard]] bool operator!=(const iterator& rhs) const noexcept { if (this->point != rhs.point)
        {
            return true;
        }
        return false; }
        [[nodiscard]] bool operator<(const iterator& rhs) const noexcept { if (this->point < rhs.point)
        {
            return true;
        }
        return false;}
        [[nodiscard]] bool operator>(const iterator& rhs) const noexcept { if (this->point > rhs.point)
        {
            return true;
        }
        return false; }
        [[nodiscard]] bool operator<=(const iterator& rhs) const noexcept { if ((this->point < rhs.point) || (this->point == rhs.point))
        {
            return true;
        }
        return false; }
        [[nodiscard]] bool operator>=(const iterator& rhs) const noexcept { if ((this->point > rhs.point) || (this->point == rhs.point))
        {
            return true;
        }
        return false; }
    };


    void clear() noexcept {_size = 0;}
};

// This ensures at compile time that the deduced argument _Iterator is a Vector<T>::iterator
// There is no way we know of to back-substitute template <typename T> for external functions
// because it leads to a non-deduced context
namespace {
    template <typename _Iterator>
    using is_vector_iterator = std::is_same<typename Vector<typename _Iterator::value_type>::iterator, _Iterator>;
}

template <typename _Iterator, bool _enable = is_vector_iterator<_Iterator>::value>
[[nodiscard]] _Iterator operator+(typename _Iterator::difference_type offset, _Iterator const& iterator) noexcept {
_Iterator diff(iterator + offset); return diff;
}

#endif
