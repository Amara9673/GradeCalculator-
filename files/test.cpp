#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <cmath>
#include <ctime>
#include <vector>

#include "authentication.h"
#include "grade_utils.h"
#include "array_utils.h"
#include "student.h"
#include "grade.h"
#include "myClass.h"

using namespace std;


int main(){
    bool check;

    int log, select;                    //user select value
    //int drop, student;                  //drops on/off and student grade displayed
    double rangeHigh, rangeLow;         //range of letter grades to be seen
    std::string file = "scores.txt";    //current selected file

srand(time(0)); //srand needs to be in here for random grade class values to work(idk why)

    std::vector<int> weight = {0,0,0,0,100};
    std::vector<int> items = {5,4,2,1,1};
    std::vector<int> maxScore = {5,10,25,50,100};
    std::vector<int> drops = {2,1,0,0,0};
    std::vector<student> students;

    myClass c(weight, items, maxScore, drops, students);

    c.printBaseStudent();
    std::cout << "\n";
    c.addStudent();
    c.addStudent();
    c.addStudent();
    c.addStudent();
    c.addStudent();
    c.addStudent();

    c.printClass();




    for(int i=0; i<c.getStudentsLength(); ++i){
        std::cout << c.getStudent(i).getTotalWeightScore() << "->" << c.getLetter(i) << "  ";
    }std::cout <<  "\n";
    //std::cout << c.getNumLetter("A") << " " << c.getNumLetter("B") << " " << (c.getNumLetter("C")*100)/c.getStudentsLength() << " " << c.getNumLetter("D") << " " << c.getNumLetter("F");
    
    /*vector<double> yes = c.getPercentLetter(); 
    for(int i=0;i<5;++i){
        std::cout << yes.at(i) << " ";
    }*/




    std::cout << "\n\n";

    c.sortGrade();
    c.printClass();

    for(int i=0; i<c.getStudentsLength(); ++i){
        std::cout << c.getStudent(i).getTotalWeightScore() << "->" << c.getLetter(i) << "  ";
    }std::cout <<  "\n";
    //std::cout << c.getNumLetter("A") << " " << c.getNumLetter("B") << " " << (c.getNumLetter("C")*100)/c.getStudentsLength() << " " << c.getNumLetter("D") << " " << c.getNumLetter("F");
    
    vector<double> yes = c.getPercentLetter(); 
    for(int i=0;i<5;++i){
        std::cout << yes.at(i) << " ";
    }

    /*grade g1(5,10,30);
    grade g2(5,100,5);

    g1.printCategory();
    
    std::cout << g1.getCategoryScore() << "% -> " <<  g1.getTotalPoints() << "/" << g1.getMaxPoints() << "\n";
    g1.dropScores(3);
    g1.printCategory();
    
    std::cout << g1.getCategoryScore() << "% -> weighted: " << g1.getCategoryWeightScore() << "%\n\n";
    g2.printCategory();
    std::cout << g2.getCategoryScore() << "%\n";
    std::cout << g2.getCategoryWeightScore()*2 << " " << g1.getCategoryWeightScore()*3;
    std::cout << "\n\n";

    student s1(12345, g2, g2, g1, g1, g1);

    s1.printStudent();
    std::cout << s1.getTotalWeightScore() << "\n";
    s1.dropQuizzes(2);
    s1.printStudent();
    std::cout << s1.getTotalWeightScore() << "\n";
    std::cout << s1.simulateDropFinals(1) << " " << s1.getFinalScore() << "\n\n";

    s1.setBaseStudent();
    s1.printStudent();*/










    /*std::vector<int> grade = {7};
    student s1(grade, grade ,grade ,grade , grade);
    student s2(s1);
    s1.setLab(2,0);
    std::cout << s1.getLab(0) << " " << s2.getLab(0);*/
    


    return 0;
}


//checking is a user input is within bounds
    /*bool check = true;
    while(check){
        //have user input
        if(input within bounds){
            check=false;
        }else{
            std::cout << "Enter a valid input";
        }
    }*/


// Compiling:    g++ test.cpp authentication.cpp -o test $(pkg-config --cflags --libs gtk4)
