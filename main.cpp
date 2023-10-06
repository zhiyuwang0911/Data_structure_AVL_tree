#include <iostream>
#include <string>
#include <regex>
#include <set>
#include <sstream>

using namespace std;

struct Node {
    string name;
    string gatorID;
    Node* left;
    Node* right;
    int height;

    Node(string n, string id) : name(n), gatorID(id), left(nullptr), right(nullptr), height(1) {}
};

class AVLTree {
public:
    Node* root;
    set<string> gatorIDSet;
    AVLTree() : root(nullptr) {}

    int Height(Node* node) {
        return (node == nullptr) ? 0 : node->height;
    }

    void updateHeight(Node* node) {
        node->height = 1 + max(Height(node->left), Height(node->right));
    }
    // four rotation case
    Node* leftRotate(Node* y) {
        Node* x = y->right;
        Node* T2 = x->left;

        x->left = y;
        y->right = T2;

        updateHeight(y);
        updateHeight(x);

        return x;
    }

    Node* rightRotate(Node* x) {
        Node* y = x->left;
        Node* T2 = y->right;

        y->right = x;
        x->left = T2;

        updateHeight(x);
        updateHeight(y);

        return y;
    }

    Node* leftRightRotate(Node* node) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    Node* rightLeftRotate(Node* node) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
    // constraint for name
    bool isValidName(const string& name) {  //reference: https://onecompiler.com/cpp/3xd8pcanc
        regex pattern("^[a-zA-Z ]+$");
        return regex_match(name, pattern);
    }
    //constraint for gatorID
    bool validateGatorID(string gatorID) {
        return regex_match(gatorID, regex("^[0-9]{8}$"));
    }

    bool isUniqueGatorID(Node* node, const string& gatorID) {
        //is unique
        if (node == nullptr) {
            return true;
        }
        //have duplicate
        if (gatorID == node->gatorID) {
            return false;
        }

        if (gatorID < node->gatorID) {
            return isUniqueGatorID(node->left, gatorID);
        } else {
            return isUniqueGatorID(node->right, gatorID);
        }
    }
    //bool isUniqueGatorID(string gatorID) {
        //return gatorIDSet.find(gatorID) == gatorIDSet.end();
    //}

    // insertion function

    Node*  insert(Node* node, string name, string gatorID) {
        if (!isValidName(name)) {
            cout << "unsuccessful" << endl;
            return node;
        }

        if (!validateGatorID(gatorID)) {
            cout << "unsuccessful" << endl;
            return node;
        }

        if (!isUniqueGatorID(root,gatorID)) {
            cout << "unsuccessful" << endl;
            return node;
        }

        gatorIDSet.insert(gatorID);
        if (node == nullptr) {
            cout << "successful" << endl;
            return new Node(name, gatorID);
        }

        if (gatorID == node->gatorID) {
            cout << "successful" << endl;
            return new Node(name, gatorID);
        }

        if (gatorID < node->gatorID) {
            node->left = insert(node->left, name, gatorID);
        } else {
            node->right = insert(node->right, name, gatorID);
        }

        updateHeight(node);

        int balance = Height(node->left) - Height(node->right);

        if (balance > 1) {
            if (gatorID < node->left->gatorID) {
                return rightRotate(node);
            } else {
                return leftRightRotate(node);
            }
        }

        if (balance < -1) {
            if (gatorID > node->right->gatorID) {
                return leftRotate(node);
            } else {
                return rightLeftRotate(node);
            }
        }

        return node;
    }

    void insert(string name, string gatorID) {
        root = insert(root, name, gatorID);
    }

    bool searchByGatorID(Node* node, string gatorID, string& result) {
        if (node == nullptr) {
            return false;
        }
        if (gatorID == node->gatorID) {
            result = node->name;
            return true;
        } else if (gatorID < node->gatorID) {
            return searchByGatorID(node->left, gatorID, result);
        } else {
            return searchByGatorID(node->right, gatorID, result);
        }
    }

    void searchByGatorID(string gatorID) {
        string result;

        if (searchByGatorID(root, gatorID, result)) {
            cout << result << endl;
        } else {
            cout << "unsuccessful" << endl;
        }
    }

    bool searchByName(Node* node, string name) {
        if (node == nullptr) {
            return false;
        }

        if (name == node->name) {
            cout << node->gatorID << endl;
            return true;}

        bool foundLeft = searchByName(node->left, name);
        bool foundRight = searchByName(node->right, name);
        return foundLeft || foundRight;

    }

    void searchByName(string name) {
        if (!searchByName(root, name)) {
            cout << "unsuccessful" << endl;
        }
    }

    Node* findMin(Node* node) {
        while (node->left != nullptr) {
            node = node->left;
        }
        return node;
    }

    Node* remove(Node* node, string gatorID) {
        if (node == nullptr) {
            //cout << "unsuccessful" << endl;
            return node;
        }

        if (gatorID < node->gatorID) {
            node->left = remove(node->left, gatorID);
        } else if (gatorID > node->gatorID) {
            node->right = remove(node->right, gatorID);
        } else {
            if (node->left == nullptr || node->right == nullptr) {
                Node* temp = (node->left != nullptr) ? node->left : node->right;

                if (temp == nullptr) {
                    temp = node;
                    node = nullptr;
                } else {
                    *node = *temp;
                }

                delete temp;
            } else {
                Node* temp = findMin(node->right);
                node->name = temp->name;
                node->gatorID = temp->gatorID;
                node->right = remove(node->right, temp->gatorID);
            }
            //cout << "successful" << endl;
        }

        if (node == nullptr) {
            return node;
        }

        updateHeight(node);

        int balance = Height(node->left) - Height(node->right);

        if (balance > 1) {
            if (Height(node->left->left) >= Height(node->left->right)) {
                return rightRotate(node);
            } else {
                return leftRightRotate(node);
            }
        }

        if (balance < -1) {
            if (Height(node->right->right) >= Height(node->right->left)) {
                return leftRotate(node);
            } else {
                return rightLeftRotate(node);
            }
        }

        return node;
    }

    void remove(string gatorID) {
        root = remove(root, gatorID);
    }

    void printInorder(Node* node, bool& isFirst) {
        if (node != nullptr) {
            printInorder(node->left, isFirst);
            if (!isFirst) {
                cout << ","<< " ";
            }
            cout << node->name;
            isFirst = false;
            printInorder(node->right, isFirst);
        }
    }

    void printInorder() {
        bool isFirst = true;
        //cout << "In-order traversal: ";
        printInorder(root, isFirst);
        cout << endl;
    }


    void printPreorder(Node* node, bool& isFirst) {
        if (node != nullptr) {
            if (!isFirst) {
                cout << ","<< " ";
            }
            cout << node->name;
            isFirst = false;
            printPreorder(node->left, isFirst);
            printPreorder(node->right, isFirst);
        }
    }

    void printPreorder() {
        bool isFirst = true;
        //cout << "Pre-order traversal: ";
        printPreorder(root, isFirst);
        cout << endl;
    }

    void printPostorder(Node* node, bool& isFirst) {
        if (node != nullptr) {
            printPostorder(node->left, isFirst);
            printPostorder(node->right, isFirst);
            if (!isFirst) {
                cout << ","<< " ";
            }
            cout << node->name;
            isFirst = false;
        }
    }

    void printPostorder() {
        bool isFirst = true;
        //cout << "Post-order traversal: ";
        printPostorder(root, isFirst);
        cout << endl;
    }


    int countLevels(Node* node) {
        if (node == nullptr) {
            return 0;
        }

        int leftLevels = countLevels(node->left);
        int rightLevels = countLevels(node->right);

        return 1 + max(leftLevels, rightLevels);
    }

    void printLevelCount() {
        int levels = countLevels(root);
        cout << levels << endl;
    }

    void removeInorderN(Node* node, int& count, int n) {
        if (node == nullptr || count >= n) {
            return;
        }

        removeInorderN(node->left, count, n);

        if (count == n) {
            string gatorID = node->gatorID;
            remove(gatorID);
        }

        count++;

        removeInorderN(node->right, count, n);
    }

    void removeInorderN(int n) {
        int count = 0;
        removeInorderN(root, count, n - 1); // Adjust n to be 0-based index
    }



};

int main() {
    AVLTree avlTree;

    int numCommands;
    cin >> numCommands;
    cin.ignore(); // Consume the newline character
    for (int i = 0; i < numCommands; i++) {
        string command, name, gatorID;
        string userInput;
        string space;
        getline(cin, userInput);
        istringstream in(userInput);
        getline(in, command, '"');
        getline(in, name, '"');
        in >> gatorID;

        if (command == "insert ") {
            avlTree.insert(name, gatorID);
        } else if (command == "printInorder") {
            avlTree.printInorder();
        } else if (command == "printPreorder") {
            avlTree.printPreorder();
        } else if (command == "printPostorder") {
            avlTree.printPostorder();
        } else if (command == "printLevelCount") {
            avlTree.printLevelCount();
        }
        else {
            istringstream newIn(command);
            newIn >> command;
            newIn >> name;
            if (command == "search") {
                if (regex_match(name, regex("^[0-9]{8}$"))) {
                    avlTree.searchByGatorID(name);
                } else {
                    avlTree.searchByName(name);
                }
            } else if (command == "removeInorder") {
                avlTree.removeInorderN(stoi(name));
                cout << "successful" << endl;
            } else if (command == "remove") {
                avlTree.remove(name);
                cout << "successful" << endl;
            }
            else {
                cout << "unsuccessful" << endl;
            }
        }
    }

    return 0;
}
