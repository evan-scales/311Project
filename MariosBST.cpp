// Written by Mario Misencik
#include <iostream>
#include <string>
#include <pthread.h>
// #include "./ObjectInterface.cpp"
// #include "./opsStruct.cpp"
#include "./EvansMap.cpp"
using namespace std;

class MariosBST : public ObjectInterface {
    private:
        struct node {
            // pthread_mutex_t mutex; // Mutex for the node
            int key; // Integer value of node
            string value; // String value of node
            node *left, *right; // Children of node

            node(int key, string value) {
                this->key = key;
                this->value = value;
                this->left = this->right = nullptr;
            }
        };

        pthread_mutex_t mutex; // Mutex for the tree
        node *root; // The root of the tree
        // bool opComplete; // Check to see if an operation is completed

        // Create a new node to insert into the tree
        // node *makeNode(int key, string value) {
        //     node *n = new node();
        //     // pthread_mutex_init(&n->mutex, NULL);
        //     n->key = key;
        //     n->value = value;
        //     n->left = n->right = nullptr;
        //     return n;
        // }

        // Returns the height of the overall subtree
        // int height(node *parent, node *child) {
        //     int lh, rh;
        //     if (child == NULL) {
        //         pthread_mutex_unlock(&parent->mutex);
        //         return 0;
        //     }
        //     pthread_mutex_lock(&child->mutex);
        //     pthread_mutex_unlock(&parent->mutex);
        //     lh = height(child, child->left);
        //     pthread_mutex_lock(&child->mutex);
        //     rh = height(child, child->right);
        //     return 1 + (max(lh, rh));
        // }

        // Checks to see if the tree is balanced; that is, its left and right subtree heights have a difference of -1, 0 or 1
        // bool isBalanced(node *parent, node *child) {
        //     int lh, rh;
        //     bool lb, rb;
        //     if (child == root) pthread_mutex_lock(&mutex);
        //     if (root == NULL) {
        //         pthread_mutex_unlock(&mutex);
        //         return true;
        //     }
        //     if (child == NULL) {
        //         pthread_mutex_unlock(&parent->mutex);
        //         return true;
        //     }
        //     pthread_mutex_lock(&child->mutex);
        //     if (parent) pthread_mutex_unlock(&parent->mutex);
        //     else pthread_mutex_unlock(&mutex);
        //     lh = height(child, child->left);
        //     pthread_mutex_lock(&child->mutex);
        //     rh = height(child, child->left);
        //     pthread_mutex_lock(&child->mutex);
        //     lb = isBalanced(child, child->left);
        //     pthread_mutex_lock(&child->mutex);
        //     rb = isBalanced(child, child->right);
        //     if (abs(lh - rh) <= 1 && lb && rb) return true;
        //     else return false;
        // }

        // Finds the node in the tree that needs to be rotated
        // node *findCulprit (node *parent, node *child) {
        //     int lh, rh;
        //     node *lc, *rc;
        //     if (child == root) pthread_mutex_lock(&mutex);
        //     if (root == NULL) {
        //         pthread_mutex_unlock(&mutex);
        //         return NULL;
        //     }
        //     if (child == NULL) {
        //         pthread_mutex_unlock(&parent->mutex);
        //         return NULL;
        //     }
        //     pthread_mutex_lock(&child->mutex);
        //     if (parent) pthread_mutex_unlock(&parent->mutex);
        //     else pthread_mutex_unlock(&mutex);
        //     lh = height(child, child->left);
        //     pthread_mutex_lock(&child->mutex);
        //     rh = height(child, child->left);
        //     pthread_mutex_lock(&child->mutex);
        //     lc = findCulprit(child, child->left);
        //     pthread_mutex_lock(&child->mutex);
        //     rc = findCulprit(child, child->right);
        //     if (abs(lh - rh) > 1) return child;
        //     else if (lc && !rc) return lc;
        //     else if (!lc && rc) return rc;
        //     else return NULL;
        // }

        // node *leftRotation(node *n) {
        //     node *cul, *ret;
        //     cul = n;
        //     ret = cul->right;
        //     cul->right = ret->left;
        //     ret->left = cul;
        //     pthread_mutex_unlock(&n->mutex);
        //     return ret;
        // }

        // node *rightRotation(node *n) {
        //     node *cul, *ret;
        //     cul = n;
        //     ret = cul->left;
        //     cul->left = ret->right;
        //     ret->right = cul;
        //     pthread_mutex_unlock(&n->mutex);
        //     return ret;
        // }

        // node *leftRightRotation(node *n) {
        //     node *cul, *temp, *ret;
        //     cul = n;
        //     temp = n->left;
        //     ret = n->left->right;
        //     cul->left = ret->right;
        //     temp->right = ret->left;
        //     ret->right = cul;
        //     ret->left = temp;
        //     pthread_mutex_unlock(&n->mutex);
        //     return ret;
        // }

        // node *rightLeftRotation(node *n) {
        //     node *cul, *temp, *ret;
        //     cul = n;
        //     temp = n->right;
        //     ret = n->right->left;
        //     cul->right = ret->left;
        //     temp->left = ret->right;
        //     ret->left = cul;
        //     ret->right = temp;
        //     pthread_mutex_unlock(&n->mutex);
        //     return ret;
        // }

        // Balances the tree. Based on various subheights, a different rotation are performed on the culprit.
        // node *balanceTree(node *culprit) {
        //     int culLH = height(culprit, culprit->left);
        //     int culRH = height(culprit, culprit->right);
        //     int lcLH = height(culprit->left, culprit->left->left);
        //     int lcRH = height(culprit->left, culprit->left->right);
        //     int rcLH = height(culprit->right, culprit->right->left);
        //     int rcRH = height(culprit->right, culprit->right->right);
        //     pthread_mutex_lock(&culprit->mutex);
        //     if (culLH - culRH == -2 && rcLH - rcRH == -1) return leftRotation(culprit);
        //     else if (culLH - culRH == 2 && lcLH - lcRH == 1) return rightRotation(culprit);
        //     else if (culLH - culRH == 2 && lcLH - lcRH == -1) return leftRightRotation(culprit);
        //     else if (culLH - culRH == -2 && rcLH - rcRH == 1) return rightLeftRotation(culprit);
        //     else return culprit;
        // }

        // Because removing the root is a special operation, it has its own method.
        // bool removeRoot() {
        //     if (root->left == NULL && root->right == NULL) {
        //         // pthread_mutex_unlock(&root->mutex);
        //         root = NULL;
        //         pthread_mutex_unlock(&mutex);
        //         return true;
        //     } else if (root->left == NULL) {
        //         root = move(root->right);
        //         pthread_mutex_unlock(&mutex);
        //         return true;
        //     } else if (root->right == NULL) {
        //         root = move(root->left);
        //         pthread_mutex_unlock(&mutex);
        //         return true;
        //     } else {
        //         node *minPrevious = root;
        //         node *minimum = root->right;
        //         // pthread_mutex_lock(&minimum->mutex);
        //         pthread_mutex_unlock(&mutex);
        //         while (true) {
        //             if (minimum->left == NULL) {
        //                 root->key = minimum->key;
        //                 root->value = minimum->value;
        //                 if (minPrevious == root) {
        //                     if (minimum->right == NULL) minPrevious->right = NULL;
        //                     else minPrevious->right = move(minimum->right);
        //                 } else {
        //                     if (minimum->right == NULL) minPrevious->left = NULL;
        //                     else minPrevious->left = move(minimum->right);
        //                     // pthread_mutex_unlock(&minPrevious->mutex);
        //                 }
        //                 // pthread_mutex_unlock(&root->mutex);
        //                 // pthread_mutex_unlock(&mutex);
        //                 return true;
        //             }
        //             // pthread_mutex_lock(&minimum->left->mutex);
        //             // if (minPrevious != root) {
        //             //     pthread_mutex_unlock(&minPrevious->mutex);
        //             // }
        //             minPrevious = minimum;
        //             minimum = minimum->left;
        //         }
        //     }
        //     // if (isBalanced(NULL, root)) return true;
        //     // else {
        //     //     node *culprit = findCulprit(NULL, root);
        //     //     if (culprit == root) root = balanceTree(root);
        //     //     else culprit = balanceTree(culprit);
        //     //     return true;
        //     // }
        // }

        // void insertNode(node *n, int key, string value) {
        //     if (n == NULL) {
        //         opComplete = true;
        //         pthread_mutex_unlock(&mutex);
        //         n = makeNode(key,value);
        //     }
        //     else if (key < n->key) n->left = insertNode(n->left,key,value);
        //     else if (key > n->key) n->right = insertNode(n->right,key,value);
        //     else {
        //         pthread_mutex_unlock(&mutex);
        //         return n;
        //     }
        // }

        // node *findMinimum(node *n) {
        //     if (n == NULL) return NULL;
        //     else if (n->left == NULL) return n;
        //     else return findMinimum(n->left);
        // }

        // node *removeNode(node *n, int key) {
        //     if (n == NULL) {
        //         pthread_mutex_unlock(&mutex);
        //         return n;
        //     }
        //     else if (key < n->key) n->left = removeNode(n->left,key);
        //     else if (key > n->key) n->right = removeNode(n->right,key);
        //     else {
        //         if (n->left == NULL) {
        //             opComplete = true;
        //             pthread_mutex_unlock(&mutex);
        //             return n->right;
        //         } else if (n->right == NULL) {
        //             opComplete = true;
        //             pthread_mutex_unlock(&mutex);
        //             return n->left;
        //         } else {
        //             node *temp = findMinimum(n->right);
        //             n->key = temp->key;
        //             n->value = temp->value;
        //             n->right = removeNode(n->right,temp->key);
        //         }
        //     }
        //     pthread_mutex_unlock(&mutex);
        //     return n;
        // }

        // string getNode(node *n, int key) {
        //     if (n == NULL) {
        //         pthread_mutex_unlock(&mutex);
        //         return "No " + to_string(key);
        //     }
        //     else if (key < n->key) return getNode(n->left,key);
        //     else if (key > n->key) return getNode(n->right,key);
        //     else {
        //         pthread_mutex_unlock(&mutex);
        //         return n->value;
        //     }
        // }

        void clearNodes(node *n) {
            if (n == nullptr) return;
            clearNodes(n->left);
            clearNodes(n->right);
            // pthread_mutex_destroy(&n->mutex);
            n = nullptr;
        }

    public:
        MariosBST() {
            pthread_mutex_init(&mutex, nullptr);
        }

        ~MariosBST() {
            pthread_mutex_destroy(&mutex);
        }

        // Inserts a node.
        bool insert(int key, string value) {
            pthread_mutex_lock(&mutex);
            if (root == nullptr) {
                root = new node(key,value);
                pthread_mutex_unlock(&mutex);
                return true;
            }
            // pthread_mutex_lock(&root->mutex);
            // pthread_mutex_unlock(&mutex);
            node *current = root;
            node *previous = nullptr;
            // while (true) {
            //     if (key < current->key) {
            //         if (current->left == nullptr) {
            //             current->left = new node(key,value);
            //             // pthread_mutex_unlock(&current->mutex);
            //             pthread_mutex_unlock(&mutex);
            //             return true;
            //         } else next = current->left;
            //     } else if (key > current->key) {
            //         if (current->right == nullptr) {
            //             current->right = new node(key,value);
            //             // pthread_mutex_unlock(&current->mutex);
            //             pthread_mutex_unlock(&mutex);
            //             return true;
            //         } else next = current->right;
            //     } else {
            //         // pthread_mutex_unlock(&current->mutex);
            //         pthread_mutex_unlock(&mutex);
            //         return false;
            //     }
            //     // pthread_mutex_lock(&next->mutex);
            //     // pthread_mutex_unlock(&current->mutex);
            //     current = next;
            // }
            // if (isBalanced(NULL, root)) return true;
            // else {
            //     node *culprit = findCulprit(NULL, root);
            //     if (culprit = root) root = balanceTree(root);
            //     else culprit = balanceTree(culprit);
            //     return true;
            // }
            // if (root) insertNode(root,key,value);
            // else {
            //     root = makeNode(key,value);
            //     return true;
            // }
            while (current) {
                if (key < current->key) {
                    previous = current;
                    current = current->left;
                } else if (key > current->key) {
                    previous = current;
                    current = current->right;
                } else {
                    pthread_mutex_unlock(&mutex);
                    return false;
                }
            }
            if (key < previous->key) previous->left = new node(key,value);
            else previous->right = new node(key,value);
            pthread_mutex_unlock(&mutex);
            return true;
        }

        // Removes a node.
        bool remove(int key) {
            pthread_mutex_lock(&mutex);
            if (root == nullptr) {
                pthread_mutex_unlock(&mutex);
                return false;
            }
            // pthread_mutex_lock(&root->mutex);
            // if (root->key == key) {
            //     return removeRoot();
            // }
            // pthread_mutex_unlock(&mutex);
            node *current = root;
            node *previous = nullptr;
            // node *next = NULL;
            // while (true) {
            //     if (key < current->key) {
            //         if (current->left == NULL) {
            //             // pthread_mutex_unlock(&current->mutex);
            //             // if (previous) pthread_mutex_unlock(&previous->mutex);
            //             // pthread_mutex_unlock(&mutex);
            //             return false;
            //         } else next = current->left;
            //     } else if (key > current->key) {
            //         if (current->right == NULL) {
            //             // pthread_mutex_unlock(&current->mutex);
            //             // if (previous) pthread_mutex_unlock(&previous->mutex);
            //             // pthread_mutex_unlock(&mutex);
            //             return false;
            //         } else next = current->right;
            //     } else {
            //         if (current->left == NULL && current->right == NULL) {
            //             if (current == previous->left) previous->left = NULL;
            //             else previous->right = NULL;
            //             // if (previous) pthread_mutex_unlock(&previous->mutex);
            //             // pthread_mutex_unlock(&mutex);
            //             return true;
            //         } else if (current->left == NULL) {
            //             if (current == previous->left) previous->left = move(current->right);
            //             else previous->right = move(current->right);
            //             // if (previous) pthread_mutex_unlock(&previous->mutex);
            //             // pthread_mutex_unlock(&mutex);
            //             return true;
            //         } else if (current->right == NULL) {
            //             if (current == previous->left) previous->left = move(current->left);
            //             else previous->right = move(current->left);
            //             // if (previous) pthread_mutex_unlock(&previous->mutex);
            //             // pthread_mutex_unlock(&mutex);
            //             return true;
            //         } else {
            //             node *minPrevious = current;
            //             node *minimum = current->left;
            //             // pthread_mutex_lock(&minimum->mutex);
            //             // pthread_mutex_unlock(&previous->mutex);
            //             while (true) {
            //                 if (minimum->left == NULL) {
            //                     current->key = minimum->key;
            //                     current->value = minimum->value;
            //                     if (minPrevious == current) {
            //                         if (minimum->right == NULL) minPrevious->right = NULL;
            //                         else minPrevious->right = move(minimum->right);
            //                     } else {
            //                         if (minimum->right == NULL) minPrevious->left = NULL;
            //                         else minPrevious->left = move(minimum->right);
            //                         // pthread_mutex_unlock(&minPrevious->mutex);
            //                     }
            //                     // pthread_mutex_unlock(&current->mutex);
            //                     // pthread_mutex_unlock(&mutex);
            //                     return true;
            //                 }
            //                 // pthread_mutex_lock(&minimum->left->mutex);
            //                 // if (minPrevious != current) {
            //                 //     pthread_mutex_unlock(&minPrevious->mutex);
            //                 // }
            //                 minPrevious = minimum;
            //                 minimum = minimum->left;
            //             }
            //         }
            //     }
            //     // pthread_mutex_lock(&next->mutex);
            //     // if (previous) pthread_mutex_unlock(&previous->mutex);
            //     previous = current;
            //     current = next;

            // }
            // if (isBalanced(NULL, root)) return true;
            // else {
            //     node *culprit = findCulprit(NULL, root);
            //     if (culprit == root) root = balanceTree(root);
            //     else culprit = balanceTree(culprit);
            //     return true;
            // }
            // opComplete = false;
            // root = removeNode(root,key);
            // pthread_mutex_unlock(&mutex);
            // return opComplete;
            while (current) {
                if (key < current->key) {
                    previous = current;
                    current = current->left;
                } else if (key > current->key) {
                    previous = current;
                    current = current->right;
                } else {
                    if (!current->left && !current->right) {
                        if (previous->left == current) previous->left = nullptr;
                        else previous->right = nullptr;
                        current = nullptr;
                    } else if (!current->left || !current->right) {
                        if (current->left) previous->left = current->left;
                        else previous->right = current->right;
                        current = nullptr;
                    } else {
                        node *minPrev = nullptr;
                        node *min = current->right;
                        while (min->left) {
                            minPrev = min;
                            min = min->left;
                        }
                        if (minPrev) minPrev->left = min->right;
                        else current->right = min->right;
                        current->key = min->key;
                        current->value = min->value;
                    }
                    pthread_mutex_unlock(&mutex);
                    return true;
                }
            }
            pthread_mutex_unlock(&mutex);
            return false;
        }

        // Retrives the string value of a node based on its key.
        string get(int key) {
            pthread_mutex_lock(&mutex);
            if (root == nullptr) {
                pthread_mutex_unlock(&mutex);
                return "No " + to_string(key);
            }
            // pthread_mutex_lock(&root->mutex);
            // pthread_mutex_unlock(&mutex);
            node *current = root;
            // node *next = NULL;
            // while (true) {
            //     if (key < current->key) {
            //         if (current->left == NULL) {
            //             // pthread_mutex_unlock(&current->mutex);
            //             // pthread_mutex_unlock(&mutex);
            //             return "No " + to_string(key);
            //         } else next = current->left;
            //     } else if (key > current->key) {
            //         if (current->right == NULL) {
            //             // pthread_mutex_unlock(&current->mutex);
            //             // pthread_mutex_unlock(&mutex);
            //             return "No " + to_string(key);
            //         } else next = current->right;
            //     } else {
            //         // pthread_mutex_unlock(&current->mutex);
            //         // pthread_mutex_unlock(&mutex);
            //         return current->value;
            //     }
            //     // pthread_mutex_lock(&next->mutex);
            //     // pthread_mutex_unlock(&current->mutex);
            //     current = next;
            // }
            // return getNode(root,key);
            while (current) {
                if (key < current->key) current = current->left;
                else if (key > current->key) current = current->right;
                else {
                    pthread_mutex_unlock(&mutex);
                    return current->value;
                }
            }
            pthread_mutex_unlock(&mutex);
            return "No " + to_string(key);
        }

        void clear() {
            pthread_mutex_lock(&mutex);
            // if (root == nullptr) return;
            // // pthread_mutex_unlock(&mutex);
            // clearNodes(root);
            // root = nullptr;
            if (root) {
                clearNodes(root);
                root = nullptr;
            }
            pthread_mutex_unlock(&mutex);
        }

        string runOp(struct opsStruct *op) { 
            // pthread_mutex_lock(&locks[hsh(op->key)]);
            // std::cout << "locked bucket " << hsh(op->key) << " for " << op->op << " " << op->value << std::endl;
            switch (op->op) {
                case 'I':
                    if (insert(op->key, op->value)) {
                        return "OK";
                    } else {
                        return "Fail";
                    }
                case 'L':
                    return get(op->key);
                case 'D':
                    if (remove(op->key)) {
                        return "OK";
                    } else {
                        return "Fail";
                    }
                default:
                    return "Fail";
            }
        }

        void print() {}

};