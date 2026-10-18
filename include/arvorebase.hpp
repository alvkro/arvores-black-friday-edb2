#ifndef ARVOREBASE_HPP
#define ARVOREBASE_HPP

#include <iostream>

struct Node {
    int data {0};
    Node* left {nullptr};
    Node* right {nullptr};
    bool red  {true};
    int height {1};

    Node(int val) : data(val) {}
};

class ArvoreBase {
    protected:
    Node* root {nullptr};
    bool hasColor {false};

    void emOrdemRecursivo(Node* no) {
        if (no != nullptr) {
            emOrdemRecursivo(no->left);
            std::cout << no->data << " ";
            emOrdemRecursivo(no->right);
        }
    }

    Node* findMin(Node* no) {
        while (no != nullptr && no->left != nullptr) {
            no = no->left;
        }
        return no;
    }

    bool search(int value, Node* no) {
        if (no == nullptr) {
            return false; // caso base
        } else if (no->data == value) {
            return true;
        }

        if (value < no->data) {
            return search(value, no->left);
        } else if (value > no->data) {
            return search(value, no->right);
        }
        return false;
    }

    public:
    ArvoreBase(Node* m_root) : root(m_root) {}
    virtual ~ArvoreBase() {}

    void printVisual(Node* no, int espaco) {
        if (no == nullptr) return;

        espaco += 5;
        printVisual(no->right, espaco);

        for (int i = 5; i < espaco; i++) std::cout << " ";
        std::cout << no->data;
        if (hasColor) {
            if (no->red) std::cout << "(R)";
            else std::cout << "(B)";
        }
        std::cout << "\n";

        printVisual(no->left, espaco);
    }

    void printTree() {
        printVisual(root, 0);
        std::cout << "------------------------\n";
    }

    bool search(int value) {
        return search(value, root); 
    }

    void printInOrder() {
        emOrdemRecursivo(root);
        std::cout << std::endl;
    }

    virtual bool insert(int value) = 0;
    virtual bool remove(int value) = 0;
};

#endif