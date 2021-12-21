#include <algorithm>
#include "package.hpp"
#include "types.hpp"
#include "storage_types.hpp"

ElementID Package::FREE_ID_ = 0;
ElementID Package::HIGHEST_ = 0;
std::vector<ElementID> Package::FREE_ID_LIST_ = {};
std::vector<ElementID> Package::USED_ID_LIST_ = {};

void Package::choose_new_free_ID(){
    if (FREE_ID_LIST_.empty()){
        if( HIGHEST_ > FREE_ID_)
            FREE_ID_ = HIGHEST_;
        else{
            FREE_ID_++;
            HIGHEST_ = FREE_ID_;
        }
    }
    else
    {
        auto result = std::min_element(FREE_ID_LIST_.begin(), FREE_ID_LIST_.end());
        HIGHEST_ = FREE_ID_;
        FREE_ID_ = *result;
    }
}

Package::Package(){
    ID_ = FREE_ID_;
    USED_ID_LIST_.push_back(ID_);
    choose_new_free_ID();
}


Package::Package(const ElementID new_id){
    if(std::find(USED_ID_LIST_.begin(), USED_ID_LIST_.end(), new_id) != USED_ID_LIST_.end())
        throw std::runtime_error("Id is already used");
    ID_ = new_id;
    FREE_ID_LIST_.erase(std::find(FREE_ID_LIST_.begin(), FREE_ID_LIST_.end(), new_id));
    if (new_id == FREE_ID_)
        choose_new_free_ID();
    USED_ID_LIST_.push_back(ID_);
}


Package::~Package(){
    FREE_ID_LIST_.push_back(ID_);
    USED_ID_LIST_.erase(std::find(FREE_ID_LIST_.begin(), FREE_ID_LIST_.end(), ID_));
    choose_new_free_ID();
}

