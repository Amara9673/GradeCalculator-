#include <iostream>
#include <cmath>
#include <ctime>
//#include <string>


#include "grade.h"

//using namespace std;

// ********* INCLUDE  srand(time(0))  IN MAIN ********** 

//constructor
grade::grade(){
    std::vector<int> temp;
    category = temp;
    maxScore = 0;
    weight = 0;
}
grade::grade(std::vector<int> categoryData, int maxScoreData, int weightData){
    category = categoryData;
    maxScore = maxScoreData;
    weight = weightData;
}
grade::grade(int items, int maxScoreData, int weightData){
    maxScore = maxScoreData;
    weight = weightData;
    createCategory(items);  //assigns category values (needs to be done after max score is set)
}
//big three constructors
grade::grade(const grade& other){
    category = other.category;
    maxScore = other.maxScore;
    weight = other.weight;
}
grade& grade::operator=(const grade& other){
    this->category = other.category;
    this->maxScore = other.maxScore;
    this->weight = other.weight;
    return *this;
}
//grade::~grade(){}                                  //destructor



//accessors
std::vector<int> grade::getCategory() const{
    return category;
}
int grade::getMaxScore() const{
    return maxScore;
}
int grade::getWeight() const{
    return weight;
}

//mutator
void grade::setCategory(const std::vector<int>& nCategory){
    category = nCategory;
}
void grade::setMaxScore(const int& nMaxScore){
    maxScore = nMaxScore;
}
void grade::setWeight(const int& nWeight){
    weight = nWeight;
}
    
//functions
int grade::getScore(const int& index) const{
    return category[index];
}
void grade::setScore(const int& nScore, const int& index){
    category[index] = nScore;
}
void grade::addScore(){
    category.push_back((maxScore/2) + rand() % ((maxScore/2)+1));
}
void grade::removeScore(const int& index){
    category.erase(category.begin() + index);
}
void grade::dropLowest(){
    int lowest = maxScore;
    int index = 0;
    for(int i=0;i<category.size();++i){
        if(category[i]<lowest){
            lowest = category[i];
            index = i;
        }
    }
    removeScore(index);
}
void grade::dropScores(const int& num){
    for(int i=0; i<num; ++i){
        dropLowest();
    }
}


void grade::createCategory(const int& items){
    for(int i=0; i<items;++i){
        addScore();
    }
}
void grade::printCategory(){
    for(int i=0; i<category.size();++i){
        std::cout << getScore(i) << " ";
    }
}
int grade::getCategoryLength() const{
    return category.size();
}

int grade::getTotalPoints() const{
    int total = 0;
    for(int i=0;i<category.size();++i){
        total += category[i];
    }
    return total;
}
int grade::getMaxPoints() const{
    return maxScore*category.size();
}
double grade::getCategoryScore() const{
    double total = 0;
    for(int i=0;i<category.size();++i){
        total += category[i];
    }
    return 100*(total/getMaxPoints());
}
double grade::getCategoryWeightScore() const{
    return (getCategoryScore()*weight)/100;
}


