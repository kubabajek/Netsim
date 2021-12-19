//
// Created by Kuba on 17.12.2021.
//
#ifndef NETSIM_STORAGE_TYPES_HPP
#define NETSIM_STORAGE_TYPES_HPP
#include "types.hpp"
#include "package.hpp"
#include <iostream>
#include <list>

enum class PackageQueueType
{
    FIFO,
    LIFO
};

class IPackageStockpile
{
public:
    virtual void push(Package&&);
    virtual bool empty();
    virtual size_type size();
    virtual ~IPackageStockpile();

    using const_iterator  = std::list<Package>::const_iterator;
};

class IPackageQueue : public IPackageStockpile
{
private:
    virtual Package pop();
    virtual PackageQueueType get_queue_type();

};

class PackageQueue : public IPackageQueue
{
private:
    PackageQueue(PackageQueueType type);
};
#endif //NETSIM_STORAGE_TYPES_HPP