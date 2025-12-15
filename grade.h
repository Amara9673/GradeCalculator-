#ifndef GRADE
#define GRADE


#include <string>
#include <vector>

class grade{
    //public interface
    public:
        //constructor
        grade();   //default constructor
        grade(std::vector<int> category, int maxScore, int weight);    //overloaded constructor
        grade(int items, int maxScore, int weight);    //overloaded constructor
        //big three constructors
        grade(const grade& other);                  //creates a copy of an existing object
        grade& operator=(const grade& other);       //assigns data from one object to another
        //~grade();                                 //destructor

        //member functions
        //accessors
        std::vector<int> getCategory() const;    //returns data
        int getMaxScore() const;
        int getWeight() const;

        //mutator
        void setCategory(const std::vector<int>& nCategory);   //assigns data
        void setMaxScore(const int& nMaxScore);
        void setWeight(const int& nWeight);

    
        //functions
        int getScore(const int& index) const;                   //returns specific score
        void setScore(const int& nScore, const int& index);     //assigns specific score
        void addScore();                                        //appends randomly generated score within current bounds
        void removeScore(const int& index);                     //deletes specific score
        void dropLowest();                                      //drops lowest score
        void dropScores(const int& num);                        //drops lowest n scores
        void createCategory(const int& items);                  //populates category vector with n random scores within bounds
        void printCategory();                                   //outputs scores separated by a space
        int getCategoryLength() const;                          //returns length of category vector
        int getTotalPoints() const;                             //returns total points of category vector
        int getMaxPoints() const;                               //returns total max points
        double getCategoryScore() const;                        //returns category score as a percent
        double getCategoryWeightScore() const;                  //returns weighted category score as a percent


        //add overloading to use desired operators on class
        //friend allows access to private data
        //friend std::ostream& operator<<(std::ostream& out, const grade& grade);
        //friend std::istream& operator>>(std::istream& in, student& student);

        

    
    //private data fields
    private:
        std::vector<int> category;      //individual assignment scores
        int maxScore;                   //max score on an individual assignment
        int weight;                     //weight as a percent (35 = 35%)

};

#endif