//
// Created by Kuba on 04.01.2022.
//

#ifndef NETSIM_FACTORY_HPP
#define NETSIM_FACTORY_HPP
#include "types.hpp"
#include "nodes.hpp"

template<class Node>
class NodeCollection
{
public:
    using container_t = typename std::list<Node>;
    using iterator = typename container_t::iterator;
    using const_iterator = typename container_t::const_iterator;
    iterator begin() { return nodes_.begin(); }
    iterator end() { return nodes_.end(); }
    const_iterator cbegin() const { return nodes_.cbegin(); }
    const_iterator cend() const { return nodes_.cend(); }

    void add(Node&& node){
        //if juz jest taki obiekt w kontenerze
        nodes_.emplace_back(std::move(node));
    };
    void remove_by_id(ElementID id){
        bool removed = false;
        for (auto it = nodes_.cbegin(); it != nodes_.cend(); ++it){
            if(it->get_id()==id){
                nodes_.erase(it);
                removed = true;
                break;
            }
        }
        if (not(removed)){
            std::cout<<"REMOVING ELEMENT WHICH DOESNT EXIST IN CONTAINER - DIDN't DELETE!"<<std::endl;
        }
    }
//    NodeCollection<Node>::iterator find_by_id( ElementID id){
//        bool found = false;
//        iterator found_iterator = nodes_.end();
//        for (iterator it = nodes_.begin(); it != nodes_.end(); ++it){
//            if(it->get_id()==id){
//                found_iterator=it;
//                found = true;
//                break;
//            }
//        }
//        if (not(found)){
//            std::cout<<"ELEMENT NOT FOUND"<<std::endl;
//        }
//    }
    NodeCollection<Node>::iterator find_by_id( ElementID id){
        std::find_if(nodes_.begin();)
    }
    NodeCollection<Node>::const_iterator find_by_id( ElementID id) const {
        bool found = false;
        const_iterator found_iterator = nodes_.end();
        for (const_iterator it = nodes_.cbegin(); it != nodes_.cend(); ++it){
            if(it->get_id()==id){
                found_iterator=it;
                found = true;
                break;
            }
        }
        if (not(found)){
            std::cout<<"ELEMENT NOT FOUND"<<std::endl;
        }
    }
private:
    container_t nodes_;
};


class Factory{
public:
    void add_ramp(Ramp&&);
    void add_worker(Worker&&);
    void add_storehouse(Storehouse&&);
    void remove_ramp(ElementID);
    void remove_worker(ElementID);
    void remove_storehouse(ElementID);
    NodeCollection<Ramp>::iterator find_ramp_by_id(ElementID id) {return ramps_.find_by_id(id);};
    NodeCollection<Worker>::iterator find_worker_by_id(ElementID id) {return workers_.find_by_id(id);};
    NodeCollection<Storehouse>::iterator find_storehouse_by_id(ElementID id) {return storehouses_.find_by_id(id);};
    NodeCollection<Ramp>::const_iterator find_ramp_by_id(ElementID id) const {return ramps_.find_by_id(id);};
    NodeCollection<Worker>::const_iterator find_worker_by_id(ElementID id) const {return workers_.find_by_id(id);};
    NodeCollection<Storehouse>::const_iterator find_storehouse_by_id(ElementID id) const {return storehouses_.find_by_id(id);};
    NodeCollection<Ramp>::const_iterator ramp_cbegin(ElementID) const {return ramps_.cbegin();};
    NodeCollection<Worker>::const_iterator worker_cbegin() const {return workers_.cbegin();};
    NodeCollection<Storehouse>::const_iterator storehouse_cbegin()const {return storehouses_.cbegin();};
    NodeCollection<Ramp>::const_iterator ramp_cend() const {return ramps_.cend();};
    NodeCollection<Worker>::const_iterator worker_cend() const {return workers_.cend();};
    NodeCollection<Storehouse>::const_iterator storehouse_cend() const {return storehouses_.cend();};
private:
    NodeCollection<Worker> workers_;
    NodeCollection<Ramp> ramps_;
    NodeCollection<Storehouse> storehouses_;
    void remove_receiver(NodeCollection<IPackageReceiver>& collection, ElementID id);
};


#endif //NETSIM_FACTORY_HPP
