//
// Created by Witek on 12.04.2020.
//

#ifndef TELE3_BINARYTREE_H
#define TELE3_BINARYTREE_H

#include <vector>

class binaryTree {
private:
    int count;
    char letter;
    binaryTree* left;
    binaryTree* right;
public:
    binaryTree(char letter, int count, binaryTree *left = nullptr, binaryTree *right = nullptr);

    void find(char character, std::vector<bool>& coded, bool& flag);

    void setCount(int count);

    void setLetter(char letter);

    void setLeft(binaryTree *left);

    void setRight(binaryTree *right);

    int getCount() const;

    char getLetter() const;

    int getDepth(int dephCounter =0) const;

    binaryTree *getLeft() const;

    binaryTree *getRight() const;

    void saveTree(std::string& saveData);
    void loadTree(std::string& fileName);
    char translateMessage(std::vector<int8_t> &messageVector);

};


#endif //TELE3_BINARYTREE_H
