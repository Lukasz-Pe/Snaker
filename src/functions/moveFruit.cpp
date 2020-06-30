//
// Created by lukasz on 29.06.2020.
//


#include "moveFruit.h"

template<typename S,typename F> double calculateDistance(S &snake, F &fruit){
    return sqrt(pow((snake.headCoordinates()._x - fruit.getPosX()), 2) + pow((snake.headCoordinates()._y - fruit.getPosY()), 2));
}

template<typename S,typename F> double calculateAngle(S &snake, F &fruit){
    double _x_vector_head_fruit{static_cast<double>(fruit.getPosX())-snake.headCoordinates()._x},
    _y_vector_head_fruit{snake.headCoordinates()._y-static_cast<double>(fruit.getPosY())};
    double _angle_head_to_fruit{0};
    if (_x_vector_head_fruit > 0 && _y_vector_head_fruit >= 0) {
        _angle_head_to_fruit = (180 / M_PI) * atan(_x_vector_head_fruit / _y_vector_head_fruit);
    } else if (_x_vector_head_fruit > 0 && _y_vector_head_fruit < 0) {
        _angle_head_to_fruit = 180 + (180 / M_PI) * atan(_x_vector_head_fruit / _y_vector_head_fruit);
    } else if (_x_vector_head_fruit < 0 && _y_vector_head_fruit >= 0) {
        _angle_head_to_fruit = (180 / M_PI) * atan(_x_vector_head_fruit / _y_vector_head_fruit);
    } else if (_x_vector_head_fruit < 0 && _y_vector_head_fruit < 0) {
        _angle_head_to_fruit = 180 + (180 / M_PI) * atan(_x_vector_head_fruit / _y_vector_head_fruit);
    }
    return _angle_head_to_fruit;
}

template<typename S,typename F> void changeFruitPosition(S &snake, F &fruit){
    if(fruitIsInRange(snake,fruit)&&fruitIsInCollectionAngle(snake,fruit)){
        moveFruit(snake,fruit);
    }
}

template<typename S,typename F> void moveFruit(S &snake, F &fruit){
    double _calculated_distance{calculateDistance(snake,fruit)},
    _calculated_angle{calculateAngle(snake, fruit)},
    _fruit_to_head_angle{[_calculated_angle]()->double{
        if(_calculated_angle+180.0<360){
            return _calculated_angle+180.0;
        }
        if((_calculated_angle-180.0)<0.0){
            return _calculated_angle+360.0;
        }
        return _calculated_angle-180.0;
    }()*(M_PI/180.0)};
    fruit.setPosition(static_cast<int>(fruit.getPosX()+_calculated_distance*sin(_fruit_to_head_angle)),
        static_cast<int>(fruit.getPosY()-_calculated_distance*cos(_fruit_to_head_angle)));
}

template<typename S,typename F> bool fruitIsInRange(S &snake, F &fruit){
    return calculateDistance(snake, fruit)<snake.collectionDistance();
}

template<typename S,typename F> bool fruitIsInCollectionAngle(S &snake, F &fruit){
    return calculateAngle(snake, fruit)<(snake.headCoordinates()._angle+snake.collectionAngle())
           ||calculateAngle(snake, fruit)>(snake.headCoordinates()._angle-snake.collectionAngle());
}
