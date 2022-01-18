#include "reports.hpp"
#include <sstream>

void generate_structure_report(const Factory& f, std::ostream& os)
{
    os << "\n== LOADING RAMPS ==\n";
    for(auto ramp = f.ramp_cbegin(); ramp != f.ramp_cend(); ramp++){
        os << "\nLOADING RAMP #" << ramp->get_id() << "\n  Delivery interval: " << ramp->get_delivery_interval() << "\n  Receivers:\n";

        std::map<ElementID, IPackageReceiver*> ramp_receivers_map_only_storehouses;
        std::map<ElementID, IPackageReceiver*> ramp_receivers_map_only_workers;
        for (auto receiver : ramp->receiver_preferences_.preferences_) {
            if (receiver.first->get_receiver_type() == ReceiverType::STOREHOUSE)
                ramp_receivers_map_only_storehouses.insert({receiver.first->get_id(), receiver.first});
            else if (receiver.first->get_receiver_type() == ReceiverType::WORKER)
                ramp_receivers_map_only_workers.insert({receiver.first->get_id(), receiver.first});
        }

        for (auto receiver : ramp_receivers_map_only_storehouses)
                os << "    " << "storehouse" << " #" << std::to_string(receiver.second->get_id()) << "\n";

        for (auto receiver : ramp_receivers_map_only_workers)
                os << "    " << "worker" << " #" << std::to_string(receiver.second->get_id()) << "\n";

    }

    char const* queue_type_string[] = {"FIFO", "LIFO"};
    os << "\n\n== WORKERS ==\n";
    for(auto worker = f.worker_cbegin(); worker != f.worker_cend(); worker++)
    {
        std::string queue_type = queue_type_string[static_cast<int> (worker->get_queue()->get_queue_type())];
        os << "\nWORKER #" << worker->get_id() << "\n  Processing time: " << worker->get_processing_duration() << "\n  Queue type: " << queue_type << "\n  Receivers:\n";

        std::map<ElementID, IPackageReceiver*> worker_receivers_map_only_storehouses;
        std::map<ElementID, IPackageReceiver*> worker_receivers_map_only_workers;
        for (auto receiver : worker->receiver_preferences_.preferences_) {
            if (receiver.first->get_receiver_type() == ReceiverType::STOREHOUSE)
                worker_receivers_map_only_storehouses.insert({receiver.first->get_id(), receiver.first});
            else if (receiver.first->get_receiver_type() == ReceiverType::WORKER)
                worker_receivers_map_only_workers.insert({receiver.first->get_id(), receiver.first});
        }

        for (auto receiver : worker_receivers_map_only_storehouses)
                os << "    " << "storehouse" << " #" << std::to_string(receiver.second->get_id()) << "\n";

        for (auto receiver : worker_receivers_map_only_workers)
                os << "    " << "worker" << " #" << std::to_string(receiver.second->get_id()) << "\n";
    }

    os << "\n\n== STOREHOUSES ==\n";
    for(auto storehouse = f.storehouse_cbegin(); storehouse != f.storehouse_cend(); storehouse++)
    {
        os << "\nSTOREHOUSE #" << storehouse->get_id() << "\n";
    }
    os << "\n";
//    std::stringstream ss;
//    ss << os.rdbuf();
//    std::string myString = ss.str();
//    std::cout << myString << std::endl;

}

void generate_simulation_turn_report(const Factory& f , std::ostream& os, Time t)
{
    std::string turn_number = std::to_string(t);
    os << "=== [ Turn: " << turn_number << " ] ===\n";
    os << "\n== WORKERS ==\n";
    for(auto worker = f.worker_cbegin(); worker != f.worker_cend(); worker++)
    {
        std::string processing_time = std::to_string(t + 1 - worker->get_package_processing_start_time());
        os << "\nWORKER #" << worker->get_id() << "\n";
        if (worker->get_processing_buffer() == std::nullopt)
        {
            os << "  PBuffer: (empty)\n";
        }
        else
        {
            std::string pblement_id = std::to_string(worker->get_processing_buffer().value().get_id());
            os << "  PBuffer: #" << pblement_id << " (pt = " << processing_time << ")\n";
        }
        os << "  Queue: ";
        auto product = worker->get_queue()->begin();
        if (product != worker->get_queue()->end())
        {
            os << "#" << product->get_id();
            product++;
            for (; product != worker->get_queue()->end(); product++)
                os << ", #" << product->get_id();
        }
        else
        {
            os << "(empty)";
        }
        std::string sending_product_it;
        if (worker->get_sending_buffer() == std::nullopt)
        {
            sending_product_it = "(empty)\n";
            os << "\n  SBuffer: " << sending_product_it;
        }
        else
        {
            sending_product_it = std::to_string(worker->get_sending_buffer()->get_id());
            os << "\n  SBuffer: #" << sending_product_it << "\n";
        }

    }
    os << "\n\n== STOREHOUSES ==\n";
    for(auto storehouse = f.storehouse_cbegin(); storehouse != f.storehouse_cend(); storehouse++)
    {
        os << "\nSTOREHOUSE #" << storehouse->get_id() << "\n";
        os << "  Stock: ";
        auto product = storehouse->begin();
        if (product != storehouse->end())
        {
            os << "#" << product->get_id();
            product++;
            for (; product != storehouse->end(); product++)
                os << ", #" << storehouse->get_id();
        }
        else
        {
            os << "(empty)";
        }
    }
    os << "\n";
    os << std::endl;
//    std::stringstream ss;
//    ss << os.rdbuf();
//    std::string myString = ss.str();
//    std::cout << myString << std::endl;
}

bool SpecificTurnsReportNotifier::should_generate_report(Time t) {
    bool is_time_in_set = false;
    if (turns_.find(t) != turns_.end())
        is_time_in_set = true;
    return is_time_in_set;
}

bool IntervalReportNotifier::should_generate_report(Time t) {
    bool is_time_to_report = false;
    if (!(t-1 % to_))
        is_time_to_report = true;
    return is_time_to_report;
}
