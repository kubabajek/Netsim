#include <algorithm>
#include "package.hpp"
#include "types.hpp"
#include "storage_types.hpp"

ElementID Package::FREE_ID_ = 0;
ElementID Package::HIGHEST_ = 0;
std::vector<ElementID> Package::FREE_ID_LIST_ = {};
std::vector<ElementID> Package::USED_ID_LIST_ = {};

void Package::choose_new_free_ID()
{
    //printf("==choose_new_free_ID==\n");
    if (FREE_ID_LIST_.empty())
    {
        //printf("Highest is: %u\n", HIGHEST_);
        //printf("Free is: %u\n", FREE_ID_);
        if( HIGHEST_ > FREE_ID_)
            FREE_ID_ = HIGHEST_;
        else
        {
            FREE_ID_ = ++HIGHEST_;
            HIGHEST_ = FREE_ID_;
        }
        if (!(USED_ID_LIST_.empty()))
        {
            while (std::find(USED_ID_LIST_.begin(), USED_ID_LIST_.end(), FREE_ID_) != USED_ID_LIST_.end())
            {
                FREE_ID_++;
                HIGHEST_ = FREE_ID_;
            }
        }
    }
    else
    {
        std::vector<ElementID>::iterator it = std::min_element(FREE_ID_LIST_.begin(), FREE_ID_LIST_.end());
        ElementID smallest_of_free = FREE_ID_LIST_.at(std::distance(FREE_ID_LIST_.begin(), it));
        HIGHEST_ = FREE_ID_;
        FREE_ID_ = smallest_of_free;
    }
    //printf("New free ID choosen: %u\n", FREE_ID_);
}

Package::Package()
{
    choose_new_free_ID();
    ID_ = FREE_ID_;
    USED_ID_LIST_.push_back(ID_);
    //choose_new_free_ID();
//    for (auto used_id : USED_ID_LIST_)
//        printf("used is: %u; ", used_id);
//    printf("\n");
}

Package::Package(const ElementID new_id)
{
    /* OLD

    if (!(std::find(USED_ID_LIST_.begin(), USED_ID_LIST_.end(), new_id) != USED_ID_LIST_.end()))
    {

        //throw std::runtime_error("Id is already used");
    }
    ID_ = new_id;
    if (!(FREE_ID_LIST_.empty()))
            FREE_ID_LIST_.erase(std::find(FREE_ID_LIST_.begin(), FREE_ID_LIST_.end(), new_id));
        if (0 != ID_)
            USED_ID_LIST_.push_back(ID_);
    */
    //printf("==Package(ElementID) Cnstr==\nnew_id given: %u\n", new_id);
    if ((std::find(USED_ID_LIST_.begin(), USED_ID_LIST_.end(), new_id) == USED_ID_LIST_.end()))
    {
        if (0 != new_id)
        {
            USED_ID_LIST_.push_back(new_id);
            if (!(FREE_ID_LIST_.empty()))
                FREE_ID_LIST_.erase(std::find(FREE_ID_LIST_.begin(), FREE_ID_LIST_.end(), new_id));
        }
    }
//    for (auto used_id : USED_ID_LIST_)
//        printf("used is: %u; ", used_id);
//    printf("\n");
    ID_ = new_id;
}

Package::Package(Package&& old) noexcept
{
    ID_= old.ID_;
    old.ID_ = 0;
}

Package& Package::operator = (Package&& old) noexcept
{
    ID_ = old.ID_;
    old.ID_ = 0;
    return *this;
}

Package::~Package()
{
    //printf("DESTRUCTION\n");
    if (0 != ID_)
    {
        FREE_ID_LIST_.push_back(ID_);
        //printf("pushed back to free: %u\n", FREE_ID_LIST_.back());
        if ((std::find(USED_ID_LIST_.begin(), USED_ID_LIST_.end(), ID_) != USED_ID_LIST_.end()))
            USED_ID_LIST_.erase(std::find(USED_ID_LIST_.begin(), USED_ID_LIST_.end(), ID_));
        //printf("erased from used: %u\n", ID_);
//        for (auto used_id : USED_ID_LIST_)
//            printf("used is: %u; ", used_id);
//        printf("\n");
    }
}


