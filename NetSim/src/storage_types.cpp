#include "package.hpp"
#include "types.hpp"
#include "storage_types.hpp"

void PackageQueue::push(Package&& a)
{
    product_queue.emplace_back(Package(std::move(a)));
}

bool PackageQueue::empty()
{
    return (product_queue.empty());
}

size_t PackageQueue::size()
{
    return (product_queue.size());
}

Package PackageQueue::pop()
{
    Package taken_package(0);
    switch(queue_type_)
    {
        case (PackageQueueType::FIFO):
            //printf("FIFO pop\n");
            taken_package = Package(std::move(product_queue.front()));
            product_queue.pop_front();
            break;
        case (PackageQueueType::LIFO):
            //printf("LIFO pop\n");
            taken_package = Package(std::move(product_queue.back()));
            product_queue.pop_back();
            break;
    }
    return taken_package;
}

PackageQueueType PackageQueue::get_queue_type()
{
    return queue_type_;
}
