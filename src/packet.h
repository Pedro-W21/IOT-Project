#include <cstdint>

using AddressSize = uint8_t;

class Packet {
    public:
        Packet();
    private:
        uint8_t phy_packet[255];

        uint8_t dynamic_byte;
        uint8_t payload_size;
        AddressSize source_address;
        uint16_t packet_number;

        /// both derived from dynamic byte when reading a packet, dynamic byte is derived from those when creating a packet
        bool direct_path;
        uint8_t path_length;

        bool has_destination;
        uint8_t hop_limit;
        

        /// Not guaranteed to be defined from this point
        AddressSize destination_addr;

};