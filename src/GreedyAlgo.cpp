#include <ctime>
#include "ProblemData.h"

#define PRO_NUM 71

int main()
{ 
    for(int i = 1; i <= PRO_NUM; i++) {
        string filePath = "../Instances/p" + to_string(i);
        cout << filePath << endl;
        clock_t startTime = clock();
        ProblemData* p = new ProblemData(filePath);
        int result = p->greedySolution();
        cout << result << " " << (clock() - startTime) * 1.0 / CLOCKS_PER_SEC << endl;
        // cout << "| p" << i << " | " << result << " | " << (clock() - startTime) * 1.0 / CLOCKS_PER_SEC << " |" << endl;
        p->printDetail();
        delete p;
        cout << "-----------------------------------------------------------\n";
    }
    return 0;
}
