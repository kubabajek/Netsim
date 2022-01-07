//
// Created by Kuba on 04.01.2022.
//
#include <optional>
#include "nodes.hpp"
#include "storage_types.hpp"
#include "package.hpp"
#include "helpers.hpp"


std::optional<Package>& PackageSender::get_sending_buffer(){
//    Package temp;
//    temp = Package(std::move(*_PackageBuffer));
//    _PackageBuffer = std::nullopt;
//    return temp;
//    std::optional<Package>& PackageBufferRef = _PackageBuffer;
//    if (_PackageBuffer)
//        _PackageBuffer = std::nullopt;
//    return PackageBufferRef;
    return _PackageBuffer;
}

void PackageSender::push_package(Package &&product) {
    _PackageBuffer = Package(std::move(product));
}


void PackageSender::send_package(void){
    if(_PackageBuffer!=std::nullopt){
        Package Package_to_send = Package(std::move(_PackageBuffer.value()));
        _PackageBuffer = std::nullopt;
        //pointer_for_next_worker_or_storehouse->receive_package(std::move(Package_to_send))
    }
}

IPackageReceiver* ReceiverPreferences::choose_receiver(void) {
    double pb = default_probability_generator();
    auto iterator = preferences_.begin();
    auto end_it = preferences_.end();
    end_it--;
    double lower_scope=0;
    double higher_scope=iterator->second;
    while(iterator!=end_it){
        if (pb>=lower_scope and pb<higher_scope) {
            break;
        }
        lower_scope=higher_scope;
        iterator++;
        higher_scope+=iterator->second;
    }
    return iterator->first;
}
