struct Node {
    int data {0};
    Node* left {nullptr};
    Node* right {nullptr};
    int height {1};

    Node(int val) : data(val) {}
};

class ArvoreBase {
    protected:
    Node* root {nullptr};

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

    bool search(int value) {
        return search(value, root); 
    }

    virtual void insert(int value) = 0;
    virtual void remove(int value) = 0;
};