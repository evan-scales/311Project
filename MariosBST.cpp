// Written by Mario Misencik
#include <iostream>
#include <string>
#include <pthread.h>
#include "./EvansMap.cpp"
using namespace std;

class MariosBST : public ObjectInterface {
    private:
        struct node {
            int key; // Integer value of node
            string value; // String value of node
            node *left, *right; // Children of node

            node(int key, string value) {
                this->key = key;
                this->value = value;
                this->left = this->right = NULL;
            }
        };

        pthread_mutex_t mutex; // Mutex for the tree
        node *root; // The root of the tree

        // Finds the node with the minimum value in the node's subtree.
        node *findMinimum(node *n) {
            if (!n) return NULL;
            while (n->left) n = n->left;
            return n;
        }

        // Recursively removes a node from the tree with the corresponding key.
        node *removeNode(node *n, int key) {
            if (!n) return NULL;
            else if (key < n->key) n->left = removeNode(n->left,key);
            else if (key > n->key) n->right = removeNode(n->right,key);
            else {
                if (!n->left) return n->right;
                else if (!n->right) return n->left;
                else {
                    node *temp = findMinimum(n->right);
                    n->key = temp->key;
                    n->value = temp->value;
                    n->right = removeNode(n->right,temp->key);
                }
            }
            return n;
        }

        // Searches in the tree for the key and determines whether it exists in it.
        bool containsKey(int key) {
            node *current = root;
            while (current) {
                if (key < current->key) current = current->left;
                else if (key > current->key) current = current->right;
                else return true;
            }
            return false;
        }

        // Recursively deletes all nodes in the tree.
        void clearNodes(node *n) {
            if (!n) return;
            clearNodes(n->left);
            clearNodes(n->right);
            delete n;
        }

    public:
        MariosBST() {
            root = NULL;
            pthread_mutex_init(&mutex, NULL);
        }

        ~MariosBST() {
            clear();
            pthread_mutex_destroy(&mutex);
        }

        // Inserts a node.
        bool insert(int key, string value) {
            node *newNode = new node(key,value);
            if (!root) {
                root = newNode;
                return true;
            }
            node *current = root;
            while (current) {
                if (key < current->key) {
                    if (!current->left) {
                        current->left = newNode;
                        return true;
                    }
                    current = current->left;
                } else if (key > current->key) {
                    if (!current->right) {
                        current->right = newNode;
                        return true;
                    }
                    current = current->right;
                } else return false;
            }
            return false;
        }

        // Removes a node if it is found in the tree.
        bool remove(int key) {
            if (!containsKey(key)) return false;
            removeNode(root,key);
            return true;
        }

        // Retrives the string value of a node based on its key.
        string get(int key) {
            node *current = root;
            while (current) {
                if (key < current->key) current = current->left;
                else if (key > current->key) current = current->right;
                else return current->value;
            }
            return "No " + to_string(key);
        }

        // Clears all the nodes, including the root.
        void clear() {
            if (root) {
                clearNodes(root);
                root = NULL;
            }
        }

        string runOp(struct opsStruct *op) { 
            pthread_mutex_lock(&mutex);
            switch (op->op) {
                case 'I':
                    if (insert(op->key, op->value)) {
                        pthread_mutex_unlock(&mutex);
                        return "OK";
                    } else {
                        pthread_mutex_unlock(&mutex);
                        return "Fail";
                    }
                case 'L':
                    pthread_mutex_unlock(&mutex);
                    return get(op->key);
                case 'D':
                    if (remove(op->key)) {
                        pthread_mutex_unlock(&mutex);
                        return "OK";
                    } else {
                        pthread_mutex_unlock(&mutex);
                        return "Fail";
                    }
                default:
                    pthread_mutex_unlock(&mutex);
                    return "Fail";
            }
        }

        void print() {}

};