#pragma once
#include <algorithm>
#include <assert.h>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <vector>

enum class Gen
{
  ENABLE,
  DISABLE,
};

struct Chromosome
{
  std::vector<Gen> gens;
  double fitness;

  Chromosome() : gens(), fitness(0) {}
};

struct Population
{
  std::vector<Chromosome> chromosomes;
};

struct Item
{
  int value, weight;

  Item(int i_v, int i_w) : value(i_v), weight(i_w) {}
};

enum class FinishConditionType
{
  NUMBER_OF_UPDATES,
  REQUIRED_FITNESS,
  CONVERGENCY_OF_FITNESS,
};


struct FinishCondition
{
  FinishConditionType finish_condition_type;
  int number_of_updates;
  double required_fitness;
  int convergence_accuracy;
};

struct AlgorithmState
{
  int number_of_updates;
  double previous_fitness;
  int convergence_counter;

  AlgorithmState()
      : number_of_updates(0), previous_fitness(0), convergence_counter(1)
  {
  }
};

/*
Lesson1

abandon-keen-jealous-tact-oath-vacant-hardship-gallant-data-unaccustomed-bachelor-qualify
give up-fond- -thoughtfulness/skill-vow-unfilled-adversity-courageous-uncommon-

*/
struct GeneticAlgorithm
{
  int number_of_items, size_of_population;
  std::vector<Item> items;
  double p_cross_over, p_mutation;
  int weight_limit;
  FinishCondition finish_condition;
  AlgorithmState algorithm_state;

  GeneticAlgorithm(const std::vector<int> &i_items_values,
                   const std::vector<int> &i_items_weights,
                   int i_size_of_population, double i_p_cross_over,
                   double i_p_mutation, int i_weight_limit,
                   FinishCondition i_finish_condition)
      : number_of_items(i_items_values.size()),
        size_of_population(i_size_of_population), p_cross_over(i_p_cross_over),
        p_mutation(i_p_mutation), weight_limit(i_weight_limit),
        finish_condition(i_finish_condition)
  {
    assert(i_items_values.size() == i_items_weights.size());

    std::srand(
        std::time(nullptr)); // use current time as seed for random generator

    for (int i = 0; i < i_items_values.size(); i++)
      items.emplace_back(i_items_values[i], i_items_weights[i]);
  }

  int get_random_number(int range)
  {
    return 1 + std::rand() / ((RAND_MAX + 1u) / range);
  }

  Chromosome generate_random_chromosome()
  {
    Chromosome chromosome;

    for (int i = 0; i < number_of_items; i++)
    {
      auto random_number = get_random_number(10);
      chromosome.gens.push_back((random_number > 5) ? Gen::ENABLE
                                                    : Gen::DISABLE);
    }

    return chromosome;
  }

  Population generate_random_population()
  {
    Population population;

    for (int i = 0; i < size_of_population; i++)
      population.chromosomes.push_back(generate_random_chromosome());

    return population;
  }

  void calculate_fitness(Population &population)
  {
    for (auto &chromosome : population.chromosomes)
    {
      double fitness = 0.0;
      int sum_of_weights = 0;
      for (int i = 0; i < chromosome.gens.size(); i++)
        if (chromosome.gens[i] == Gen::ENABLE)
        {
          fitness += static_cast<double>(items[i].value) /
                     static_cast<double>(items[i].weight);
          sum_of_weights += items[i].weight;
        }

      if (sum_of_weights > weight_limit)
        fitness = 0;

      chromosome.fitness = fitness;
    }
  }

  void print_population(const Population &population)
  {
    for (const auto &chromosome : population.chromosomes)
    {
      std::cout << "[";
      for (const auto &gen : chromosome.gens)
        std::cout << (gen == Gen::ENABLE ? "1, " : "0, ");
      std::cout << "] : " << chromosome.fitness << std::endl;
    }
    std::cout << std::endl;
  }

  void mutate(Chromosome &chromosome)
  {
    for (auto &gen : chromosome.gens)
      if (get_random_number(100) <= 15)
      {
        gen = (gen == Gen::DISABLE ? Gen::ENABLE : Gen::DISABLE);
      }
  }

  void cross_over(Chromosome &c1, Chromosome &c2)
  {
    int cross_point = get_random_number(c1.gens.size() - 1);

    for (int i = 0; i < cross_point; i++)
      std::swap(c1.gens[i], c2.gens[i]);
  }

  void update_population(Population &population)
  {
    std::sort(population.chromosomes.begin(), population.chromosomes.end(),
              [](const Chromosome &c1, const Chromosome &c2) {
                return c1.fitness > c2.fitness;
              });

    for (int i = population.chromosomes.size() % 2;
         i < population.chromosomes.size() - 1; i += 2)
      if (get_random_number(100) <= p_cross_over * 100)
        cross_over(population.chromosomes[i], population.chromosomes[i + 1]);

    for (auto &chromosome : population.chromosomes)
      if (get_random_number(100) <= p_mutation * 100)
        mutate(chromosome);
  }

  Chromosome get_best_fit(const Population &population)
  {
    Chromosome best_fit = population.chromosomes.front();

    for (const auto &chromosome : population.chromosomes)
      if (chromosome.fitness > best_fit.fitness)
        best_fit = chromosome;

    return best_fit;
  }

  bool are_requirements_met(const Population &population)
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
      return (get_best_fit(population).fitness >=
              finish_condition.required_fitness);
    }
    return false;
  }

  void update_algorithm_state(const Population &population)
  {
    if (get_best_fit(population).fitness == algorithm_state.previous_fitness)
    {
      algorithm_state.convergence_counter++;
    }
    else
    {
      algorithm_state.convergence_counter = 1;
      algorithm_state.previous_fitness = get_best_fit(population).fitness;
    }

    algorithm_state.number_of_updates += 1;
  }

  Chromosome start_algorithm()
  {
    auto population = generate_random_population();

    while (true)
    {
      calculate_fitness(population);

      update_algorithm_state(population);

      print_population(population);

      if (are_requirements_met(population))
        break;

      update_population(population);
    }

    return get_best_fit(population);
  }
};