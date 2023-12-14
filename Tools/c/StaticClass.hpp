#pragma once

// Makes the class static by deleting implicit constructors and destructor.
#define STATIC(className)\
public:\
    className() = delete;\
    className(const className&) = delete;\
    className(className&&) = delete;\
    ~className() = delete;
