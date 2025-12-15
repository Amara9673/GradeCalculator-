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



/*



//date::~date(){        //destructor
//}


//vailididty checker
void date::checkMonth(){
    int temp;
    bool check = true;
    while(check){    //checks if month is valid
        if(getMonth()<1 || getMonth()>12){
            std::cout << "Please enter a valid month: ";
            std::cin >> temp;
            setMonth(temp);
        }else{
            check = false;
        }
    }
    checkDay();     //ensures the day is still a valid day in the new month
}
void date::checkDay(){
    int temp;
    bool check = true;
    while(check){       //checks if day is valid
        if(getDay()<1 || getDay()>31){
            std::cout << "Please enter a valid day: ";
            std::cin >> temp;
            setDay(temp);
        }else if((getMonth()==4 || getMonth()==6 || getMonth()==9 || getMonth()==11) && getDay()>30){
            std::cout << "Please enter a valid day: ";
            std::cin >> temp;
            setDay(temp);
        }else if(getMonth()==2 && ((!(getYear()%4) && !((getYear()/100)%4) && getDay()>29) || (((getYear()%4) || ((getYear()/100)%4)) && getDay()>28))){
            std::cout << "Please enter a valid day: ";
            std::cin >> temp;
            setDay(temp);
        }else{
            check = false;
        }
    }
}



//functions
std::string date::convertMonth(){   //returns month in word form
    std::string months[12] = {"January","February","March","April","May","June","July","August","September","October","November","December"};
    return months[this->month-1];
}
//displays the date
void date::displaySlashDate(){      //day/month/year
    std::string day = std::to_string(this->month);
    if(day.length() < 2){
        day = "0" + day;
    }
    std::cout << day;

    day = std::to_string(this->day);
    if(day.length() < 2){
        day = "0" + day;
    }
    std::cout << "/" << day << "/" << this->year << "\n";
}
void date::displayAmericanDate(){   //Month day, year
    std::cout << convertMonth() << " " << this->day << ", " << this->year << "\n";
}
void date::displayNormalDate(){     //day month year
    std::cout << this->day << " " << convertMonth() << " " << this->year << "\n";
}

bool date::isLeapYear(){    //returns true if it's a leap year
    bool i = this->year % 4;
    if(!(this->year%100)){
        if(this->year%400){
            i=1;
        }
    }
    return !i;
}

int date::days(){   //converts the date into an integer of its days relative to 0
    enum days {feb=28, normal=30, longer=31, yr=365};   //days in a month/year
    int total = (this->year*yr) + this->day + (this->year/4) - 1*(this->year/100) + (this->year/400);
    if(this->month>1){
        total += longer;
    }
    if(this->month>2){
        total += feb;
    }
    if(this->month>3){
        total += longer;
    }
    if(this->month>4){
        total += normal;
    }
    if(this->month>5){
        total += longer;
    }
    if(this->month>6){
        total += normal;
    }
    if(this->month>7){
        total += longer;
    }
    if(this->month>8){
        total += longer;
    }
    if(this->month>9){
        total += normal;
    }
    if(this->month>10){
        total += longer;
    }
    if(this->month>11){
        total += normal;
    }
    return total;
}

int date::daysBetween(const date& otherDate){   //returns absolute value of days between two dates
    date otherPointer = otherDate;
    int total1 = this->days();
    int total2 = otherPointer.days();
    return abs(total1-total2);
}

int date::daysCompare(const date& otherDate){   //returns 1,0, or -1 depending on which date is larger
    date otherPointer = otherDate;
    int i = this->days();
    int j = otherPointer.days();
    if(i>j){
        return 1;
    }else if(i==j){
        return 0;
    }else{
        return -1;
    }
}


void date::increaseDate(){      //increases the date by 1
    int temp = getDay() + 1;
    int month = getMonth();
    if(month==12 && temp>31){
        temp=1;
        month=1;
        setYear(getYear()+1);
    }else if(temp>31 || ((month==4 || month==6 || month==9 || month==11) && temp>30)){
        temp=1;
        month+=1;
    }else if(month==2  &&  ((!(getYear()%4) && !((getYear()/100)%4) && temp>29)  ||  (((getYear()%4) || ((getYear()/100)%4)) && temp>28))){
        temp=1;
        month+=1;
    }

    setDay(temp);
    setMonth(month);
}
void date::decreaseDate(){      //decreases the date by 1
    int temp = getDay() - 1;
    int month = getMonth();
    if(month==1 && temp<1){
        temp=31;
        month=12;
        setYear(getYear()-1);
    }else if(temp<1){
        month-=1;
        if(month==4 || month==6 || month==9 || month==11){
            temp=30;
        }else if(month==2  &&  (!(getYear()%4) && !((getYear()/100)%4))){
            temp=29;
        }else if(month==2 && ((getYear()%4) || ((getYear()/100)%4))){
            temp=28;
        }else{
            temp=31;
        }
    }

    setMonth(month);
    setDay(temp);
}



//allows use of the '<<' and '>>' operator to output and input data about a date object
std::ostream &operator<<(std::ostream &out, const date &date){
    std::string months[12] = {"January","February","March","April","May","June","July","August","September","October","November","December"};
    out << months[date.month-1] << " " << date.day << ", " << date.year << "\n";
    return out;
}

/*std::istream &operator>>(std::istream &in, date &date){
    in>>date.month;
    in>>date.day;
    in>>date.year;
    return in;
}*/
/*
//increases date by i days
const date date::operator+(int i) const{
    date temp = *this;
    for(int x=0;x<i;++x){
        temp.increaseDate();
    }
    return temp;
}

//decreases date by i days
const date date::operator-(int i) const{
    date temp = *this;
    for(int x=0;x<i;++x){
        temp.decreaseDate();
    }
    return temp;
}

//returns number of days between dates
const int date::operator-(const date& otherDate) const{
    date temp = *this;
    return temp.daysBetween(otherDate);
}



//checks if one date is greater than another date
bool date::operator>(const date& otherDate) const{
    date temp = *this;
    return temp.daysCompare(otherDate)==1;
}

//checks if one date is less than another date
bool date::operator<(const date& otherDate) const{
    date temp = *this;
    return temp.daysCompare(otherDate)==-1;
}

//checks if one date is equal to another date
bool date::operator==(const date& otherDate) const{
    date temp = *this;
    return temp.daysCompare(otherDate)==0;
}

//checks if one date isn't equal to another date
bool date::operator!=(const date& otherDate) const{
    date temp = *this;
    return temp.daysCompare(otherDate)!=0;
}



//increases date by 1
date date::operator++(){
    date temp = *this;
    temp.increaseDate();
    return temp;
}
date date::operator++(int dummy){
    date temp = *this;
    temp.increaseDate();
    return temp;
}

//decreases date by 1
date date::operator--(){
    date temp = *this;
    temp.decreaseDate();
    return temp;
}
date date::operator--(int dummy){
    date temp = *this;
    temp.decreaseDate();
    return temp;
}*/