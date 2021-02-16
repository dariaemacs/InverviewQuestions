/* Description: booking system implementation -*- C++ -*-
 * Usage: BOOK time hotel_name client_id room_count|CLIENTS hotel_name|ROOMS hotel_name
 * @file booking.cpp
 * @author Daria Emacs
 * Contact: dariaemacs@gmail.com
 */

#include <algorithm>
#include <iostream>
#include <numeric>
#include <chrono>
#include <vector>
#include <map>

class Booking{
  using days = std::chrono::duration<int, std::ratio_multiply<std::ratio<24>, std::chrono::hours::period>>;

public:

  enum {
    BOOK,
    CLIENTS,
    ROOMS,
    // ... insert new command here
    COUNT
  };
  
  static const std::string Commands[COUNT];
  
  Booking(){
  }
  
  struct Info{
    long long timestamp;
    int client_id;
    int room_count;
  };

  using info_vec_t    = std::vector<Info>;
  using client_map_t  = std::map<int, long long>;
  using info_client_t = std::pair<info_vec_t, client_map_t>;
  using hotel_info_t  = std::map<std::string, info_client_t>;

  void book(std::string& name, Info&& info){
    current_timestamp = info.timestamp;

    if(auto it = hotel_name_infos.find(name); it != hotel_name_infos.end()){
      auto& [v, clients] = it->second;
      v.emplace_back(std::move(info));
      clients.emplace(info.client_id, info.timestamp);
    }else{
      info_vec_t v;
      client_map_t clients;
      v.emplace_back(std::move(info));
      clients.emplace(info.client_id, info.timestamp);

      hotel_name_infos.emplace(name, std::make_pair(std::move(v), std::move(clients)));
    }
  }
  
  info_client_t* get_info_client(const std::string& name){
    if(auto it = hotel_name_infos.find(name); it != hotel_name_infos.end()){
      return &(it->second);
    }
    return nullptr;
  }

  info_vec_t::iterator
  threshold_day_count(info_vec_t& infos){
    if (infos.empty()) return infos.end();
    
    auto threshold_time = threshold_day_time();
    return lower_bound(infos.begin(), infos.end(), threshold_time, [](auto& info, const long long ts) { return info.timestamp <= ts; });
  }

  int clients(client_map_t& clients){
    auto threshold_time = threshold_day_time();
    
    for(auto it = clients.begin(); it != clients.end(); ) {
      if (it->second <= threshold_time) {
        it = clients.erase(it);
      } else {
        ++it;
      }
    }
  
    return clients.size();
  }

  int rooms(info_vec_t& infos){
    auto it { threshold_day_count(infos) };

    unsigned int sum = accumulate(it, infos.end(), 0u, [](unsigned int value, auto& info) { return info.room_count + value; });
    return sum;
  }

private:

  long long current_time(){
    return current_timestamp;
  }

  long long threshold_day_time(){
    return current_time() - seconds_per_day.count();
  }
  
  std::chrono::seconds seconds_per_day = days{1};
  hotel_info_t hotel_name_infos;
  long long current_timestamp;
};

const std::string Booking::Commands[] = { "BOOK", "CLIENTS", "ROOMS" }; 

int main(int, char**){
  Booking booking;
  
  auto Q = 0u;
  std::cin >> Q;

  for(auto i = 0u; i < Q; ++i){
    std::string command;
    std::cin >> command;

    if(command == Booking::Commands[Booking::BOOK]){
      long long timestamp;
      std::cin >> timestamp;

      std::string hotel_name;
      std::cin >> hotel_name;

      int client_id;
      std::cin >> client_id;

      int room_count;
      std::cin >> room_count;
      
      booking.book(hotel_name, Booking::Info{timestamp, client_id, room_count});

    }else{
      std::string hotel_name;
      std::cin >> hotel_name;

      Booking::info_client_t* info_clients = booking.get_info_client(hotel_name);
      if(info_clients == nullptr){
        std::cout << "0\n";
        continue;
      }
  
      if(command == Booking::Commands[Booking::CLIENTS]){

        std::cout << booking.clients(info_clients->second) << '\n';

      }else if(command == Booking::Commands[Booking::ROOMS]){
        
        std::cout << booking.rooms(info_clients->first) << '\n';
        
      }else{
        std::cerr << "\033[31m" << "Wrong command: " << command << "\033[39m" << '\n';
      }
    }
  }
}
