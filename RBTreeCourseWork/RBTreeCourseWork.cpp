#include <iostream>
#include <fstream>
#include <string>
#include <chrono>

using namespace std;
using namespace chrono;

steady_clock::time_point startTime;
steady_clock::time_point endTime;

enum Color { BLACK, RED};

struct Node {
    int key;
    Color color;
    Node* left;
    Node* right;
    Node* Parent;
};

struct trunk {
    string str;
    trunk* prev;
    trunk(trunk* prev, string str) : prev(prev), str(str) {}
};

Node* searchSibling(Node*& node);

void rotateLeft(Node*& root, Node* node) {
    Node* rightChild = node->right;
    node->right = rightChild->left;
    if (rightChild->left != nullptr) {
        rightChild->left->Parent = node;
    }
    rightChild->Parent = node->Parent;
    if (node->Parent == nullptr) {
        root = rightChild;
        rightChild->color = BLACK;
    }
    else if (node == node->Parent->left) {
        node->Parent->left = rightChild;
    }
    else {
        node->Parent->right = rightChild;
    }
    rightChild->left = node;
    node->Parent = rightChild;
}



void rotateRight(Node*& root, Node* node) {
    Node* leftChild = node->left;
    node->left = leftChild->right;
    if (leftChild->right != nullptr) {
        leftChild->right->Parent = node;
    }
    leftChild->Parent = node->Parent;
    if (node->Parent == nullptr) {
        root = leftChild;
    }
    else if (node == node->Parent->right) {
        node->Parent->right = leftChild;
    }
    else {
        node->Parent->left = leftChild;
    }
    leftChild->right = node;
    node->Parent = leftChild;
}


void fixInsert(Node*& root, Node* node) {
    while (node->Parent != nullptr && node->Parent->color == RED) {
        if (node->Parent == node->Parent->Parent->left) {
            Node* uncle = node->Parent->Parent->right;
            if (uncle != nullptr && uncle->color == RED) {
                node->Parent->color = BLACK;
                uncle->color = BLACK;
                node->Parent->Parent->color = RED;
                node = node->Parent->Parent;
            }
            else {
                if (node == node->Parent->right) {
                    node = node->Parent;
                    rotateLeft(root, node);
                }
                node->Parent->color = BLACK;
                node->Parent->Parent->color = RED;
                rotateRight(root, node->Parent->Parent);
            }
        }
        else {
            Node* y = node->Parent->Parent->left;
            if (y != nullptr && y->color == RED) {
                node->Parent->color = BLACK;
                y->color = BLACK;
                node->Parent->Parent->color = RED;
                node = node->Parent->Parent;
            }
            else {
                if (node == node->Parent->left) {
                    node = node->Parent;
                    rotateRight(root, node);
                }
                node->Parent->color = BLACK;
                node->Parent->Parent->color = RED;
                rotateLeft(root, node->Parent->Parent);
            }
        }
    }
    root->color = BLACK;
}

void insert(Node*& root, int key) {
    Node* newNode = new Node;
    newNode->key = key;
    newNode->left = nullptr;
    newNode->right = nullptr;
    newNode->color = RED;

    Node* parent = nullptr;
    Node* current = root;
    while (current != nullptr) {
        parent = current;
        if (newNode->key < current->key) {
            current = current->left;
        }
        else {
            current = current->right;
        }
    }
    newNode->Parent = parent;
    if (parent == nullptr) {
        root = newNode;
    }
    else if (newNode->key < parent->key) {
        parent->left = newNode;
    }
    else {
        parent->right = newNode;
    }
    fixInsert(root, newNode);
}

Node* getMin(Node*& node) {
    while (node && node->left) {
        node = node->left;
    }
    return node;
}

Node* getMax(Node*& node) {
    while (node->right) {
        node = node->right;
    }
    return node;
}

Node* searchSibling(Node*& node)
{
    if (node && node->Parent)
    {
        if (node == node->Parent->left)
            return node->Parent->right;
        else
            return node->Parent->left;
    }
    return NULL;
}

void deleteFull(Node*& root)
{
    if (root == nullptr) {
        return;
    }
    deleteFull(root->left);
    deleteFull(root->right);
    delete root;
}

void beforeByPass(Node*& root)
{
    if (root)
    {
        cout << root->key << " ";
        beforeByPass(root->left);
        beforeByPass(root->right);
    }
}

void symmetByPass(Node* root)
{
    if (root)
    {
        symmetByPass(root->left);
        cout << root->key << " ";
        symmetByPass(root->right);
    }
}


void showTrunkConsole(trunk* p, int& count) {
    if (p != nullptr) {
        showTrunkConsole(p->prev, count);
        count++;
        cout << p->str;
    }
}

void print(Node*& tree, trunk* prev, bool isRight) {
    if (tree != nullptr) {
        string prevStr = "    ";
        trunk* temp = new trunk(prev, prevStr);
        if (tree->right != nullptr) {
            print(tree->right, temp, 1);
        }

        if (!prev) {
            temp->str = "-->";
        }
        else if (isRight) {
            temp->str = ".-->";
            prevStr = "   |";
        }
        else {
            temp->str = "`-->";
            prev->str = prevStr;
        }

        int count = 0;
        showTrunkConsole(temp, count);
        if (tree->color == RED) {
            cout << "(R)";
        }
        else {
            cout << "(B)";
        }
        cout << tree->key << endl;
        if (prev) {
            prev->str = prevStr;
        }
        temp->str = "   |";
        if (tree->left != nullptr) {
            print(tree->left, temp, 0);
        }
        delete temp;
    }
    else {
        return;
    }
}

Node* random(int size) {
    srand(time(nullptr));
    Node* root = nullptr;
    if (size == 0) {
        cout << "The tree is empty";
        return root;
    }
    else {
        for (int i = 0; i < size; i++) {
            int key = rand() % 199 - 99;
            insert(root, key);
        }
    }
    return root;
}

Node* createWeightTree(int size, int*& array) {
    Node* root = nullptr;
    if (size == 0) {
        cout << "The tree is empty";
        return root;
    }
    else {
        for (int i = 0; i < size; i++) {
            int key = array[i];
            insert(root, key);
        }
    }
    return root;
}

Node* createWeight() {
    string inputData;
    string str;
    cout << "Enter Items separated by spaces: \n";
    cin.ignore();
    getline(cin, inputData);
    int size = 1;
    for (int i = 0; i < inputData.size(); i++) {
        if (inputData[i] == ' ') {
            size++;
        }
    }

    int* array = new int[size];
    str = "";
    int localIndex = 0;
    for (int i = 0; i < inputData.size(); i++) {
        if (inputData[i] != ' ') {
            str.push_back(inputData[i]);
        }
        else {
            array[localIndex] = stoi(str);
            localIndex++;
            str = "";
        }
    }
    if (!str.empty()) {
        array[localIndex] = stoi(str);
    }
    Node* tree = createWeightTree(size, array);
    return tree;
}

Node* getKey(Node*& root, int key)
{
    if (root)
    {
        if (key == root->key) {
            cout << "The element is found\n";
            return root;
        }
        if (key < root->key)
            return getKey(root->left, key);
        else
            return getKey(root->right, key);
    }
    else {
        cout << "The element is not found!\n";
        return nullptr;
    }
}

Node* findNode(Node*& root, int data)
{
    if (!root || root->key == data) {
        return root;
    }
    if (root->key < data)
        return findNode(root->right, data);
    else
        return findNode(root->left, data);
}


Node* rotateLeft(Node* node) {
    Node* right = node->right;
    Node* rightLeft = right->left;
    right->left = node;
    node->right = rightLeft;
    return right;
}

Node* rotateRight(Node* node) {
    Node* left = node->left;
    Node* leftRight = left->right;
    left->right = node;
    node->left = leftRight;
    return left;
}

bool balRemLeft(Node** root) {
    Node* node = *root;
    Node* left = node->left;
    Node* right = node->right;
    Node* rightLeft = right->left;
    Node* rightRight = right->right;

    if (left and left->color == RED) {
        left->color = BLACK;
        return false;
    }
    if (right and right->color == RED) {
        node->color = RED;
        right->color = BLACK;
        node = *root = rotateLeft(node);
        if (balRemLeft(&node->left)) {
            node->left->color = BLACK;
        }
        return false;
    }

    if ((!rightLeft or !rightLeft->color) and (!rightRight or !rightRight->color)) {
        right->color = RED;
        return true;
    }

    if (rightLeft and rightLeft->color == RED) {
        right->color = RED;
        rightLeft->color = BLACK;
        right = node->right = rotateRight(right);
        rightRight = right->right;
    }

    if (rightRight and rightRight->color == RED) {
        right->color = node->color;
        rightRight->color = node->color = BLACK;
        *root = rotateLeft(node);
    }

    return false;
}

bool balRemRight(Node** root) {
    Node* node = *root;
    Node* left = node->left;
    Node* right = node->right;
    Node* leftLeft = left->left;
    Node* leftRight = left->right;

    if (right and right->color) {
        right->color = BLACK;
        return false;
    }

    if (left and left->color) {
        node->color = RED;
        left->color = BLACK;
        node = *root = rotateRight(node);
        if (balRemRight(&node->right)) {
            node->right->color = BLACK;
        }
        return false;
    }

    if ((!leftRight or leftRight->color == BLACK) and (!leftLeft or leftLeft->color == BLACK)) {
        left->color = RED;
        return true;
    }

    if (leftRight and leftRight->color == RED) {
        left->color = RED;
        leftRight->color = BLACK;
        left = node->left = rotateLeft(left);
        leftLeft = left->left;
    }

    if (leftLeft and leftLeft->color) {
        left->color = node->color;
        leftLeft->color = node->color = BLACK;
        *root = rotateRight(node);
    }

    return false;
}

bool colorMinRigh(Node** rightDeleteNode, Node** deleteNode) {
    Node* node = *rightDeleteNode;
    if (node->left) {
        if (colorMinRigh(&node->left, deleteNode)) {
            return balRemLeft(rightDeleteNode);
        }
    }
    else {
        *rightDeleteNode = node->right;
        *deleteNode = node;
        return !node->color;
    }
    return false;
}

bool remove(int value, Node** root) {
    Node* tempNode, * node = *root;
    if (node) {
        if (value > node->key) {
            if (remove(value, &node->right)) {
                return balRemRight(root);
            }
        }
        else if (value < node->key) {
            if (remove(value, &node->left)) {
                return balRemRight(root);
            }
        }
        else {
            bool color;
            if (!node->right) { 
                *root = node->left; 
                color = !node->color;
                delete node;
                return color;
            }
            else {
                color = colorMinRigh(&node->right, root); 
                tempNode = *root;
                tempNode->color = node->color;
                tempNode->left = node->left;
                tempNode->right = node->right;
                delete node;
                if (color) {
                    color = balRemRight(root);
                }
                return color;
            }
        }
        return false;
    }
    else {
        return false;
    }
}


void menu(Node*& tree, trunk*& prev, bool isRight, Node* delNode);

void actionsMenu(Node*& tree, trunk*& prev, bool isRight, Node* delNode)
{
    srand(time(NULL));
    int picker;
    {
        cout << "What action you want to perform?\n"
            << "1 - Insert item\n"
            << "2 - Delete item\n"
            << "3 - Get item on Key\n"
            << "4 - Main Menu\n"
            << "0 - Exit.\n";
    }
    cin >> picker;

    switch (picker)
    {
    case 1: {
        cout << "\nEnter key to Insert: ";
        int keyAdd;
        cin >> keyAdd;

        startTime = steady_clock::now();
        insert(tree, keyAdd);
        endTime = steady_clock::now();

        cout << "\n\n";
        print(tree, prev, isRight);
        cout << "\n\tTime Insert: " << duration_cast<nanoseconds>(endTime - startTime).count() << " ns\n";


        system("Pause");

        actionsMenu(tree, prev, isRight, delNode);
        break;
    }
    case 2: {
        cout << "Enter key to Remove: ";
        int keyDelete;
        cin >> keyDelete;

        startTime = steady_clock::now();
        remove(keyDelete, &tree);
        endTime = steady_clock::now();

        print(tree, prev, isRight);
        cout << "\n\Time Delete: " << duration_cast<nanoseconds>(endTime - startTime).count() << " ns\n";
        system("Pause");
        actionsMenu(tree, prev, isRight, delNode);
        break;
    }
    case 3: {
        cout << "Enter key to Get: ";
        int keyGet;
        cin >> keyGet;

        startTime = steady_clock::now();
        getKey(tree, keyGet);
        endTime = steady_clock::now();

        print(tree, prev, isRight);
        cout << "\n\tTime of Get: " << duration_cast<nanoseconds>(endTime - startTime).count() << " ns\n";
        system("Pause");
        actionsMenu(tree, prev, isRight, delNode);
        break;
    }
    case 4:
        system("Pause");
        system("cls");
        menu(tree, prev, isRight, delNode);
        break;
    case 0:
        deleteFull(tree);
        system("cls");
        break;
    default:
        cout << "Error! Try again\n";
        system("Pause");
        system("cls");
        actionsMenu(tree, prev, isRight, delNode);
        break;
    }

}

void menu(Node*& tree, trunk*& prev, bool isRight, Node* delNode)
{
    int mainMenu;
    {
        cout << "Choose input method:\n"
            << "1 - Enter only Size Tree\n"
            << "2 - Enter every item\n";
    }
    cin >> mainMenu;

    switch (mainMenu)
    {
    case 1: {
        system("cls");
        int size;
        cout << "Enter Size Tree: ";
        cin >> size;

        startTime = steady_clock::now();
        tree = random(size);
        endTime = steady_clock::now();

        cout << "\nSymmetrical bypass:\n";
        symmetByPass(tree);
        cout << "\Before bypass:\n";
        beforeByPass(tree);

        cout << "\n\n";
        cout << "Tree Red Black:\n\n";
        print(tree, prev, isRight);

        cout << "\n\n";
        cout << "\n\tTime of Random creating time: " << duration_cast<nanoseconds>(endTime - startTime).count() << " ns\n";
        system("Pause");
        actionsMenu(tree, prev, isRight, delNode);
        break;
    }
    case 2: {
        system("cls");

        tree = createWeight();

        cout << "\Before bypass:\n";
        beforeByPass(tree);
        cout << "\nTree Red Black:\n\n";
        print(tree, prev, isRight);
        cout << "\n\n";

        system("Pause");
        actionsMenu(tree, prev, isRight, delNode);;
        break;
    }
    default:
        cout << "Error! Try again\n";
        system("Pause");
        system("cls");
        menu(tree, prev, isRight, delNode);
        break;
    }

}

int main()
{
    Node* tree = nullptr;
    Node* delNode = tree;
    trunk* prev = nullptr;
    bool isRight = false;
    menu(tree, prev, isRight, delNode);
    return 0;
}
