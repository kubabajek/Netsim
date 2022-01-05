//
// Created by Kuba on 04.01.2022.
//
#include <optional>
#include "nodes.hpp"
#include "storage_types.hpp"
#include "package.hpp"


std::optional<Package>& PackageSender::get_sending_buffer(){
//    Package temp;
//    temp = Package(std::move(*_PackageBuffer));
//    _PackageBuffer = std::nullopt;
//    return temp;
    std::optional<Package>& PackageBufferRef = _PackageBuffer;
    if (_PackageBuffer)
        _PackageBuffer = std::nullopt;
    return PackageBufferRef;
}

void PackageSender::push_package(Package &&product) {
    _PackageBuffer = Package(std::move(product));
}
