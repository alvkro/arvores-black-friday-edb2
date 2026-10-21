#ifndef ARVORE_AVL_HPP
#define ARVORE_AVL_HPP

#include "arvorebase.hpp"

class AVL : public ArvoreBase {
private:
    /// Altura guardada no no (0 para nullptr)
    int getHeight(Node* no) {
        return no == nullptr ? 0 : no->height;
    }

    void updateHeight(Node* no) {
        no->height = 1 + std::max(getHeight(no->left), getHeight(no->right));
    }

    /// Fator de balanceamento = altura(esq) - altura(dir); valido: -1, 0 ou 1
    int getBalanceFactor(Node* no) {
        if (no == nullptr) return 0;
        return getHeight(no->left) - getHeight(no->right);
    }

    Node* rotateRight(Node* y) {
        Node* x = y->left;
        y->left = x->right;
        x->right = y;
        // primeiro o no que desceu (y), depois o que subiu (x)
        updateHeight(y);
        updateHeight(x);
        this->total_rotations++;
        return x;
    }

    Node* rotateLeft(Node* x) {
        Node* y = x->right;
        x->right = y->left;
        y->left = x;
        updateHeight(x);
        updateHeight(y);
        this->total_rotations++;
        return y;
    }

    /// Atualiza a altura e corrige o desbalanceamento (se houver) no no atual
    Node* rebalance(Node* no) {
        updateHeight(no);
        int balance = getBalanceFactor(no);

        // LL: rotacao simples a direita
        if (balance > 1 && getBalanceFactor(no->left) >= 0) {
            return rotateRight(no);
        }
        // LR: rotacao dupla (esquerda no filho, direita no no)
        if (balance > 1 && getBalanceFactor(no->left) < 0) {
            no->left = rotateLeft(no->left);
            return rotateRight(no);
        }
        // RR: rotacao simples a esquerda
        if (balance < -1 && getBalanceFactor(no->right) <= 0) {
            return rotateLeft(no);
        }
        // RL: rotacao dupla (direita no filho, esquerda no no)
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
        // rebalanceia na volta da recursao, do no mais fundo para a raiz
        return rebalance(no);
    }

    Node* removeAVL(int value, Node* no) {
        if (no == nullptr) return nullptr;

        if (value < no->data) {
            no->left = removeAVL(value, no->left);
        } else if (value > no->data) {
            no->right = removeAVL(value, no->right);
        } else {
            // 0 ou 1 filho: o filho (ou nullptr) ocupa o lugar do no
            if (no->left == nullptr || no->right == nullptr) {
                Node* filho = no->left ? no->left : no->right;
                delete no;
                return filho;
            }
            // 2 filhos: copia o sucessor em ordem e o remove da subarvore direita
            Node* sucessor = findMin(no->right);
            no->data = sucessor->data;
            no->right = removeAVL(sucessor->data, no->right);
        }
        return rebalance(no);
    }

public:
    AVL() : ArvoreBase(nullptr) {}

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