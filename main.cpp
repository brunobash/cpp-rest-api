#include <cpprest/http_listener.h>
#include <cpprest/json.h>
#include <iostream>
#include <map>
#include <string>

using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;

//a map to store items(bd sim)
std::map<int, json::value> items;

//function to handle with GET requests
void handle_get(http_request request) {
	json::value response_data;

	for(const auto& item : items){
		response_data[item.first] = item.second;
	}

	request.reply(status_code::OK, response_data);
}

//function to hadle with POST requests
void handle_post(http_request request) {
	request
		.extract_json();
		.then([=](json::value item_data){
			static int current_id = 0;
			item[++current_id] = item_data;
			request.reply(status_codes::Created, json::value::number(current_id));
		})
		.wait();
}

// function to handle with PUT requests
void handle_put(http_request request){
	auto path = uri::split_path(uri::decode(request.relative_uri().path()));

	if(path.size() != 1) {
		request.reply(status_codes::BadRequest, U("Invalid ID"));
		return;
	}

	int id = std::stoi(path[0]);

	request
		.extract_json()
		.then([=](json::value item_data){
				if(items.find(id) != items.end()){
					items[id] = item_data;
					request.reply(status_codes::OK, U("Item updated"));
				} else {
					request.reply(status_codes::NotFound, U("Item not found"));
				}
			})
			.wait();
}
// function to handle with delete requests
void handle_delete(http_request request) {
    auto path = uri::split_path(uri::decode(request.relative_uri().path()));

    if (path.size() != 1) {
        request.reply(status_codes::BadRequest, U("Invalid ID"));
        return;
    }

    int id = std::stoi(path[0]);

    if (items.find(id) != items.end()) {
        items.erase(id);
        request.reply(status_codes::OK, U("Item deleted"));
    } else {
        request.reply(status_codes::NotFound, U("Item not found"));
    }
}

// Main function
int main() {
    http_listener listener(U("http://localhost:8080/items"));

    listener.support(methods::GET, handle_get);
    listener.support(methods::POST, handle_post);
    listener.support(methods::PUT, handle_put);
    listener.support(methods::DEL, handle_delete);

    try {
        listener
            .open()
            .then([&listener]() { std::cout << "Servidor ouvindo em " << listener.uri().to_string() << std::endl; })
            .wait();

        std::cout << "Pressione Ctrl+C para sair." << std::endl;
        std::string line;
        std::getline(std::cin, line);
    } catch (const std::exception& e) {
        std::cerr << "Erro: " << e.what() << std::endl;
    }

    return 0;
}
