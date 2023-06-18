#include "crow.h"
#include <vector>

struct Animals {
    std::string type;
    std::string name;
    long long age;
};

std::vector<Animals> animals;

int main()
{
    crow::SimpleApp app;

    CROW_ROUTE(app, "/")([]() {
        return "Hello, World!";
    });

    //Create new animal
    CROW_ROUTE(app, "/animal").methods("POST"_method)([](const crow::request& req) {
        auto x = crow::json::load(req.body);

        if (!x) {
            return crow::response(crow::status::BAD_REQUEST); // same as crow::response(400)
        }

        animals.push_back(Animals{ x["type"].s(), x["name"].s(), x["age"].i() });

        return crow::response{"Animal Created"};
    });

    //Get all animals
    CROW_ROUTE(app, "/animal").methods("GET"_method)([] {
        crow::json::wvalue x({ {"type", ""}, {"name", ""}, {"age", 0} });

        for (int i = 0; i < animals.size(); i++)
        {
            x[i]["type"] = animals[i].type;
            x[i]["name"] = animals[i].name;
            x[i]["age"] = animals[i].age;
        }
        return x;
    });

    //Get animal filter by name
    CROW_ROUTE(app, "/animal/<string>").methods("GET"_method)([](std::string name) {
        crow::json::wvalue x({ {"type", ""}, {"name", ""}, {"age", 0} });

        for (int i = 0; i < animals.size(); i++)
        {
            if (animals[i].name == name) {
                x["type"] = animals[i].type;
                x["name"] = animals[i].name;
                x["age"] = animals[i].age;
            }
        }

        return x;
    });

    //Delete animal filter by name
    CROW_ROUTE(app, "/animal/<string>").methods("DELETE"_method)([](std::string name) {

        for (int i = 0; i < animals.size(); i++)
        {
            if (animals[i].name == name) {
                animals.erase(animals.begin() + i);
            }
        }

        return name + " deleted";
    });

    app.port(18080).multithreaded().run();
}