#ifndef COMMAND_H
#define COMMAND_H

#include <string>
#include <cstring>
#include <stdexcept>

struct Command {
    uint8_t type;         // how the recived data will be handled
    uint32_t dataSize;    // Size of the data
    char data[256];          // Data payload (e.g., player position, name)

    // Create Command from string
    static Command* Create(uint8_t type, std::string str) {
        Command* cmd = new Command();
        cmd->type = type;
		cmd->dataSize = str.size();
		memcpy(cmd->data, str.data(), str.size());
        return cmd;
    }

    // Convert Command to string
    std::string ToString() const {
        std::string result;
        result += static_cast<char>(type); // Append command type
        result += std::string(reinterpret_cast<const char*>(&dataSize), sizeof(dataSize)); // Append dataSize
        result += std::string(data, dataSize); // Append data
        return result;
    }

    // Convert string to Command 
    static Command* FromString(const std::string& str) {
        Command* cmd = new Command();
        if (str.size() < sizeof(uint8_t) + sizeof(uint32_t)) {
            throw std::invalid_argument("Invalid command string");
        }

        cmd->type = static_cast<uint8_t>(str[0]); // Read command type
        cmd->dataSize = *reinterpret_cast<const uint32_t*>(str.data() + sizeof(uint8_t)); // Read dataSize

        if (cmd->dataSize > sizeof(cmd->data) || str.size() < sizeof(uint8_t) + sizeof(uint32_t) + cmd->dataSize) {
            throw std::invalid_argument("Invalid command data size");
        }

        std::memcpy(cmd->data, str.data() + sizeof(uint8_t) + sizeof(uint32_t), cmd->dataSize); // Read data
        return cmd;
    }
};
#endif