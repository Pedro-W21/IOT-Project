#include <cstdint>

using AddressSize = uint8_t;

class Packet {
    public:
        Packet(
            AddressSize source_address,
            uint16_t packet_number,
            uint8_t payload_size,
            uint8_t *payload,
            bool direct_path,
            uint8_t path_length,
            AddressSize *path,
            bool has_destination,
            AddressSize destination,
            uint8_t hop_limit
        );
        Packet(
            uint8_t packet_size,
            uint8_t *phy_packet
        );
    private:
        uint8_t _phy_packet[255];

        uint8_t dynamic_byte;
        uint8_t _payload_size;
        AddressSize _source_adress;
        uint16_t _packet_number;

        uint8_t total_size;

        /// both derived from dynamic byte when reading a packet, dynamic byte is derived from those when creating a packet
        bool _direct_path;
        uint8_t _path_length;

        bool _has_destination;
        uint8_t _hop_limit;

        bool bad_packet;
        

        /// Not guaranteed to be defined from this point
        AddressSize _destination_adress;

};