#include "MerkleTree.h"
#include <iostream>
#include <future>

using namespace std;

SHA256 merkle_tree::hash_function;

merkle_tree::merkle_tree() : root(nullptr), size(0){}

merkle_tree::merkle_tree(const merkle_tree& object) : size(object.size){
    root.release();
    traverse_trees_and_copy(root.get(), object.root.get());
}

merkle_tree::merkle_tree(merkle_tree&& object) : root(move(object.root)), size(move(object.size)){}

merkle_tree& merkle_tree::operator = (const merkle_tree& object){
    if(this != &object){
        root.release();
        traverse_trees_and_copy(root.get(), object.root.get());
    }
    return *this;
}

merkle_tree& merkle_tree::operator = (merkle_tree&& object){
    if(this != &object){
        root = move(object.root);
        size = move(object.size);
    }
    return *this;
}

void merkle_tree::traverse_trees_and_copy(node* old_root, const node* const new_root){
    old_root->hash = new_root->hash;

    if(new_root->left.get())
    {
        old_root->left = make_unique<node>();
        traverse_trees_and_copy(old_root->left.get(), new_root->left.get());
    }

    if(new_root->right.get())
    {
        old_root->right = make_unique<node>();
        traverse_trees_and_copy(old_root->right.get(), new_root->right.get());
    }
}

void merkle_tree::traverse_nodes(std::vector<std::string>& hashes, node* traverse_root) const{

    hashes.push_back(traverse_root->hash);

    if(traverse_root->left) traverse_nodes(hashes, traverse_root->left.get());
    if(traverse_root->right) traverse_nodes(hashes, traverse_root->right.get());
}

void merkle_tree::get_all_hashes_preorder(std::vector<std::string>& hashes) const{
    traverse_nodes(hashes, root.get());
}

void merkle_tree::clear(){
    root.release();
    size = 0;
}

unique_ptr<merkle_tree::node> merkle_tree::create_node(data_container&& data){
    unique_ptr<node> new_node = nullptr;

    if(data.size() <= 1)
    {
        new_node = make_unique<node>();
        const auto& element = *data.begin();
        new_node->hash = hash_function(element.first, element.second);
    }else{
        size_t half = data.size() / 2;
        new_node = make_unique<node>(create_node(data_container(data.begin(), data.begin() + half)),
                                     create_node(data_container(data.begin() + half, data.end())));
        new_node->hash = hash_function(new_node->left->hash + new_node->right->hash);
    }

    size++;

    return new_node;
}

void merkle_tree::create_merkle_tree(data_container data){
    root = create_node(move(data));
}


