//
// Created by Kuba on 17.12.2021.
//

#ifndef NETSIM_PACKAGE_HPP
#define NETSIM_PACKAGE_HPP
#include "types.hpp"
class Package
{
    Package();
    Package(ElementID id);
    Package(Package&&);
    Package& operator= ( Package&& );

    ElementID get_id();
    virtual ~Package()


};

#endif //NETSIM_PACKAGE_HPP
