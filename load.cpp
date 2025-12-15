#include <fstream>
#include <vector>
#include <string>

#include "load.h"

bool loadScoresFile(const std::string& filename, myClass& cls)
{
    std::ifstream in(filename);
    if(!in)
        return false;

    std::vector<int> weight(5);
    std::vector<int> items(5);
    std::vector<int> drops(5);

    // read weights
    for(int i = 0; i < 5; i++)
        in >> weight[i];

    // read item counts
    for(int i = 0; i < 5; i++)
        in >> items[i];

    // read drops
    for(int i = 0; i < 5; i++)
        in >> drops[i];

    // read max scores by category
    std::vector<int> maxLabs(items[0]);
    std::vector<int> maxQuiz(items[1]);
    std::vector<int> maxMid(items[2]);
    std::vector<int> maxProj(items[3]);
    std::vector<int> maxFinal(items[4]);

    for(int i = 0; i < items[0]; i++) in >> maxLabs[i];
    for(int i = 0; i < items[1]; i++) in >> maxQuiz[i];
    for(int i = 0; i < items[2]; i++) in >> maxMid[i];
    for(int i = 0; i < items[3]; i++) in >> maxProj[i];
    for(int i = 0; i < items[4]; i++) in >> maxFinal[i];

    // flatten maxScore (myClass expects one vector)
    std::vector<int> maxScore;
    maxScore.insert(maxScore.end(), maxLabs.begin(), maxLabs.end());
    maxScore.insert(maxScore.end(), maxQuiz.begin(), maxQuiz.end());
    maxScore.insert(maxScore.end(), maxMid.begin(), maxMid.end());
    maxScore.insert(maxScore.end(), maxProj.begin(), maxProj.end());
    maxScore.insert(maxScore.end(), maxFinal.begin(), maxFinal.end());

    cls.setWeight(weight);
    cls.setItems(items);
    cls.setDrops(drops);
    cls.setMaxScore(maxScore);

        // read students
    std::vector<student> students;

    while(true)
    {
        int id;
        in >> id;
        if(!in)
            break;

        student s(
            id,
            items[0], items[1], items[2], items[3], items[4],
            maxLabs[0], maxQuiz[0], maxMid[0], maxProj[0], maxFinal[0],
            weight[0], weight[1], weight[2], weight[3], weight[4]
        );

        // labs
        for(int i = 0; i < items[0]; i++)
        {
            int val;
            in >> val;
            s.getLab().setScore(val, i);
        }

        // quizzes
        for(int i = 0; i < items[1]; i++)
        {
            int val;
            in >> val;
            s.getQuiz().setScore(val, i);
        }

        // midterms
        for(int i = 0; i < items[2]; i++)
        {
            int val;
            in >> val;
            s.getMidterm().setScore(val, i);
        }

        // project
        for(int i = 0; i < items[3]; i++)
        {
            int val;
            in >> val;
            s.getProject().setScore(val, i);
        }

        // final
        for(int i = 0; i < items[4]; i++)
        {
            int val;
            in >> val;
            s.getFinal().setScore(val, i);
        }

        students.push_back(s);
    }


    cls.setStudents(students);
    return true;
}
