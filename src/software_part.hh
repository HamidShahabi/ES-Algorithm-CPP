#pragma once
#include <algorithm>
#include <assert.h>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <vector>

#include "genetic_algorithm_types.hh"

using namespace GeneticAlgorithmTypes;

namespace SoftwarePart
{
int get_random_number(int range)
{
  return 1 + std::rand() / ((RAND_MAX + 1u) / range);
}

static void update_algorithm_state(AlgorithmState &algorithm_state,
                                   double best_fitness,
                                   const Population &population)
{
  if (best_fitness == algorithm_state.previous_fitness)
    algorithm_state.convergence_counter++;
  else
  {
    algorithm_state.convergence_counter = 1;
    algorithm_state.previous_fitness = best_fitness;
  }

  algorithm_state.number_of_updates += 1;
}

static Chromosome generate_random_chromosome(int number_of_items)
{
  Chromosome chromosome;

  for (int i = 0; i < number_of_items; i++)
  {
    auto random_number = get_random_number(10);
    chromosome.gens.push_back((random_number > 5) ? Gen::ENABLE : Gen::DISABLE);
  }

  return chromosome;
}

static Population generate_random_population(int size_of_population,
                                             int number_of_items)
{
  Population population;

  for (int i = 0; i < size_of_population; i++)
    population.chromosomes.push_back(
        generate_random_chromosome(number_of_items));

  return population;
}

static void print_population(const Population &population)
{
  std::cout << population << std::endl;
}

static bool are_requirements_met(const AlgorithmState &algorithm_state,
                                 const FinishCondition &finish_condition,
                                 double best_fitness)
{
  switch (finish_condition.finish_condition_type)
  {
  case FinishConditionType::CONVERGENCY_OF_FITNESS:
    return algorithm_state.convergence_counter >=
           finish_condition.convergence_accuracy;
  case FinishConditionType::NUMBER_OF_UPDATES:
    return algorithm_state.number_of_updates >=
           finish_condition.number_of_updates;
  case FinishConditionType::REQUIRED_FITNESS:
    return (best_fitness >= finish_condition.required_fitness);
  }
  return false;
}

} // namespace SoftwarePart
