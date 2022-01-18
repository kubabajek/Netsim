#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "factory.hpp"
#include "reports.hpp"

#include <functional>

using ::testing::ContainerEq;

void perform_report_check2(std::function<void(std::ostringstream&)>& reporting_function,
                          std::vector<std::string>& expected_report_lines) {
    // Wygeneruj raport.
    std::ostringstream report_oss;
    reporting_function(report_oss);

    std::vector<std::string> output_lines;
    std::istringstream iss;
    iss.str(report_oss.str());
    std::string line;
    while (std::getline(iss, line, '\n')) {
        output_lines.push_back(line);
    }

    std::ostringstream expected_report_oss;
    for (auto& line : expected_report_lines) {
        expected_report_oss << line << "\n";
    }

    ASSERT_EQ(output_lines.size(), output_lines.size());
    std::string delimiter = "----";
    EXPECT_THAT(output_lines, ContainerEq(expected_report_lines))
                        << "Actual (\"" << delimiter << "\" were added for readability):\n"
                        << delimiter << "\n" << report_oss.str() << delimiter << "\n"
                        << "Expected (\"" << delimiter << "\" were added for readability):\n"
                        << delimiter << "\n" << expected_report_oss.str() << delimiter << "\n";

    // Dodane dla czytelności.
    for (std::size_t i = 0; i < output_lines.size(); ++i) {
        ASSERT_EQ(output_lines[i], expected_report_lines[i]) << "(in element no.: " << (i + 1) << ")";
    }
}

void perform_turn_report_check2(const Factory& factory, Time t, std::vector<std::string>& expected_report_lines) {
    std::function<void(std::ostringstream&)> reporting_function = [&factory, t](
            std::ostringstream& oss) { generate_simulation_turn_report(factory, oss, t); };
    perform_report_check2(reporting_function, expected_report_lines);
}

void perform_structure_report_check2(const Factory& factory, std::vector<std::string>& expected_report_lines) {
    std::function<void(std::ostringstream&)> reporting_function = [&factory](
            std::ostringstream& oss) { generate_structure_report(factory, oss); };
    perform_report_check2(reporting_function, expected_report_lines);
}


TEST(ReportsTest, OurTest1) {
    // Utwórz fabrykę.
    Factory factory;

    factory.add_ramp(Ramp(1, 1));
    factory.add_worker(Worker(2, 10, std::make_unique<PackageQueue>(PackageQueueType::LIFO)));
    factory.add_worker(Worker(1, 10, std::make_unique<PackageQueue>(PackageQueueType::FIFO)));
    factory.add_storehouse(Storehouse(2));
    factory.add_storehouse(Storehouse(1));

    Ramp& r1 = *(factory.find_ramp_by_id(1));
    r1.receiver_preferences_.add_receiver(&(*factory.find_worker_by_id(1)));

    Worker& w1 = *(factory.find_worker_by_id(1));
    w1.receiver_preferences_.add_receiver(&(*factory.find_storehouse_by_id(2)));
    w1.receiver_preferences_.add_receiver(&(*factory.find_storehouse_by_id(1)));
    w1.receiver_preferences_.add_receiver(&(*factory.find_worker_by_id(2)));

    Worker& w2 = *(factory.find_worker_by_id(2));
    w2.receiver_preferences_.add_receiver(&(*factory.find_storehouse_by_id(1)));
    // -----------------------------------------------------------------------

    std::vector<std::string> expected_report_lines{
            "",
            "== LOADING RAMPS ==",
            "",
            "LOADING RAMP #1",
            "  Delivery interval: 1",
            "  Receivers:",
            "    worker #1",
            "",
            "",
            "== WORKERS ==",
            "",
            "WORKER #1",
            "  Processing time: 10",
            "  Queue type: FIFO",
            "  Receivers:",
            "    storehouse #1",
            "    storehouse #2",
            "    worker #2",
            "",
            "WORKER #2",
            "  Processing time: 10",
            "  Queue type: LIFO",
            "  Receivers:",
            "    storehouse #1",
            "",
            "",
            "== STOREHOUSES ==",
            "",
            "STOREHOUSE #1",
            "",
            "STOREHOUSE #2",
            "",
    };

    perform_structure_report_check2(factory, expected_report_lines);
}

TEST(ReportsTest, OurTest2) {
    // Utwórz fabrykę.
    Factory factory;

    factory.add_ramp(Ramp(1, 1));
    factory.add_ramp(Ramp(3, 5));
    factory.add_ramp(Ramp(2, 60));
    factory.add_worker(Worker(2, 10, std::make_unique<PackageQueue>(PackageQueueType::LIFO)));
    factory.add_worker(Worker(1, 10, std::make_unique<PackageQueue>(PackageQueueType::FIFO)));
    factory.add_worker(Worker(5, 90, std::make_unique<PackageQueue>(PackageQueueType::FIFO)));
    factory.add_worker(Worker(3, 2, std::make_unique<PackageQueue>(PackageQueueType::FIFO)));
    factory.add_storehouse(Storehouse(2));
    factory.add_storehouse(Storehouse(1));
    factory.add_storehouse(Storehouse(3));
    factory.add_storehouse(Storehouse(4));

    Ramp& r1 = *(factory.find_ramp_by_id(1));
    r1.receiver_preferences_.add_receiver(&(*factory.find_storehouse_by_id(1)));
    r1.receiver_preferences_.add_receiver(&(*factory.find_worker_by_id(2)));

    Ramp& r2 = *(factory.find_ramp_by_id(2));
    r2.receiver_preferences_.add_receiver(&(*factory.find_worker_by_id(3)));
    r2.receiver_preferences_.add_receiver(&(*factory.find_worker_by_id(5)));

    Ramp& r3 = *(factory.find_ramp_by_id(3));
    r3.receiver_preferences_.add_receiver(&(*factory.find_worker_by_id(1)));
    r3.receiver_preferences_.add_receiver(&(*factory.find_worker_by_id(2)));
    r3.receiver_preferences_.add_receiver(&(*factory.find_worker_by_id(3)));
    r3.receiver_preferences_.add_receiver(&(*factory.find_worker_by_id(5)));
    r3.receiver_preferences_.add_receiver(&(*factory.find_storehouse_by_id(3)));
    r3.receiver_preferences_.add_receiver(&(*factory.find_storehouse_by_id(2)));
    r3.receiver_preferences_.add_receiver(&(*factory.find_storehouse_by_id(1)));

    Worker& w1 = *(factory.find_worker_by_id(1));
    w1.receiver_preferences_.add_receiver(&(*factory.find_storehouse_by_id(2)));
    w1.receiver_preferences_.add_receiver(&(*factory.find_storehouse_by_id(1)));
    w1.receiver_preferences_.add_receiver(&(*factory.find_worker_by_id(2)));

    Worker& w2 = *(factory.find_worker_by_id(2));
    w2.receiver_preferences_.add_receiver(&(*factory.find_storehouse_by_id(1)));
    w2.receiver_preferences_.add_receiver(&(*factory.find_storehouse_by_id(4)));
    w2.receiver_preferences_.add_receiver(&(*factory.find_worker_by_id(3)));

    Worker& w3 = *(factory.find_worker_by_id(3));
    w3.receiver_preferences_.add_receiver(&(*factory.find_storehouse_by_id(1)));

    Worker& w5 = *(factory.find_worker_by_id(5));
    w5.receiver_preferences_.add_receiver(&(*factory.find_storehouse_by_id(1)));
    w5.receiver_preferences_.add_receiver(&(*factory.find_storehouse_by_id(3)));
    w5.receiver_preferences_.add_receiver(&(*factory.find_worker_by_id(5)));
    w5.receiver_preferences_.add_receiver(&(*factory.find_worker_by_id(3)));
    w5.receiver_preferences_.add_receiver(&(*factory.find_worker_by_id(1)));


    // -----------------------------------------------------------------------

    std::vector<std::string> expected_report_lines{
            "",
            "== LOADING RAMPS ==",
            "",
            "LOADING RAMP #1",
            "  Delivery interval: 1",
            "  Receivers:",
            "    storehouse #1",
            "    worker #2",
            "",
            "LOADING RAMP #2",
            "  Delivery interval: 60",
            "  Receivers:",
            "    worker #3",
            "    worker #5",
            "",
            "LOADING RAMP #3",
            "  Delivery interval: 5",
            "  Receivers:",
            "    storehouse #1",
            "    storehouse #2",
            "    storehouse #3",
            "    worker #1",
            "    worker #2",
            "    worker #3",
            "    worker #5",
            "",
            "",
            "== WORKERS ==",
            "",
            "WORKER #1",
            "  Processing time: 10",
            "  Queue type: FIFO",
            "  Receivers:",
            "    storehouse #1",
            "    storehouse #2",
            "    worker #2",
            "",
            "WORKER #2",
            "  Processing time: 10",
            "  Queue type: LIFO",
            "  Receivers:",
            "    storehouse #1",
            "    storehouse #4",
            "    worker #3",
            "",
            "WORKER #3",
            "  Processing time: 2",
            "  Queue type: FIFO",
            "  Receivers:",
            "    storehouse #1",
            "",
            "WORKER #5",
            "  Processing time: 90",
            "  Queue type: FIFO",
            "  Receivers:",
            "    storehouse #1",
            "    storehouse #3",
            "    worker #1",
            "    worker #3",
            "    worker #5",
            "",
            "",
            "== STOREHOUSES ==",
            "",
            "STOREHOUSE #1",
            "",
            "STOREHOUSE #2",
            "",
            "STOREHOUSE #3",
            "",
            "STOREHOUSE #4",
            "",
    };

    perform_structure_report_check2(factory, expected_report_lines);
}