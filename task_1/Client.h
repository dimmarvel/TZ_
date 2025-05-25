
#pragma once

#include "server/Packet.h"
#include "Lockable.h"
#include <memory>
#include <string>
#include <unordered_map>

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
    Player *player;

    using PacketHandler = std::function<void(const server::Packet &)>;
    std::unordered_map<server::PacketType, PacketHandler> packet_handlers;

    void on_packet(const server::Packet &packet);

    // Примеры обработчиков пакетов
    void params_set(const server::Packet &packet) const;
    void buy(const server::Packet &packet);
    void login(const server::Packet &packet);
};
