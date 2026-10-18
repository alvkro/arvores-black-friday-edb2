#ifndef ARVORE_AVL_HPP
#define ARVORE_AVL_HPP

#include "arvorebase.hpp"
#include <algorithm>

class AVL : public ArvoreBase {
private:
    int getHeight(Node* no) {
        return no == nullptr ? 0 : no->height;
    }

    void updateHeight(Node* no) {
        no->height = 1 + std::max(getHeight(no->left), getHeight(no->right));
    }

    int getBalanceFactor(Node* no) {
        if (no == nullptr) return 0;
        return getHeight(no->left) - getHeight(no->right);
    }

    Node* rotateRight(Node* y) {
        Node* x = y->left;
        y->left = x->right;
        x->right = y;
        updateHeight(y);
        updateHeight(x);
        return x;
    }

    Node* rotateLeft(Node* x) {
        Node* y = x->right;
        x->right = y->left;
        y->left = x;
        updateHeight(x);
        updateHeight(y);
        return y;
    }

    Node* rebalance(Node* no) {
        updateHeight(no);
        int balance = getBalanceFactor(no);

        // LL
        if (balance > 1 && getBalanceFactor(no->left) >= 0) {
            return rotateRight(no);
        }
        // LR
        if (balance > 1 && getBalanceFactor(no->left) < 0) {
            no->left = rotateLeft(no->left);
            return rotateRight(no);
        }
        // RR
        if (balance < -1 && getBalanceFactor(no->right) <= 0) {
            return rotateLeft(no);
        }
        // RL
        if (balance < -1 && getBalanceFactor(no->right) > 0) {
            no->right = rotateRight(no->right);
            return rotateLeft(no);
        }
        return no;
    }

    Node* insertAVL(int value, Node* no) {
        if (no == nullptr) return new Node(value);

        if (value < no->data) {
            no->left  = insertAVL(value, no->left);
        }
        else if (value > no->data) {
            no->right = insertAVL(value, no->right);
        }
        else {
            return no;
        }
        return rebalance(no);
    }

    Node* removeAVL(int value, Node* no) {
        if (no == nullptr) return nullptr;

        if (value < no->data) {
            no->left = removeAVL(value, no->left);
        } else if (value > no->data) {
            no->right = removeAVL(value, no->right);
        } else {
            // 0 ou 1 filho
            if (no->left == nullptr || no->right == nullptr) {
                Node* filho = no->left ? no->left : no->right;
                delete no;
                return filho;
            }
            // 2 filhos:
            Node* sucessor = findMin(no->right);
            no->data = sucessor->data;
            no->right = removeAVL(sucessor->data, no->right);
        }
        return rebalance(no);
    }
    public:
        AVL() : ArvoreBase(nullptr) {}
        virtual ~AVL() {}
        bool insert(int value) override { 
            if (search(value)) {
                return false;
            }
            root = insertAVL(value, root); 
            return true;
        }
        bool remove(int value) override { 
            if (!search(value)) {
                return false;
            }
            root = removeAVL(value, root);
            return true;
        }
};

#endif