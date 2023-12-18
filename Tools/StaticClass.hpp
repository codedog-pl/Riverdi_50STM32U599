/**
 * @file        StaticClass.hpp
 * @author      CodeDog
 *
 * @brief       Defines `STATIC` macro that deletes constructors and destructor to make a class static.
 *
 * @copyright   (c)2023 CodeDog, All rights reserved.
 */

#pragma once

// Makes the class static by deleting implicit constructors and destructor.
#define STATIC(className)\
public:\
    className() = delete;\
    className(const className&) = delete;\
    className(className&&) = delete;\
    ~className() = delete;
