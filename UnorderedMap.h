#include <cstddef>    // size_t
#include <functional> // std::hash
#include <ios>
#include <utility>    // std::pair
#include <iostream>

#include "primes.h"



template <typename Key, typename T, typename Hash = std::hash<Key>, typename Pred = std::equal_to<Key>>
class UnorderedMap {
    public:

    using key_type = Key;
    using mapped_type = T;
    using const_mapped_type = const T;
    using hasher = Hash;
    using key_equal = Pred;
    using value_type = std::pair<const key_type, mapped_type>;
    using reference = value_type &;
    using const_reference = const value_type &;
    using pointer = value_type *;
    using const_pointer = const value_type *;
    using size_type = size_t;
    using difference_type = ptrdiff_t;

    private:

    struct HashNode {
        HashNode *next;
        value_type val;

        HashNode(HashNode *next = nullptr) : next{next} {}
        HashNode(const value_type & val, HashNode * next = nullptr) : next { next }, val { val } { }
        HashNode(value_type && val, HashNode * next = nullptr) : next { next }, val { std::move(val) } { }
    };

    size_type _bucket_count;
    HashNode **_buckets;

    HashNode * _head;
    size_type _size;

    Hash _hash;
    key_equal _equal;

    static size_type _range_hash(size_type hash_code, size_type bucket_count) {
        return hash_code % bucket_count;
    }

    public:

    template <typename pointer_type, typename reference_type, typename _value_type>
    class basic_iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = _value_type;
        using difference_type = ptrdiff_t;
        using pointer = value_type *;
        using reference = value_type &;

    private:
        friend class UnorderedMap<Key, T, Hash, key_equal>;
        using HashNode = typename UnorderedMap<Key, T, Hash, key_equal>::HashNode;

        const UnorderedMap * _map;
        HashNode * _ptr;

        explicit basic_iterator(UnorderedMap const * map, HashNode *ptr) noexcept { /* TODO */ 
            _map = map;
            _ptr = ptr;
        }

    public:
        basic_iterator() { /* TODO */ _map = nullptr; _ptr = nullptr; };

        basic_iterator(const basic_iterator &) = default;
        basic_iterator(basic_iterator &&) = default;
        ~basic_iterator() = default;
        basic_iterator &operator=(const basic_iterator &) = default;
        basic_iterator &operator=(basic_iterator &&) = default;
        reference operator*() const { /* TODO */ return _ptr->val;}
        pointer operator->() const { /* TODO */ return &(_ptr->val);}
        basic_iterator &operator++() { 
            if (_ptr->next != nullptr)
            {
                _ptr = _ptr->next;
                return *this;
            }
                
            size_type x = _map->_bucket(_ptr->val) + 1;
            for(size_type i = x; i < _map->_bucket_count;i++)
            {
                if (_map->_buckets[i] != nullptr)
                {
                    _ptr = _map->_buckets[i];
                    return *this;
                }
                
            }
            _ptr = nullptr;
            return *this;
            /* TODO */ }
        basic_iterator operator++(int) { 
            basic_iterator temp(_map, _ptr);
            if (_ptr->next != nullptr)
            {
                _ptr = _ptr->next;
                return temp;
            }
            
            size_type x = _map->_bucket(_ptr->val) + 1; //bucket index
            for(size_type i = x; i < _map->_bucket_count;i++)
            {
                if (_map->_buckets[i] != nullptr)
                {
                    _ptr = _map->_buckets[i];
                    return temp;
                } 
            }
            _ptr = nullptr;
            return temp;
            
            /* TODO */ }

        bool operator==(const basic_iterator &other) const noexcept { 
            if (this->_ptr == other._ptr)
            {
                return true;
            }
            return false;
            /* TODO */ }
        bool operator!=(const basic_iterator &other) const noexcept { 
            if (this->_ptr != other._ptr)
            {
                return true;
            }
            return false;
            /* TODO */ }
    };

    using iterator = basic_iterator<pointer, reference, value_type>;
    using const_iterator = basic_iterator<const_pointer, const_reference, const value_type>;

    class local_iterator {
        public:
            using iterator_category = std::forward_iterator_tag;
            using value_type = std::pair<const key_type, mapped_type>;
            using difference_type = ptrdiff_t;
            using pointer = value_type *;
            using reference = value_type &;

        private:
            friend class UnorderedMap<Key, T, Hash, key_equal>;
            using HashNode = typename UnorderedMap<Key, T, Hash, key_equal>::HashNode;

            HashNode * _node;

            explicit local_iterator( HashNode * node ) noexcept { 
                _node = node;
                /* TODO */ }

        public:
            local_iterator() { 
                _node = nullptr;
                /* TODO */ }

            local_iterator(const local_iterator &) = default;
            local_iterator(local_iterator &&) = default;
            ~local_iterator() = default;
            local_iterator &operator=(const local_iterator &) = default;
            local_iterator &operator=(local_iterator &&) = default;
            reference operator*() const { 
                return _node->val;
                /* TODO */ }
            pointer operator->() const { 
               return &(_node->val);
                
                /* TODO */ }
            local_iterator & operator++() { 
                if (_node->next != nullptr)
                {
                    _node = _node->next;
                    return *this;
                }
                _node = nullptr;
                return *this;
                /* TODO */ 
                }
            local_iterator operator++(int) { 
                local_iterator tmp{_node};
                _node = _node->next;
                return tmp;
                /* TODO */ }

            bool operator==(const local_iterator &other) const noexcept { 
                if (this->_node == other._node)
                {
                    return true;
                }
                return false;
                /* TODO */ }
            bool operator!=(const local_iterator &other) const noexcept { 
                if (this->_node != other._node)
                {
                    return true;
                }
                return false;
                /* TODO */ }
    };

private:

    size_type _bucket(size_t code) const { /* TODO */ 
        size_type index = code % _bucket_count;
        return index;
    }
    size_type _bucket(const Key & key) const { /* TODO */ 
        size_t code = _hash(key);
        return _bucket(code);
    }
    size_type _bucket(const value_type & val) const { /* TODO */
        Key k = val.first;
        return _bucket(k);
     }

    HashNode*& _find(size_type code, size_type bucket, const Key & key) { 
        
       HashNode **temp = &_buckets[bucket];
       while (*temp)
       {
            if (_equal((*temp)->val.first, key))
            {
                return *temp;
            }
            temp = &((*temp)->next);
       }
       return *temp;

        /* TODO */ }

    HashNode*& _find(const Key & key) { 
        return _find(_hash(key), _bucket(key), key);
        /* TODO */ }

    HashNode * _insert_into_bucket(size_type bucket, value_type && value) 
    { 
        /* TODO */ 
        HashNode*& temp = _buckets[bucket];
        HashNode* newNode = new HashNode(std::move(value), temp);
        temp = newNode;
        if ((!_head) || (_bucket(_head->val.first) >= bucket))
        {
            _head = newNode;
        }
        _size++;
        return newNode;
    }

    void _move_content(UnorderedMap & src, UnorderedMap & dst) { /* TODO */ }

public:
    explicit UnorderedMap(size_type bucket_count, const Hash & hash = Hash { },
                const key_equal & equal = key_equal { }){ 
                    _bucket_count = next_greater_prime(bucket_count);
                    _hash = hash;
                    _equal = equal;
                    _buckets = new HashNode*[_bucket_count] {};
                    _size = 0;
                    _head = nullptr;
                    /* TODO */ }

    ~UnorderedMap() { 
        clear();
        _bucket_count = 0;
        delete [] _buckets;
        /* TODO */ }

    UnorderedMap(const UnorderedMap & other) { 
        /* TODO */ 
        _bucket_count = other._bucket_count;
         _hash = other._hash;
        _equal = other._equal;
        _size = 0;
        _head = nullptr;
        _buckets = new HashNode* [_bucket_count] {};
        auto it = other.cbegin();
        while (it != other.cend())
        {
            insert(*it);
            it++;
        }
        }

    UnorderedMap(UnorderedMap && other) { 
        _head = other._head;
        _hash = other._hash;
        _equal = other._equal;
        _size = other._size;
        _buckets = other._buckets;
        _bucket_count = other._bucket_count;
        other._buckets = new HashNode* [_bucket_count] {};
        other._size = 0;
        other._head = nullptr;
        /* TODO */ }

    UnorderedMap & operator=(const UnorderedMap & other) 
    { /* TODO */ 
         if (this == &other)
        {
            return *this;
        }
        clear();
        delete [] _buckets;
        _bucket_count = other._bucket_count;
         _hash = other._hash;
        _equal = other._equal;
        _size = 0;
        _head = nullptr;
        _buckets = new HashNode* [_bucket_count] {};
        auto it = other.cbegin();
        while (it != other.cend())
        {
            insert(*it);
            it++;
        }
        return *this;
    }

    UnorderedMap & operator=(UnorderedMap && other) 
    { /* TODO */ 
        if (this == &other)
        {
            return *this;
        }
        clear();
        delete [] _buckets;
        _head = other._head;
        _hash = other._hash;
        _equal = other._equal;
        _size = other._size;
        _buckets = other._buckets;
        _bucket_count = other._bucket_count;
        other._buckets = new HashNode* [_bucket_count] {};
        other._size = 0;
        other._head = nullptr;
    }

    void clear() noexcept { 
        for (size_type i = 0; i < _bucket_count;i++)
        {
            while (_buckets[i])
            {
                auto delNode = _buckets[i];
                _buckets[i] = _buckets[i]->next;
                delete delNode;
            }
            _buckets[i] = nullptr;
        }
        _head = nullptr;
        _size = 0;
        
        /* TODO */ }

    size_type size() const noexcept { /* TODO */
        return _size; }

    bool empty() const noexcept { 
        if (_size == 0)
        {
            return true;
        } 
        return false;
        /* TODO */ 
        }

    size_type bucket_count() const noexcept { /* TODO */ 
    return _bucket_count;
    }

    iterator begin() { 
        return iterator(this, _head);
        /* TODO */ }

    iterator end() { 
       return iterator(this, nullptr); 
        /* TODO */ }

    const_iterator cbegin() const { /* TODO */ 
        return const_iterator(this, _head);
    };
    const_iterator cend() const { /* TODO */ return const_iterator(this, nullptr);}

    local_iterator begin(size_type n) { /* TODO */ return local_iterator(_buckets[n]);}
    local_iterator end(size_type n) { /* TODO */  return local_iterator(nullptr);}

    size_type bucket_size(size_type n) { 
        auto ptr = _buckets[n];
        size_type sizeBuc = 0;
        while (ptr != nullptr)
        {
            sizeBuc++;
            ptr = ptr->next;
        }
        return sizeBuc;
        /* TODO */ }

    float load_factor() const 
    { /* TODO */ 
        return ((float)_size) / _bucket_count;
    }

    size_type bucket(const Key & key) const { 
        return _bucket(key);
        /* TODO */ }

    std::pair<iterator, bool> insert(value_type && value) 
    { /* TODO */ 
        std::pair<iterator, bool> p;
        //p = (_find(value.first), false);
        if (_find(value.first))
        {
            return  std::pair<iterator, bool>(find(value.first), false);//p;
        }
        size_type index = _bucket(value.first);
        HashNode* newNode = _insert_into_bucket(index, std::move(value));
        return std::pair<iterator,bool>(iterator(this, newNode), true);
    }

    std::pair<iterator, bool> insert(const value_type & value) { 
        if (_find(value.first))
        {
            return std::pair<iterator, bool>(find(value.first), false);
        }
        size_type index = _bucket(value.first);
        value_type copyVal = value;
        HashNode* newNode = _insert_into_bucket(index, std::move(copyVal));
        return std::pair<iterator,bool>(iterator(this, newNode), true);
        /* TODO */ }

    iterator find(const Key & key) { 
        return iterator(this, _find(key));
        /* TODO */ }

    T& operator[](const Key & key) 
    { /* TODO */ 
        HashNode* found = _find(key);
        if(found != nullptr)
        {
            return found->val.second;
        }
        value_type newValue(key,T());
        iterator curr = insert(std::move(newValue)).first;
        return curr->second;
    }

    iterator erase(iterator pos) 
    { /* TODO */ 
    HashNode*& found = _find((*pos).first);
        if ( found != nullptr)
        {
            iterator curr(this, found);
            ++curr;

            if (found == _head)
            {
                _head = curr._ptr;
            }
            auto delNode = found;
            found = found->next;
            delete delNode;
            _size--;
            return curr;
        }
        return end();
    }

    size_type erase(const Key & key) 
    { /* TODO */ 
        HashNode* found = _find(key);
        if (found != nullptr)
        {
            iterator curr(this, found);
            erase(curr);
            return 1;
        }
        return 0;
    }

    template<typename KK, typename VV>
    friend void print_map(const UnorderedMap<KK, VV> & map, std::ostream & os);
};

template<typename K, typename V>
void print_map(const UnorderedMap<K, V> & map, std::ostream & os = std::cout) {
    using size_type = typename UnorderedMap<K, V>::size_type;
    using HashNode = typename UnorderedMap<K, V>::HashNode;

    for(size_type bucket = 0; bucket < map.bucket_count(); bucket++) {
        os << bucket << ": ";

        HashNode const * node = map._buckets[bucket];

        while(node) {
            os << "(" << node->val.first << ", " << node->val.second << ") ";
            node = node->next;
        }

        os << std::endl;
    }
}
