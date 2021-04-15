#include "Map.h"

Map::Map() { m_root = nullptr; }

// destructor auxiliary function
void Map::DestroyMap(Node* curr) {
    if (curr == nullptr) return;      // nothing to destroy
    DestroyMap(curr->m_left);         // process left
    DestroyMap(curr->m_right);        // process right
    delete curr;                      // destroy the current node
}

Map::~Map() {
    DestroyMap(m_root);
    m_root = nullptr;
}

// copy constructor auxiliary function
void Map::CopyTree(Node* &thisRoot, Node* &otherRoot) {
    if (otherRoot == nullptr) thisRoot = nullptr;
    else {
        thisRoot = new Node(otherRoot->m_key, otherRoot->m_value);  // copy the root
        CopyTree(thisRoot->m_left, otherRoot->m_left);              // copy the left subtree
        CopyTree(thisRoot->m_right, otherRoot->m_right);            // copy the right subtree
    }
}

Map::Map(const Map& other) {
    
    if(this != &other) {
        Node* rhs = other.m_root;
        CopyTree(this->m_root, rhs);
    }
    return;
}

Map& Map::operator=(const Map& rhs) {
    if (this != &rhs) {
        Map temp(rhs);      // if the destructor is written correctly, temp should be destroyed at the end
        swap(temp);
    }
    return *this;           // *this should be temp right before it is destroyed
}

bool Map::empty() const { return (m_root == nullptr); }

// size auxiliary function
int Map::countNodes(Node* curr) const {
    int count = 1;
    if (curr == nullptr) return 0;      // if empty this will just return 0 right away
    count += countNodes(curr->m_left);  // count the nodes in the left subtree
    count += countNodes(curr->m_right); // count the nodes in the right subtree
    return count;
}

int Map::size() const {
    return countNodes(m_root);
}

bool Map::insert(const KeyType& key, const ValueType& value) {
    
    if (contains(key)) return false;
    
    // if the map is empty, insert using the root pointer
    if (m_root == nullptr) {
        m_root = new Node(key, value);
        return true;
    }
    
    Node* curr = m_root;
    while (true) {

        if (key < curr->m_key) {
            if (curr->m_left != nullptr) {
                curr = curr->m_left;                    // go left if the left pointer is not nullptr
                continue;
            } else {
                curr->m_left = new Node(key, value);    // if it is nullptr insert the key/value here
                return true;
            }
        }
        
        if (key > curr->m_key) {
            if (curr->m_right != nullptr) {
                curr = curr->m_right;                   // go right if the right pointer is not nullptr
                continue;
            } else {
                curr->m_right = new Node(key, value);   // if it is nullptr, insert the key/value here
                return true;
            }
        }
    }
}

bool Map::update(const KeyType& key, const ValueType& value) {
    
    if (!contains(key)) return false;
    Node* curr = m_root;
    while (true) {                  // while no value has been returned
        if (key == curr->m_key) {
            curr->m_value = value;  // assign current value value if the key is the same
            return true;
        }
        if (key < curr->m_key) {    // traverse left if key is less than current key
            curr = curr->m_left;
            continue;
        }
        if (key > curr->m_key) {    // traverse right if key is greater than current key
            curr = curr->m_right;
            continue;
        }
    }
}

// overloaded contains auxiliary function
bool Map::contains(Node* curr, const KeyType& key) const {
    if (curr == nullptr) return false;              // if the Map is empty, then it doesn't contain anything
    if (curr->m_key == key) return true;            // if key is the current key, it contains key
    if (contains(curr->m_left, key)) return true;   // if the left subtree contains key return true
    else return contains(curr->m_right, key);       // if the right subtree contains key return true
    
}

bool Map::contains(const KeyType& key) const {
    Node* curr = m_root;
    return contains(curr, key);
}

bool Map::insertOrUpdate(const KeyType& key, const ValueType& value) {
    if (contains(key)) return update(key, value);   // if the Map contains key, update it
    else return insert(key, value);                 // otherwise, insert it
}

void Map::swap(Map& other) {
    Node* temp = this->m_root;
    this->m_root = other.m_root;
    other.m_root = temp;
}

// eraseNode auxiliary function
Map::Node* Map::minimumNode(Node* curr) {
    while (curr->m_left != nullptr) curr = curr->m_left;    // BST so min node is left-most node
    return curr;
}


// erase auxiliary function
// deletes the node with key as its m_key and then returns the new root of the tree
Map::Node* Map::eraseNode(Node* &root, const KeyType& key) {

    if (root == nullptr) return root;
    else if (key < root->m_key) root->m_left = eraseNode(root->m_left, key);    // traverse left subtree if less than
    else if (key > root->m_key) root->m_right = eraseNode(root->m_right, key);  // traverse right subtree if less than
    else {                                                                      // once we find the node we want to erase
        if (root->m_left == nullptr && root->m_right == nullptr) {              // if root is leaf node
            delete root;
            root = nullptr;
        }
        else if (root->m_left == nullptr) {                         // if root has no left child
            Node* temp = root;                                      // point to the node to be erased
            root = root->m_right;                                   // update what the right child is
            delete temp;                                            // erase the node
        }
        
        else if (root->m_right == nullptr) {                        // if root has no right child
            Node* temp = root;                                      // point to the node to be erased
            root = root->m_left;                                    // update what the left child is
            delete temp;                                            // erase the node
        }
        
        else {                                                      // if root has two children
            Node* min = minimumNode(root->m_right);                 // get the min node of right subtree
            root->m_key = min->m_key;                               // copy min's key into root
            root->m_value = min->m_value;                           // copy min's value into root
            root->m_right = eraseNode(root->m_right, min->m_key);   // erase the min node from the right subtree
        }
    }
    
    return root;
}

bool Map::erase(const KeyType& key) {
    if (!contains(key)) return false;   // if the Map doesn't contain key, can't erase it
    eraseNode(m_root, key);             // otherwise, erase it
    return true;
}

bool Map::get(const KeyType& key, ValueType& value) const {
    
    if (!contains(key)) return false;
    
    Node* curr = m_root;
    while (true) {                                      // while we haven't returned a value
        if (key < curr->m_key) curr = curr->m_left;     // traverse left if key is less than current key
        
        if (key == curr->m_key) {                       // if key is current key
            value = curr->m_value;                      // get the value
            return true;
        }
        if (key > curr->m_key) curr = curr->m_right;    // traverse right if key is less than current key
    }
}

// auxiliary function for three-argument get function
void Map::getPreOrder(int i, int& count, Node* &curr, KeyType& k, ValueType& v) const {
    if (curr) {                                     // if not at end of leaf
        if (count == i) {                           // if at i
            k = curr->m_key;                        // get the key
            v = curr->m_value;                      // get the value
            count = -1;                             // to prevent if statement from executing again on the wrong node when function calls are popping off the stack.
            return;
        }
        count++;                                    // increase the current index
        getPreOrder(i, count, curr->m_left, k, v);  // traverse left subtree
        getPreOrder(i, count, curr->m_right, k, v); // traverse right subtree
    }
}

bool Map::get(int i, KeyType& key, ValueType& value) const {
    
    if (i < 0 || i >= size()) return false;
    Node* curr = m_root;
    int count = 0;                                  // initialize starting index
    getPreOrder(i, count, curr, key, value);        // from count, pre-order traverse to i and get (key, value) pair
    return true;
}


bool combine(const Map& m1, const Map& m2, Map& result) {

    result = m1;                            // deal with result possibly being nonempty by using the assignment operator
    bool flag = true;
    int n = m2.size();                      // this way only have to call size() once
    for (int i = 0; i < n; i++) {           // for each node in the tree
        KeyType key;
        ValueType value;
        m2.get(i, key, value);              // get the node
        if (result.contains(key)) {         // if the key is in m1
            ValueType val;
            result.get(key, val);           // get the value
            if (value != val) {             // check the value
                flag = false;               // if key is the same but value is different
                result.erase(key);          // erase the node from the result
            }
        }
        else result.insert(key, value);     // if the key is not in m1 insert it into result
    }
    return flag;                            // if we tried adding in a key with conflicting values from m1 and m2, should return false
}

void subtract(const Map& m1, const Map& m2, Map& result) {

    result = m1;                            // deal with result possibly being nonempty by using the assignment operator
    int n = m2.size();                      // this way only have to call size() once
    for (int i = 0; i < n; i++) {
        KeyType key;
        ValueType value;
        m2.get(i, key, value);              // for each node in m2 get the key value
        if (result.contains(key))           // if it is in result
            result.erase(key);              // get rid of it
    }
}
