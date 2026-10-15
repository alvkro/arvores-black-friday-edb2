struct Node {
    int data {0};
    Node* left {nullptr};
    Node* right {nullptr};

    Node(int val) : data(val) {}
};

bool search(int value, Node* raiz) {
    if (raiz == nullptr) {
        return false; // caso base
    } else if (raiz->data == value) {
        return true;
    }

    if (value < raiz->data) {
        search(value, raiz->left);
    } else if (value > raiz->data) {
        search(value, raiz->right);
    }
    return false;
}

Node* insert(int value, Node* raiz) {
    if (raiz == nullptr) {
        return new Node(value); // caso base
    }

    if (value < raiz->data) {
        raiz->left = insert(value, raiz->left);
    } else if (value > raiz->data) {
        raiz->right = insert(value, raiz->right);
    }

    return raiz;
}

Node* remove(int value, Node* raiz) {
    if (raiz == nullptr) {
        return nullptr;
    }

    if (value < raiz->data) {
        raiz->left = remove(value, raiz->left);
    } else if (value > raiz->data) {
        raiz->right = remove(value, raiz->right);
    } else {

        // Caso 1: zero filhos
        if (raiz->left == nullptr and raiz->right == nullptr) {
            delete raiz;
            return nullptr;
        }

        // Caso 2: um filho
        if (raiz->left != nullptr) {
            Node* temp_node = raiz->left;
            delete raiz;
            return temp_node;
        } else if (raiz->right != nullptr) {
            Node* temp_node = raiz->right;
            delete raiz;
            return temp_node;
        }

        // Caso 3: dois filhos
    } 
}