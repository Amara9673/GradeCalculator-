#ifndef STUDENT
#define STUDENT


#include <string>
#include <vector>
#include "grade.h"

class student{
    //public interface
    public:
        //constructor
        student();   //default constructor
        student(int ID, grade labs, grade quizzes, grade midterms, grade projects, grade finals);    //overloaded constructor
        student(int ID, int nLabs, int nQuizzes, int nMidterms, int nProjects, int nFinals, int labMaxScore, int quizMaxScore, int midtermMaxScore, int projectMaxScore, int finalMaxScore, int labWeight, int quizWeight, int midtermWeight, int projectWeight, int finalWeight);
        student(int ID, int nLabs, int nQuizzes, int nMidterms, int nProjects, int nFinals, int labWeight, int quizWeight, int midtermWeight, int projectWeight, int finalWeight);
        //big three constructors
        student(const student& other);                  //creates a copy of an existing object
        student& operator=(const student& other);       //assigns data from one object to another
        //~student();                                     //destructor

        //member functions
        //accessors
        int getID() const;          //returns data
        grade getLab() const;
        grade getQuiz() const;
        grade getMidterm() const;
        grade getProject() const;
        grade getFinal() const;

        //mutator
        void setID(const int& nID);         //assigns data
        void setLab(const grade& nLab);
        void setQuiz(const grade& nQuiz);
        void setMidterm(const grade& nMidterm);
        void setProject(const grade& nProject);
        void setFinal(const grade& nFinal);

    
        //functions
        double getLabScore();               //returns category score as a percent
        double getQuizScore();
        double getMidtermScore();
        double getProjectScore();
        double getFinalScore();
        double getLabWeightScore();         //returns weighted category score as a percent
        double getQuizWeightScore();
        double getMidtermWeightScore();
        double getProjectWeightScore();
        double getFinalWeightScore();
        double getTotalWeightScore();       //returns student's grade as a percent
        std::string returnLetterGrade();    //returns student's grade as a letter
        void dropLabs(const int& num);      //drops n assignments (permenant)
        void dropQuizzes(const int& num);
        void dropMidterms(const int& num);
        void dropProjects(const int& num);
        void dropFinals(const int& num);
        double simulateDropLabs(const int& num);    //drops n assignments (actual scores are unaffected)
        double simulateDropQuizzes(const int& num);
        double simulateDropMidterms(const int& num);
        double simulateDropProjects(const int& num);
        double simulateDropFinals(const int& num);
        void printStudent();                //prints student's data
        void setBaseStudent();              //assigns max scores to this student



        //add overloading to use desired operators on class
        //friend allows access to private data
        //friend std::ostream& operator<<(std::ostream& out, const student& student);
        //friend std::istream& operator>>(std::istream& in, student& student);

        

    
    //private data fields
    private:
        int ID;
        grade labs;
        grade quizzes;
        grade midterms;
        grade projects;
        grade finals;

};

#endif