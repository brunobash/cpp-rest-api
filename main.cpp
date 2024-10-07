#include <cpprest/http_listener.h>
#include <cpprest/json.h>
#include <iostream>
#include <map>
#include <string>

using namespace web;
using namespace web::http;
using namespace web::http:experimental::listener;

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
		.
}
