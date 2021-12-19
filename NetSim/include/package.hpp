//
// Created by Kuba on 17.12.2021.
//
#ifndef NETSIM_PACKAGE_HPP
#define NETSIM_PACKAGE_HPP
#include "types.hpp"
#include <vector>

class Package
{
private:
    Package();
    Package(ElementID);
    Package(Package&&);
    Package& operator = ( Package&& );
    ElementID get_id() {return ID_;};
    static void choose_new_free_ID();
    ~Package();

    static ElementID FREE_ID_;
    static ElementID HIGHEST_;
    static std::vector<ElementID> FREE_ID_LIST_;
    static std::vector<ElementID> USED_ID_LIST_;


    ElementID ID_;
};

#endif //NETSIM_PACKAGE_HPP
