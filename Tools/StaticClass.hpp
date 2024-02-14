/**
 * @file        StaticClass.hpp
 * @author      Adam Łyskawa
 *
 * @brief       Defines `STATIC` macro that deletes constructors and destructor to make a class static.
 * @remark      A part of the Woof Toolkit (WTK).
 *
 * @copyright   (c)2024 CodeDog, All rights reserved.
 */

#pragma once

// Makes the class static by deleting implicit constructors and destructor.
#define STATIC(className)\
public:\
    className() = delete;\
    className(const className&) = delete;\
    className(className&&) = delete;\
    ~className() = delete;
