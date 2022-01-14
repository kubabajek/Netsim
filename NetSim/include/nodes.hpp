//
// Created by Kuba on 04.01.2022.
//

#ifndef NETSIM_NODES_HPP
#define NETSIM_NODES_HPP

#include <map>
#include <memory>
#include <utility>
#include "storage_types.hpp"
#include "types.hpp"
#include "helpers.hpp"

enum class ReceiverType{
    WORKER,
    STOREHOUSE
};

class IPackageReceiver{
public:
    using const_iterator = IPackageStockpile::const_iterator;
    virtual void receive_package(Package&& p)=0;
    ElementID get_id() const {return id_;};
    ReceiverType get_receiver_type() const {return rt_;};
    virtual ~IPackageReceiver() = default;
protected:
    ElementID id_;
    ReceiverType rt_;
};

class Storehouse : public IPackageReceiver{
public:
    const_iterator begin() { return d_->begin(); }
    const_iterator end() { return d_->end(); }
    const_iterator cbegin() { return d_->begin(); }
    const_iterator cend() { return d_->end(); }
    explicit Storehouse(ElementID id, std::unique_ptr<IPackageStockpile> d = std::make_unique<PackageQueue>(PackageQueueType::FIFO));
    void receive_package(Package &&p) override {d_->push(std::move(p));};
private:
    std::unique_ptr<IPackageStockpile> d_;
};

class ReceiverPreferences{ //Pamiętaj, aby zapewnić użytkownikom tego szablonu dostęp do wszystkich metod “tylko do odczytu” [c]begin() i [c]end() kontenera użytego do przechowywania preferencji (tj. łącznie do 4 metod).
public:
    using preferences_t = std::map<IPackageReceiver*, double>;
    using const_iterator = preferences_t::const_iterator;
    const_iterator begin() { return preferences_.begin(); }
    const_iterator end() { return preferences_.end(); }
    const_iterator cbegin() { return preferences_.begin(); }
    const_iterator cend() { return preferences_.end(); }

    preferences_t preferences_; //Publiczna, żeby PackageSender miał do niej dostęp w relacji kompozycji
    ProbabilityGenerator generator_;

    explicit ReceiverPreferences(ProbabilityGenerator pb = probability_generator) : generator_(std::move(pb)) {};
    void add_receiver(IPackageReceiver* r);
    void remove_receiver(IPackageReceiver* r);
    IPackageReceiver* choose_receiver();
    preferences_t get_preferences() const {return preferences_;}; //COPIED OBJECT!!! CAUTION, WET FLOOR =^.^=
};

class PackageSender{
public:
    ReceiverPreferences receiver_preferences_;
    PackageSender() : PackageBuffer_(std::nullopt){};
    PackageSender(PackageSender&&) = default;
    void send_package();
    std::optional<Package>& get_sending_buffer() {return PackageBuffer_;};

protected:
    void push_package(Package&& product) {PackageBuffer_ = Package(std::move(product));};
    std::optional<Package> PackageBuffer_;
};

class Ramp : public PackageSender{
public:
    Ramp(ElementID id, TimeOffset di) : id_(id), di_(di) {};
    void deliver_goods(Time t);
    TimeOffset get_delivery_interval() const {return di_;};
    ElementID get_id() const {return id_;};

private:

    ElementID id_;
    TimeOffset di_;
};

class Worker : public PackageSender, public IPackageReceiver{
public:
    const_iterator begin() { return q_->begin(); }
    const_iterator end() { return q_->end(); }
    const_iterator cbegin() { return q_->begin(); }
    const_iterator cend() { return q_->end(); }
    Worker(ElementID id, TimeOffset pd, std::unique_ptr<IPackageQueue> q);
    void do_work(Time t);
    TimeOffset get_processing_duration() const {return pd_;};
    Time get_package_processing_start_time() const {return processing_start_time_;};
    void receive_package(Package&& p) override {q_->push(std::move(p));};
    const IPackageQueue* get_queue() const {return &(*q_);};
    const std::optional<Package>& get_processing_buffer() {return WorkingBuffer_;};
private:
    TimeOffset pd_;
    Time processing_start_time_=1;
    std::unique_ptr<IPackageQueue> q_;
    std::optional<Package> WorkingBuffer_;
};
#endif //NETSIM_NODES_HPP