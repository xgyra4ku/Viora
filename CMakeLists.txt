cmake_minimum_required(VERSION 3.10)
project(Viora)

# Указываем стандарт C++
set(CMAKE_CXX_STANDARD 17)

# Добавляем подкаталоги для сервера и клиента
add_subdirectory(server)
add_subdirectory(client)
