#include "gattlib.h"

#include <array>
#include <iostream>
#include <sstream>
#include <string>
#include <memory>
#include <chrono>
#include <ctime>
#include <iomanip>

using TimeFormat = std::array<char,5>;

// https://codereview.stackexchange.com/questions/175353/getting-current-timezone
TimeFormat generate_send_data()
{
    TimeFormat send_data;
    auto t = std::time(nullptr);
    uint32_t unix_timestamp = std::chrono::seconds(t).count();
    auto const tm = *std::localtime(&t);
    std::ostringstream os;
    os << std::put_time(&tm, "%z");
    std::string s = os.str();
    // s is in ISO 8601 format: "±HHMM"
    char h = std::stoi(s.substr(0,3), nullptr, 10);
    //int m = std::stoi(s[0]+s.substr(3), nullptr, 10);
    //assume little endian
    memcpy(send_data.data(), &unix_timestamp, 4);
    send_data[4] = h;
    return send_data;
}

static void ble_discovered_device(void *adapter, const char* addr, const char* name, void *user_data) {
    // std::cout << addr << std::endl;
    return;
}

gatt_connection_t* connect(const char* mac_addr) {
    gatt_connection_t* connection = gattlib_connect(nullptr, mac_addr, GATTLIB_CONNECTION_OPTIONS_LEGACY_DEFAULT);
	if (connection != nullptr)
        return connection;
    
    void* adapter;
    if (auto ret = gattlib_adapter_open(nullptr, &adapter);ret) {
        throw std::runtime_error("Failed to open adapter");
    }
    if (auto ret = gattlib_adapter_scan_enable(adapter, ble_discovered_device, 6, nullptr); ret){
        throw std::runtime_error("Failed to scan.");
    }
    // reconnect
    connection = gattlib_connect(nullptr, mac_addr, GATTLIB_CONNECTION_OPTIONS_LEGACY_DEFAULT);
    if (connection == nullptr) {
        throw std::runtime_error("Failed to connect to the bluetooth device.");
    }

    return connection;
}

int main (void) {
    constexpr auto uuid_time_char = "EBE0CCB7-7A0A-4B0C-8A1A-6FF2997DA3A6";
    constexpr auto mac = "E7:50:59:21:84:AC";

    std::unique_ptr<uuid_t> uuid_time(new uuid_t());
    if (auto err = gattlib_string_to_uuid(uuid_time_char, strlen(uuid_time_char), uuid_time.get()); err != GATTLIB_SUCCESS) {
       std::cerr << "Failed to convert uuid_char to uuid_t" << std::endl; 
       return 1;
    }

    gatt_connection_t* connection;
    try {
        connection = connect(mac);
    }
    catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    TimeFormat send_data = generate_send_data();
    gattlib_write_char_by_uuid(connection, uuid_time.get(), send_data.data(), sizeof(send_data));
    gattlib_disconnect(connection);
    return 0;
}