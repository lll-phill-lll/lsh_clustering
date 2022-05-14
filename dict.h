#pragma once

class IDict {
public:
    virtual int operator[](char c) const = 0;
    virtual char operator[](int i) const = 0;
    virtual int size() const = 0;
    virtual char get_extra() const = 0;
};
