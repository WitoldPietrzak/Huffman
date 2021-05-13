#include <iostream>
#include "codeMessage.h"
#include "binaryInputOutput.h"


int main() {
    std::string Message;
    const char* compressedMessage = "skompresowana_wiadomosc";
    char* address;// = "127.0.0.1";
    int portNumber = 20020;
    int choice;
    std::cout << "1 - Wyslij wiadomosc \n2 - Odbierz wiadomosc \n";
    std::cin >> choice;

    if (choice == 1) {
        std::cout << "Podaj adres IP\n";
        std::string buffor_string;
        std::cin>>buffor_string;
        address= new char[buffor_string.size()+1];
        for(int i =0;i<buffor_string.size();i++){
            address[i]=buffor_string[i];
        }
        address[buffor_string.size()]='\0';
        std::cout << "Podaj numer portu\n";
        std::cin>> portNumber;
        std::cout<<"Podaj nazwe pliku \n";
        std::cin>>Message;
        codeMessage(Message, compressedMessage);
        if (!sendMessage(compressedMessage, address, portNumber)) {
            std::cout << "Nie udalo sie wyslac wiadomosci \n ";
            return 0;
        }
        remove(compressedMessage);
    } else if (choice == 2) {
        std::cout << "Podaj numer portu\n";
        std::cin>> portNumber;
        std::cout<<"Podaj nazwe pliku do zapisania \n";
        std::cin>>Message;

        receiveMessage(compressedMessage, portNumber);
        decodeMessage(compressedMessage, Message);
        remove(compressedMessage);
    }

    return 0;
}
