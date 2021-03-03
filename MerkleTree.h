#ifndef MERKLETREE_H
#define MERKLETREE_H

#include <hash-library/sha256.h>
#include <memory>
#include <string>
#include <vector>

// Implementation of Merkle Tree data structure with SHA-256 hash function.
class merkle_tree{
public:

    // Node structure.
    struct node{
        std::unique_ptr<node> left = nullptr;
        std::unique_ptr<node> right = nullptr;
        std::string hash;

        node() = default;
        node(std::unique_ptr<node> new_left, std::unique_ptr<node> new_right)
            : left(std::move(new_left)), right(std::move(new_right)){}
    };

private:
    // Hash function
    static SHA256 hash_function;

    // Root node of a tree.
    std::unique_ptr<node> root;

    // Number of elements in a tree.
    size_t size;

    // Just alias for serialized data.
    using data_container = std::vector<std::pair<void*, size_t>>;

    // Creates recursively nodes for a tree.
    // Takes serialized data.
    // Returns created node.
    std::unique_ptr<node> create_node(data_container&& data);

    // Recursively traverse a tree and put hashes from nodes into array.
    void traverse_nodes(std::vector<std::string>& hashes, node* traverse_root) const;

    // Recursively traverse a tree and copy data from a new tree.
    void traverse_trees_and_copy(node* old_root, const node* const new_root);

public:
    merkle_tree();
    merkle_tree(const merkle_tree&);
    merkle_tree(merkle_tree&&);
    ~merkle_tree() = default;

    // Creates a Merkle tree with serialized data and sizes in bytes of serialized data.
    // We need here serialized data, because we want contain all types of data in a tree.
    void create_merkle_tree(data_container&& data);

    // Returns all hashes that tree contains in preorder way.
    void get_all_hashes_preorder(std::vector<std::string>& hashes) const;

    // Clears all tree.
    void clear();

    // Returns root node.
    inline const node* const get_root() const { return root.get(); }

    // Returns root hash.
    inline std::string get_root_hash() const { return root ? root->hash : "" ;}

    //Returns number of elements in a tree.
    inline size_t get_size() const { return size; }

    // Compares hashes in both trees.
    // Returns true if hashes are equal, otherwise returns false.
    inline bool operator == (const merkle_tree& object) const {
        return root && object.root ? root.get()->hash == object.root.get()->hash : false;
    }

    // Compares hashes in both trees.
    // Returns true if hashes aren't equal, otherwise returns false.
    inline bool operator != (const merkle_tree& object) const {
        return root && object.root ? root.get()->hash != object.root.get()->hash : false;
    }

    merkle_tree& operator = (const merkle_tree&);
    merkle_tree& operator = (merkle_tree&&);
};

#endif // MERKLETREE_H
