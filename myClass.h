#ifndef CLASS
#define CLASS

#include <string>
#include <vector>
#include "grade.h"
#include "student.h"

class myClass{
public:
    // constructors
    myClass();   
    myClass(std::vector<int> weight,
            std::vector<int> items,
            std::vector<int> maxScore,
            std::vector<int> drops,
            std::vector<student> students);

    // big three
    myClass(const myClass& other);
    myClass& operator=(const myClass& other);
    //~myClass();

    // accessors
    std::vector<int> getWeight() const;
    std::vector<int> getItems() const;
    std::vector<int> getMaxScore() const;
    std::vector<int> getDrops() const;
    std::vector<student> getStudents() const;

    // mutators
    void setWeight(const std::vector<int>& nWeight);
    void setItems(const std::vector<int>& nItems);
    void setMaxScore(const std::vector<int>& nMaxScore);
    void setDrops(const std::vector<int>& nDrops);
    void setStudents(const std::vector<student>& nStudents);

    // core functions
    student createBaseStudent();
    void printBaseStudent();
    student getStudent(const int& index);
    void setStudent(const student& nStudent, const int& index);
    void addStudent();
    void addxStudents(const int& num);   // 
    void removeStudent(const int& index);
    int getStudentsLength();
    std::string getLetter(const int& index);
    double getNumLetter(const std::string& letter);
    std::vector<double> getPercentLetter();
    double getLabScore(const int& index);
    double getQuizScore(const int& index);
    double getMidtermScore(const int& index);
    double getProjectScore(const int& index);
    double getFinalScore(const int& index);
    double getClassLabScore();
    double getClassQuizScore();
    double getClassMidtermScore();
    double getClassProjectScore();
    double getClassFinalScore();
    double getTotalScore(const int& index);
    void sortID();
    void sortLetter();
    void sortGrade();
    void printStudents();
    void printClass();

private:
    std::vector<int> weight;
    std::vector<int> items;
    std::vector<int> maxScore;
    std::vector<int> drops;
    std::vector<student> students;
};

#endif
