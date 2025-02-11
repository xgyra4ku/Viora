#ifndef STRUCTURES_H
#define STRUCTURES_H

/**
 *  Includes
 */
// Map - для работы с хеш-таблицами
#include <map>
// String - для работы со строками
#include <string>
// Vector - для работы с векторами
#include <vector>
// Cstring - для работы с C строками
#include <cstring>

/**
 * sMessage - Структура сообщения
 */
struct sMessage {
    // Данные сообщения
    std::map<std::string, std::string> data;

    /**
     * @brief Serialize - сериализация в массив байт
     * @return std::vector<char> буфер сериализованных данных
     */
    [[nodiscard]] std::vector<char> serialize() const {
        //вектор байт
        std::vector<char> buffer;
        // цикл проходящий по всем данным
        for (const auto& [key, value] : data) {
            // запись размеров ключа и значения
            size_t key_size = key.size();
            size_t value_size = value.size();
            // запись в буфер
            buffer.insert(buffer.end(), reinterpret_cast<const char*>(&key_size), reinterpret_cast<const char*>(&key_size) + sizeof(size_t));
            buffer.insert(buffer.end(), key.begin(), key.end());
            // запись в буфер
            buffer.insert(buffer.end(), reinterpret_cast<const char*>(&value_size), reinterpret_cast<const char*>(&value_size) + sizeof(size_t));
            buffer.insert(buffer.end(), value.begin(), value.end());
        }
        // возвращаем буфер
        return buffer;
    }

    /**
     * @brief Deserialize - десериализация из массива байт
     * @param buffer буфер сериализованных данных
     */
    void deserialize(const std::vector<char>& buffer) {
        // очищаем данные
        data.clear();
        size_t offset = 0;
        // цикл проходящий по всем данным
        while (offset < buffer.size()) {
            // если конец буфера тогда выходим из цикла
            if (offset + sizeof(size_t) > buffer.size()) break;

            // Чтение размера ключа
            size_t key_size;
            std::memcpy(&key_size, buffer.data() + offset, sizeof(size_t));
            offset += sizeof(size_t);
            if (offset + key_size > buffer.size()) break;

            // Чтение ключа
            std::string key(buffer.data() + offset, key_size);
            offset += key_size;

            if (offset + sizeof(size_t) > buffer.size()) break;

            // Чтение размера значения
            size_t value_size;
            std::memcpy(&value_size, buffer.data() + offset, sizeof(size_t));
            offset += sizeof(size_t);
            if (offset + value_size > buffer.size()) break;

            // Чтение значения
            const std::string value(buffer.data() + offset, value_size);
            offset += value_size;
            // записываем в данные
            data[key] = value;
        }
    }
};

/**
 * sClientInfo - Структура информации о клиенте
 */
struct sClientInfo {
    std::string username; // Имя пользователя
    std::string id; // ID УСТРОЙСТВА
    SOCKET socket;        // Сокет клиента
};

#endif // STRUCTURES_H
