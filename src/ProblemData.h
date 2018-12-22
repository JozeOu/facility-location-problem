#include <iostream>
#include <fstream>
#include <string>
#include <math.h>

#define ITERATION_NUM 1000
#define INIT_TEMP 100
#define RATE 0.8

using namespace std;

class ProblemData {
public:
    ProblemData(string filePath) {
        init(filePath);
        result = 0;
    }

    ~ProblemData() {
        delete []capacity;
        delete []openingCost;
        delete []demand;
        
        for(int i = 0; i < m; i++){
            delete []assignCost[i];
        }
        delete []assignCost;

        delete []statusOfFac;
        delete []assignSol;
    }

    void printArray() {
        for(int i = 0; i < n; i++) {
            cout << capacity[i] << " " << openingCost[i] << endl;
        }  
        for(int i = 0; i < m; i++) {
            cout << demand[i] << " ";
        }
        cout << endl;
        for(int i = 0; i < m; i++) {           
            for(int j = 0; j < n; j++){
                cout << assignCost[i][j] << " ";
            }   
            cout << endl;
        }
    }

    // print detailed solution
    void printDetail() {
        cout << "Result: " << result << endl;

        cout << "Status of facilities: ";
        for(int i = 0; i < n; i++) {
            cout << statusOfFac[i] << " ";
        }
        cout << endl << "The assignment of customers to facility:\n";
        for(int i = 0; i < m; i++) {
            cout << assignSol[i] << " ";
        }
        cout << endl;
    }

    // greedy algorithm solution
    int greedySolution() {       
        for(int i = 0; i < m; i++) {
            int *customeriCost = assignCost[i];
            int minCost = customeriCost[0], index = 0;
            for(int j = 1; j < n; j++) {               
                if (customeriCost[j] < minCost && capacity[j] >= demand[i]) {
                    minCost = customeriCost[j];
                    index = j;
                }               
            }
            result += minCost;        
            if (statusOfFac[index] == 0) { 
                statusOfFac[index] = 1;
                result += openingCost[index];
            }
            capacity[index] -= demand[i];
            assignSol[i] = index + 1;
        }
        return result;
    }

    // Simulated Annealing algorithm solution
    int SASolution() {
        // Greedy Algorithm generate init status
        int *sumOfDemand = new int[n]; // The sum of demand assigned to the facilities
        for(int i = 0; i < n; i++) {
            sumOfDemand[i] = 0;
        }
        for(int i = 0; i < m; i++) {
            int *customeriCost = assignCost[i];
            int minCost = customeriCost[0], index = 0;
            for(int j = 1; j < n; j++) {               
                if (customeriCost[j] < minCost && capacity[j] >= sumOfDemand[j]) {
                    minCost = customeriCost[j];
                    index = j;
                }               
            }
            result += minCost;        
            if (statusOfFac[index] == 0) { 
                statusOfFac[index] = 1;
                result += openingCost[index];
            }
            sumOfDemand[index] += demand[i];
            assignSol[i] = index + 1;
        }
        double T = INIT_TEMP; // The temperature
        srand(time(NULL));
        while(T > 0.1){
            for(int i = 0; i < ITERATION_NUM; i++) {               
                for(int i = 0; i < m; i++) {
                    // generate new status
                    int ranNum = rand() % n;
                    int oldNum = assignSol[i] - 1;            
                    if (ranNum == oldNum) {
                        continue;
                    }
                    int change = assignCost[i][ranNum] - assignCost[i][oldNum];
                    bool fromZero = sumOfDemand[ranNum] == 0;
                    bool toZero = sumOfDemand[oldNum] - demand[i] == 0;              
                    if (fromZero) {
                        change += openingCost[ranNum];
                    }                   
                    if(toZero) {
                        change -= openingCost[oldNum];
                    }
                    // determine to receive new status
                    if(change < 0 || (rand() % 1000 / 1000.0) < exp(-1 * change / T)) {
                        if (fromZero) {
                            statusOfFac[ranNum] = 1;
                        }                   
                        if(toZero) {
                            statusOfFac[oldNum] = 0;
                        }
                        sumOfDemand[ranNum] += demand[i];
                        sumOfDemand[oldNum] -= demand[i];
                        assignSol[i] = ranNum + 1;
                        result += change;
                    }
                }
                
            }
            T *= RATE; // Reduce the temperature
        }
        delete []sumOfDemand;
        return result;
    }
 
private:
    int n; // No.facility
    int m; // No.customer
    int *capacity; // Capacity of n facilities
    int *openingCost; // Opening cost of n facilities
    int *demand; // Demand of m customers
    int **assignCost; // Assignment cost, No.row - No.customer, No.column - No.facility

    int result; // The minimize sum of the opening cost and the assignment cost
    int *statusOfFac; // Status of facilities
    int *assignSol; // The assignment of customers to facilities

    void init(string& filePath) {
        // read data from file
        ifstream in;
        in.open(filePath);

        in >> n;
        in >> m;

        // allocate memory for array
        capacity = new int[n];
        openingCost = new int[n];
        demand = new int[m];
        assignCost = new int*[m];    
        for(int i = 0; i < m; i++) {
            assignCost[i] = new int[n];
        }
        statusOfFac = new int[n];
        assignSol = new int[m];

        // put data into array
        for(int i = 0; i < n; i++) {
            in >> capacity[i];
            in >> openingCost[i];
        }  
        float temp;
        for(int i = 0; i < m; i++) {
            in >> temp;
            demand[i] = (int)temp;
        }        
        for(int i = 0; i < m; i++) {           
            for(int j = 0; j < n; j++){
                in >> temp;
                assignCost[i][j] = temp;
            }
            
        }
        in.close();
        
        // init the array to record result
        for(int i = 0; i < n; i++) {
            statusOfFac[i] = 0;
        }
        for(int i = 0; i < m; i++) {
            assignSol[i] = 0;
        }       
    }
};
