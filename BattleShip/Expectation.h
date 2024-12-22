#ifndef BATTTLESIP_EXPECTATION_H
#define BATTTLESIP_EXPECTATION_H
#pragma once

#include <exception>

class NoAbility : public std::exception {
public:
    virtual const char* what() const noexcept override {
        return "No abilities in queue!";
    }
};

class Overlap : public std::exception {
public:
    virtual const char* what() const noexcept override {
        return "Ship overlap!";
    }
};

class OutOfBounds : public std::exception {
public:
    virtual const char* what() const noexcept override {
        return "Out of bounds!";
    }
};

class NoSuchShipSize : public std::exception {
public:
    virtual const char* what() const noexcept override {
        return "No such ship size in manager!";
    }
};

class NoSuchAbility : public std::exception {
public:
    virtual const char* what() const noexcept override {
        return "Unknown ability!";
    }
};

class SizeMismatch : public std::exception {
public:
    virtual const char* what() const noexcept override {
        return "Size mismatch!";
    }
};

class NoShipsLeft : public std::exception {
public:
    virtual const char* what() const noexcept override {
        return "No ships left!";
    }
};
#endif //BATTTLESIP_EXPECTATION_H
