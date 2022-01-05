//
// Created by Kuba on 04.01.2022.
//

#ifndef NETSIM_NODES_HPP
#define NETSIM_NODES_HPP

#include <map>
#include <memory>
#include "storage_types.hpp"
#include "types.hpp"

enum class ReceiverType{
    RAMP,
    WORKER,
    STOREHOUSE
};

class IPackageReceiver{
public:
    void receive_package(Package&& p);
    ElementID get_id(void);
};

class Storehouse{
public:
    Storehouse(ElementID id, std::unique_ptr<IPackageStockpile> d);
};

class ReceiverPreferences{
public:
    using preferences_t = std::map<IPackageReceiver*, double>;
    using const_iterator = preferences_t::const_iterator;

    preferences_t preferences_;
    ReceiverPreferences(ProbabilityGenerator pb);
    void add_receiver(IPackageReceiver* r);
    void remove_receiver(IPackageReceiver* r);
    IPackageReceiver* choose_receiver(void);
    preferences_t& get_preferences(void);
};

class PackageSender{
public:
    ReceiverPreferences receiver_preferences_;
    PackageSender(PackageSender&&) = default;
    void send_package(void);
    std::optional<Package>& get_sending_buffer(void);

protected:
    void push_package(Package&&);
    std::optional<Package> _PackageBuffer = std::nullopt;
};

class Ramp : public PackageSender{
public:
    Ramp(ElementID id, TimeOffset di);
    void deliver_goods(Time t);
    TimeOffset get_delivery_interval(void);
    ElementID get_id(void);
};

class Worker : public PackageSender {
public:
    Worker(ElementID id, TimeOffset pd, std::unique_ptr<IPackageQueue> q);
    void do_work(Time t);
    TimeOffset get_processing_duration(void);
    Time get_package_processing_start_time(void);
};
#endif //NETSIM_NODES_HPP

