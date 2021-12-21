#ifndef NETSIM_PACKAGE_HPP
#define NETSIM_PACKAGE_HPP
#include "types.hpp"
#include <vector>

class Package{
public:
    Package();
    explicit Package(ElementID);
    Package(Package&&) noexcept;
    Package& operator = (Package&&) noexcept;
    ElementID get_id() const {return ID_;};
    ~Package();

private:
    static void choose_new_free_ID();
    static ElementID FREE_ID_;
    static ElementID HIGHEST_;
    static std::vector<ElementID> FREE_ID_LIST_;
    static std::vector<ElementID> USED_ID_LIST_;
    ElementID ID_;
};


#endif //NETSIM_PACKAGE_HPP

