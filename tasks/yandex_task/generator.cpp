/* Description: generator records for booking system implementation -*- C++ -*-
 * Usage: 
 * количество строк :  100000
 * количество отелей (если 0 то max) :  0
 * количество клиент ID  : 1000000000
 * количество комнат у клиента  : 1000
 * Начало времен  1 or -1  : 1
 * отношение запросов ROOMS и CLIENTS к  массиву бронирований  1 к 20 : 19
 * @file generator.cpp
 * @author https://github.com/alehay
 * Contact: alehaymkm@gmail.com
 */

#include <iostream>
#include <fstream>  
#include <vector>

#include <cstdlib> // для функций rand() и srand()
#include <ctime> // для функции time()

// Генерируем рандомное число между значениями min и max.
// Предполагается, что функцию srand() уже вызывали
int getRND(int min, int max)
{
    static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0); 
    // Равномерно распределяем рандомное число в нашем диапазоне
    return static_cast<int>(rand() * fraction * (max - min + 1) + min);
}


int main(int , char** ) {

    // устанавливаем значение системных часов в качестве стартового числа
    srand(static_cast<unsigned int>(time(0))); 

    std::vector <std::string> vec_hotel_mane;
    std::ifstream hotel_name("hotels.txt");
    
    if (!hotel_name) {
        std::cout << " hotels.txt НЕ НАЙДЕН " <<std::endl; 
        return 1; 
    }


    // читаем имена отелей 
    std::string str; 
    while (std::getline(hotel_name, str))
    {
        if (str.size() >= 12 ){ 
            str = str.substr(0,12) ;
        }
        vec_hotel_mane.push_back(str);
        //std::cout << str << std::endl;
    }
    
    
    std::ofstream outfile ("test_hotel.txt");
    long long time {0}; 
    //1e4; 

    std::cout << " количество строк :  ";
    int count;
    std::cin >> count ; 
    
    std::cout << " количество отелей (если 0 то max) :  ";
    int hotelCount;
    std::cin >> hotelCount;
    if (hotelCount == 0 || hotelCount >= vec_hotel_mane.size() )  {
        hotelCount = vec_hotel_mane.size() - 1;
    } else {
        hotelCount -= 1;
    }

    std::cout << " количество клиент ID  : ";
    int clientID;
    std::cin >> clientID;

    std::cout << " количество комнат у клиента  : ";
    int clientRoom;
    std::cin >> clientRoom;

    std::cout << " Начало времен  1 or -1  : ";
    int time_start;
    std::cin >> time_start;
    switch (time_start)
    {
    case 1 :
        time = 1e6;
        break;
    case -1 :
        time = -1e18;
        break;
    default:
        time = 0;
        break;
    }


    std::cout << "отношение запросов ROOMS и CLIENTS к  массиву бронирований  1 к 20 : ";
    int rndControl;
    std::cin >> rndControl;
    
    outfile << count << std::endl;
    for (int i = 0; i < count; ++i) {
        if ( getRND (0, 20 ) >  rndControl ) {
            if (getRND (0,1)) {
                outfile << "CLIENTS" << " "; // префикс
            } else {
                outfile << "ROOMS" << " " ; // префикс
            }
            outfile<< vec_hotel_mane.at ( getRND(0, hotelCount))<< std::endl;

        } else {
            time += getRND(0, 100);
            outfile << "BOOK" << " "  // префикс        
            << time << " "  // время
            << vec_hotel_mane.at ( getRND(0, hotelCount)) << " " // имя отеля
            << getRND(1,clientID) << " "   //  клиент  ID
            << getRND(1,clientRoom)   //  количество комнат 
            << std::endl;
        }
    }
    outfile.close();
    return 0;

}
