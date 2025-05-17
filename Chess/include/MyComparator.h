//
// Created by HALAA on 11/05/2025.
//

#ifndef CHESS_MYCOMPARATOR_H
#define CHESS_MYCOMPARATOR_H
template<typename T>
class MyComparator {
public:
    bool operator()(const T& a, const T& b) const {
        return a > b;
    }
};

#endif //CHESS_MYCOMPARATOR_H
