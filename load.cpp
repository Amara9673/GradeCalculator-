#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

#include "load.h"
#include "myClass.h"
#include "student.h"
#include "grade.h"

static int max_in_block(const std::vector<int>& v, int start, int count)
{
    int m = 0;
    for (int i = 0; i < count; i++)
        m = std::max(m, v[start + i]);
    return m;
}

bool loadScoresFile(const std::string& filename, myClass& cls)
{
    std::ifstream in(filename);
    if (!in)
        return false;

    const int CATS = 5;

    // --- read weights, items, drops ---
    std::vector<int> weight(CATS), items(CATS), drops(CATS);

    for (int i = 0; i < CATS; i++)
        if (!(in >> weight[i])) return false;

    for (int i = 0; i < CATS; i++)
        if (!(in >> items[i])) return false;

    for (int i = 0; i < CATS; i++)
        if (!(in >> drops[i])) return false;

    // total assessments per student (labs+quizzes+midterms+project+final)
    int totalItems = 0;
    for (int i = 0; i < CATS; i++)
    {
        if (items[i] < 0) return false;
        totalItems += items[i];
    }
    if (totalItems <= 0) return false;

    // --- read ALL per-assessment max scores (keep stream aligned) ---
    std::vector<int> maxAll(totalItems);
    for (int i = 0; i < totalItems; i++)
        if (!(in >> maxAll[i])) return false;

    // Convert to ONE max per category (because your grade uses one maxScore per category)
    std::vector<int> maxPerCat(CATS);
    int off = 0;
    for (int c = 0; c < CATS; c++)
    {
        if (items[c] == 0)
            maxPerCat[c] = 0;
        else
            maxPerCat[c] = max_in_block(maxAll, off, items[c]);
        off += items[c];
    }

    // --- store class metadata ---
    cls.setWeight(weight);
    cls.setItems(items);
    cls.setDrops(drops);
    cls.setMaxScore(maxPerCat);

    // --- read students ---
    std::vector<student> students;

    while (true)
    {
        int id;
        if (!(in >> id))
            break; // normal EOF

        std::vector<int> labs(items[0]), quizzes(items[1]),
                         midterms(items[2]), projects(items[3]), finals(items[4]);

        for (int i = 0; i < items[0]; i++) if (!(in >> labs[i])) return false;
        for (int i = 0; i < items[1]; i++) if (!(in >> quizzes[i])) return false;
        for (int i = 0; i < items[2]; i++) if (!(in >> midterms[i])) return false;
        for (int i = 0; i < items[3]; i++) if (!(in >> projects[i])) return false;
        for (int i = 0; i < items[4]; i++) if (!(in >> finals[i])) return false;

        grade gLabs(labs,     maxPerCat[0], weight[0]);
        grade gQuiz(quizzes,  maxPerCat[1], weight[1]);
        grade gMid(midterms,  maxPerCat[2], weight[2]);
        grade gProj(projects, maxPerCat[3], weight[3]);
        grade gFinal(finals,  maxPerCat[4], weight[4]);

        student s(id, gLabs, gQuiz, gMid, gProj, gFinal);

        // apply drops per spec
        if (drops[0] > 0) s.dropLabs(drops[0]);
        if (drops[1] > 0) s.dropQuizzes(drops[1]);
        if (drops[2] > 0) s.dropMidterms(drops[2]);
        if (drops[3] > 0) s.dropProjects(drops[3]);
        if (drops[4] > 0) s.dropFinals(drops[4]);

        students.push_back(s);
    }

    cls.setStudents(students);
    return (cls.getStudentsLength() > 0);
}
