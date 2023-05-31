#include "DataStructures/RedBlack.h"
#include <iostream>
#include <math.h>

RedBlack::RedBlack() : Tree() {}

RedBlack::RedBlack(std::vector<int> elements) : RedBlack() {
    if (elements.empty()) return;

    // Add data to the tree
    for (auto element : elements) {
        add(element);
    }
}

RedBlack::~RedBlack() {
    while (root != nullptr) {
        remove(root->value);
    }
}

void RedBlack::add(int value) {
    // Dont allow nodes with the same value
    if (find(value) != nullptr) return;

    // If tree is emptpy new value becomes root 
    if (root == nullptr) {
        root = new Node_RB(nullptr, value);
        root->colour = BLACK;   // root always black
        return;
    }

    // Look for a parent for the new node. Go down the tree (left or right) according to the key
    // Search ends when a suitable parent is found for the new node
    Node_RB* new_element_parent = root;
    while(new_element_parent != nullptr) {
        if (new_element_parent->left == nullptr && value < new_element_parent->value) {
            new_element_parent->left = new Node_RB(new_element_parent, value);
            balanceTreeAdd(new_element_parent->parent, new_element_parent->left);
            return;
        }

        if (new_element_parent->right == nullptr && value >= new_element_parent->value) {
            new_element_parent->right = new Node_RB(new_element_parent, value);
            balanceTreeAdd(new_element_parent->parent, new_element_parent->right);
            return;
        }

        if (new_element_parent->value > value) new_element_parent = new_element_parent->left;
        else new_element_parent = new_element_parent->right;
    }
}

void RedBlack::remove(int value) {
    Node_RB* node = find(value);

    // If value not found in the tree - return
    if (node == nullptr) return;

    // If node has child/children we have to get its successor
    // The successor will have at most one child
    if (node->left != nullptr || node->right != nullptr) {
        Node_RB* successor = getSuccessor(node);
        node->value = successor->value;
        node = successor;
    }

    // If node is a leaf (no children) then we have to simply disconnect it from the tree
    // If node was the root the BST becomes empty 
    if (node->left == nullptr && node->right == nullptr) {
        if (node->parent != nullptr && node->parent->left == node) node->parent->left = nullptr;
        else if (node->parent != nullptr) node->parent->right = nullptr;
        else root = nullptr;
    }

    // If node has a child then we connect the child with node's parent and delete the node
    // If node that we delete was root then the child becomes new root
    else {
        Node_RB* child;

        if (node->left != nullptr) child = node->left;
        else child = node->right;

        if (node->parent != nullptr && node->parent->left == node) node->parent->left = child;
        else if (node->parent != nullptr) node->parent->right = child;
        else root = child;

        child->parent = node->parent;
    }

    balanceTreeRemove(node->parent);
    delete node;
}

void RedBlack::balanceTreeAdd(Node_RB* main_node, Node_RB* new_node) {
    if (main_node == nullptr || new_node == nullptr) return;

    int balance_factor = getBalanceFactor(main_node);

    // Left rotate
    if (balance_factor < -1 && new_node->value > main_node->right->value) {
        leftRotation(main_node);
    }

    // Left Right rotate
    else if (balance_factor > 1 && new_node->value > main_node->left->value) {
        leftRightRotation(main_node);
    }

    // Right rotate
    else if (balance_factor > 1 && new_node->value < main_node->left->value) {
        rightRotation(main_node);
    }

    // Right Left rotate
    else if (balance_factor < -1 && new_node->value < main_node->right->value) {
        rightLeftRotation(main_node);
    }
}

void RedBlack::balanceTreeRemove(Node_RB* node) {
   if (node == nullptr) return;

    int balance_factor = getBalanceFactor(node);

    // Left rotate
    if (balance_factor < -1 && getBalanceFactor(node->right) <= 0) {
        leftRotation(node);
    }

    // Left Right rotate
    else if (balance_factor > 1 && getBalanceFactor(node->left) == -1) {
        leftRightRotation(node);
    }

    // Right rotate
    else if (balance_factor > 1 && getBalanceFactor(node->left) >= 0) {
        rightRotation(node);
    }

    // Right Left rotate
    else if (balance_factor < -1 && getBalanceFactor(node->right) == 1) {
        rightLeftRotation(node);
    }
}


void RedBlack::leftRotation(Node_RB* node) {
    // If node or node's left child doesnt exist - dont rotate
    if (node == nullptr || node->right == nullptr) return;

    Node_RB* new_right_root = node->right;

    //Rotation
    node->right = new_right_root->left;
    if (node->right != nullptr) {
        node->right->parent = node;
    }

    // Copy data
    new_right_root->left = node;
    new_right_root->parent = node->parent;
    node->parent = new_right_root;

    // If given node was root we have to assign it again
    // Else we have to assign new child 
    if (new_right_root->parent == nullptr) {
        root = new_right_root;
        return;
    }

    if (new_right_root->value < new_right_root->parent->value) new_right_root->parent->left = new_right_root;
    else new_right_root->parent->right = new_right_root;
}

void RedBlack::rightRotation(Node_RB* node) {
    // If node or node's left child doesnt exist - dont rotate
    if (node == nullptr || node->left == nullptr) return;

    Node_RB* new_left_root = node->left;

    // Rotation
    node->left = new_left_root->right;
    if (node->left != nullptr) {
        node->left->parent = node; 
    }

    // Copy data 
    new_left_root->right = node;
    new_left_root->parent = node->parent;
    node->parent = new_left_root;

    // If given node was root we have to assign it again
    // Else we have to assign new child 
    if (new_left_root->parent == nullptr) {
        root = new_left_root;
        return;
    }

    if (new_left_root->value < new_left_root->parent->value) new_left_root->parent->left = new_left_root;
    else new_left_root->parent->right = new_left_root;
}

void RedBlack::leftRightRotation(Node_RB* node) {
    // If node or node's left child doesnt exist - dont rotate
    if (node == nullptr || node->left == nullptr) return;

    Node_RB* new_left_node = node->left->right;

    // Rotation
    node->left->right = new_left_node->left;
    if (node->left->right != nullptr) {
        node->left->right->parent = node->left;
    }

    // Copy data
    new_left_node->left = node->left;
    node->left->parent = new_left_node;
    node->left = new_left_node;
    new_left_node->parent = node;

    rightRotation(node);
}

void RedBlack::rightLeftRotation(Node_RB* node) {
    // If node or node's left child doesnt exist - dont rotate
    if (node == nullptr || node->right == nullptr) return;

    Node_RB* new_right_node = node->right->left;

    // Rotation
    node->right->left = new_right_node->right;
    if (node->right->left != nullptr) {
        node->right->left->parent = node->right;
    }

    // Copy data
    new_right_node->right = node->right;
    node->right->parent = new_right_node;
    node->right = new_right_node;
    new_right_node->parent = node;

    leftRotation(node);
}

void RedBlack::fixColours() {
    
}

void RedBlack::print() {
    std::vector<Node_RB*> nodes;
    indexNodes(nodes, root, 0);

    printf("-----------------------------------------------------------------------------\n");

    if(nodes.size() == 0){
        printf("puste\n");
        return;
    }

    unsigned height = 2*floor(log2(nodes.size()))+1;
    unsigned width = 2*pow(2, floor(log2(nodes.size()))) - 1;
    
    int p = width+1;
    int already_printed = 0;

    for(int y = 1; y <= height; y++){

        if(y%2 == 0) {
            printf("\n\n");
            continue;
        }

        for(int x = 1; x <= width; x++){

            
            if(x%p == p/2) {
                if(nodes[already_printed] != nullptr) {
                    printf("%4d", nodes[already_printed]->value);
                    if (nodes[already_printed]->colour == RED) printf("R");
                }
                else printf("  ");
                already_printed++;
            }
            else printf("  ");

            if(already_printed == nodes.size()){
                printf("\n\n");
                return;
            }

        }
        if(y%2 == 1) p /= 2;
    }
}