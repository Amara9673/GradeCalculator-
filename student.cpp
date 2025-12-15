#include <iostream>
#include <cmath>
#include <iomanip>
//#include <string>


#include "student.h"
#include "grade.h"

//using namespace std;


//constructor
student::student(){
    ID = 0;
    labs = grade();
    quizzes = grade();
    midterms = grade();
    projects = grade();
    finals = grade();
}
student::student(int IDdata, grade labData, grade quizData, grade midtermData, grade projectData, grade finalData){
    ID = IDdata;
    labs = grade(labData);
    quizzes = grade(quizData);
    midterms = grade(midtermData);
    projects = grade(projectData);
    finals = grade(finalData);
}
student::student(int IDdata, int nLabs, int nQuizzes, int nMidterms, int nProjects, int nFinals, int labMaxScore, int quizMaxScore, int midtermMaxScore, int projectMaxScore, int finalMaxScore, int labWeight, int quizWeight, int midtermWeight, int projectWeight, int finalWeight){
    ID = IDdata;
    labs = grade(nLabs, labMaxScore, labWeight);
    quizzes = grade(nQuizzes, quizMaxScore, quizWeight);
    midterms = grade(nMidterms, midtermMaxScore, midtermWeight);
    projects = grade(nProjects, projectMaxScore, projectWeight);
    finals = grade(nFinals, finalMaxScore, finalWeight);
}
//big three constructors
student::student(const student& other){
    ID = other.ID;
    labs = other.labs;
    quizzes = other.quizzes;
    midterms = other.midterms;
    projects = other.projects;
    finals = other.finals;
}
student& student::operator=(const student& other){
    this->ID = other.ID;
    this->labs = other.labs;
    this->quizzes = other.quizzes;
    this->midterms = other.midterms;
    this->projects = other.projects;
    this->finals = other.finals;
    return *this;
}
//studnet::~student(){}                                  //destructor



//accessors
int student::getID() const{
    return ID;
}
grade student::getLab() const{
    return labs;
}
grade student::getQuiz() const{
    return quizzes;
}
grade student::getMidterm() const{
    return midterms;
}
grade student::getProject() const{
    return projects;
}
grade student::getFinal() const{
    return finals;
}

//mutator
void student::setID(const int& nID){
    ID = nID;
}
void student::setLab(const grade& nLab){
    labs = nLab;
}
void student::setQuiz(const grade& nQuiz){
    quizzes = nQuiz;
}
void student::setMidterm(const grade& nMidterm){
    midterms = nMidterm;
}
void student::setProject(const grade& nProject){
    projects = nProject;
}
void student::setFinal(const grade& nFinal){
    finals = nFinal;
}

    
//functions
double student::getLabScore(){
    return labs.getCategoryScore();
}
double student::getQuizScore(){
    return quizzes.getCategoryScore();
}
double student::getMidtermScore(){
    return midterms.getCategoryScore();
}
double student::getProjectScore(){
    return projects.getCategoryScore();
}
double student::getFinalScore(){
    return finals.getCategoryScore();
}
double student::getLabWeightScore(){
    return labs.getCategoryWeightScore();
}
double student::getQuizWeightScore(){
    return quizzes.getCategoryWeightScore();
}
double student::getMidtermWeightScore(){
    return midterms.getCategoryWeightScore();
}
double student::getProjectWeightScore(){
    return projects.getCategoryWeightScore();
}
double student::getFinalWeightScore(){
    return finals.getCategoryWeightScore();
}
double student::getTotalWeightScore(){
    return labs.getCategoryWeightScore() + quizzes.getCategoryWeightScore() + midterms.getCategoryWeightScore() + projects.getCategoryWeightScore() + finals.getCategoryWeightScore();
}
std::string student::returnLetterGrade(){
    enum grades {A=90,B=80,C=70,D=60};  //grade values
    if(getTotalWeightScore()>=static_cast<double>(A)){
        return "A";
    }else if(getTotalWeightScore()>=static_cast<double>(B)){
        return "B";
    }else if(getTotalWeightScore()>=static_cast<double>(C)){
        return "C";
    }else if(getTotalWeightScore()>=static_cast<double>(D)){
        return "D";
    }else{
        return "F";
    }
}

void student::dropLabs(const int& num){
    labs.dropScores(num);
}
void student::dropQuizzes(const int& num){
    quizzes.dropScores(num);
}
void student::dropMidterms(const int& num){
    midterms.dropScores(num);
}
void student::dropProjects(const int& num){
    projects.dropScores(num);
}
void student::dropFinals(const int& num){
    finals.dropScores(num);
}

double student::simulateDropLabs(const int& num){
    grade temp(this->getLab());
    temp.dropScores(num);
    return temp.getCategoryScore();
}
double student::simulateDropQuizzes(const int& num){
    grade temp(this->getQuiz());
    temp.dropScores(num);
    return temp.getCategoryScore();
}
double student::simulateDropMidterms(const int& num){
    grade temp(this->getMidterm());
    temp.dropScores(num);
    return temp.getCategoryScore();
}
double student::simulateDropProjects(const int& num){
    grade temp(this->getProject());
    temp.dropScores(num);
    return temp.getCategoryScore();
}
double student::simulateDropFinals(const int& num){
    grade temp(this->getFinal());
    temp.dropScores(num);
    return temp.getCategoryScore();
}

void student::printStudent(){
    std::cout << std::setfill('0') << std::setw(9) << ID << " ";
    labs.printCategory();
    quizzes.printCategory();
    midterms.printCategory();
    projects.printCategory();
    finals.printCategory();
}

void student::setBaseStudent(){
    ID = 123456789;
    for(int i=0;i<labs.getCategoryLength();++i){
        labs.setScore(labs.getMaxScore(), i);
    }
    for(int i=0;i<quizzes.getCategoryLength();++i){
        quizzes.setScore(quizzes.getMaxScore(), i);
    }
    for(int i=0;i<midterms.getCategoryLength();++i){
        midterms.setScore(midterms.getMaxScore(), i);
    }
    for(int i=0;i<projects.getCategoryLength();++i){
        projects.setScore(projects.getMaxScore(), i);
    }
    for(int i=0;i<finals.getCategoryLength();++i){
        finals.setScore(finals.getMaxScore(), i);
    }
}