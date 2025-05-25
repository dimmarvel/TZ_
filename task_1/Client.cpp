
#include "Client.h"
#include "IO.h"
#include "Requests.h"
#include "Player.h"
#include "ClientEvent.h"
#include "Log.h"

Client::Client(IO *io) :
    io(io), player(nullptr)
{
    this->requests = std::make_shared<Requests>(this);
    packet_handlers = {
        { server::PacketType::PARAMS_SET, [this](const auto &p){ this->params_set(p); } },
        { server::PacketType::BUY,        [this](const auto &p){ this->buy(p);        } },
        { server::PacketType::LOGIN,      [this](const auto &p){ this->login(p);      } }
    };
}

Client::~Client() = default;

void Client::disconnect() const
{
    this->io->stop();
}

void Client::send(const server::Packet &packet) const
{
    io->write(packet.serialize());
}

void Client::on_event(const ClientEvent &event)
{
    // Пример обработки событий клиента
    switch (event.get_type())
    {
        case ClientEvent::Type::DISCONNECT:
            this->disconnect();
            break;

        default:
            logger->warning("Unhandled event type {}", event.get_type());
    }
}

void Client::on_packet(const server::Packet &packet)
{
    const auto it = packet_handlers.find(packet.get_type());
    if (it != packet_handlers.end())
        it->second(packet);
    else
        logger->warning("Unhandled packet type {}", static_cast<int>(packet.get_type()));

}

void Client::params_set(const server::Packet &packet) const
{
    auto params = packet.get_params();

    if (!params)
        return;

    player->params->set(params);
    logger->info("Client {} params set", player->id);
}

void Client::buy(const server::Packet &packet)
{
    auto item_id = packet.get_item_id();

    if (!player->balance->can_afford(item_id))
    {
        logger->warning("Client {} can't afford item {}", player->id, item_id);
        return;
    }

    player->balance->deduct(item_id);
    player->inventory->add(item_id);

    logger->info("Client {} bought item {}", player->id, item_id);
}

void Client::login(const server::Packet &packet)
{
    auto credentials = packet.get_credentials();

    if (!credentials.valid())
    {
        logger->warning("Invalid login attempt from client");
        return;
    }

    player = Player::authorize(credentials);

    if (!player)
    {
        logger->warning("Authorization failed for credentials {}", credentials.username);
        return;
    }

    logger->info("Client {} logged in successfully", player->id);
}
