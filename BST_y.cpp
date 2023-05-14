#include "BST_y.h"
#include <stack>
#include <iostream>


BinarySearchTree::Node::Node(Key key, Value value, Node *parent, Node *left, Node *right)
        : keyValuePair(key, value), parent(parent), left(left), right(right) {}


BinarySearchTree::Node::Node(const BinarySearchTree::Node &other) {
    keyValuePair = other.keyValuePair;
    parent = other.parent;
    if (other.left == nullptr) {
        left = nullptr;
    } else {
        left = new Node(*other.left);
    }
    if (other.right == nullptr) {
        right = nullptr;
    } else {
        right = new Node(*other.right);
    }
}

void BinarySearchTree::Node::output_node(const std::string &prefix, const BinarySearchTree::Node *node, bool isLeft) {
    if (node == nullptr) {
        return;
    }

    std::cout << prefix;

    if (isLeft) {
        std::cout << "├── ";
    } else {
        std::cout << "└── ";
    }

    // Вывод информации о ключе (или других полезных данный в узле)
    std::cout << "(" << node->keyValuePair.first << ", " << node->keyValuePair.second << ")" << std::endl;

    // Префиксы для потомков
    std::string childPrefix = prefix;
    if (isLeft) {
        childPrefix += "│   ";
    } else {
        childPrefix += "    ";
    }

    // Рекурсивный вывод левого поддерева
    output_node(childPrefix, node->left, true);

    // Рекурсивный вывод правого поддерева
    output_node(childPrefix, node->right, false);
}

void BinarySearchTree::Node::insert(const Key &key, const Value &value) {
    if (key < keyValuePair.first) {
        if (left == nullptr) {
            left = new Node(key, value, this);
        } else {
            left->insert(key, value);
        }
    } else if (key > keyValuePair.first) {
        if (right == nullptr) {
            right = new Node(key, value, this);
        } else {
            right->insert(key, value);
        }
    } else { // Если ключи равны
        if (value < keyValuePair.second) {
            if (left == nullptr) {
                left = new Node(key, value, this);
            } else {
                left->insert(key, value);
            }
        } else {
            if (right == nullptr) {
                right = new Node(key, value, this);
            } else {
                right->insert(key, value);
            }
        }
    }
}

void BinarySearchTree::Node::erase(const Key &key) {
    if (key < keyValuePair.first) {
        if (left != nullptr) {
            left->erase(key);
        }
    } else if (key > keyValuePair.first) {
        if (right != nullptr) {
            right->erase(key);
        }
    } else {
        if (left == nullptr && right == nullptr) {
            // Case 1: Удаляемый узел - листовой узел
            if (parent != nullptr) {
                if (parent->left == this) {
                    parent->left = nullptr;
                } else {
                    parent->right = nullptr;
                }
            }
            delete this;
        } else if (left == nullptr) {
            // Case 2: Удаляемый узел имеет только правого потомка
            if (parent != nullptr) {
                if (parent->left == this) {
                    parent->left = right;
                } else {
                    parent->right = right;
                }
            }
            right->parent = parent;
            delete this;
        } else if (right == nullptr) {
            // Case 3: Удаляемый узел имеет только левого потомка
            if (parent != nullptr) {
                if (parent->left == this) {
                    parent->left = left;
                } else {
                    parent->right = left;
                }
            }
            left->parent = parent;
            delete this;
        } else {
            // Case 4: Удаляемый узел имеет и левого, и правого потомка
            // замена узла преемником
            Node *successor = right;
            while (successor->left != nullptr) {
                successor = successor->left;
            }
            keyValuePair = successor->keyValuePair;
            successor->erase(successor->keyValuePair.first);
        }
    }
}

bool BinarySearchTree::Node::operator==(const BinarySearchTree::Node &other) const {
    return keyValuePair == other.keyValuePair && parent == other.parent && left == other.left && right == other.right;
}

BinarySearchTree::BinarySearchTree(const BinarySearchTree &other) {
    if (other._root == nullptr) {
        _root = nullptr;
    } else {
        _root = new Node(*other._root);
    }
    _size = other._size;
}


BinarySearchTree &BinarySearchTree::operator=(const BinarySearchTree &other) {
    if (this == &other) {
        return *this; // самоприсваивание
    }
    this->~BinarySearchTree(); // то же самое, что clear() в векторе
    if (other._root == nullptr) {
        _root = nullptr;
    } else {
        _root = new Node(*other._root);
    }
    _size = other._size;
    return *this;
}

BinarySearchTree::BinarySearchTree(BinarySearchTree &&other) noexcept {
    _root = other._root;
    other._root = nullptr;
    _size = other._size;
    other._size = 0;
}

BinarySearchTree &BinarySearchTree::operator=(BinarySearchTree &&other) noexcept {
    // Тож само, что оператор, но есть чек на самоприсв и clear() this'a
    if (this == &other) {
        return *this;
    }
    this->~BinarySearchTree();
    _root = other._root;
    other._root = nullptr;
    _size = other._size;
    other._size = 0;
    return *this;
}

BinarySearchTree::~BinarySearchTree() {
    if (_root != nullptr) {
        std::stack<Node*> nodes;
        nodes.push(_root);
        while (!nodes.empty()) {
            Node* node = nodes.top();
            nodes.pop();
            if (node->left != nullptr) {
                nodes.push(node->left);
            }
            if (node->right != nullptr) {
                nodes.push(node->right);
            }
            delete node;
        }
    }
}

BinarySearchTree::Iterator::Iterator(BinarySearchTree::Node *node) {
    _node = node;
}

std::pair<Key, Value> &BinarySearchTree::Iterator::operator*() {
    return _node->keyValuePair;
}

const std::pair<Key, Value> &BinarySearchTree::Iterator::operator*() const {
    return _node->keyValuePair;
}

std::pair<Key, Value> *BinarySearchTree::Iterator::operator->() {
    return &_node->keyValuePair;
}

const std::pair<Key, Value> *BinarySearchTree::Iterator::operator->() const {
    return &_node->keyValuePair;
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator++() {
    // Проверяем, что итератор не указывает на конец дерева
    if (_node != nullptr) {
        // Если есть правый потомок, переходим к нему
        if (_node->right != nullptr) {
            _node = _node->right;
            // Переходим к крайнему левому потомку правого поддерева
            while (_node->left != nullptr) {
                _node = _node->left;
            }
        } else {
            // Если нет правого потомка, поднимаемся вверх по родительским узлам,
            // пока не найдем первый узел, в котором текущий узел является левым потомком
            Node *parent = _node->parent;
            while (parent != nullptr && _node == parent->right) {
                _node = parent;
                parent = parent->parent;
            }
            // Устанавливаем текущий узел в найденный родительский узел
            _node = parent;
        }
    }
    // Возвращаем измененный итератор
    return *this;
}


BinarySearchTree::Iterator BinarySearchTree::Iterator::operator++(int) {
    Iterator temp = *this;
    ++(*this);
    return temp;
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator--() {
    // Проверяем, что итератор не указывает на конец дерева
    if (_node != nullptr) {
        if (_node->left != nullptr) {
            // Если у текущего узла есть левый подузел, перемещаемся к самому правому узлу в его поддереве
            _node = _node->left;
            while (_node->right != nullptr) {
                _node = _node->right;
            }
        } else {
            // Если у текущего узла нет левого подузла, перемещаемся вверх по дереву
            // до первого узла, который является правым потомком своего родителя
            while (_node->parent != nullptr && _node->parent->right != _node) {
                _node = _node->parent;
            }
            _node = _node->parent; // Перемещаемся к найденному узлу или нулевому узлу, если достигли корня
        }
    }

    return *this;
}


BinarySearchTree::Iterator BinarySearchTree::Iterator::operator--(int) {
    Iterator temp = *this;
    --(*this);
    return temp;
}

bool BinarySearchTree::Iterator::operator==(const BinarySearchTree::Iterator &other) const {
    return _node == other._node;
}

bool BinarySearchTree::Iterator::operator!=(const BinarySearchTree::Iterator &other) const {
    return _node != other._node;
}


BinarySearchTree::ConstIterator::ConstIterator(const BinarySearchTree::Node *node) {
    _node = node;
}

const std::pair<Key, Value> &BinarySearchTree::ConstIterator::operator*() const {
    return _node->keyValuePair;
}

const std::pair<Key, Value> *BinarySearchTree::ConstIterator::operator->() const {
    return &_node->keyValuePair;
}

BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator++() {
    // Проверяем, что итератор не указывает на конец дерева
    if (_node != nullptr) {
        // Если есть правый потомок, переходим к нему
        if (_node->right != nullptr) {
            _node = _node->right;
            // Переходим к крайнему левому потомку правого поддерева
            while (_node->left != nullptr) {
                _node = _node->left;
            }
        } else {
            // Если нет правого потомка, поднимаемся вверх по родительским узлам,
            // пока не найдем первый узел, в котором текущий узел является левым потомком
            Node *parent = _node->parent;
            while (parent != nullptr && _node == parent->right) {
                _node = parent;
                parent = parent->parent;
            }
            // Устанавливаем текущий узел в найденный родительский узел
            _node = parent;
        }
    }
    // Возвращаем измененный итератор
    return *this;
}

BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator++(int) {
    ConstIterator tmp = *this;
    ++(*this);
    return tmp;
}

BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator--() {
    // Проверяем, что итератор не указывает на конец дерева
    if (_node != nullptr) {
        if (_node->left != nullptr) {
            // Если у текущего узла есть левый подузел, перемещаемся к самому правому узлу в его поддереве
            _node = _node->left;
            while (_node->right != nullptr) {
                _node = _node->right;
            }
        } else {
            // Если у текущего узла нет левого подузла, перемещаемся вверх по дереву
            // до первого узла, который является правым потомком своего родителя
            while (_node->parent != nullptr && _node->parent->right != _node) {
                _node = _node->parent;
            }
            _node = _node->parent; // Перемещаемся к найденному узлу или нулевому узлу, если достигли корня
        }
    }

    return *this;
}

BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator--(int) {
    ConstIterator tmp = *this;
    --(*this);
    return tmp;
}

bool BinarySearchTree::ConstIterator::operator==(const BinarySearchTree::ConstIterator &other) const {
    return _node == other._node;
}

bool BinarySearchTree::ConstIterator::operator!=(const BinarySearchTree::ConstIterator &other) const {
    return _node != other._node;
}

void BinarySearchTree::insert(const Key &key, const Value &value) {
    _size++;
    if (_root == nullptr) {
        _root = new Node(key, value);
    } else {
        _root->insert(key, value);
    }

}

void BinarySearchTree::erase(const Key &key) {
    Iterator it = this->find(key);
    Iterator root_it(_root);
    while (_root != nullptr && it->first == key) {
        _size--;
        if (it == root_it && (_root->left == nullptr || _root->right == nullptr)) {
            Node *to_del = _root;
            if (_root->left != nullptr) {
                _root = _root->left;
                _root->parent = nullptr;
            } else if (_root->right != nullptr) {
                _root = _root->right;
                _root->parent = nullptr;
            } else {
                _root = nullptr;
            }
            root_it = Iterator(_root);
            delete to_del;
        } else {
            _root->erase(key);
        }
        it = this->find(key);
    }
}


BinarySearchTree::Iterator BinarySearchTree::find(const Key &key) {
    Node *node = _root;
    while (node != nullptr) {
        if (key < node->keyValuePair.first) {
            node = node->left;
        } else if (key > node->keyValuePair.first) {
            node = node->right;
        } else {
            break;
        }
    }
    if (node == nullptr) {
        return end();
    }
    return Iterator(node);
}

BinarySearchTree::ConstIterator BinarySearchTree::find(const Key &key) const {
    Node *node = _root;
    while (node != nullptr) {
        if (key < node->keyValuePair.first) {
            node = node->left;
        } else if (key > node->keyValuePair.first) {
            node = node->right;
        } else {
            break;
        }
    }
    if (node == nullptr) {
        return cend();
    }
    return ConstIterator(node);
}

std::pair<BinarySearchTree::Iterator, BinarySearchTree::Iterator> BinarySearchTree::equalRange(const Key &key) {
    Iterator it_start = find(key);
    if (it_start == end()) {
        return std::make_pair(it_start, it_start);
    }
    Iterator it_tmp = it_start;
    Iterator it_end = it_start;
    it_tmp--;
    Iterator begin_it = begin();
    while (it_tmp->first == key && it_tmp != begin_it) {
        it_start--;
        it_tmp--;
    }
    it_tmp = it_end;
    it_tmp++;
    Iterator end_it = end();
    while (it_tmp->first == key && it_tmp != end_it) {
        it_end++;
        it_tmp++;
    }
    it_end++;
    return std::make_pair(it_start, it_end);
}

std::pair<BinarySearchTree::ConstIterator, BinarySearchTree::ConstIterator>
BinarySearchTree::equalRange(const Key &key) const {
    ConstIterator it_start = find(key);
    if (it_start == cend()) {
        return std::make_pair(it_start, it_start);
    }
    ConstIterator it_tmp = it_start;
    ConstIterator it_end = it_start;
    it_tmp--;
    ConstIterator begin_it = cbegin();
    while (it_tmp->first == key && it_tmp != begin_it) {
        it_start--;
        it_tmp--;
    }
    it_tmp = it_end;
    it_tmp++;
    ConstIterator end_it = cend();
    while (it_tmp->first == key && it_tmp != end_it) {
        it_end++;
        it_tmp++;
    }
    it_end++;
    return std::make_pair(it_start, it_end);
}

BinarySearchTree::ConstIterator BinarySearchTree::min(const Key &key) const {
    Node *current = _root;

    // Перейти к самому левому потомку
    while (current != nullptr && current->left != nullptr) {
        current = current->left;
    }

    // Вернуть итератор на текущий узел
    return ConstIterator(current);
}

BinarySearchTree::ConstIterator BinarySearchTree::max(const Key &key) const {
    Node *current = _root;

    // Перейти к самому правому потомку
    while (current != nullptr && current->right != nullptr) {
        current = current->right;
    }

    // Вернуть итератор на текущий узел
    return ConstIterator(current);
}


BinarySearchTree::Iterator BinarySearchTree::begin() {
    Node *node = _root;
    if (node == nullptr) {
        return Iterator(node);
    }
    while (node->left != nullptr) {
        node = node->left;
    }
    return Iterator(node);
}

BinarySearchTree::ConstIterator BinarySearchTree::cbegin() const {
    Node *node = _root;
    if (node == nullptr) {
        return ConstIterator(node);
    }
    while (node->left != nullptr) {
        node = node->left;
    }
    return ConstIterator(node);
}

BinarySearchTree::Iterator BinarySearchTree::end() {
    Node *node = _root;
    if (node == nullptr) {
        return Iterator(node);
    }
    while (node->right != nullptr) {
        node = node->right;
    }
    return Iterator(node);
}

BinarySearchTree::ConstIterator BinarySearchTree::cend() const {
    Node *node = _root;
    if (node == nullptr) {
        return ConstIterator(node);
    }
    while (node->right != nullptr) {
        node = node->right;
    }
    return ConstIterator(node);
}

size_t BinarySearchTree::size() const {
    return _size;
}

void BinarySearchTree::output_tree() {
    _root->output_node("", _root, false);
}