//
// Created by Fesh on 9/24/2021.
//


#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include <bits/stdc++.h>

using namespace std;

// Item structure, will hold each item which has a weight and value
struct Item{
    int Weight;
    int Value;
};

// Knapsack structure, will hold  total weight, total value, a vector of whether each "bit" (representing individual items) is on or off, and fitness (fitness is equal to total value).
struct Knapsack{
    int Fitness{};
    int TotalWeight{};
    int TotalValue{};
    vector<bool> ItemsInBag;
    vector<Item> Members;
};

// Use to sort knapsack vectors
bool operator<(const Knapsack& a, const Knapsack& b){
    return a.Fitness > b.Fitness;
}

// Main workhorse of program, contains the main loop algorithm.
void survivalOfTheFittest(vector<Knapsack> knapsacks, int pop, int maxWeight, int numGenerations, int numRunsPerGen, double crossover, double mutation, const vector<Item>& generatedPopulation){
    // Copy the initial population of knapsacks
    vector<Knapsack> oldKnapsacks = std::move(knapsacks);

    // Sort them in descending order
    sort(oldKnapsacks.begin(), oldKnapsacks.end());

    int generations = 0;

    // Loop through until amount of generations is met
    for(int i = 0; i < numGenerations; i++){
        generations++;

        // Make a new knapsack vector to hold new knapsacks
        vector<Knapsack> newKnapsacks;

        newKnapsacks.push_back(oldKnapsacks.at(0));
        newKnapsacks.push_back(oldKnapsacks.at(1));

        oldKnapsacks.erase(oldKnapsacks.begin());
        oldKnapsacks.erase(oldKnapsacks.begin());

        // Loop until new knapsack is full with parent/child pairs
        while(newKnapsacks.size() < numRunsPerGen){

            // Choose two knapsacks at random
            Knapsack firstSelected = oldKnapsacks.at(rand() % oldKnapsacks.size());
            Knapsack secondSelected = oldKnapsacks.at(rand() % oldKnapsacks.size());
            Knapsack thirdSelected = oldKnapsacks.at(rand() % oldKnapsacks.size());
            Knapsack fourthSelected = oldKnapsacks.at(rand() % oldKnapsacks.size());

            Knapsack winnerOne;
            Knapsack winnerTwo;

            if(firstSelected.Fitness > secondSelected.Fitness)
                winnerOne = firstSelected;
            else
                winnerOne = secondSelected;

            if(thirdSelected.Fitness > fourthSelected.Fitness)
                winnerTwo = thirdSelected;
            else
                winnerTwo = fourthSelected;

            // Randomly decide whether crossover will occur
            if(double(rand() % 101) < crossover){
                // Create a splitpoint in the DNA where it will be mixed
                int splitPoint = rand() % pop;

                // Taking first half of Parent One DNA and second half of Parent Two DNA
                vector<bool> dnaChildOneOne(winnerOne.ItemsInBag.begin(), winnerOne.ItemsInBag.begin() + splitPoint);
                vector<bool> dnaChildOneTwo(winnerTwo.ItemsInBag.begin() + splitPoint, winnerTwo.ItemsInBag.end());

                // Create first child with (initially) just first half of DNA from Parent One
                vector<bool> childOne = dnaChildOneOne;

                // Finished creation of Child One by concatonating second half of Parent Two
                childOne.insert(childOne.end(), dnaChildOneTwo.begin(), dnaChildOneTwo.end());

                // Same thing as Child One but using first half of Parent Two and second half of Parent One
                vector<bool> dnaChildTwoOne(winnerOne.ItemsInBag.begin() + splitPoint, winnerOne.ItemsInBag.end());
                vector<bool> dnaChildTwoTwo(winnerTwo.ItemsInBag.begin(), winnerTwo.ItemsInBag.begin() + splitPoint);

                vector<bool> childTwo = dnaChildTwoTwo;

                childTwo.insert(childTwo.end(), dnaChildTwoOne.begin(), dnaChildTwoOne.end());

                // Creating the knapsack of child one
                Knapsack knapsackChildOne;

                knapsackChildOne.Fitness = 0;
                knapsackChildOne.TotalWeight = 0;
                knapsackChildOne.TotalValue = 0;
                knapsackChildOne.Members = generatedPopulation;
                knapsackChildOne.ItemsInBag = childOne;

                // Creating the knapsack of child two
                Knapsack knapsackChildTwo;

                knapsackChildTwo.Fitness = 0;
                knapsackChildTwo.TotalWeight = 0;
                knapsackChildTwo.TotalValue = 0;
                knapsackChildTwo.Members = generatedPopulation;
                knapsackChildTwo.ItemsInBag = childTwo;

                // Mutation function, for loop is to decide if child one or two the mutation will occur to
                for(int k = 0; k < 2; k++){
                    if(double(rand() % 101) < mutation){
                        // select a random number for the genome
                        int randomGenome = rand() % pop;
                        // Flip to opposite value
                        if(k == 0){
                            if(knapsackChildOne.ItemsInBag.at(randomGenome))
                                knapsackChildOne.ItemsInBag.at(randomGenome) = false;
                            else
                                knapsackChildOne.ItemsInBag.at(randomGenome) = true;
                        }
                        else{
                            if(knapsackChildTwo.ItemsInBag.at(randomGenome))
                                knapsackChildTwo.ItemsInBag.at(randomGenome) = false;
                            else
                                knapsackChildTwo.ItemsInBag.at(randomGenome) = true;
                        }
                    }
                }

                // Using new DNA to get weight/value of knapsacks
                for(int j = 0; j < pop; j++){
                    if(knapsackChildOne.ItemsInBag.at(j)){
                        if(knapsackChildOne.TotalWeight + knapsackChildOne.ItemsInBag.at(j) > maxWeight)
                            knapsackChildOne.ItemsInBag.at(j) = false;
                        else{
                            knapsackChildOne.TotalWeight += knapsackChildOne.Members.at(j).Weight;
                            knapsackChildOne.TotalValue += knapsackChildOne.Members.at(j).Value;
                        }
                    }
                    if(knapsackChildTwo.ItemsInBag.at(j)){
                        if(knapsackChildTwo.TotalWeight + knapsackChildTwo.ItemsInBag.at(j) > maxWeight)
                            knapsackChildTwo.ItemsInBag.at(j) = false;
                        else{
                            knapsackChildTwo.TotalWeight += knapsackChildTwo.Members.at(j).Weight;
                            knapsackChildTwo.TotalValue += knapsackChildTwo.Members.at(j).Value;
                        }
                    }
                }

                if(knapsackChildOne.TotalWeight > maxWeight)
                    knapsackChildOne.Fitness = 0;
                else
                    knapsackChildOne.Fitness = knapsackChildOne.TotalValue;

                if(knapsackChildTwo.TotalWeight > maxWeight)
                    knapsackChildTwo.Fitness = 0;
                else
                    knapsackChildTwo.Fitness = knapsackChildTwo.TotalValue;


                // Push the new knapsacks to the newKnapsacks list
                newKnapsacks.push_back(knapsackChildOne);
                newKnapsacks.push_back(knapsackChildTwo);
            }
            // If crossover doesnt occur, roll for mutation on parents and add them to newKnapsacks
            else{
                for(int j = 0; j < 2; j++){
                    if(double(rand() % 101) < mutation){
                        int randomGenome = rand() % pop;
                        if(j == 0){
                            if(winnerOne.ItemsInBag.at(randomGenome)){
                                winnerOne.ItemsInBag.at(randomGenome) = false;
                                winnerOne.TotalWeight -= winnerOne.Members.at(randomGenome).Weight;
                                winnerOne.TotalValue -= winnerOne.Members.at(randomGenome).Value;
                            }
                            else{
                                if(winnerOne.TotalWeight + winnerOne.ItemsInBag.at(randomGenome) > maxWeight)
                                    continue;
                                else{
                                    winnerOne.ItemsInBag.at(randomGenome) = true;
                                    winnerOne.TotalWeight += winnerOne.Members.at(randomGenome).Weight;
                                    winnerOne.TotalValue += winnerOne.Members.at(randomGenome).Value;

                                }
                            }
                        }
                        else{
                            if(winnerTwo.ItemsInBag.at(randomGenome)) {
                                winnerTwo.ItemsInBag.at(randomGenome) = false;
                                winnerTwo.TotalWeight -= winnerTwo.Members.at(randomGenome).Weight;
                                winnerTwo.TotalValue -= winnerTwo.Members.at(randomGenome).Value;
                            }
                            else {
                                if(winnerTwo.TotalWeight + winnerTwo.ItemsInBag.at(randomGenome) > maxWeight)
                                    continue;
                                else{
                                    winnerTwo.ItemsInBag.at(randomGenome) = true;
                                    winnerTwo.TotalWeight += winnerTwo.Members.at(randomGenome).Weight;
                                    winnerTwo.TotalValue += winnerTwo.Members.at(randomGenome).Value;
                                }
                            }
                        }
                    }
                }

                if(winnerOne.TotalWeight > maxWeight)
                    winnerOne.Fitness = 0;
                else
                    winnerOne.Fitness = winnerOne.TotalValue;

                if(winnerTwo.TotalWeight > maxWeight)
                    winnerTwo.Fitness = 0;
                else
                    winnerTwo.Fitness = winnerTwo.TotalValue;

                newKnapsacks.push_back(winnerOne);
                newKnapsacks.push_back(winnerTwo);
            }
        }

        // New knapsacks are now the current (old) generation to be parents
        oldKnapsacks = newKnapsacks;

        // Sort new (now old) knapsacks
        sort(oldKnapsacks.begin(), oldKnapsacks.end());

        cout << "Generation " << generations << endl;
        cout << "Best Total Weight: " << oldKnapsacks.at(0).TotalWeight << " Best Fitness: " << oldKnapsacks.at(0).Fitness << endl;

    }
}


vector<Knapsack> initialKnapsackPopulation(vector<Item> items, int pop, int maxWeight, int numRunsPerGen){

    vector<Knapsack> generatedKnapsacks;

    for(int i = 0; i < numRunsPerGen; i++){
        vector<bool> itemsInBag(pop, false);
        Knapsack tempKnapsack{0, 0, 0, itemsInBag, items};
        for(int j = 0; j < pop; j++){
            if(rand() % 2 == 1) {
                if(tempKnapsack.TotalWeight + items.at(j).Weight > maxWeight)
                    continue;
                else{
                    tempKnapsack.ItemsInBag.at(j) = true;
                    tempKnapsack.TotalWeight += items.at(j).Weight;
                    tempKnapsack.TotalValue += items.at(j).Value;
                }
            }
        }

        tempKnapsack.Fitness = tempKnapsack.TotalValue;
        generatedKnapsacks.push_back(tempKnapsack);
    }

    return generatedKnapsacks;
}

// Creates all of our items to start run
vector<Item> createItems(int pop){

    vector<Item> generatedItems;
    timeval t1{};
    mingw_gettimeofday(&t1, nullptr);
    srand(t1.tv_usec * t1.tv_sec);

    for(int i = 0; i < pop; i++){
        Item tempMember{};
        tempMember.Value = rand() % 101;
        tempMember.Weight = rand() % 501;

        generatedItems.push_back(tempMember);
    }

    return generatedItems;
}

void geneticAlgorithm(int pop, double crossover, double mutation, int maxWeight, int runs, int numberOfRunsPerGeneration){
    vector<Item> generatedPopulation;

    ifstream in(R"(C:\Users\Fesh\CLionProjects\CSCIU580\Assignment2\test01.txt)");
    string line;


    while(getline(in, line)){
        stringstream ss;
        int weight, value;
        ss<<line;

        ss>>weight>>value;

        Item temp{};
        temp.Weight = weight;
        temp.Value = value;

        generatedPopulation.push_back(temp);
    }

    vector <Knapsack> initialKnapsacks = initialKnapsackPopulation(generatedPopulation, pop, maxWeight, numberOfRunsPerGeneration);
    survivalOfTheFittest(initialKnapsacks, pop, maxWeight, runs, numberOfRunsPerGeneration, crossover, mutation, generatedPopulation);
}

int main(){

    double mutation{};
    double crossover{};
    int items{};
    int maxWeight{};
    int numberOfRuns{};
    int numberOfRunsPerGeneration{};

    cout << "What mutation chance do you want? (From 0 to 10, with 1 being 10% chance) ";
    cin >> mutation;
    cout << "What crossover rate do you want? (From 0 to 10, with 1 being 10% chance) ";
    cin >> crossover;
    cout << "How many items do you want to choose from? ";
    cin >> items;
    cout << "What is the max weight of the knapsack? ";
    cin >> maxWeight;
    cout << "How many generations? ";
    cin >> numberOfRuns;
    cout << "What is the population per generation (AKA how many knapsacks will be filled per generation?)  ";
    cin >> numberOfRunsPerGeneration;

    // Passing all arguments to genetic algorithm
    geneticAlgorithm(items, crossover, mutation, maxWeight, numberOfRuns, numberOfRunsPerGeneration);

}
