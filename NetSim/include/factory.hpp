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

    NodeCollection<Node>::iterator find_by_id( ElementID id){
        return std::find_if(nodes_.begin(),--nodes_.end(),[id](const auto& node){return node.get_id() == id;});
    }
    NodeCollection<Node>::const_iterator find_by_id( ElementID id) const{
        return std::find_if(nodes_.begin(),--nodes_.end(),[id](const auto& node){return node.get_id() == id;});
    }

    void add(Node&& node){
        nodes_.emplace_back(std::move(node));
    };
    void remove_by_id(ElementID id) {
        const_iterator it = find_by_id(id);
        if (it->get_id() == id)
            nodes_.erase(it);
        else
            std::cout<<"Object to delete doesn't exist in collection"<<std::endl;
    }

private:
    container_t nodes_;
};


class Factory{
public:
    void add_ramp(Ramp&& ramp_to_add) {ramps_.add(std::move(ramp_to_add));};
    void add_worker(Worker&& worker_to_add) {workers_.add(std::move(worker_to_add));};
    void add_storehouse(Storehouse&& storehouse_to_add){storehouses_.add(std::move(storehouse_to_add));};
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
    bool is_consistent();
    void do_deliveries(Time);
    void do_package_passing();
    void do_work(Time);
private:
    NodeCollection<Worker> workers_;
    NodeCollection<Ramp> ramps_;
    NodeCollection<Storehouse> storehouses_;
    void remove_receiver(NodeCollection<Worker>& collection, ElementID id) {collection.remove_by_id(id);};
    void remove_receiver(NodeCollection<Storehouse>& collection, ElementID id) {collection.remove_by_id(id);};
};


#endif //NETSIM_FACTORY_HPP
