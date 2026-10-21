#ifndef ARVORE_RB_HPP
#define ARVORE_RB_HPP

#include "arvorebase.hpp"
class RB : public ArvoreBase {
    private:
    bool isRed(Node* no) {
        if (no == nullptr) {
            return false;
        }
        return no->red;
    }

    Node* rotateRight(Node* y) {
        if (y == nullptr) {
            return nullptr;
        }
        auto newNo = y->left;
        y->left = newNo->right;
        newNo->right = y;
        newNo->red = y->red;
        y->red = true;  

        this->total_rotations++;
        return newNo;
    }

    Node* rotateLeft(Node* x) {
        if (x == nullptr) {
            return nullptr;
        }
        auto newNo = x->right;
        x->right = newNo->left;
        newNo->left = x;
        newNo->red = x->red;
        x->red = true;

        this->total_rotations++;
        return newNo;
    }

    void flipColors(Node* no) {
        no->red = !no->red;
        no->left->red = !no->left->red;
        no->right->red = !no->right->red;
        this->total_flips++;
    }

    Node* rebalance(Node* no) {
        if (isRed(no->right) and !isRed(no->left)) {
            no = rotateLeft(no);
        }

        if (isRed(no->left) and isRed(no->left->left)) {
            no = rotateRight(no);
        }

        if (isRed(no->left) and isRed(no->right)) {
            flipColors(no);
        }

        return no;
    }

    Node* moveRedLeft(Node* no) {
        flipColors(no);

        if (isRed(no->right->left)) {
            no->right = rotateRight(no->right);
            no = rotateLeft(no);
            flipColors(no);
        }
        return no;
    }

    Node* moveRedRight(Node* no) {
        flipColors(no);

        if (isRed(no->left->left)) {
            no = rotateRight(no);
            flipColors(no);
        }
        return no;
    }

    Node* insertRB(int value, Node* no) {
        if (no == nullptr) {
            return new Node(value);
        }

        if (value < no->data) {
            no->left = insertRB(value, no->left);
        } else if (value > no->data) {
            no->right = insertRB(value, no->right);
        } else {
            return no;
        }

        return rebalance(no);
    }

    Node* removeMin(Node* no) {
        if (no->left == nullptr) {
            delete no;
            return nullptr;
        }

        if (!isRed(no->left) and !isRed(no->left->left)) {
            no = moveRedLeft(no);
        }

        no->left = removeMin(no->left);

        return rebalance(no);
    }

    Node* removeRB(int value, Node* no) {
        if (value < no->data) {
            if (!isRed(no->left) and !isRed(no->left->left)) {
                no = moveRedLeft(no);
            }
            no->left = removeRB(value, no->left);
        } else {
            if (isRed(no->left)) {
                no = rotateRight(no);
            }

            if (value == no->data and no->right == nullptr) {
                delete no;
                return nullptr;
            }

            if (!isRed(no->right) and !isRed(no->right->left)) {
                no = moveRedRight(no);
            }

            if (value == no->data) {
                no->data = findMin(no->right)->data;
                no->right = removeMin(no->right);
            } else {
                no->right = removeRB(value, no->right);
            }
        }

        return rebalance(no);
    }


    public:
    RB() : ArvoreBase(nullptr) { hasColor = true; }

    bool insert(int value) override {
        if (search(value)) {
            return false;
        }

        root = insertRB(value, root);
        root->red = false;

        return true;
    }

    bool remove(int value) override {
        if (!search(value, root)) {
            return false;
        }

        if (!isRed(root->left) and !isRed(root->right)) {
            root->red = true;
        }

        root = removeRB(value, root);

        if (root != nullptr) {
            root->red = false;
        }

        return true;
    }
};

#endif