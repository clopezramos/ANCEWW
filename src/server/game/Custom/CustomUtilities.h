/*
 * Copyright (C) 2018 AzerothCore <www.azerothcore.org>
 * Copyright (C) 2018 clopezramos <https://github.com/clopezramos>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CUSTOM_UTILITIES_H
#define CUSTOM_UTILITIES_H

#include "Common.h"
#include <string>
#include <unordered_map>
#include <memory>
#include <type_traits>

class DataMap
{
    public:
        /**
         * Base class that you should inherit in your script.
         * Inheriting classes can be stored to DataMap
         */
        class Base
        {
        public:
            virtual ~Base() = default;
        };

        /**
         * Default constructs value on the given key, if value already exists, do nothing
         */
        template<class T>
        bool Construct(std::string const& key)
        {
            static_assert(std::is_base_of<Base, T>::value, "T must derive from Base");

            auto itr = _container.find(key);
            if (itr != _container.end())
                return false;

            auto pair = _container.emplace(key, std::make_unique<T>());
            return pair.second;
        }

        /**
         * Returns a pointer to object of requested type stored with given key or nullptr
         */
        template<class T>
        T* Get(std::string const& key) const
        {
            static_assert(std::is_base_of<Base, T>::value, "T must derive from Base");
            if (_container.empty())
                return nullptr;

            auto itr = _container.find(key);
            if (itr != _container.end())
                return dynamic_cast<T*>(itr->second.get());
            return nullptr;
        }

        /**
         * Returns a pointer to object of requested type stored with given key
         * or default constructs one and returns that one
         */
        template<class T, typename std::enable_if<std::is_default_constructible<T>::value, int>::type = 0>
        T* GetDefault(std::string const& key)
        {
            static_assert(std::is_base_of<Base, T>::value, "T must derive from Base");
            if (T* value = Get<T>(key))
                return value;

            T* value = new T();
            _container.emplace(std::piecewise_construct, std::forward_as_tuple(key), std::forward_as_tuple(value));
            return value;
        }

        /**
         * Stores a new object that inherits the Base class with the given key
         */
        void Set(std::string const& key, Base* value)
        {
            _container.erase(key);
            _container.emplace(std::piecewise_construct, std::forward_as_tuple(key), std::forward_as_tuple(value));
        }

        /**
         * Removes objects with given key and returns true if one was removed, false otherwise
         */
        bool Erase(std::string const& key)
        {
            return _container.erase(key) != 0;
        }

    private:
        std::unordered_map<std::string, std::unique_ptr<Base>> _container;
};

#endif //CUSTOM_UTILITIES_H
