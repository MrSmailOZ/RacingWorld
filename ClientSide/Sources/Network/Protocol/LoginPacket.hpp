/*
 * Copyright 2018 Vladimir Balun
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include "EPacketType.hpp"
#include "NetworkPacket.hpp"
#include "../../Utils/Configuration.hpp"

namespace Platforms { namespace Network { namespace Protocol {

    #pragma pack(push, 1)

    class LoginPacket : public NetworkPacket<LoginPacket>, IPacketToServer
    {
    public:
        explicit LoginPacket() : NetworkPacket(LOGIN_PACKET) {}
        void setEmail(const wchar_t* email);
        void setPassword(const wchar_t* password);
        void setPacketNumber(std::int32_t number) noexcept;
        void setEmailSize(std::int16_t emailSize) noexcept;
        void setPasswordSize(std::int16_t passwordSize) noexcept;
        char* toBuffer() noexcept;
    private:
        wchar_t mEmail[Configuration::Game::MAX_SIZE_EMAIL];
        std::int16_t mEmailSize;
        wchar_t mPassword[Configuration::Game::MAX_SIZE_PASSWORD];
        std::int16_t mPasswordSize;
    };

    #pragma pack(pop)

    inline void LoginPacket::setEmail(const wchar_t* email)
    {
        for (std::uint16_t i = 0; i < Configuration::Game::MAX_SIZE_EMAIL; i++)
            mEmail[i] = email[i];
    }

    inline void LoginPacket::setPassword(const wchar_t* password)
    {
        for (std::uint8_t i = 0; i < Configuration::Game::MAX_SIZE_PASSWORD; i++)
            mPassword[i] = password[i];
    }

    inline void LoginPacket::setPacketNumber(std::int32_t number) noexcept
    {
        mPacketNumber = number;
    }

    inline void LoginPacket::setEmailSize(std::int16_t emailSize) noexcept
    {
        mEmailSize = emailSize;
    }

    inline void LoginPacket::setPasswordSize(std::int16_t passwordSize) noexcept
    {
        mPasswordSize = passwordSize;
    }

    inline char* LoginPacket::toBuffer() noexcept
    {
        return reinterpret_cast<char*>(this);
    }

} } }