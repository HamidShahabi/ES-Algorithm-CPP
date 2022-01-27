#include <iostream>

#include "genetic_algorithm.hh"



int main()
{
    std::vector<int> values = {1, 2, 1, 3};
    std::vector<int> weights = {2, 1, 4, 5};//0.5+2+0.6=3.1

    int size_of_population = 5;

    FinishCondition finish_condition;
    finish_condition.finish_condition_type = FinishConditionType::CONVERGENCY_OF_FITNESS;
    finish_condition.convergence_accuracy = 5;

    GeneticAlgorithm ga(values, weights, size_of_population, 0.3, 0.2, 8, finish_condition);

    auto result = ga.start_algorithm();

    std::cout << "result choromosome : [";
    for (const auto &gen : result.gens)
        std::cout << (gen == Gen::ENABLE ? "1, " : "0, ");
    std::cout << "] : " << result.fitness << std::endl;

    return 0;
}