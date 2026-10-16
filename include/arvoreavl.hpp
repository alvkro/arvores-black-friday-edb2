#include "arvorebase.hpp"
#include <algorithm>

class AVL : public ArvoreBase {
    private:
    /// @brief Getter do height 
    /// @param no 
    /// @return height 
    int getHeight(Node* no) {
        if (no == nullptr) {
            return 0;
        }
        return no->height;
    }

    /// @brief  Acha a altura de uma sub-árvore
    /// @param no 
    /// @return altura (height) da sub-árvore
    int findHeight(Node* no) {
        if (no == nullptr) {
            return 0;
        }
        auto left = findHeight(no->left);
        auto right = findHeight(no->right);
        return 1 + std::max(left, right);
    }

    int getBalanceFactor(Node* no) {
        if (no == nullptr) {
            return 0;
        }

        auto h_left = findHeight(no->left);
        auto h_right = findHeight(no->right);

        return h_left - h_right;
    }

    Node* rotateRight(Node* y) {
        if (y == nullptr) {
            return nullptr;
        }
        auto newNo = y->left;
        y->left = newNo->right;
        newNo->right = y;

        return newNo;
    }

    Node* rotateLeft(Node* x) {
        if (x == nullptr) {
            return nullptr;
        }
        auto newNo = x->right;
        x->right = newNo->left;
        newNo->left = x;

        return newNo;
    }

    Node* insertAVL(int value, Node* no) {
        if (no == nullptr) {
            return new Node(value); // caso base
        }

        if (value < no->data) {
            no->left = insert(value, no->left);
        } else if (value > no->data) {
            no->right = insert(value, no->right);
        }
        
        no->height = 1 + std::max(getHeight(no->left), getHeight(no->right));

        int balance = getBalanceFactor(no);

        // Left Left Case (LL)
        if (balance > 1 && value < no->left->data) {
            return rotateRight(no);
        }
        // Right Right Case (RR)
        if (balance < -1 && value > no->right->data) {
            return rotateLeft(no);
        }
        // Left Right Case (LR)
        if (balance > 1 && value > no->left->data) {
            no->left = rotateLeft(no->left);
            return rotateRight(no);
        }
        // Right Left Case (RL)
        if (balance < -1 && value < no->right->data) {
            no->right = rotateRight(no->right);
            return rotateLeft(no);
        }

        return no;
    }

    Node* removeAVL(int value, Node* no) {
            if (no == nullptr) {
                return nullptr;
            }
        
            if (value < no->data) {
                no->left = remove(value, no->left);
            } else if (value > no->data) {
                no->right = remove(value, no->right);
            } else {
            
            // Caso 1: zero filhos
            if (no->left == nullptr and no->right == nullptr) {
                delete no;
                return nullptr;
            }
        
            // Caso 2: um filho
            if (no->left != nullptr) {
                Node* temp_node = no->left;
                delete no;
                return temp_node;
            } else if (no->right != nullptr) {
                Node* temp_node = no->right;
                delete no;
                return temp_node;
            }
        
            // Caso 3: dois filhos
            Node* sucessor = no->right;
            while (sucessor->left != nullptr) {
                sucessor = sucessor->left;
            }
            no->data = sucessor->data;
            no->right = removeAVL(sucessor->data, no->right);
        
            no->height = 1 + std::max(getHeight(no->left), getHeight(no->right));
        
            int balance = getBalanceFactor(no);
        
            // Caso LL
            if (balance > 1 && getBalanceFactor(no->left) >= 0) {
                return rotateRight(no);
            }
        
            // Caso LR
            if (balance > 1 && getBalanceFactor(no->left) < 0) {
                no->left = rotateLeft(no->left);
                return rotateRight(no);
            }
        
            // Caso RR
            if (balance < -1 && getBalanceFactor(no->right) <= 0) {
                return rotateLeft(no);
            }
        
            // Caso RL
            if (balance < -1 && getBalanceFactor(no->right) > 0) {
                no->right = rotateRight(no->right);
                return rotateLeft(no);
            }
        
            return no;
        }
    }
};  