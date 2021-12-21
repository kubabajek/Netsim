#ifndef NETSIM_STORAGE_TYPES_HPP
#define NETSIM_STORAGE_TYPES_HPP
#include "types.hpp"
#include "package.hpp"
#include <iostream>
#include <list>

enum class PackageQueueType{
    FIFO,
    LIFO
};

class IPackageStockpile{
public:
    virtual void push(Package&&) = 0;
    virtual bool empty() = 0;
    virtual size_t size() = 0;
    using const_iterator = std::list<Package>::const_iterator;
    virtual ~IPackageStockpile() = default;
};

class IPackageQueue{
public:
    virtual Package pop() = 0;
    virtual PackageQueueType get_queue_type() = 0;
    virtual ~IPackageQueue() = default;
};

class PackageQueue : public IPackageQueue, public IPackageStockpile{
public:
    explicit PackageQueue(PackageQueueType x) : queue_type_(x) {};
    void push(Package&& product) override {product_queue.emplace_back(Package(std::move(product)));};
    bool empty() override {return (product_queue.empty());};
    size_t size() override {return (product_queue.size());};
    Package pop() override;
    PackageQueueType get_queue_type() override {return queue_type_;};
    ~PackageQueue() override = default;
private:
    std::list<Package> product_queue;
    PackageQueueType queue_type_;
};

#endif //NETSIM_STORAGE_TYPES_HPP
