#ifndef CLASS
#define CLASS


#include <string>
#include <vector>
#include "grade.h"
#include "student.h"

class myClass{
    //public interface
    public:
        //constructor
        myClass();   //default constructor
        myClass(std::vector<int> weight, std::vector<int> items, std::vector<int> maxScore, std::vector<int> drops, std::vector<student> students);    //overloaded constructor
        //big three constructors
        myClass(const myClass& other);                  //creates a copy of an existing object
        myClass& operator=(const myClass& other);       //assigns data from one object to another
        //~student();                                     //destructor

        //member functions
        //accessors
        std::vector<int> getWeight() const;          //returns data
        std::vector<int> getItems() const;
        std::vector<int> getMaxScore() const;
        std::vector<int> getDrops() const;
        std::vector<student> getStudents() const;

        //mutator
        void setWeight(const std::vector<int>& nWeight);         //assigns data
        void setItems(const std::vector<int>& nItems);
        void setMaxScore(const std::vector<int>& nMaxScore);
        void setDrops(const std::vector<int>& nDrops);
        void setStudents(const std::vector<student>& nStudents);
    
        //functions
        student createBaseStudent();                //creates student with max scores
        void printBaseStudent();
        student getStudent(const int& index);       //returns specific student object
        void setStudent(const student& nStudent, const int& index);
        void addStudent();                          //appends student with randomly generated values
        void removeStudent(const int& index);       //removes student at an index
        int getStudentsLength();                    //returns number of students
        std::string getLetter(const int& index);    //returns letter grade of student at an index
        double getNumLetter(const std::string& letter);     //returns number of students receiving a specific letter grade
        std::vector<double> getPercentLetter();     //returns vector of percentage of students with grades, w/index[0]->A  &  index[4]->F
        double getLabScore(const int& index);       //returns lab score of a specific student
        double getQuizScore(const int& index);      //returns quiz score of a specific student
        double getMidtermScore(const int& index);   //returns midterm score of a specific student
        double getProjectScore(const int& index);   //returns project score of a specific student
        double getFinalScore(const int& index);     //returns final score of a specific student
        double getClassLabScore();                  //returns lab score of the class
        double getClassQuizScore();                 //returns quiz score of the class
        double getClassMidtermScore();              //returns midterm score of the class
        double getClassProjectScore();              //returns project score of the class
        double getClassFinalScore();                //returns final score of the class
        double getTotalScore(const int& index);     //returns total score of a specific student
        void sortID();                              //sorts students in ascending ID order
        void sortLetter();                          //sorts students in descending grade letter order
        void sortGrade();                           //sorts students in descending grade percentage order
        void printStudents();                       //prints students' data
        void printClass();                          //prints class' data



        //add overloading to use desired operators on class
        //friend allows access to private data
        //friend std::ostream& operator<<(std::ostream& out, const myClass& class);
        //friend std::istream& operator>>(std::istream& in, student& student);

        

    
    //private data fields
    private:
        std::vector<int> weight;
        std::vector<int> items;
        std::vector<int> maxScore;
        std::vector<int> drops;
        std::vector<student> students;

};

#endif