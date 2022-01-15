#ifndef NETSIM_REPORTS_HPP
#define NETSIM_REPORTS_HPP
#include "factory.hpp"
#include <set>
#include <utility>

void generate_structure_report(const Factory& f, std::ostream& os);
void generate_simulation_turn_report(const Factory& f , std::ostream& os, Time t);
class SpecificTurnsReportNotifier {
public:
    explicit SpecificTurnsReportNotifier(std::set<Time> turns) : turns_(std::move(turns)) {};
    bool should_generate_report(Time t);
private:
    std::set<Time> turns_;
};

class IntervalReportNotifier {
public:
    explicit IntervalReportNotifier(TimeOffset to) : to_(to) {};
    bool should_generate_report(Time t);
private:
    TimeOffset to_;
};



#endif //NETSIM_REPORTS_HPP
