/* Description: counter testing system for booking system implementation -*- C++ -*-
 * Usage: ./counter
 * @file counter.cpp
 * @author https://github.com/alehay
 * Contact: alehaymkm@gmail.com
 */

#include <iostream>
#include <fstream>  
#include <vector>
#include <set>
#include <numeric>

struct Book {
    long long time;
    std::string hotel_name; 
    int client_id;
    int room_count;    
};

// 
int findHotel (std::vector <Book> & vec, std::string str) {
    if (vec.empty()) {return 0;}

    //забираем время у последнего элемента и считаем его текущим
    long long curTime = vec.back().time;

    //    std::cout << "time: "<< vec[3].time << std::endl;

    std::set <int> clientSet;

    for(auto it = vec.rbegin(); it != vec.rend(); ++it){
      auto & [t, hotel_name, client_id, room_count] = *it;
      
      // если вывалились за сутки, бежим
      // std::cout << "(" << (curTime - 86400) << " >= " << it -> time << ")"<< std::endl;
      if ( (curTime - 86400) >= it -> time  ) { break; }
      
      if (it -> hotel_name == str) {
        clientSet.insert(it -> client_id);
      }
    }
    

    // for ( auto & [t, hotel_name, client_id, room_count]:  vec ) {
      
    //   // если вывалились за сутки, бежим
    //   std::cout << "(" << (curTime - 86400) << " >= " << t << ")"<< std::endl;
    //   if ( (curTime - 86400) >= t  ) { continue; }
      
    //   if (hotel_name == str) {
    //     clientSet.insert(client_id);
    //   }
    // }
    return clientSet.size();
} 

int main(int , char** ) {
    std::string book {"BOOK"};
    std::string clients {"CLIENTS"};
    std::string rooms {"ROOMS"};


    std::ifstream hotel("test_hotel.txt");
    if (!hotel) {
        std::cout << " hotels.txt НЕ НАЙДЕН " <<std::endl; 
         return 1; 
    }

    
    int rows_count;
    hotel >> rows_count;

    std::vector <Book> rows;
    
    // читаем строки 

    while ( hotel )
    {
        std::string Word; //= str.substr(0, str.find(" "));
        hotel >> Word;
        if (Word == book ) {
            Book tmp;
            hotel >> tmp.time >> tmp.hotel_name >> tmp.client_id >> tmp.room_count;
            //std::cout << tmp.time << std::endl;
            rows.emplace_back(std::move(tmp));
            //std::cout << "time: "<< rows.back().time << std::endl;
        } else {
            std::string hotel_name_tmp; 
            hotel >> hotel_name_tmp;
            if (Word == clients) {
              //std::cout << hotel_name_tmp << "_"
              std::cout <<findHotel(rows, hotel_name_tmp) << '\n';
            }
            long long curTime = rows.back().time;

            if (Word == rooms) {
              std::cout << std::accumulate ( rows.begin(), rows.end(), 0, [&curTime, &hotel_name_tmp](int value , auto &r){


                    if (hotel_name_tmp == r.hotel_name && (curTime - 86400) < r.time)
                      { return value + r.room_count;} return value;
                } ) << '\n';
            

            }
        

        }
        //std::cout << str << std::endl;




    }



}
