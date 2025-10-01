#include <iostream>
#include <algorithm>
#include <memory>
#include <queue>

template <typename T>
class AVLTree {
private:
    struct Node {
        T data;
        int height;
        std::unique_ptr<Node> left;
        std::unique_ptr<Node> right;

        explicit Node(const T& val) : data(val), height(1), left(nullptr), right(nullptr) {}
    };

    std::unique_ptr<Node> root;

    int height(const std::unique_ptr<Node>& node) const {
        return node ? node->height : 0;
    }

    int balanceFactor(const std::unique_ptr<Node>& node) const {
        return node ? height(node->left) - height(node->right) : 0;
    }

    void updateHeight(std::unique_ptr<Node>& node) {
        if (node) {
            node->height = 1 + std::max(height(node->left), height(node->right));
        }
    }

    std::unique_ptr<Node> rotateRight(std::unique_ptr<Node> y) {
        auto x = std::move(y->left);
        y->left = std::move(x->right);
        updateHeight(y);
        x->right = std::move(y);
        x->right = std::move(y);
        updateHeight(x);
        return x;
    }

    std::unique_ptr<Node> rotateLeft(std::unique_ptr<Node> x) {
        auto y = std::move(x->right);
        x->right = std::move(y->left);
        updateHeight(x);
        y->left = std::move(x);
        updateHeight(y);
        return y;
    }

    std::unique_ptr<Node> balance(std::unique_ptr<Node> node) {
        updateHeight(node);
        int bf = balanceFactor(node);

        if (bf > 1) {
            if (balanceFactor(node->left) < 0) {
                node->left = rotateLeft(std::move(node->left));
            }
            return rotateRight(std::move(node));
        }
        if (bf < -1) {
            if (balanceFactor(node->right) > 0) {
                node->right = rotateRight(std::move(node->right));
            }
            return rotateLeft(std::move(node));
        }
        return node;
    }

    std::unique_ptr<Node> insert(std::unique_ptr<Node> node, const T& val) {
        if (!node) {
            return std::make_unique<Node>(val);
        }
        if (val < node->data) {
            node->left = insert(std::move(node->left), val);
        } else if (val > node->data) {
            node->right = insert(std::move(node->right), val);
        } else {
            return node; // Duplicate values are not allowed
        }
        return balance(std::move(node));
    }

    void inorder(const std::unique_ptr<Node>& node) const {
        if (!node) return;
            inorder(node->left);
            std::cout << node->data << " ";
            inorder(node->right);
    }

    void levelOrder(const std::unique_ptr<Node>& node) const {
        if (!node) return;
        std::queue<const Node*> q;
        q.push(node.get());
        while (!q.empty()) {
            const Node* current = q.front();
            q.pop();
            std::cout << current->data << "(h:" << current->height << ") ";
            if (current->left) q.push(current->left.get());
            if (current->right) q.push(current->right.get());
        }
    }

public:
    AVLTree() : root(nullptr) {}

    void insert(const T& val) {
        root = insert(std::move(root), val);
    }

    void displayInorder() const {
        std::cout << "Inorder: ";
        inorder(root);
        std::cout << std::endl;
    }

    void displayLevelOrder() const {
        std::cout << "Level Order: ";
        levelOrder(root);
        std::cout << std::endl;
    }

    int getHeight() const {
        return height(root);
    }
};

int main() {
    AVLTree<int> tree;

    std::cout << "=== AVL Tree Demonstration ===" << std::endl << std::endl;

    std::cout << "Inserting values: 10, 20, 30, 40, 50, 25" << std::endl;
    for (int val : {10, 20, 30, 40, 50, 25}) {
        tree.insert(val);
    }

    std::cout << std::endl;
    tree.displayInorder();
    tree.displayLevelOrder();
    std::cout << "Tree Height: " << tree.getHeight() << std::endl;

    return 0;
}