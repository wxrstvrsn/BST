#include "BST.h"
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
        std::cout << "в”њв”Ђв”Ђ ";
    } else {
        std::cout << "в””в”Ђв”Ђ ";
    }

    // Р’С‹РІРѕРґ РёРЅС„РѕСЂРјР°С†РёРё Рѕ РєР»СЋС‡Рµ (РёР»Рё РґСЂСѓРіРёС… РїРѕР»РµР·РЅС‹С… РґР°РЅРЅС‹Р№ РІ СѓР·Р»Рµ)
    std::cout << "(" << node->keyValuePair.first << ", " << node->keyValuePair.second << ")" << std::endl;

    // РџСЂРµС„РёРєСЃС‹ РґР»СЏ РїРѕС‚РѕРјРєРѕРІ
    std::string childPrefix = prefix;
    if (isLeft) {
        childPrefix += "в”‚   ";
    } else {
        childPrefix += "    ";
    }

    // Р РµРєСѓСЂСЃРёРІРЅС‹Р№ РІС‹РІРѕРґ Р»РµРІРѕРіРѕ РїРѕРґРґРµСЂРµРІР°
    output_node(childPrefix, node->left, true);

    // Р РµРєСѓСЂСЃРёРІРЅС‹Р№ РІС‹РІРѕРґ РїСЂР°РІРѕРіРѕ РїРѕРґРґРµСЂРµРІР°
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
    } else { // Р•СЃР»Рё РєР»СЋС‡Рё СЂР°РІРЅС‹
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
            // Case 1: РЈРґР°Р»СЏРµРјС‹Р№ СѓР·РµР» - Р»РёСЃС‚РѕРІРѕР№ СѓР·РµР»
            if (parent != nullptr) {
                if (parent->left == this) {
                    parent->left = nullptr;
                } else {
                    parent->right = nullptr;
                }
            }
            delete this;
        } else if (left == nullptr) {
            // Case 2: РЈРґР°Р»СЏРµРјС‹Р№ СѓР·РµР» РёРјРµРµС‚ С‚РѕР»СЊРєРѕ РїСЂР°РІРѕРіРѕ РїРѕС‚РѕРјРєР°
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
            // Case 3: РЈРґР°Р»СЏРµРјС‹Р№ СѓР·РµР» РёРјРµРµС‚ С‚РѕР»СЊРєРѕ Р»РµРІРѕРіРѕ РїРѕС‚РѕРјРєР°
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
            // Case 4: РЈРґР°Р»СЏРµРјС‹Р№ СѓР·РµР» РёРјРµРµС‚ Рё Р»РµРІРѕРіРѕ, Рё РїСЂР°РІРѕРіРѕ РїРѕС‚РѕРјРєР°
            // Р·Р°РјРµРЅР° СѓР·Р»Р° РїСЂРµРµРјРЅРёРєРѕРј
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
        return *this; // СЃР°РјРѕРїСЂРёСЃРІР°РёРІР°РЅРёРµ
    }
    this->~BinarySearchTree(); // С‚Рѕ Р¶Рµ СЃР°РјРѕРµ, С‡С‚Рѕ clear() РІ РІРµРєС‚РѕСЂРµ
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
    // РўРѕР¶ СЃР°РјРѕ, С‡С‚Рѕ РѕРїРµСЂР°С‚РѕСЂ, РЅРѕ РµСЃС‚СЊ С‡РµРє РЅР° СЃР°РјРѕРїСЂРёСЃРІ Рё clear() this'a
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
        std::stack<Node *> nodes;
        nodes.push(_root);
        while (!nodes.empty()) {
            Node *node = nodes.top();
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
    // РџСЂРѕРІРµСЂСЏРµРј, С‡С‚Рѕ РёС‚РµСЂР°С‚РѕСЂ РЅРµ СѓРєР°Р·С‹РІР°РµС‚ РЅР° РєРѕРЅРµС† РґРµСЂРµРІР°
    if (_node != nullptr) {
        // Р•СЃР»Рё РµСЃС‚СЊ РїСЂР°РІС‹Р№ РїРѕС‚РѕРјРѕРє, РїРµСЂРµС…РѕРґРёРј Рє РЅРµРјСѓ
        if (_node->right != nullptr) {
            _node = _node->right;
            // РџРµСЂРµС…РѕРґРёРј Рє РєСЂР°Р№РЅРµРјСѓ Р»РµРІРѕРјСѓ РїРѕС‚РѕРјРєСѓ РїСЂР°РІРѕРіРѕ РїРѕРґРґРµСЂРµРІР°
            while (_node->left != nullptr) {
                _node = _node->left;
            }
        } else {
            // Р•СЃР»Рё РЅРµС‚ РїСЂР°РІРѕРіРѕ РїРѕС‚РѕРјРєР°, РїРѕРґРЅРёРјР°РµРјСЃСЏ РІРІРµСЂС… РїРѕ СЂРѕРґРёС‚РµР»СЊСЃРєРёРј СѓР·Р»Р°Рј,
            // РїРѕРєР° РЅРµ РЅР°Р№РґРµРј РїРµСЂРІС‹Р№ СѓР·РµР», РІ РєРѕС‚РѕСЂРѕРј С‚РµРєСѓС‰РёР№ СѓР·РµР» СЏРІР»СЏРµС‚СЃСЏ Р»РµРІС‹Рј РїРѕС‚РѕРјРєРѕРј
            Node *parent = _node->parent;
            while (parent != nullptr && _node == parent->right) {
                _node = parent;
                parent = parent->parent;
            }
            // РЈСЃС‚Р°РЅР°РІР»РёРІР°РµРј С‚РµРєСѓС‰РёР№ СѓР·РµР» РІ РЅР°Р№РґРµРЅРЅС‹Р№ СЂРѕРґРёС‚РµР»СЊСЃРєРёР№ СѓР·РµР»
            _node = parent;
        }
    }
    // Р’РѕР·РІСЂР°С‰Р°РµРј РёР·РјРµРЅРµРЅРЅС‹Р№ РёС‚РµСЂР°С‚РѕСЂ
    return *this;
}


BinarySearchTree::Iterator BinarySearchTree::Iterator::operator++(int) {
    Iterator temp = *this;
    ++(*this);
    return temp;
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator--() {
    // РџСЂРѕРІРµСЂСЏРµРј, С‡С‚Рѕ РёС‚РµСЂР°С‚РѕСЂ РЅРµ СѓРєР°Р·С‹РІР°РµС‚ РЅР° РєРѕРЅРµС† РґРµСЂРµРІР°
    if (_node != nullptr) {
        if (_node->left != nullptr) {
            // Р•СЃР»Рё Сѓ С‚РµРєСѓС‰РµРіРѕ СѓР·Р»Р° РµСЃС‚СЊ Р»РµРІС‹Р№ РїРѕРґСѓР·РµР», РїРµСЂРµРјРµС‰Р°РµРјСЃСЏ Рє СЃР°РјРѕРјСѓ РїСЂР°РІРѕРјСѓ СѓР·Р»Сѓ РІ РµРіРѕ РїРѕРґРґРµСЂРµРІРµ
            _node = _node->left;
            while (_node->right != nullptr) {
                _node = _node->right;
            }
        } else {
            // Р•СЃР»Рё Сѓ С‚РµРєСѓС‰РµРіРѕ СѓР·Р»Р° РЅРµС‚ Р»РµРІРѕРіРѕ РїРѕРґСѓР·Р»Р°, РїРµСЂРµРјРµС‰Р°РµРјСЃСЏ РІРІРµСЂС… РїРѕ РґРµСЂРµРІСѓ
            // РґРѕ РїРµСЂРІРѕРіРѕ СѓР·Р»Р°, РєРѕС‚РѕСЂС‹Р№ СЏРІР»СЏРµС‚СЃСЏ РїСЂР°РІС‹Рј РїРѕС‚РѕРјРєРѕРј СЃРІРѕРµРіРѕ СЂРѕРґРёС‚РµР»СЏ
            while (_node->parent != nullptr && _node->parent->right != _node) {
                _node = _node->parent;
            }
            _node = _node->parent; // РџРµСЂРµРјРµС‰Р°РµРјСЃСЏ Рє РЅР°Р№РґРµРЅРЅРѕРјСѓ СѓР·Р»Сѓ РёР»Рё РЅСѓР»РµРІРѕРјСѓ СѓР·Р»Сѓ, РµСЃР»Рё РґРѕСЃС‚РёРіР»Рё РєРѕСЂРЅСЏ
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
    // РџСЂРѕРІРµСЂСЏРµРј, С‡С‚Рѕ РёС‚РµСЂР°С‚РѕСЂ РЅРµ СѓРєР°Р·С‹РІР°РµС‚ РЅР° РєРѕРЅРµС† РґРµСЂРµРІР°
    if (_node != nullptr) {
        // Р•СЃР»Рё РµСЃС‚СЊ РїСЂР°РІС‹Р№ РїРѕС‚РѕРјРѕРє, РїРµСЂРµС…РѕРґРёРј Рє РЅРµРјСѓ
        if (_node->right != nullptr) {
            _node = _node->right;
            // РџРµСЂРµС…РѕРґРёРј Рє РєСЂР°Р№РЅРµРјСѓ Р»РµРІРѕРјСѓ РїРѕС‚РѕРјРєСѓ РїСЂР°РІРѕРіРѕ РїРѕРґРґРµСЂРµРІР°
            while (_node->left != nullptr) {
                _node = _node->left;
            }
        } else {
            // Р•СЃР»Рё РЅРµС‚ РїСЂР°РІРѕРіРѕ РїРѕС‚РѕРјРєР°, РїРѕРґРЅРёРјР°РµРјСЃСЏ РІРІРµСЂС… РїРѕ СЂРѕРґРёС‚РµР»СЊСЃРєРёРј СѓР·Р»Р°Рј,
            // РїРѕРєР° РЅРµ РЅР°Р№РґРµРј РїРµСЂРІС‹Р№ СѓР·РµР», РІ РєРѕС‚РѕСЂРѕРј С‚РµРєСѓС‰РёР№ СѓР·РµР» СЏРІР»СЏРµС‚СЃСЏ Р»РµРІС‹Рј РїРѕС‚РѕРјРєРѕРј
            Node *parent = _node->parent;
            while (parent != nullptr && _node == parent->right) {
                _node = parent;
                parent = parent->parent;
            }
            // РЈСЃС‚Р°РЅР°РІР»РёРІР°РµРј С‚РµРєСѓС‰РёР№ СѓР·РµР» РІ РЅР°Р№РґРµРЅРЅС‹Р№ СЂРѕРґРёС‚РµР»СЊСЃРєРёР№ СѓР·РµР»
            _node = parent;
        }
    }
    // Р’РѕР·РІСЂР°С‰Р°РµРј РёР·РјРµРЅРµРЅРЅС‹Р№ РёС‚РµСЂР°С‚РѕСЂ
    return *this;
}

BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator++(int) {
    ConstIterator tmp = *this;
    ++(*this);
    return tmp;
}

BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator--() {
    // РџСЂРѕРІРµСЂСЏРµРј, С‡С‚Рѕ РёС‚РµСЂР°С‚РѕСЂ РЅРµ СѓРєР°Р·С‹РІР°РµС‚ РЅР° РєРѕРЅРµС† РґРµСЂРµРІР°
    if (_node != nullptr) {
        if (_node->left != nullptr) {
            // Р•СЃР»Рё Сѓ С‚РµРєСѓС‰РµРіРѕ СѓР·Р»Р° РµСЃС‚СЊ Р»РµРІС‹Р№ РїРѕРґСѓР·РµР», РїРµСЂРµРјРµС‰Р°РµРјСЃСЏ Рє СЃР°РјРѕРјСѓ РїСЂР°РІРѕРјСѓ СѓР·Р»Сѓ РІ РµРіРѕ РїРѕРґРґРµСЂРµРІРµ
            _node = _node->left;
            while (_node->right != nullptr) {
                _node = _node->right;
            }
        } else {
            // Р•СЃР»Рё Сѓ С‚РµРєСѓС‰РµРіРѕ СѓР·Р»Р° РЅРµС‚ Р»РµРІРѕРіРѕ РїРѕРґСѓР·Р»Р°, РїРµСЂРµРјРµС‰Р°РµРјСЃСЏ РІРІРµСЂС… РїРѕ РґРµСЂРµРІСѓ
            // РґРѕ РїРµСЂРІРѕРіРѕ СѓР·Р»Р°, РєРѕС‚РѕСЂС‹Р№ СЏРІР»СЏРµС‚СЃСЏ РїСЂР°РІС‹Рј РїРѕС‚РѕРјРєРѕРј СЃРІРѕРµРіРѕ СЂРѕРґРёС‚РµР»СЏ
            while (_node->parent != nullptr && _node->parent->right != _node) {
                _node = _node->parent;
            }
            _node = _node->parent; // РџРµСЂРµРјРµС‰Р°РµРјСЃСЏ Рє РЅР°Р№РґРµРЅРЅРѕРјСѓ СѓР·Р»Сѓ РёР»Рё РЅСѓР»РµРІРѕРјСѓ СѓР·Р»Сѓ, РµСЃР»Рё РґРѕСЃС‚РёРіР»Рё РєРѕСЂРЅСЏ
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

    // РџРµСЂРµР№С‚Рё Рє СЃР°РјРѕРјСѓ Р»РµРІРѕРјСѓ РїРѕС‚РѕРјРєСѓ
    while (current != nullptr && current->left != nullptr) {
        current = current->left;
    }

    // Р’РµСЂРЅСѓС‚СЊ РёС‚РµСЂР°С‚РѕСЂ РЅР° С‚РµРєСѓС‰РёР№ СѓР·РµР»
    return ConstIterator(current);
}

BinarySearchTree::ConstIterator BinarySearchTree::max(const Key &key) const {
    Node *current = _root;

    // РџРµСЂРµР№С‚Рё Рє СЃР°РјРѕРјСѓ РїСЂР°РІРѕРјСѓ РїРѕС‚РѕРјРєСѓ
    while (current != nullptr && current->right != nullptr) {
        current = current->right;
    }

    // Р’РµСЂРЅСѓС‚СЊ РёС‚РµСЂР°С‚РѕСЂ РЅР° С‚РµРєСѓС‰РёР№ СѓР·РµР»
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