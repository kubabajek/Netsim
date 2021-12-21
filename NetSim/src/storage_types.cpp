#include "package.hpp"
#include "types.hpp"
#include "storage_types.hpp"

Package PackageQueue::pop()
{
    Package taken_package(0);
    switch(queue_type_)
    {
        case (PackageQueueType::FIFO):
            taken_package = Package(std::move(product_queue.front()));
            product_queue.pop_front();
            break;
        case (PackageQueueType::LIFO):
            taken_package = Package(std::move(product_queue.back()));
            product_queue.pop_back();
            break;
    }
    return taken_package;
}