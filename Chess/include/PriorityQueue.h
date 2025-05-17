//
// Created by HALAA on 11/05/2025.
//

#ifndef CHESS_PRIORITYQUEUE_H
#define CHESS_PRIORITYQUEUE_H
#include <iostream>
#include <stdexcept>
#include <sstream>
#include <list>
#include <iterator>
#include <algorithm>

using namespace std;
template <typename T, typename Comparator>
class PriorityQueue {
private:
    int size;
    list<T> list_data;
    Comparator comp;

public:
    explicit PriorityQueue(int size = 5):size(size){};

    [[nodiscard]] bool isEmpty()const{return list_data.empty();}

    void insert(const T& element) {
        list_data.push_back(element);
        list_data.sort(comp);  // ממיין לפי הקומפרטור
        if (list_data.size() > size) {
            list_data.pop_back();  // מסיר את הפחות מועדף
        }
    }



    T popMax() {
        if (isEmpty()) {
            throw runtime_error("Heap is empty!");
        }
        auto ret = *list_data.begin();
        list_data.pop_front();
        return ret;
    }

//// Assuming T can be cast to string
    [[nodiscard]] string toString()const{
        std::ostringstream oss;
        oss << "[";
        for (auto it = list_data.begin(); it != list_data.end(); ++it) {
            oss << *it;
            if (std::next(it) != list_data.end()) {
                oss << ",";
            }
        }
        oss << "]";
        return oss.str();
    }

};

#endif //CHESS_PRIORITYQUEUE_H
