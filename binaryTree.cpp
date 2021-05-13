//
// Created by Witek on 12.04.2020.
//

#include <iostream>
#include "binaryTree.h"

binaryTree::binaryTree(char letter, int count, binaryTree *left, binaryTree *right) : count(count), letter(letter),
                                                                                      left(left), right(right) {}
//Zwraca ścieżkę do podanego chara
void binaryTree::find(char character, std::vector<bool> &coded, bool &flag) {
    if (this->right != nullptr) {
        coded.push_back(true);
        right->find(character, coded, flag);
    }
    if (flag) {
        return;
    }
    if (this->left != nullptr) {
        coded.push_back(false);
        left->find(character, coded, flag);
    }
    if (flag) {
        return;
    }
    if (this->letter == character) {
        flag = true;
        return;
    } else {
        coded.pop_back();
        return;

    }
}

void binaryTree::setCount(int count) {
    binaryTree::count = count;
}

void binaryTree::setLetter(char letter) {
    binaryTree::letter = letter;
}

void binaryTree::setLeft(binaryTree *left) {
    binaryTree::left = left;
}

void binaryTree::setRight(binaryTree *right) {
    binaryTree::right = right;
}

int binaryTree::getCount() const {
    return count;
}

char binaryTree::getLetter() const {
    return letter;
}

binaryTree *binaryTree::getLeft() const {
    return left;
}

binaryTree *binaryTree::getRight() const {
    return right;
}
//Sprawdza maksyma;ną głebokość drzewa
int binaryTree::getDepth(int depthCounter) const {
    depthCounter++;
    if (this->right == nullptr && this->left == nullptr) {
        return depthCounter;
    } else {
        int leftCounter = this->left->getDepth(depthCounter);
        int rightCounter = this->right->getDepth(depthCounter);
        if (leftCounter >= rightCounter) {
            return leftCounter;
        } else { return rightCounter; }
    }
}
//Zapisuje drzewo do stringa
void binaryTree::saveTree(std::string &saveData) {
    if (this->left == nullptr && this->right == nullptr) {
        saveData += this->getLetter();
        return;
    }
    saveData += this->getLetter();
    this->left->saveTree(saveData);
    this->right->saveTree(saveData);
}
//Wczytuje drzewo ze stringa
void binaryTree::loadTree(std::string &fileName) {
    this->letter = fileName[0];
    fileName.erase(fileName.begin());
    if (this->letter == '\0') {
        if (this->left == nullptr && this->right == nullptr) {
            this->left = new binaryTree('\0', 0);
            this->right = new binaryTree('\0', 0);
        }
        this->left->loadTree(fileName);
        this->right->loadTree(fileName);
    }
}
//Zamienia ścieżkę na chara do którego prowadzi
char binaryTree::translateMessage(std::vector<int8_t> &messageVector) {
    bool flag = messageVector[0];
    if (this->right == nullptr && this->left == nullptr) {
        return this->letter;
    }
    messageVector.erase(messageVector.begin());
    if (flag) {
        return this->right->translateMessage(messageVector);
    } else {
        return this->left->translateMessage(messageVector);
    }
}
