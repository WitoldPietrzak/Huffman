//
// Created by Witek on 13.04.2020.
//

#include "codeMessage.h"
#include <utility>
#include <vector>
#include "binaryInputOutput.h"
#include "binaryTree.h"

//Tworzy drzewo Huffmana dla podanej wiadomości
binaryTree createTree(const std::string &message) {
    std::vector<binaryTree *> trees;
    std::vector<char> letters;
    std::vector<int> letters_count;
    //Tworzę wektor z literami w wiadomości i odpowiadający mu wektor zawierający ilosć danej litery
    for (char message_letter : message) {
        bool flag = false;
        for (int j = 0; j < letters.size(); j++) {
            if (letters[j] == message_letter) {
                flag = true;
                letters_count[j]++;
            }
        }
        if (!flag) {
            letters.push_back(message_letter);
            letters_count.push_back(1);
        }
    }
    // Tworzę wektor zawierający drzewa binarne z poszczególnymi literami i ilością ich wystąpień.
    trees.reserve(letters.size());
    for (int i = 0; i < letters.size(); i++) {
        trees.push_back(new binaryTree(letters[i], letters_count[i]));
    }
    letters.clear();
    letters_count.clear();
    // Buduję drzewo Huffmana, łącząc dwa drzewa o najmniejszej ilości wystąpień zawartych w nich liter aż zostanie tylko jedno drzewo.
    while (trees.size() > 1) {
        int firstSmallestTreeIndex = 0;
        int secondSmallestTreeIndex = 0;
        for (int i = 0; i < trees.size(); i++) {
            // Znajduję 2 drzewa o najmniejszej ilosci wystąpień ich liter
            if (trees[i]->getCount() < trees[firstSmallestTreeIndex]->getCount()) {
                secondSmallestTreeIndex = firstSmallestTreeIndex;
                firstSmallestTreeIndex = i;
            } else if (trees[i]->getCount() < trees[secondSmallestTreeIndex]->getCount() || (trees[i]!=trees[firstSmallestTreeIndex] && trees[firstSmallestTreeIndex]==trees[secondSmallestTreeIndex])) {
                secondSmallestTreeIndex = i;
            }
        }
        // łączę je w nowe drzewo.
        trees.insert(trees.begin(), new binaryTree('\0', trees[firstSmallestTreeIndex]->getCount() +
                                                         trees[secondSmallestTreeIndex]->getCount(),
                                                   trees[firstSmallestTreeIndex], trees[secondSmallestTreeIndex]));
        if (secondSmallestTreeIndex > firstSmallestTreeIndex) {
            secondSmallestTreeIndex--;
        }
        // Usuwam stare drzewa z wektora.
        trees.erase(trees.begin() + firstSmallestTreeIndex + 1);
        trees.erase(trees.begin() + secondSmallestTreeIndex + 1);
    }
    return *trees[0];
}
// Korzystając z metody find klasy binaryTree koduję wiadomość do wektora
void createCodedVector(const std::string &message,std::vector<bool>&codedMessage, binaryTree Tree) {
    for (char i : message) {
        bool flag = false;
        Tree.find(i,codedMessage,flag);

    }

}
//Koduje wiadomość z inputFileName do savedFileName
void codeMessage(const std::string& inputFileName,std::string saveFileName){

    std::string message;
    readToString(inputFileName, message);
    std::vector<bool>codeMessage;
    binaryTree Tree = createTree(message);
    createCodedVector(message,codeMessage,Tree);
    std::vector<bool>codeMessage2 = codeMessage;
    saveCompressedMessageToBinaryFile(codeMessage, Tree, std::move(saveFileName));

}
//Odkodowywuje wiadomość z readFileName do saveFileName
void decodeMessage(std::string readFileName,const std::string& saveFileName){
    std::string message;
    readMessageFromFile(message,std::move(readFileName));
    saveMessageToFile(message,saveFileName);
}