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

bool Factory::is_consistent()
{
    std::map<PackageSender*,State> nodes_state_map;
    for (auto & ramp : collection_ramps_)
    {
        nodes_state_map.insert(std::pair<PackageSender*,State>(&ramp, State::UNVISITED));
    }
    for (auto & worker : collection_workers_)
    {
        nodes_state_map.insert(std::pair<PackageSender*,State>(&worker, State::UNVISITED));
    }
    bool consistent = false;
    for (auto & ramp : collection_ramps_)
    {
        try
        {
            consistent = is_receiver_connected_to_storehouse(&ramp, nodes_state_map);
        }
        catch (std::logic_error &)
        {
            return false;
        }
    }
    return consistent;
}


bool Factory::is_receiver_connected_to_storehouse(PackageSender* sender, std::map<PackageSender*,State> map)
{
    if (map.at(sender) == State::VERYFIED)
        return true;
    map.at(sender) = State::VISITED;

    if (sender->receiver_preferences_.preferences_.empty())
        throw std::logic_error("Error");

    bool sender_has_at_least_one_reciever = false;
    for (auto pair : sender->receiver_preferences_.preferences_)
    {
        IPackageReceiver* reciver = pair.first;
        if (reciver->get_receiver_type() == ReceiverType::STOREHOUSE)
            sender_has_at_least_one_reciever = true;
        else
        {
            IPackageReceiver* receiver_ptr = reciver;
            auto worker_ptr = dynamic_cast<Worker*>(receiver_ptr);
            auto sendrecv_ptr = dynamic_cast<PackageSender*>(worker_ptr);

            if (sendrecv_ptr == sender)
                break;
            sender_has_at_least_one_reciever = true;
            if (map.at(sendrecv_ptr) == State::UNVISITED)
                is_receiver_connected_to_storehouse(sendrecv_ptr, map);
        }
    }
    map.at(sender) = State::VERYFIED;

    if (sender_has_at_least_one_reciever)
        return true;
    else
        throw std::logic_error("Error");
}
