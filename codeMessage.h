//
// Created by Witek on 13.04.2020.
//

#ifndef TELE3_CODEMESSAGE_H
#define TELE3_CODEMESSAGE_H

#include <string>
#include <vector>

class binaryTree;

binaryTree createTree(const std::string& message);
void createCodedVector(const std::string &message,std::vector<bool>&codedMessage, binaryTree Tree);
void codeMessage(const std::string& inputFileName,std::string saveFileName);
void decodeMessage(std::string readFileName,const std::string& saveFileName);


#endif //TELE3_CODEMESSAGE_H
