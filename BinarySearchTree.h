#pragma once

#include <functional> // std::less
#include <iostream>
#include <queue> // std::queue
#include <utility> // std::pair

template <typename K, typename V, typename Comparator = std::less<K>>
class BinarySearchTree
{
  public:
    using key_type        = K;
    using value_type      = V;
    using key_compare     = Comparator;
    using pair            = std::pair<key_type, value_type>;
    using pointer         = pair*;
    using const_pointer   = const pair*;
    using reference       = pair&;
    using const_reference = const pair&;
    using difference_type = ptrdiff_t;
    using size_type       = size_t;

  private:
    struct BinaryNode
    {
        pair element;
        BinaryNode *left;
        BinaryNode *right;

        BinaryNode( const_reference theElement, BinaryNode *lt, BinaryNode *rt )
          : element{ theElement }, left{ lt }, right{ rt } { }
        
        BinaryNode( pair && theElement, BinaryNode *lt, BinaryNode *rt )
          : element{ std::move( theElement ) }, left{ lt }, right{ rt } { }
    };

    using node           = BinaryNode;
    using node_ptr       = node*;
    using const_node_ptr = const node*;

    node_ptr _root;
    size_type _size;
    key_compare comp;

  public:
    BinarySearchTree() {
        _root = nullptr;
        _size = 0;
        // TODO
    }
    BinarySearchTree( const BinarySearchTree & rhs ) {
        _size = rhs._size;
        _root = clone(rhs._root);
        // TODO
    }
    BinarySearchTree( BinarySearchTree && rhs ) {
        _size = rhs._size;
        _root = rhs._root;
        rhs._size = 0;
        rhs._root = nullptr;
        // TODO
    }
    ~BinarySearchTree() {
        clear();
        // TODO
    }

    const_reference min() const { return min( _root )->element; }
    const_reference max() const { return max( _root )->element; }
    const_reference root() const {
        return _root->element;
        // TODO
    }
    bool contains( const key_type & x ) const { return contains( x, _root ); }
    value_type & find( const key_type & key ) { return find( key, _root )->element.second; }
    const value_type & find( const key_type & key ) const { return find( key, _root )->element.second; }
    bool empty() const {
        if(_root == nullptr)
        {
            return true;
        }
        return false;
        // TODO
    }
    size_type size() const {
        return _size;
        // TODO
    }

    void clear() {
        clear( _root );
        _size = 0;
    }
    void insert( const_reference x ) { insert( x, _root ); }
    void insert( pair && x ) { insert( std::move( x ), _root ); }
    void erase( const key_type & x ) { erase(x, _root); }

    BinarySearchTree & operator=( const BinarySearchTree & rhs ) {
        if(&rhs == this)
        {
            return *this;
        }
        this->clear();
        _root = clone(rhs._root);
        this->_size = rhs._size;
        return *this;
        

        // TODO
    }
    BinarySearchTree & operator=( BinarySearchTree && rhs ) {
        // TODO
        if(&rhs == this)
        {
            return *this;
        }
        this->clear();
        this->_root = rhs._root;
        this->_size = rhs._size;
        rhs._size = 0;
        rhs._root = nullptr;
        return *this;
    }

  private:
    void insert( const_reference x, node_ptr & t ) {
        if (t == nullptr) //base case
        {
            t = new BinaryNode(x, nullptr, nullptr);
            _size++;
        }
        else if (comp(x.first,t->element.first))
        {
            insert(x,t->left);
        }
        else if (comp(t->element.first, x.first))
        {
            insert(x, t->right);
        }
        else 
        {
            t->element.second = x.second;
        }
        // TODO
    }
    void insert( pair && x, node_ptr & t ) {
        if (t == nullptr) //base case
        {
            t = new BinaryNode(std::move(x), nullptr, nullptr);
            _size++;
        }
        else if (comp(x.first,t->element.first))
        {
            insert(std::move(x),t->left);
        }
        else if (comp(t->element.first, x.first))
        {
            insert(std::move(x), t->right);
        }
        else 
        {
            t->element.second = std::move(x.second);
        }
        // TODO
    }

    void erase( const key_type & x, node_ptr & t ) {
        
        if (comp(x, t->element.first))
        {
            erase(x, t->left);
        }
        else if (comp(t->element.first, x))
        {
            erase(x,t->right);
        }
        else
        {
            //case 1 leaf
            if (t->left == nullptr && t->right == nullptr)
            {
                delete t;
                t = nullptr;
                _size--;
            }
            //case 2 if only either right or left is nullptr
            else if (t->left != nullptr && t->right == nullptr)
            {
                node_ptr temp = t;
                t = t->left;
                delete temp;
                temp = nullptr;
                _size--;
            }
            else if (t->left == nullptr && t->right != nullptr)
            {
                node_ptr temp = t;
                t = t->right;
                delete temp;
                temp = nullptr;
                _size--;
            }
            //case 3 if both right and left are not nullptrs
            else
            {
                t->element = min(t->right)->element;
                erase(min(t->right)->element.first,t->right);
            }
        }

        
        // TODO
    }

    const_node_ptr min( const_node_ptr t ) const {
        if (t->left == nullptr)
        {
            return t;
        }
        min(t->left);
        // TODO
    }
    const_node_ptr max( const_node_ptr t ) const {
        if(t->right == nullptr)
        {
            return t;
        }
        max(t->right);
        // TODO
    }

    bool contains( const key_type & x, const_node_ptr t ) const {
        // if (t->element.first == x)
        // {
        //     return true;
        // }
        // else if (t == nullptr)
        // {
        //     return false;
        // }
        // else if (comp(t->element.first, x))
        // {
        //     contains(x,t->right);
        // }
        // else if (comp(x, t->element.first))
        // {
        //     contains(x, t->left);
        // }
        return find(x,t);

        
        // TODO
    }
    node_ptr find( const key_type & key, node_ptr t ) {
        if (!t)
        {
            return nullptr;
        }
        else if (comp(t->element.first, key))
        {
            find(key,t->right);
        }
        else if (comp(key, t->element.first))
        {
            find(key, t->left);
        }
        else{
            return t;
        }
        // TODO
    }
    const_node_ptr find( const key_type & key, const_node_ptr t ) const {
        if (!t)
        {
            return nullptr;
        }
        else if (comp(key, t->element.first))
        {
            find(key, t->left);
        }
        else if (comp(t->element.first, key))
        {
            find(key,t->right);
        }
        else
        {
            return t;
        }
        // TODO
    }

    void clear( node_ptr & t ) {
        if (t == nullptr) //base case
        {
            return;
        }
        if (t->left != nullptr)
        {
            clear(t->left);
        }
        if (t->right != nullptr)
        {
            clear(t->right);
        }
        delete t;
        t = nullptr;
        // TODO
    }
    
    node_ptr clone ( const_node_ptr t ) const {
        node_ptr newT = nullptr;
        if (t != nullptr)
        {
            newT = new BinaryNode(t->element, clone(t->left), clone(t->right));
            return newT;
        }
        return newT;
        // TODO
    }
    void print_helper(std::ostream& o, const_node_ptr t) {
     if (t == nullptr) {
          return;
     }
     print_helper(o, t->left);
     print_helper(o, t->right);
     o << "(" << t->element.first << ", " << t->element.second << ")" << std::endl;
}

void print(std::ostream& o) {
      print_helper(o, _root);
}

  public:
    template <typename KK, typename VV, typename CC>
    friend void printLevelByLevel( const BinarySearchTree<KK, VV, CC>& bst, std::ostream & out );

    template <typename KK, typename VV, typename CC>
    friend std::ostream& printNode(std::ostream& o, const typename BinarySearchTree<KK, VV, CC>::node& bn);

    template <typename KK, typename VV, typename CC>
    friend void printTree( const BinarySearchTree<KK, VV, CC>& bst, std::ostream & out );

    template <typename KK, typename VV, typename CC>
    friend void printTree(typename BinarySearchTree<KK, VV, CC>::const_node_ptr t, std::ostream & out, unsigned depth );

    template <typename KK, typename VV, typename CC>
    friend void vizTree(
        typename BinarySearchTree<KK, VV, CC>::const_node_ptr node, 
        std::ostream & out,
        typename BinarySearchTree<KK, VV, CC>::const_node_ptr prev
    );

    template <typename KK, typename VV, typename CC>
    friend void vizTree(
        const BinarySearchTree<KK, VV, CC> & bst, 
        std::ostream & out
    );
};

template <typename KK, typename VV, typename CC>
std::ostream& printNode(std::ostream & o, const typename BinarySearchTree<KK, VV, CC>::node & bn) {
    return o << '(' << bn.element.first << ", " << bn.element.second << ')';
}

template <typename KK, typename VV, typename CC>
void printLevelByLevel( const BinarySearchTree<KK, VV, CC>& bst, std::ostream & out = std::cout ) {
    using node = typename BinarySearchTree<KK, VV, CC>::node;
    using node_ptr = typename BinarySearchTree<KK, VV, CC>::node_ptr;
    using const_node_ptr = typename BinarySearchTree<KK, VV, CC>::const_node_ptr;
    
    // TODO -- Guide in Instructions
    if (bst.empty())
    {
        return;
    }
    std::queue<node_ptr> q;//this queue contains elements from a level and its children
    q.push(bst._root);
    int elementsInLevel = 1; //Elements in the current level
    bool nonNullChild = false;
    while (elementsInLevel > 0)
    {
        node_ptr curr = q.front();
        q.pop();
        elementsInLevel--;
        if (curr != nullptr)
        {
            printNode<KK,VV,CC>(out, *curr);
            q.push(curr->left);
            q.push(curr->right);
            if (curr->left != nullptr || curr->right != nullptr)
            {
                nonNullChild = true;
            }
        }
        else
        {
            out << "null";
            q.push(nullptr); //these nulls represent the descendants of the empty node
            q.push(nullptr);
        }

        if (elementsInLevel == 0) //Reached the end of current level
        {
            std::cout << std::endl;
            if (nonNullChild == true) //next level is non-empty
            {
                nonNullChild = false;
                elementsInLevel = q.size();
            }
        }
        
    }
}



template <typename KK, typename VV, typename CC>
void printTree( const BinarySearchTree<KK, VV, CC> & bst, std::ostream & out = std::cout ) { printTree<KK, VV, CC>(bst._root, out ); }

template <typename KK, typename VV, typename CC>
void printTree(typename BinarySearchTree<KK, VV, CC>::const_node_ptr t, std::ostream & out, unsigned depth = 0 ) {
    if (t != nullptr) {
        printTree<KK, VV, CC>(t->right, out, depth + 1);
        for (unsigned i = 0; i < depth; ++i)
            out << '\t';
        printNode<KK, VV, CC>(out, *t) << '\n';
        printTree<KK, VV, CC>(t->left, out, depth + 1);
    }
}

template <typename KK, typename VV, typename CC>
void vizTree(
    typename BinarySearchTree<KK, VV, CC>::const_node_ptr node, 
    std::ostream & out,
    typename BinarySearchTree<KK, VV, CC>::const_node_ptr prev = nullptr
) {
    if(node) {
        std::hash<KK> khash{};

        out << "\t" "node_" << (uint32_t) khash(node->element.first)
            << "[label=\"" << node->element.first 
            << " [" << node->element.second << "]\"];" << std::endl;
        
        if(prev)
            out << "\tnode_" << (uint32_t) khash(prev->element.first) <<  " -> ";
        else
            out << "\t";
        
        out << "node_" << (uint32_t) khash(node->element.first) << ";" << std::endl;
    
        vizTree<KK, VV, CC>(node->left, out, node);
        vizTree<KK, VV, CC>(node->right, out, node);
    }
}

template <typename KK, typename VV, typename CC>
void vizTree(
    const BinarySearchTree<KK, VV, CC> & bst, 
    std::ostream & out = std::cout
) {
    out << "digraph Tree {" << std::endl;
    vizTree<KK, VV, CC>(bst._root, out);
    out << "}" << std::endl;
}
