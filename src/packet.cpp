#include "packet.h"

Packet::Packet(
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
) {
    bad_packet = false;
    dynamic_byte = 0;
    uint8_t header_bytes = 5;
    if (direct_path) {
        if (path_length < 32) {
            _direct_path = true;
            _has_destination = true;
            dynamic_byte |= 0b10000000;
            _path_length = path_length;
            dynamic_byte |= path_length;
            // destination is implied when you have a path
            header_bytes += path_length + 1;
            _destination_adress = destination;
            for (int i = 0; i<path_length; i++) {
                _phy_packet[i + header_bytes] = path[i];
            }
        }
        else {
            bad_packet = true;
        }
    }
    else {
        _direct_path = false;
        if (has_destination) {
            if (hop_limit < 64) {
                _phy_packet[5] = destination;
                _has_destination = true;
                dynamic_byte |= 0b01000000;
                dynamic_byte |= hop_limit;
                _path_length = 0;
                _hop_limit = hop_limit;
                header_bytes += 1;
            }
            else {
                bad_packet = true;
            }
            
        }
        else {
            _has_destination = false;
        }
    }
    _phy_packet[0] = dynamic_byte;
    _source_adress = source_address;
    _phy_packet[1] = source_address;
    _packet_number = packet_number;
    _phy_packet[2] = (uint8_t) (packet_number >> 8);
    _phy_packet[3] = (uint8_t) packet_number;
    if (payload_size <= 255 - header_bytes) {
        _payload_size = payload_size;
        _phy_packet[4] = payload_size;
    }
    else {
        bad_packet = true;
    }
    for (int i = 0; i<header_bytes; i++) {
        _phy_packet[i + header_bytes] = payload[i];
    }
}

Packet::Packet(uint8_t packet_size, uint8_t *phy_packet) {
    // Check if larger than minimum header size, if not it's a bad packet
    bad_packet = false;
    if (packet_size >= 5) {
        dynamic_byte = phy_packet[0];
        _source_adress = phy_packet[1];
        _packet_number = ((uint16_t) phy_packet[2]) << 8 + ((uint16_t) phy_packet[3]);
        _payload_size = phy_packet[4];
        if (packet_size >= 6) {
            _has_destination = true;
            _destination_adress = phy_packet[5];
            if (packet_size > 6) {
                _direct_path = true;
                _path_length = (dynamic_byte << 2) >> 2;
            }
            else {
                _hop_limit = (dynamic_byte << 1) >> 1;
            }
        }
        else {
            _has_destination = false;
        }
        for (int i = 0; i<packet_size; i++) {
            _phy_packet[i] = phy_packet[i];
        }

    }
    else {
        bad_packet = true;
    }
}

bool Packet::equal_to(Packet other) {
    return this->bad_packet == other.bad_packet
        && this->dynamic_byte == other.dynamic_byte
        && this->_destination_adress == other._destination_adress
        && this->_direct_path == other._direct_path
        && this->_has_destination == other._has_destination
        && this->_hop_limit == other._hop_limit
        && this->_packet_number == other._packet_number
        && this->_path_length == other._path_length
        && this->_payload_size == other._payload_size
        && this->_source_adress == other._source_adress;

}