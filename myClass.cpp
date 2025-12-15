#include <iostream>
#include <cmath>
#include <ctime>
//#include <string>


#include "grade.h"
#include "student.h"
#include "myClass.h"

//using namespace std;


//constructor
myClass::myClass(){
    std::vector<int> weight;
    std::vector<int> items;
    std::vector<int> drops;
    std::vector<student> students;
}
myClass::myClass(std::vector<int> weightData, std::vector<int> itemsData, std::vector<int> maxScoreData, std::vector<int> dropsData, std::vector<student> studentsData){
    weight = weightData;
    items = itemsData;
    maxScore = maxScoreData;
    drops = dropsData;
    students = studentsData;
}
//big three constructors
myClass::myClass(const myClass& other){
    weight = other.weight;
    items = other.items;
    maxScore = other.maxScore;
    drops = other.drops;
    students = other.students;
}
myClass& myClass::operator=(const myClass& other){
    this->weight = other.weight;
    this->items = other.items;
    this->maxScore = other.maxScore;
    this->drops = other.drops;
    this->students = other.students;
    return *this;
}
//myClass::~myClass(){}                                  //destructor



//accessors
std::vector<int> myClass::getWeight() const{
    return weight;
}
std::vector<int> myClass::getItems() const{
    return items;
}
std::vector<int> myClass::getMaxScore() const{
    return maxScore;
}
std::vector<int> myClass::getDrops() const{
    return drops;
}
std::vector<student> myClass::getStudents() const{
    return students;
}


//mutator
void myClass::setWeight(const std::vector<int>& nWeight){
    weight = nWeight;
}
void myClass::setItems(const std::vector<int>& nItems){
    items = nItems;
}
void myClass::setMaxScore(const std::vector<int>& nMaxScore){
    maxScore = nMaxScore;
}
void myClass::setDrops(const std::vector<int>& nDrops){
    drops = nDrops;
}
void myClass::setStudents(const std::vector<student>& nStudents){
    students = nStudents;
}
    
//functions
student myClass::createBaseStudent(){
    student base(123456789, items.at(0), items.at(1), items.at(2), items.at(3), items.at(4), maxScore.at(0), maxScore.at(1), maxScore.at(2), maxScore.at(3), maxScore.at(4), weight.at(0), weight.at(1), weight.at(2), weight.at(3), weight.at(4));
    return base;
}
void myClass::printBaseStudent(){
    student base = createBaseStudent();
    base.setBaseStudent();
    base.printStudent();
    std::cout << "\n";
}
student myClass::getStudent(const int& index){
    return students.at(index);
}
void myClass::setStudent(const student& nStudent, const int& index){
    students.at(index) = nStudent;
}
void myClass::addStudent(){
    int rndm = (rand() % 100000) + 100000*(rand()%10000);
    student nStudent(rndm, items.at(0), items.at(1), items.at(2), items.at(3), items.at(4), maxScore.at(0), maxScore.at(1), maxScore.at(2), maxScore.at(3), maxScore.at(4), weight.at(0), weight.at(1), weight.at(2), weight.at(3), weight.at(4));
    students.push_back(nStudent);
}
void myClass::addxStudents(const int& num){
    for(int i=0;i<num;++i){
        addStudent();
    }
}
void myClass::removeStudent(const int& index){
    students.erase(students.begin() + index);
}
int myClass::getStudentsLength(){
    return students.size();
}

std::string myClass::getLetter(const int& index){
    return students.at(index).returnLetterGrade();
}
double myClass::getNumLetter(const std::string& letter){
    double temp = 0;
    for(int i=0;i<students.size();++i){
        if(getLetter(i) == letter){
            temp++;
        }
    }
    return temp;
}
std::vector<double> myClass::getPercentLetter(){
    std::vector<double> percents;
    percents.push_back((getNumLetter("A")*100)/students.size());
    percents.push_back((getNumLetter("B")*100)/students.size());
    percents.push_back((getNumLetter("C")*100)/students.size());
    percents.push_back((getNumLetter("D")*100)/students.size());
    int total = 100-(percents.at(0) + percents.at(1) + percents.at(2) + percents.at(3));
    percents.push_back(total);
    return percents;
}

double myClass::getLabScore(const int& index){
    return students.at(index).getLabScore();
}
double myClass::getQuizScore(const int& index){
    return students.at(index).getQuizScore();
}
double myClass::getMidtermScore(const int& index){
    return students.at(index).getMidtermScore();
}
double myClass::getProjectScore(const int& index){
    return students.at(index).getProjectScore();
}
double myClass::getFinalScore(const int& index){
    return students.at(index).getFinalScore();
}
double myClass::getClassLabScore(){
    double score = 0;
    for(int i=0; i<students.size();++i){
        score +=getLabScore(i);
    }
    return score/students.size();
}
double myClass::getClassQuizScore(){
    double score = 0;
    for(int i=0; i<students.size();++i){
        score +=getQuizScore(i);
    }
    return score/students.size();
}
double myClass::getClassMidtermScore(){
    double score = 0;
    for(int i=0; i<students.size();++i){
        score +=getMidtermScore(i);
    }
    return score/students.size();
}
double myClass::getClassProjectScore(){
    double score = 0;
    for(int i=0; i<students.size();++i){
        score +=getProjectScore(i);
    }
    return score/students.size();
}
double myClass::getClassFinalScore(){
    double score = 0;
    for(int i=0; i<students.size();++i){
        score +=getFinalScore(i);
    }
    return score/students.size();
}

double myClass::getTotalScore(const int& index){
    return students.at(index).getTotalWeightScore();
}

void myClass::sortID(){
    std::vector<int> ids;
    for(int i=0; i<students.size();++i){
        ids.push_back(students.at(i).getID());
    }

    int length = students.size();
    for(int i = 1; i < length; i++) {
        int j = i;
        while(j > 0 && ids[j] < ids[j-1]) {
            int temp = ids[j];
            ids[j] = ids[j-1];
            ids[j-1] = temp;

            student mid(students.at(j));
            students.at(j) = students.at(j-1);
            students.at(j-1) = mid;

            j--;
        }
    }
}
void myClass::sortLetter(){
    std::vector<std::string> letters;
    for(int i=0; i<students.size();++i){
        letters.push_back(getLetter(i));
    }

    int length = students.size();
    for(int i = 1; i < length; i++) {
        int j = i;
        while(j > 0 && letters[j] < letters[j-1]) {
            std::string temp = letters[j];
            letters[j] = letters[j-1];
            letters[j-1] = temp;

            student mid(students.at(j));
            students.at(j) = students.at(j-1);
            students.at(j-1) = mid;

            j--;
        }
    }
}
void myClass::sortGrade(){
    std::vector<double> scores;
    for(int i=0; i<students.size();++i){
        scores.push_back(getTotalScore(i));
    }

    int length = students.size();
    for(int i = length-1; i >0; i--) {
        int j = i;
        while(j > 0 && scores[j] > scores[j-1]) {
            double temp = scores[j];
            scores[j] = scores[j-1];
            scores[j-1] = temp;

            student mid(students.at(j));
            students.at(j) = students.at(j-1);
            students.at(j-1) = mid;

            j--;
        }
    }
}

void myClass::printStudents(){
    for(int i=0;i<students.size();++i){
        getStudent(i).printStudent();
        std::cout << "\n";
    }
}
void myClass::printClass(){
    for(int i=0; i<weight.size();++i){
        std::cout << weight.at(i) << " ";
    }
    std::cout << "\n";
    for(int i=0; i<items.size();++i){
        std::cout << items.at(i) << " ";
    }
    std::cout << "\n";
    for(int i=0; i<drops.size();++i){
        std::cout << drops.at(i) << " ";
    }
    std::cout << "\n";
    printBaseStudent();
    printStudents();
}