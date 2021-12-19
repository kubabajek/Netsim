//
// Created by Kuba on 17.12.2021.
//
#ifndef NETSIM_STORAGE_TYPES_HPP
#define NETSIM_STORAGE_TYPES_HPP
#include "types.hpp"
#include <iostream>
#include <list>

enum class PackageQueueType {FIFO, LIFO};

class IPackageStockpile
{

public:
    virtual void push(Package&&);
    virtual bool empty() {return (0 == PackageQueue.size())}; //NOT WORKING LOLXD
    virtual size_type size();
    virtual ~IPackageStockpile();
    using const_iterator  = std::list<Package>::const_iterator;

};


class PackageQueue
{
    PackageQueue(PackageQueueType type);
};

class IPackageQueue
{
public:

};
#endif //NETSIM_STORAGE_TYPES_HPP
