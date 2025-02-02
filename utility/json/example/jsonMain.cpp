#include "../JsonSerializer.hpp"

class ExampleObject {
public:
    ExampleObject(int x, int y, std::string name)
        : x(x), y(y), name(name) {}

    void print() {
        std::cout << "x = " << x << '\n';
        std::cout << "y = " << y << '\n';
        std::cout << "name = " << name << "\n\n";
    }

    int x;
    int y;
    std::string name;
};

template <>
Json JsonConvert::to_json(const std::unique_ptr<ExampleObject>& object) {
    static int id = 0;

    Json json;

    json.putValue("ID", id++);
    json.putValue("x", object->x);
    json.putValue("y", object->y);
    json.putValue("name", object->name);
    
    return json;
}

template <>
std::unique_ptr<ExampleObject> JsonConvert::from_json(const Json& json) {
    std::cout << "Loaded object with id: " << json.getValue<size_t>("ID") << '\n';
    auto x = json.getValue<int>("x");
    auto y = json.getValue<int>("y");
    auto name = json.getValue<std::string>("name");
    return std::make_unique<ExampleObject>(x, y, name);
}

int main() {
    std::array<std::unique_ptr<ExampleObject>, 3> objects = {
        std::make_unique<ExampleObject>(5, 10, "Object 1"),
        std::make_unique<ExampleObject>(15, 110, "Object 2"),
        std::make_unique<ExampleObject>(25, 1110, "Object 3"),
    };

    for (const auto& obj : objects) {
        JsonSerializer::put("objects", obj);
    }

    std::cout << "object blok elements number: " << JsonSerializer::getSize("objects") << '\n';

    std::unique_ptr<ExampleObject> emptyObj{};
    JsonSerializer::put("objects", emptyObj);

    JsonSerializer::putValue("block 1", "name", 1);
    JsonSerializer::putValue("block 1", "name", 2);
    JsonSerializer::putValue("block 1", "name", 3);
    JsonSerializer::putValue("block 1", "text", "description");

    std::cout << "block 1 elements number: " << JsonSerializer::getSize("block 1") << '\n';

    JsonSerializer::putValue("block 2", 1, "name", 1);
    JsonSerializer::putValue("block 2", 2, "name", 2);
    JsonSerializer::putValue("block 2", 1, "name", 3);
    JsonSerializer::putValue("block 2", 5, "name", 5);

    std::cout << "block 2 elements number: " << JsonSerializer::getSize("block 2") << '\n';

    JsonSerializer::save("example.json");

    JsonSerializer::clear();

    JsonSerializer::load("example.json");

    std::cout << JsonSerializer::getValue<int>("block 1", "name").value() << '\n';
    std::cout << JsonSerializer::getValue<std::string>("block 1", "text").value() << '\n';

    std::cout << JsonSerializer::getValue<int>("block 2", 1, "name").value() << '\n';
    std::cout << JsonSerializer::getValue<int>("block 2", 2, "name").value() << '\n';
    std::cout << JsonSerializer::getValue<int>("block 2", 5, "name").value() << '\n';

    auto object2 = JsonSerializer::get<std::unique_ptr<ExampleObject>>("objects", 5);
    if (object2) {
        object2.value()->print();
    }

    object2 = JsonSerializer::get<std::unique_ptr<ExampleObject>>("objects", 1);
    if (object2) {
        object2.value()->print();
    }

    std::cout << "where: " << JsonSerializer::getSize("where") << '\n';
    JsonSerializer::getValue<int>("block 1", "clue");
    JsonSerializer::getValue<int>("block 2", 4, "name");
    JsonSerializer::save("example2.json");

    return 0;
}
