cmake_minimum_required(VERSION 3.15)
project(CRUDSocketServer LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find nlohmann_json (installed via apt or vcpkg)
find_package(nlohmann_json REQUIRED)

# Add server executable
add_executable(server server.cpp)
target_link_libraries(server PRIVATE nlohmann_json::nlohmann_json)

# Add client executable
add_executable(client client.cpp)

# Optional: install targets
install(TARGETS server client DESTINATION bin)
