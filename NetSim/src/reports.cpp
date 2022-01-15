#include "reports.hpp"
#include <sstream>

void generate_structure_report(const Factory& f, std::ostream& os)
{
    char const* receiver_type_string[] = {"worker", "storehouse"};
    const std::string worker_t =  receiver_type_string[(static_cast<int>(ReceiverType::WORKER))];
    const std::string storehouse_t =  receiver_type_string[(static_cast<int>(ReceiverType::STOREHOUSE))];
    os << "\n== LOADING RAMPS ==\n";
    for(auto ramp = f.ramp_cbegin(); ramp != f.ramp_cend(); ramp++)
    {
        os << "\nLOADING RAMP #" << ramp->get_id() << "\n  Delivery interval: " << ramp->get_delivery_interval() << "\n  Receivers:\n";
        for(auto ramp_reciever = ramp->receiver_preferences_.preferences_.begin(); //for worker
            ramp_reciever != ramp->receiver_preferences_.preferences_.end();
            ramp_reciever++)
        {
            std::string receiver_type = receiver_type_string[static_cast<int> (ramp_reciever->first->get_receiver_type())];
            if(worker_t == receiver_type)
            {
                std::string id = std::to_string(ramp_reciever->first->get_id());
                os << "    " << receiver_type << " #" << id << "\n";
            }
        }
        for(auto ramp_reciever = ramp->receiver_preferences_.preferences_.begin(); //for storehouse
            ramp_reciever != ramp->receiver_preferences_.preferences_.end();
            ramp_reciever++)
        {
            std::string receiver_type = receiver_type_string[static_cast<int> (ramp_reciever->first->get_receiver_type())];
            if(storehouse_t == receiver_type)
            {
                std::string id = std::to_string(ramp_reciever->first->get_id());
                os << "    " << receiver_type << " #" << id << "\n";
            }
        }

    }
    char const* queue_type_string[] = {"FIFO", "LIFO"};
    os << "\n\n== WORKERS ==\n";
    for(auto worker = f.worker_cbegin(); worker != f.worker_cend(); worker++)
    {
        std::string queue_type = queue_type_string[static_cast<int> (worker->get_queue()->get_queue_type())];
        os << "\nWORKER #" << worker->get_id() << "\n  Processing time: " << worker->get_processing_duration() << "\n  Queue type: " << queue_type << "\n  Receivers:\n";
        for(auto worker_reciever = worker->receiver_preferences_.preferences_.begin(); //for worker
            worker_reciever != worker->receiver_preferences_.preferences_.end();
            worker_reciever++)
        {
            std::string receiver_type = receiver_type_string[static_cast<int> (worker_reciever->first->get_receiver_type())];
            if(worker_t == receiver_type)
            {
                std::string id = std::to_string(worker_reciever->first->get_id());
                os << "    " << receiver_type << " #" << id << "\n";
            }
        }
        for(auto worker_reciever = worker->receiver_preferences_.preferences_.begin(); //for storehouse
            worker_reciever != worker->receiver_preferences_.preferences_.end();
            worker_reciever++)
        {
            std::string receiver_type = receiver_type_string[static_cast<int> (worker_reciever->first->get_receiver_type())];
            if(storehouse_t == receiver_type)
            {
                std::string id = std::to_string(worker_reciever->first->get_id());
                os << "    " << receiver_type << " #" << id << "\n";
            }
        }
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
