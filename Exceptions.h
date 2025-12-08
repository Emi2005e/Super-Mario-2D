//
// Created by Emi on 12/7/2025.
//
#pragma once
#ifndef PROIECT_POO_EXCEPTIONS_H
#define PROIECT_POO_EXCEPTIONS_H

#include <exception>
#include <string>
#include <utility>

class GameException : public std::exception {
protected:
    std::string msg;
public:
    explicit GameException(std::string m) : msg(std::move(m)) {}


    [[nodiscard]] const char* what() const noexcept override { return msg.c_str(); }
};

class ResourceException : public GameException {
public:

    explicit ResourceException(const std::string& filename)
        : GameException("Lipseste: " + filename) {}
};

class OutOfBoundsException : public GameException {
public:
    OutOfBoundsException() : GameException("Index out of bounds") {}
};

class LevelLoadException : public GameException {
public:

    explicit LevelLoadException(const std::string& filename)
        : GameException("Critical Error: Cannot open level file -> " + filename) {}
};

#endif //PROIECT_POO_EXCEPTIONS_H