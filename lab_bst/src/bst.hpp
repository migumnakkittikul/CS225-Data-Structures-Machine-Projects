/**
 * @file bst.cpp
 * Definitions of the binary tree functions you'll be writing for this lab.
 * You'll need to modify this file.
 */

template <class K, class V>
V BST<K, V>::find(const K& key)
{
    auto find1 = find(root, key); 
    if (find1 != nullptr){
        return find1->value;
    }
    return V();
}

template <class K, class V>
typename BST<K, V>::Node* BST<K, V>::find(Node* & subtree, const K& key) // Adjusted to return a pointer
{
    if (!subtree || subtree->key == key) 
    {
        return subtree; 
    }
    if (key < subtree->key) 
    {
        return find(subtree->left, key); 
    }
    return find(subtree->right, key);
}

template <class K, class V>
void BST<K, V>::insert(const K & key, const V & value)
{
    // your code here
    insert(root, key, value);
}

template <class K, class V>
void BST<K, V>::insert(Node*& subtree, const K& key, const V& value)
{
    // your code here
    if (subtree == nullptr) 
{
    subtree = new Node(key, value);
} 
    else {
        if (key < subtree->key) {
            insert(subtree->left, key, value);
    } 
        else if (key > subtree->key) {
            insert(subtree->right, key, value);
    } 
}

}

template <class K, class V>
void BST<K, V>::swap(Node*& first, Node*& second)
{
    // your code here
    std::swap(first->key, second->key);
    std::swap(first->value, second->value);
}

template <class K, class V>
void BST<K, V>::remove(const K& key)
{
    // your code here
    remove(root, key);
}

template <class K, class V>
void BST<K, V>::remove(Node*& subtree, const K& key)
{
    // your code here
    if (subtree == nullptr)
    return;

if (key < subtree->key){
    remove(subtree->left, key);
}
else if (key > subtree->key){
    remove(subtree->right, key);
}
else {
    if (!subtree->left) {
        Node* toDelete = subtree;
        subtree = subtree->right;
        delete toDelete;
    }
    else if (!subtree->right){
        Node* toDelete = subtree;
        subtree = subtree->left;
        delete toDelete;
    }
    else {
        Node* successor = subtree->right;
        while (successor->left) 
        {
            successor = successor->left;
        }
        swap(subtree, successor);
        remove(subtree->right, key);
    }
}

}

template <class K, class V>
BST<K, V> listBuild(std::vector<std::pair<K, V>> inList){
    // your code here
    BST<K, V> tree;
    for(const auto& element : inList) {
        tree.insert(element.first, element.second);
}
    return tree;

}

template <class K, class V>
std::vector<int> allBuild(std::vector<std::pair<K, V>> inList){
    // your code here
    std::vector<int> outputList(inList.size(), 0);
    std::sort(begin(inList), end(inList));

    do {
        auto temporarytree = listBuild(inList);
        outputList[temporarytree.height()]++;
}   while (std::next_permutation(begin(inList), end(inList)));

    return outputList;

}