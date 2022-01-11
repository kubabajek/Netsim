//
// Created by Kuba on 04.01.2022.
//

#include <factory.hpp>

void Factory::do_deliveries(Time t) {
    for (auto & ramp_from_collection : collection_ramps_){
        ramp_from_collection.deliver_goods(t);
    }
}

void Factory::do_work(Time t) {
    for (auto & worker_from_collection : collection_workers_){
        worker_from_collection.do_work(t);
    }
}

void Factory::do_package_passing() {
    for(auto & ramp_from_collection: collection_ramps_){
        ramp_from_collection.send_package();
    }
    for(auto & worker_from_collection : collection_workers_){
        worker_from_collection.send_package();
    }
}

//Consistency checking - all code below
bool Factory::is_consistent() {
    return false;
}
