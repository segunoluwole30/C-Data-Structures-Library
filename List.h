#pragma once

#include <cstddef> // size_t
#include <iterator> // std::bidirectional_iterator_tag
#include <type_traits> // std::is_same, std::enable_if

template <class T>
class List {
    private:
    struct Node {
        Node *next, *prev;
        T data;
        explicit Node(Node* prev = nullptr, Node* next = nullptr)
        : next{next}, prev{prev} {}
        explicit Node(const T& data, Node* prev = nullptr, Node* next = nullptr)
        : next{next}, prev{prev}, data{data} {}
        explicit Node(T&& data, Node* prev = nullptr, Node* next = nullptr)
        : next{next}, prev{prev}, data{std::move(data)} {}
    };

    template <typename pointer_type, typename reference_type>
    class basic_iterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type        = T;
        using difference_type   = ptrdiff_t;
        using pointer           = pointer_type;
        using reference         = reference_type;
    private:
        friend class List<value_type>;
        using Node = typename List<value_type>::Node;

        Node* node;

        explicit basic_iterator(Node* ptr) noexcept : node{ptr} {}
        explicit basic_iterator(const Node* ptr) noexcept : node{const_cast<Node*>(ptr)} {}

    public:
        basic_iterator() { /* TODO */ node = nullptr; };
        basic_iterator(const basic_iterator&) = default;
        basic_iterator(basic_iterator&&) = default;
        ~basic_iterator() = default;
        basic_iterator& operator=(const basic_iterator&) = default;
        basic_iterator& operator=(basic_iterator&&) = default;

        reference operator*() const {
            return node->data;
            // TODO
        }
        pointer operator->() const {
            return &node->data;
            // TODO
        }

        // Prefix Increment: ++a
        basic_iterator& operator++() {
            node = node->next;
            return *this;
            // TODO
        }
        // Postfix Increment: a++
        basic_iterator operator++(int) {
            basic_iterator tmp(node);
            node = node->next;
            return tmp;
            // TODO
        }
        // Prefix Decrement: --a
        basic_iterator& operator--() {
            node = node->prev;
            return *this;
            // TODO
        }
        // Postfix Decrement: a--
        basic_iterator operator--(int) {
            basic_iterator tmp{node};
            node = node->prev;
            return tmp;
            // TODO
        }

        bool operator==(const basic_iterator& other) const noexcept {
            if (this->node == other.node)
            {
                return true;
            }
            return false;
            // TODO
        }
        bool operator!=(const basic_iterator& other) const noexcept {
            if (this->node != other.node)
            {
                return true;
            }
            return false;
            // TODO
        }
    };

public:
    using value_type      = T;
    using size_type       = size_t;
    using difference_type = ptrdiff_t;
    using reference       = value_type&;
    using const_reference = const value_type&;
    using pointer         = value_type*;
    using const_pointer   = const value_type*;
    using iterator        = basic_iterator<pointer, reference>;
    using const_iterator  = basic_iterator<const_pointer, const_reference>;

private:
    Node head, tail;
    size_type _size;

public:
    List() :  _size(0){
        head.next = &tail;
        head.prev = nullptr;
        tail.prev =  &head;
        tail.next = nullptr;
        // TODO - Don't forget the list initialier
}
    List( size_type count, const T& value ) {
        // TODO - Don't forget the list initialier
        head.next = &tail;
        tail.prev = &head;
        for (size_t i = 0; i < count;i++)
        {
            Node* newNode = new Node(value, &head, head.next);
            head.next->prev = newNode;
            head.next = newNode;
        }
        _size = count;
    }
    explicit List( size_type count ) {
        head.next = &tail;
        tail.prev = &head;
        for (size_t i = 0; i < count; i++)
        {
            Node* newNode =  new Node(T(), &head, head.next);
            head.next->prev = newNode;
            head.next = newNode;
        }
        _size = count;
        // TODO - Don't forget the list initialier
    }
    List( const List& other ) {
        //FIXXX
        head = other.head;
        tail = other.tail;
        head.next = &tail;
        tail.prev = &head;
       for (basic_iterator it = other.cbegin(); it != other.cend();it++)
       {
        push_back(*it);
       }
        _size = other._size;
        // TODO - Don't forget the list initialier


    }
    List( List&& other ) {
        head = other.head;
        tail = other.tail;
        _size = other._size;
        other.head.next->prev = &head;
        other.tail.prev->next = &tail;
        other.head.next = &other.tail;
        other.tail.prev = &other.head;
        other._size = 0;
        // TODO - Don't forget the list initialier
        //The move constructor works by reassigning the head and tail pointers. 
        //Essentially you point the head to the new list and the tail to the new list 
        //and then you make sure that the old list's head and tail can no longer access the list
    }
    ~List() {
        clear();
        // TODO
    }
    List& operator=( const List& other ) {
        if (&other == this)
        {
            return *this;
        }
        this->clear();
        this->head = other.head;
        this->tail = other.tail;
        this->head.next = &tail;
        this->tail.prev = &head;
        for (basic_iterator it = other.cbegin(); it != other.cend();it++)
        {
            push_back(*it);
        }
        this->_size = other._size;
        return *this;
        // TODO
    }
    List& operator=( List&& other ) noexcept {
        if (&other == this)
        {
            return *this;
        }
        this->clear();
        this->_size = other._size;
        this->head = other.head;
        this->tail = other.tail;
        other.head.next->prev = &head;
        other.tail.prev->next = &tail;
        other._size = 0;
        other.head.next = &other.tail;
        other.tail.prev = &other.head;
        return *this;
        // TODO
    }

    reference front() {
        return *begin();
        // TODO
    }
    const_reference front() const {
        return *begin();
        // TODO
    }
	
    reference back() {
        // TODO
        return *--end();
    }
    const_reference back() const {
        return *--end();
        // TODO
    }
	
    iterator begin() noexcept {
        if (_size == 0)
        {
            return iterator{&tail};
        }
        return iterator{head.next};
        // TODO
    }
    const_iterator begin() const noexcept {
        if (_size == 0)
        {
            return const_iterator{&tail};
        }
        return const_iterator{head.next};
        // TODO
    }
    const_iterator cbegin() const noexcept {
        if (_size == 0)
        {
            return const_iterator{&tail};
        }
        return const_iterator{head.next};
        // TODO
    }

    iterator end() noexcept {
        return iterator{&tail};
        // TODO
    }
    const_iterator end() const noexcept {
        return const_iterator{&tail};
        // TODO
    }
    const_iterator cend() const noexcept {
        return const_iterator{&tail};
        // TODO
    }

    bool empty() const noexcept {
        if (_size == 0)
        {
            return true;
        }
        return false;
        // TODO
    }

    size_type size() const noexcept {
        // TODO
        return _size;
    }

    void clear() noexcept {
        Node* curr = head.next;
        Node* delNode;
        while(curr != &tail)
        {
            delNode = curr;
            curr = curr->next;
            delete delNode;
        }
        _size = 0;
        head.next = &tail;
        head.prev = nullptr;
        tail.prev = &head;
        tail.next = nullptr;
        // TODO
    }

    iterator insert( const_iterator pos, const T& value ) 
    {
        Node* newNode = new Node(value);
        auto  before = pos.node->prev;
        before->next = newNode;
        newNode->prev = before;
        newNode->next = pos.node;
        pos.node->prev = newNode;
        _size++;
        return iterator{newNode};
        // TODO
    }
    iterator insert( const_iterator pos, T&& value ) {
        Node* newNode = new Node(std::move(value));
        auto  before = pos.node->prev;
        newNode->prev = before;
        newNode->next = pos.node;
        pos.node->prev = newNode;
        _size++;
        return iterator{newNode};
        // TODO
    }

    iterator erase( const_iterator pos ) {
        Node* temp = pos.node->next;
        Node* before = pos.node->prev;
        Node* after = pos.node->next;
        before->next = after;
        after->prev = before;
        delete pos.node;
        _size--;
        return iterator{temp};
        // TODO
    }

    void push_back( const T& value ) {
        Node* newNode = new Node(value, tail.prev, &tail);
        tail.prev->next = newNode;
        tail.prev = newNode;
        _size++;
        // TODO
    }
    void push_back( T&& value ) {
        Node* newNode = new Node(std::move(value), tail.prev, &tail);
        tail.prev->next = newNode;
        tail.prev = newNode;
        _size++;
        // TODO
    }

    void pop_back() {
        auto delNode = tail.prev;
        delNode->prev->next = &tail;
        tail.prev = delNode->prev;
        delete delNode;
        _size--;
        // TODO
    }
	
    void push_front( const T& value ) {
        //insert(begin(), value);
        Node* newNode = new Node(value, &head, head.next);
        head.next->prev = newNode;
        head.next = newNode;
        _size++;
        // TODO
    }
	void push_front( T&& value ) {
        //insert(begin(), std::move(value));
        Node* newNode = new Node(std::move(value), &head, head.next);
        head.next->prev = newNode;
        head.next = newNode;
        _size++;
        // TODO
    }

    void pop_front() {
        erase(begin());
        // TODO
    }

    /*
      You do not need to modify these methods!
      
      These method provide the non-const complement 
      for the const_iterator methods provided above.
    */
    iterator insert( iterator pos, const T & value) { 
        return insert((const_iterator &) (pos), value);
    }

    iterator insert( iterator pos, T && value ) {
        return insert((const_iterator &) (pos), std::move(value));
    }

    iterator erase( iterator pos ) {
        return erase((const_iterator&)(pos));
    }
};


/*
    You do not need to modify these methods!

    These method provide a overload to compare const and 
    non-const iterators safely.
*/
 
namespace {
    template<typename Iter, typename ConstIter, typename T>
    using enable_for_list_iters = typename std::enable_if<
        std::is_same<
            typename List<typename std::iterator_traits<Iter>::value_type>::iterator, 
            Iter
        >{} && std::is_same<
            typename List<typename std::iterator_traits<Iter>::value_type>::const_iterator,
            ConstIter
        >{}, T>::type;
}

template<typename Iterator, typename ConstIter>
enable_for_list_iters<Iterator, ConstIter, bool> operator==(const Iterator & lhs, const ConstIter & rhs) {
    return (const ConstIter &)(lhs) == rhs;
}

template<typename Iterator, typename ConstIter>
enable_for_list_iters<Iterator, ConstIter, bool> operator==(const ConstIter & lhs, const Iterator & rhs) {
    return (const ConstIter &)(rhs) == lhs;
}

template<typename Iterator, typename ConstIter>
enable_for_list_iters<Iterator, ConstIter, bool> operator!=(const Iterator & lhs, const ConstIter & rhs) {
    return (const ConstIter &)(lhs) != rhs;
}

template<typename Iterator, typename ConstIter>
enable_for_list_iters<Iterator, ConstIter, bool> operator!=(const ConstIter & lhs, const Iterator & rhs) {
    return (const ConstIter &)(rhs) != lhs;
}