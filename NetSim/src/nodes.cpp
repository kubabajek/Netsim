//
// Created by Kuba on 04.01.2022.
//
#include <optional>
#include "nodes.hpp"
#include "storage_types.hpp"
#include "package.hpp"
#include "helpers.hpp"



void PackageSender::send_package(){
    if(PackageBuffer_!=std::nullopt){
        if (receiver_preferences_.get_preferences().empty())
            std::cout<<"BRAK ODBIORCY WYSLANEGO PRODUKTU!"<<std::endl; //zostawilbym to w kodzie, moze pomoc diagnozowac przyszle problemy
        else{
            Package Package_to_send = Package(std::move(PackageBuffer_.value()));
            PackageBuffer_ = std::nullopt;
            receiver_preferences_.choose_receiver()->receive_package(std::move(Package_to_send));
        }
    }
}

IPackageReceiver* ReceiverPreferences::choose_receiver() {
    double pb = generator_();
    auto iterator = preferences_.begin();
    auto end_it = preferences_.end();
    end_it--;
    double lower_scope=0;
    double higher_scope=iterator->second;
    while(iterator!=end_it){
        if (pb>=lower_scope and pb<higher_scope) {
            break;
        }
        lower_scope=higher_scope;
        iterator++;
        higher_scope+=iterator->second;
    }
    return iterator->first;
}

//                      Zuzia's algorithm
//
//void ReceiverPreferences::add_receiver(IPackageReceiver *r) {
//    if (preferences_.find(r) == preferences_.end()) {
//        double new_receiver_p = 1; //KAZDY NOWY WCHODZI Z PRAWDOPODOBIENSTWEM 1, reszta się przeskaluje!!
//        double all_p_divisor = 1 + new_receiver_p;
//        if (preferences_.empty())
//            all_p_divisor = new_receiver_p;
//        preferences_.insert(std::pair<IPackageReceiver *, double>(r, new_receiver_p));
//        for (auto &preference : preferences_) {
//            preference.second = preference.second / all_p_divisor;
//        }
//    }
//}
//
//void ReceiverPreferences::remove_receiver(IPackageReceiver *r) {
//    if (preferences_.find(r) != preferences_.end()) { //jesli obiekt usuwany juz nie istnieje nic sie nie zrobi
//        double all_p_divisor =
//                1 - preferences_[r]; //jesli =0 oznacza, ze mapa jest pusta, dzielenie w petli sie nie wykona
//        preferences_.erase(r);
//        for (auto &preference : preferences_) {
//            preference.second = preference.second / all_p_divisor;
//        }
//    }
//}

//                  Oskar's algorithm
void ReceiverPreferences::add_receiver(IPackageReceiver *r) {
    if (preferences_.find(r) == preferences_.end()) { //doda sie tylko gdy jeszcze go nie bylo dodanego do odbiorcow
        preferences_.insert(std::pair<IPackageReceiver *, double>(r, 0));
        double each_receiver_p = 1.0 / double(preferences_.size());
        for (auto &preference : preferences_)
            preference.second = each_receiver_p;
    }
}

void ReceiverPreferences::remove_receiver(IPackageReceiver *r) {
    if (preferences_.find(r) != preferences_.end()) { //jesli obiekt usuwany juz nie istnieje nic sie nie zrobi
        preferences_.erase(r);
        if (not(preferences_.empty())) {
            double each_receiver_p = 1.0 / double(preferences_.size());
            for (auto &preference : preferences_)
                preference.second = each_receiver_p;
        }
    }
}


void Ramp::deliver_goods(Time t) { //Wywolywanie w kazdej turze symulacji. Faza: Dostawa
    if ((double(t-1)/double(di_) == floor(double(t-1)/double(di_))) or t==1) // t % di == 0 then spawn package
        push_package(Package());
}

void Worker::do_work(Time t) {
    if (WorkingBuffer_ == std::nullopt) {
        if (not(q_->empty())) {
            WorkingBuffer_ = q_->pop();
            processing_start_time_ = t;
        } else
            WorkingBuffer_ = std::nullopt;
    }
    if (WorkingBuffer_ != std::nullopt)
        if (processing_start_time_ + pd_ -1 == t) { //skonczenie pracy nad produktem (-1 żeby zwrócić produkt w odpowiednim momencie)
            push_package(std::move(WorkingBuffer_.value()));
            WorkingBuffer_ = std::nullopt;
        }
}

Worker::Worker(ElementID id, TimeOffset pd, std::unique_ptr<IPackageQueue> q) {
    id_=id;
    rt_=ReceiverType::WORKER;
    pd_=pd;
    q_=std::move(q);
}

Storehouse::Storehouse(ElementID id, std::unique_ptr<IPackageStockpile> d) {
    id_ = id;
    rt_=ReceiverType::STOREHOUSE;
    d_ = std::move(d);

}
