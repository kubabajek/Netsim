#include <cmath>
#include <iostream>
#include "package.hpp"
#include "types.hpp"
#include "storage_types.hpp"
#include "helpers.hpp"
#include "nodes.hpp"
#include <map>

void print(std::vector<ElementID> a) {
    for (unsigned int i : a)
        std::cout<<i<<" ";
    std::cout<<std::endl;
}
void bufferstatus (Ramp& rampa,std::string message=""){
    bool a = rampa.get_sending_buffer().has_value();
    if (a)
        std::cout<<"BUFOR JEST PELNY!"<<message<<std::endl;
    else
        std::cout<<"BUFOR JEST PUSTY!"<<message<<std::endl;
}
void bufferstatus (Worker& robol,std::string message=""){
    bool a = robol.get_sending_buffer().has_value();
    if (a)
        std::cout<<"BUFOR JEST PELNY!"<<message<<std::endl;
    else
        std::cout<<"BUFOR JEST PUSTY!"<<message<<std::endl;
}

int main() {
    std::cout << "Welcome in spiulkolot! =^.^=" << std::endl;


    //Test dodawania odbiorcy do preferences_ w ReceiverPreferenes
    ReceiverPreferences rp;
    Storehouse magazyn1 = Storehouse(1);
    Storehouse magazyn2 = Storehouse(2);
    Storehouse magazyn3 = Storehouse(3);
    rp.add_receiver(&magazyn1);
    rp.add_receiver(&magazyn2);
    rp.add_receiver(&magazyn2); //sprawdzenie czy dodanie jeszcze raz istniejacego nie rozwali prawdopodobienstw
    rp.add_receiver(&magazyn3);
    auto it = rp.begin();
    it++;
    it++;
    std::cout<<it->second<<std::endl;



    //Test Rampy
    Ramp r(1,2);
    auto recv = std::make_unique<Storehouse>(1);
    r.receiver_preferences_.add_receiver(recv.get());
    r.deliver_goods(1);
    r.send_package();
    r.deliver_goods(2);
    r.deliver_goods(3);
    r.send_package();
    r.deliver_goods(4);
    r.deliver_goods(5);
    r.send_package();
    bufferstatus(r," (Test Rampy - czy pojawil sie produkt w buforze)");


    //Test Workera
    Worker w(1, 2, std::make_unique<PackageQueue>(PackageQueueType::FIFO));
    Storehouse magazyn5 = Storehouse(5);
    w.receiver_preferences_.add_receiver(&magazyn5);
    w.receive_package(Package(4)); //UWAGA NA ZAJETE WCZESNIEJ ID!!! JESLI JEST ZAJETE BEDZIE 0
    w.do_work(1);
    w.receive_package(Package(5));
    w.receive_package(Package(6));
    w.receive_package(Package(7));
    w.do_work(2);
    w.do_work(3);
    w.do_work(4);
    w.do_work(5);
    w.do_work(6);
    //w.do_work(7);

    bufferstatus(w," (Test Workera - czy ma produkt na rece (w buforze))");
    auto& buffer = w.get_sending_buffer();
    if(buffer.has_value())
        std::cout<<"ID produktu z bufora "<<buffer.value().get_id()<<std::endl;



//    //Test ChooseID
//    //pb ustawione na 0.30
//    ReceiverPreferences rp2;
//    Storehouse magazyn10 = Storehouse(10);
//    Storehouse magazyn20 = Storehouse(20);
//    Storehouse magazyn30 = Storehouse(30);
//    rp2.add_receiver(&magazyn10);
//    rp2.add_receiver(&magazyn20);
////    rp2.add_receiver(&magazyn30);
////    auto i = rp2.preferences_.begin();
////    i++;
////    std::cout<<i->second<<"!!!";
//
//    auto ite = rp2.choose_receiver();
//    ElementID idtest = ite->get_id();
//    if (rp2.begin()->first == &magazyn10)
//        std::cout<<"Wanted ID: 10"<<std::endl;
//    else
//        std::cout<<"Wanted ID: 20"<<std::endl;
//    std::cout<<"Choosen ID: "<<idtest<<std::endl;


    return 0;
}
