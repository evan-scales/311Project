#include <iostream>
#include <string>
#include <pthread.h>
using namespace std;

class MariosBST : public ObjectInterface {
    private:
        struct node {
            pthread_mutex_t mutex;
            int key;
            string value;
            node *left, *right;
        };

        pthread_mutex_t mutex;
        node *root;

        node *makeNode(int key, string value) {
            node *n = new node();
            pthread_mutex_init(&n->mutex, NULL);
            n->key = key;
            n->value = value;
            n->left = n->right = NULL;
            return n;
        }

        int height(node *parent, node *child) {
            int lh, rh;
            if (child == NULL) {
                pthread_mutex_unlock(&parent->mutex);
                return 0;
            }
            pthread_mutex_lock(&child->mutex);
            pthread_mutex_unlock(&parent->mutex);
            lh = height(child, child->left);
            pthread_mutex_lock(&child->mutex);
            rh = height(child, child->right);
            return 1 + (max(lh, rh));
        }

        bool isBalanced(node *parent, node *child) {
            int lh, rh;
            bool lb, rb;
            if (child == root) pthread_mutex_lock(&mutex);
            if (root == NULL) {
                pthread_mutex_unlock(&mutex);
                return true;
            }
            if (child == NULL) {
                pthread_mutex_unlock(&parent->mutex);
                return true;
            }
            pthread_mutex_lock(&child->mutex);
            if (parent) pthread_mutex_unlock(&parent->mutex);
            else pthread_mutex_unlock(&mutex);
            lh = height(child, child->left);
            pthread_mutex_lock(&child->mutex);
            rh = height(child, child->left);
            pthread_mutex_lock(&child->mutex);
            lb = isBalanced(child, child->left);
            pthread_mutex_lock(&child->mutex);
            rb = isBalanced(child, child->right);
            if (abs(lh - rh) <= 1 && lb && rb) return true;
            else return false;
        }

        node *findCulprit (node *parent, node *child) {
            int lh, rh;
            node *lc, *rc;
            if (child == root) pthread_mutex_lock(&mutex);
            if (root == NULL) {
                pthread_mutex_unlock(&mutex);
                return NULL;
            }
            if (child == NULL) {
                pthread_mutex_unlock(&parent->mutex);
                return NULL;
            }
            pthread_mutex_lock(&child->mutex);
            if (parent) pthread_mutex_unlock(&parent->mutex);
            else pthread_mutex_unlock(&mutex);
            lh = height(child, child->left);
            pthread_mutex_lock(&child->mutex);
            rh = height(child, child->left);
            pthread_mutex_lock(&child->mutex);
            lc = findCulprit(child, child->left);
            pthread_mutex_lock(&child->mutex);
            rc = findCulprit(child, child->right);
            if (abs(lh - rh) > 1) return child;
            else if (lc && !rc) return lc;
            else if (!lc && rc) return rc;
            else return NULL;
        }

        node *leftRotation(node *n) {
            node *cul, *ret;
            cul = n;
            ret = cul->right;
            cul->right = ret->left;
            ret->left = cul;
            pthread_mutex_unlock(&n->mutex);
            return ret;
        }

        node *rightRotation(node *n) {
            node *cul, *ret;
            cul = n;
            ret = cul->left;
            cul->left = ret->right;
            ret->right = cul;
            pthread_mutex_unlock(&n->mutex);
            return ret;
        }

        node *leftRightRotation(node *n) {
            node *cul, *temp, *ret;
            cul = n;
            temp = n->left;
            ret = n->left->right;
            cul->left = ret->right;
            temp->right = ret->left;
            ret->right = cul;
            ret->left = temp;
            pthread_mutex_unlock(&n->mutex);
            return ret;
        }

        node *rightLeftRotation(node *n) {
            node *cul, *temp, *ret;
            cul = n;
            temp = n->right;
            ret = n->right->left;
            cul->right = ret->left;
            temp->left = ret->right;
            ret->left = cul;
            ret->right = temp;
            pthread_mutex_unlock(&n->mutex);
            return ret;
        }

        node *balanceTree(node *culprit) {
            int culLH = height(culprit, culprit->left);
            int culRH = height(culprit, culprit->right);
            int lcLH = height(culprit->left, culprit->left->left);
            int lcRH = height(culprit->left, culprit->left->right);
            int rcLH = height(culprit->right, culprit->right->left);
            int rcRH = height(culprit->right, culprit->right->right);
            pthread_mutex_lock(&culprit->mutex);
            if (culLH - culRH == -2 && rcLH - rcRH == -1) return leftRotation(culprit);
            else if (culLH - culRH == 2 && lcLH - lcRH == 1) return rightRotation(culprit);
            else if (culLH - culRH == 2 && lcLH - lcRH == -1) return leftRightRotation(culprit);
            else if (culLH - culRH == -2 && rcLH - rcRH == 1) return rightLeftRotation(culprit);
            else return culprit;
        }

        bool removeRoot() {
            if (root->left == NULL && root->right == NULL) {
                pthread_mutex_unlock(&root->mutex);
                root = NULL;
                pthread_mutex_unlock(&mutex);
                return true;
            } else if (root->left == NULL) {
                root = move(root->right);
                pthread_mutex_unlock(&mutex);
                return true;
            } else if (root->right == NULL) {
                root = move(root->left);
                pthread_mutex_unlock(&mutex);
                return true;
            } else {
                node *minPrevious = root;
                node *minimum = root->left;
                pthread_mutex_lock(&minimum->mutex);
                while (true) {
                    if (minimum->left == NULL) {
                        root->key = minimum->key;
                        root->value = minimum->value;
                        if (minPrevious == NULL) {
                            if (minimum->right == NULL) minPrevious->right = NULL;
                            else minPrevious->right = move(minimum->right);
                        } else {
                            if (minimum->right == NULL) minPrevious->left = NULL;
                            else minPrevious->left = move(minimum->right);
                            pthread_mutex_unlock(&minPrevious->mutex);
                        }
                        pthread_mutex_unlock(&root->mutex);
                        return true;
                    }
                    pthread_mutex_lock(&minimum->left->mutex);
                    if (minPrevious != root) {
                        pthread_mutex_unlock(&minPrevious->mutex);
                    }
                    minPrevious = minimum;
                    minimum = minimum->left;
                }
            }
            if (isBalanced(NULL, root)) return true;
            else {
                node *culprit = findCulprit(NULL, root);
                if (culprit = root) root = balanceTree(root);
                else culprit = balanceTree(culprit);
                return true;
            }
        }

    public:
        MariosBST() {
            pthread_mutex_init(&mutex, NULL);
        }

        ~MariosBST() {
            pthread_mutex_destroy(&mutex);
        }

        bool insert(int key, string value) {
            pthread_mutex_lock(&mutex);
            if (root == NULL) {
                root = makeNode(key, value);
                pthread_mutex_unlock(&mutex);
                return true;
            }
            pthread_mutex_lock(&root->mutex);
            pthread_mutex_unlock(&mutex);
            node *current = root;
            node *next = NULL;
            while (true) {
                if (key < current->key) {
                    if (current->left == NULL) {
                        current->left = makeNode(key, value);
                        pthread_mutex_unlock(&current->mutex);
                        break;
                    } else next = current->left;
                } else if (key > current->key) {
                    if (current->right == NULL) {
                        current->right = makeNode(key, value);
                        pthread_mutex_unlock(&current->mutex);
                        break;
                    } else next = current->right;
                } else {
                    pthread_mutex_unlock(&current->mutex);
                    return false;
                }
                pthread_mutex_lock(&next->mutex);
                pthread_mutex_unlock(&current->mutex);
                current = next;
            }
            if (isBalanced(NULL, root)) return true;
            else {
                node *culprit = findCulprit(NULL, root);
                if (culprit = root) root = balanceTree(root);
                else culprit = balanceTree(culprit);
                return true;
            }
        }

        bool remove(int key) {
            pthread_mutex_lock(&mutex);
            if (root == NULL) {
                pthread_mutex_unlock(&mutex);
                return false;
            }
            pthread_mutex_lock(&root->mutex);
            if (root->key == key) {
                return removeRoot();
            }
            pthread_mutex_unlock(&mutex);
            node *current = root;
            node *previous = NULL;
            node *next = NULL;
            while (true) {
                if (key < current->key) {
                    if (current->left == NULL) {
                        pthread_mutex_unlock(&current->mutex);
                        if (previous) pthread_mutex_unlock(&previous->mutex);
                        return false;
                    } else next = current->left;
                } else if (key > current->key) {
                    if (current->right == NULL) {
                        pthread_mutex_unlock(&current->mutex);
                        if (previous) pthread_mutex_unlock(&previous->mutex);
                        return false;
                    } else next = current->right;
                } else {
                    if (current->left == NULL && current->right == NULL) {
                        if (current = previous->left) previous->left = NULL;
                        else previous->right = NULL;
                        if (previous) pthread_mutex_unlock(&previous->mutex);
                        break;
                    } else if (current->left == NULL) {
                        if (current = previous->left) previous->left = move(current->right);
                        else previous->right = move(current->right);
                        if (previous) pthread_mutex_unlock(&previous->mutex);
                        break;
                    } else if (current->right == NULL) {
                        if (current = previous->left) previous->left = move(current->left);
                        else previous->right = move(current->left);
                        if (previous) pthread_mutex_unlock(&previous->mutex);
                        break;
                    } else {
                        node *minPrevious = current;
                        node *minimum = current->left;
                        pthread_mutex_lock(&minimum->mutex);
                        pthread_mutex_unlock(&previous->mutex);
                        while (true) {
                            if (minimum->left == NULL) {
                                current->key = minimum->key;
                                current->value = minimum->value;
                                if (minPrevious == NULL) {
                                    if (minimum->right == NULL) minPrevious->right = NULL;
                                    else minPrevious->right = move(minimum->right);
                                } else {
                                    if (minimum->right == NULL) minPrevious->left = NULL;
                                    else minPrevious->left = move(minimum->right);
                                    pthread_mutex_unlock(&minPrevious->mutex);
                                }
                                pthread_mutex_unlock(&current->mutex);
                                break;
                            }
                            pthread_mutex_lock(&minimum->left->mutex);
                            if (minPrevious != current) {
                                pthread_mutex_unlock(&minPrevious->mutex);
                            }
                            minPrevious = minimum;
                            minimum = minimum->left;
                        }
                        break;
                    }
                }
                pthread_mutex_lock(&next->mutex);
                if (previous) pthread_mutex_unlock(&previous->mutex);
                previous = current;
                current = next;
            }
            if (isBalanced(NULL, root)) return true;
            else {
                node *culprit = findCulprit(NULL, root);
                if (culprit == root) root = balanceTree(root);
                else culprit = balanceTree(culprit);
                return true;
            }
        }

        string get(int key) {
            pthread_mutex_lock(&mutex);
            if (root == NULL) {
                pthread_mutex_unlock(&mutex);
                return "No " + to_string(key);
            }
            pthread_mutex_lock(&root->mutex);
            pthread_mutex_unlock(&mutex);
            node *current = root;
            node *next = NULL;
            while (true) {
                if (key < current->key) {
                    if (current->left == NULL) {
                        pthread_mutex_unlock(&current->mutex);
                        return "No " + to_string(key);
                    } else next = current->left;
                } else if (key > current->key) {
                    if (current->right == NULL) {
                        pthread_mutex_unlock(&current->mutex);
                        return "No " + to_string(key);
                    } else next = current->right;
                } else {
                    pthread_mutex_unlock(&current->mutex);
                    return current->value;
                }
                pthread_mutex_lock(&next->mutex);
                pthread_mutex_unlock(&current->mutex);
                current = next;
            }
        }

        void print() {

        }
};