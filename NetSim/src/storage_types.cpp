#include "package.hpp"
#include "types.hpp"
#include "storage_types.hpp"


void PackageQueue::push(Package&& a){
    product_queue.emplace_back(Package(std::move(a)));
}

bool PackageQueue::empty(){
    return (product_queue.empty());
}

size_t PackageQueue::size(){
    return (product_queue.size());
}

Package PackageQueue::pop(){
    Package taken;
    switch(queue_type_){
        case (PackageQueueType::FIFO):
            taken = Package(std::move(product_queue.front()));
            product_queue.pop_front();
            break;
        case (PackageQueueType::LIFO):
            taken = Package(std::move(product_queue.back()));
            product_queue.pop_back();
            break;
    }
    return taken;
}

PackageQueueType PackageQueue::get_queue_type(){
    return queue_type_;
}
