//
// Created by Witek on 13.04.2020.
//
#include <fstream>
#include <iostream>
#include <vector>
#include <winsock2.h>
#include "binaryTree.h"

//Funkcja do wczytywania do stringa wiadomości
bool readToString(const std::string &fileName, std::string &message) {
    message.clear();
    std::ifstream file;
    file.open(fileName, std::ios::binary);
    auto *buffor = new unsigned char;
    if (!file.is_open()) {
        std::cout << "Nie udalo sie otworzyc pliku " << fileName << std::endl;
        return false;
    }
    while (file.read((char *) (buffor), sizeof(char))) {
        message.push_back(*buffor);
    }
    file.close();
    return true;

}


// Funkcja zapisująca do pliku binarnie skompresowaną wiadomość
bool saveCompressedMessageToBinaryFile(std::vector<bool> boolVector, binaryTree Tree, std::string fileName) {
    std::ofstream file;
    //Otwieram plik
    file.open(fileName, std::ios::binary);
    if (!file.is_open()) {
        std::cout << "Otwarcie pliku nie powiodlo sie";
        return false;
    }
    //Zapisuję do pliku rozmiar i strukturę drzewa
    std::string treeData;
    Tree.saveTree(treeData);
    //Rozmiar to short int bo maksymalny rozmiar drzewa to 511 bajtów, co da się zapisać na 2 bajtach.
    short int treeSize = treeData.size();
    file.write((char *) &treeSize, sizeof(treeSize));
    for (int (i) = 0; (i) < treeSize; i++) {
        char x = treeData[i];
        file.write((char *) &x, sizeof(x));
    }
    // Zapisuję do pliku ile bitów w ostatnim bajcie ma być ignorowanych przy odczycie (bo nie są częścią wiadomości).
    char ignoredBites = char((8 - boolVector.size() % 8) % 8);
    file.write((char *) &ignoredBites, sizeof(ignoredBites));
    //Obliczam ile bajtów zajmie wiadomość
    unsigned long long int size = boolVector.size() / 8;
    if (boolVector.size() % 8 != 0) {
        size++;
    }
    // Tworzę bajt z każdych 8 bitów w wektorze i zapisuję go do pliku.
    for (unsigned long long int i = 0; i < size; i++) {
        char container = char(0);
        for (int j = 0; j < 8; j++) {
            if (boolVector[8 * i + j] == true && 8 * i + j < boolVector.size()) {
                container++;
            }
            if (j != 7) {
                container = container << 1;
            }
        }
        file.write((char *) &container, sizeof(container));

    }
    file.close();
    return true;
}

//Funkcja odczytująca z pliku i dekompresująca wiadomość
bool readMessageFromFile(std::string &message, std::string fileName) {
    message.clear();
    std::ifstream file;
    file.open(fileName, std::ios::binary);
    auto *buffor = new unsigned char;
    short int treeSize = 0;
    std::string treeStructure;
    char ignoredBits = 0;
    std::vector<int8_t> codedMessage;
    if (!file.is_open()) {
        std::cout << "Nie udalo sie otworzyc pliku " << fileName << std::endl;
        return false;
    }
    //Wczytuję rozmiar struktury drzewa
    file.read((char *) (&treeSize), sizeof(short int));
    //Wczytuję strukturę drzewa
    for (int i = 0; i < treeSize; i++) {
        unsigned char x;
        file.read((char *) (&x), sizeof(char));
        treeStructure += x;
    }
    //Wczytuję ilość ignorowanych bitów w ostatnim bajcie.
    file.read((char *) (&ignoredBits), sizeof(char));

    //Wczytuję skompresowaną wiadomość i zamieniam z charów na wektor  8bitowych intów
    while (file.read((char *) (buffor), sizeof(char))) {
        unsigned char buff = *buffor;
        for (int i = 0; i < 8; i++) {
            auto comparer = (unsigned char) (128 >> i);
            //Dla każdego wczytanego bajtu sprawdzam wartość każdego bitu i dodaję ją do wektora.
            codedMessage.push_back((buff & comparer) == comparer);
        }
    }
    //Usuwam bity które nie stanowią części wiadomości.
    for (int i = 0; i < ignoredBits; i++) {
        codedMessage.pop_back();
    }
    //Tworzę drzewo z wczytanej struktury.
    binaryTree Tree('\0', 0);
    Tree.loadTree(treeStructure);
    //Odczytuję kolejne bajty wiadomości.
    while (!codedMessage.empty()) {
        message += Tree.translateMessage(codedMessage);
    }
    return true;

}
//Funkcja zapisująca stringa do pliku
bool saveMessageToFile(const std::string &message, const std::string &fileName) {
    std::ofstream file;
    //Otwieram plik
    file.open(fileName, std::ios::binary);
    if (!file.is_open()) {
        std::cout << "Nie udalo sie otworzyc pliku " << fileName << std::endl;
        return false;
    }
    file << message;

    file.close();
    return true;
}

//Funkcja wysyłająca wiadomość przez socket
bool sendMessage(const std::string &fileName, const char* address, int portNumber) {
    WSADATA wsa;
    WSAStartup(0x0202, &wsa);
    SOCKET Socket;
    struct sockaddr_in cel;
    struct sockaddr_in clientAddress;
    //Ustawuam sin_family na IPv4
    cel.sin_family = AF_INET;
    //Ustawiam numer portu
    cel.sin_port = htons(portNumber);
    //Ustawiam adres IPv4 na który wysłana ma zostać wiadomość
    cel.sin_addr.s_addr = inet_addr(address);
    //Inicjalizuję socket używający IPv4, obukierunkowej komunikacji i protokołu TCP
    Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (Socket < 0) {
        std::cout << "Nie udalo sie utworzyc socketa";
        return false;
    }
    //łPróbuję nawiązać połączenie
    if (connect(Socket, (struct sockaddr *) &cel, sizeof(cel)) < 0) {
        if (Socket) {
            closesocket(Socket);
            WSACleanup();
        }
        std::cout << "Nie udalo sie polaczyc  \n" << std::endl;
        return false;
    }

    //Wczytuję dane z pliku

    std::string plik;
    readToString(fileName, plik);
    char buffor[plik.size() + 1];
    plik.copy(buffor, plik.size());
    //Wysyłam wczytane dane
    send(Socket, buffor, plik.size(), 0);
    closesocket(Socket);
    WSACleanup();
    return true;
}

bool receiveMessage(const std::string &fileName, int portNumber) {
    WSADATA wsa;
    WSAStartup(0x0202, &wsa);
    SOCKET Socket;
    struct sockaddr_in cel{};
    struct sockaddr_in clientAddress{};
    char buffor[1];
    std::vector<char> message;
    //Ustawuam sin_family na IPv4
    cel.sin_family = AF_INET;
    //Ustawiam numer portu
    cel.sin_port = htons(portNumber);
    //Socket ma przyjmować próby połączenia z dowolnego adresu IPv4
    cel.sin_addr.s_addr = INADDR_ANY;
    //Inicjalizuję socket używający IPv4, obukierunkowej komunikacji i protokołu TCP
    Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (Socket < 0) {
        std::cout << "Nie udalo sie utworzyc socketa \n";
        return false;
    }

    if (bind(Socket, (struct sockaddr *) &cel, sizeof(cel)) < 0) {
        if (Socket) {
            closesocket(Socket);
            WSACleanup();
        }
        std::cout << "Nie udalo sie zbindowac \n";
        return false;
    }
    //Oczekuję na przychodzące dane
    std::cout << "Wlaczono nasluchiwanie!" << std::endl;
    if (listen(Socket, 2) < 0) {
        if (Socket) {
            closesocket(Socket);
            WSACleanup();
        }
        return false;
    }
    //Akceptuję połączenie i sprawdzam czy wiadomość zostałą poprawnie odebrana
    int clientSize = sizeof(clientAddress);
    Socket = accept(Socket, (struct sockaddr *) &clientAddress, (int *) &clientSize);
    if (Socket < 0) {
        return false;
    }
    std::cout << "Nawiazano polaczenie!" << std::endl;
    //Wczytuję przychodzącą wiadomość do wektora
    int incomingSize;
    do {
        incomingSize = recv(Socket, buffor, 1, 0);
        if (incomingSize) {
            message.push_back(buffor[0]);
        }
    } while (incomingSize > 0);
    closesocket(Socket);
    WSACleanup();
    //Przepisuję wiadomość z wektora do tablicy charów
    char *message2 = new char[message.size()];
    for (int i = 0; i < message.size(); i++) {
        message2[i] = message[i];
    }
    //Zapisuję odebraną wiadomość do pliku
    std::ofstream file;
    file.open(fileName, std::ios::binary);
    if (!file.is_open()) {
        std::cout << "Nie udalo sie otworzyc pliku " << fileName << std::endl;
        return false;
    }
    for (int i = 0; i < message.size(); i++) {
        file.write(&message[i], sizeof(message[i]));
    }
    file.close();

    return true;
}