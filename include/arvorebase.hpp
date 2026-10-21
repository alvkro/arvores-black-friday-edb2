#ifndef ARVOREBASE_HPP
#define ARVOREBASE_HPP

#include <algorithm>
#include <climits>
#include <iostream>
#include <string>
#include <vector>

struct Node {
    int data {0};
    Node* left {nullptr};
    Node* right {nullptr};
    bool red  {true}; // RB
    int height {1}; // AVL

    Node(int val) : data(val) {}
};


class ArvoreBase {
    protected:
    int total_rotations {0};
    int total_flips {0};      
    Node* root {nullptr};
    bool hasColor {false};    

    int calcHeight(Node* no) {
        if (no == nullptr) {
            return 0;
        }
        return 1 + std::max(calcHeight(no->left), calcHeight(no->right));
    }

    int calcSize(Node* no) {
        if (no == nullptr) {
            return 0;
        }
        return 1 + calcSize(no->left) + calcSize(no->right);
    }

    void emOrdemRecursivo(Node* no) {
        if (no != nullptr) {
            emOrdemRecursivo(no->left);
            std::cout << no->data << " ";
            emOrdemRecursivo(no->right);
        }
    }

    void coletar(Node* no, std::vector<int>& valores) {
        if (no == nullptr) {
            return;
        }
        coletar(no->left, valores);
        valores.push_back(no->data);
        coletar(no->right, valores);
    }

    void destroy(Node* no) {
        if (no == nullptr) {
            return;
        }
        destroy(no->left);
        destroy(no->right);
        delete no;
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
    ArvoreBase(const ArvoreBase&) = delete;
    ArvoreBase& operator=(const ArvoreBase&) = delete;
    virtual ~ArvoreBase() {
        destroy(root);
    }

void printVisual(Node* no, const std::string& prefixo, bool ultimo) {
    std::cout << prefixo << (ultimo ? "└── " : "├── ");
    if (no == nullptr) {
        std::cout << "·\n";
        return;
    }
    printNo(no);

    if (no->left == nullptr and no->right == nullptr) {
        return;
    }
    std::string novoPrefixo = prefixo + (ultimo ? "    " : "│   ");
    printVisual(no->left, novoPrefixo, false);
    printVisual(no->right, novoPrefixo, true);
}

void printNo(Node* no) {
    std::cout << no->data;
    if (hasColor) {
        std::cout << (no->red ? "(R)" : "(B)");
    } else {
        std::cout << "(h=" << no->height << ")";
    }
    std::cout << "\n";
}

void printTree() {
    if (root == nullptr) {
        std::cout << "(arvore vazia)\n";
    } else {
        printNo(root);
        if (root->left != nullptr or root->right != nullptr) {
            printVisual(root->left, "", false);
            printVisual(root->right, "", true);
        }
    }
    std::cout << "------------------------\n";
}

    void printInOrder() {
        emOrdemRecursivo(root);
        std::cout << std::endl;
    }

    bool search(int value) {
        return search(value, root);
    }

    int height() {
        return calcHeight(root);
    }

    int size() {
        return calcSize(root);
    }

    std::vector<int> toVector() {
        std::vector<int> valores;
        coletar(root, valores);
        return valores;
    }

    int getTotalRotacoes() {
        return total_rotations;
    }

    int getTotalFlips() {
        return total_flips;
    }

    void resetContadores() {
        total_rotations = 0;
        total_flips = 0;
    }

    bool usaCor() {
        return hasColor;
    }

    virtual bool insert(int value) = 0;
    virtual bool remove(int value) = 0;
};

#endif