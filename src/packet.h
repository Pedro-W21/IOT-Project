#include <cstdint>

class Packet {
    public:
        Packet();
    private:
        uint8_t dynamic_byte;
        uint8_t payload_size;
        uint16_t source_address;
        uint16_t packet_number;

        /// both derived from dynamic byte when reading a packet, dynamic byte is derived from those when creating a packet
        bool direct_path;
        uint8_t path_length;

        bool has_destination;
        uint8_t hop_limit;
        

        /// Not guaranteed to be defined from this point
        uint16_t destination_addr;
        uint16_t* path_addrs;

};