//
// Created by Witek on 13.04.2020.
//

#ifndef TELE3_BINARYINPUTOUTPUT_H
#define TELE3_BINARYINPUTOUTPUT_H

#include <string>
class binaryTree;

bool readToString(const std::string& fileName, std::string &message);

bool saveCompressedMessageToBinaryFile(std::vector<bool>boolVector, binaryTree Tree, std::string fileName);

bool readMessageFromFile(std::string &message,std::string fileName);
bool saveMessageToFile(const std::string& message,const std::string& fileName);
bool sendMessage(const std::string& fileName, const char *address, int portNumber);
bool receiveMessage(const std::string &fileName, int portNumber);
#endif //TELE3_BINARYINPUTOUTPUT_H
