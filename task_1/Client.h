
#pragma once

#include "server/Packet.h"
#include "Lockable.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <memory>

class IO;
class Requests;
class Player;
class ClientEvent;

class Client final : public Lockable
{
public:
    explicit Client(IO *io);
    ~Client();

    void disconnect() const;
    void send(const server::Packet &packet) const;
    void on_event(const ClientEvent &event);

private:
    IO *io;
    std::shared_ptr<Requests> requests;
    std::shared_ptr<Player> player; // nullptr if didnt login in

    using PacketHandler = std::function<void(const server::Packet &)>;
    std::unordered_map<server::PacketType, PacketHandler> packet_handlers;

    void on_packet(const server::Packet &packet);

    // Return true if login in
    [[nodiscard]] bool is_authorized() const noexcept { return !!player; }
    // If didnt authorized - send error and return false
    bool ensure_authorized(server::PacketType bad_call);

    // Примеры обработчиков пакетов
    void params_set(const server::Packet &packet) const;
    void buy(const server::Packet &packet);
    void login(const server::Packet &packet);
};
