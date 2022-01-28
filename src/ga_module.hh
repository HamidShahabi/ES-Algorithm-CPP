#pragma once
#include <systemc.h>

#include "genetic_algorithm_types.hh"
#include "population_updator_module.hh"
#include "software_part.hh"

using namespace GeneticAlgorithmTypes;

SC_MODULE(GeneticAlgorithm)
{
  sc_in<int> size_of_population;
  sc_in<int> number_of_items;
  sc_in<double> p_cross_over, p_mutation;
  sc_in<int> weight_limit;
  std::vector<Item> items;
  FinishCondition finish_condition;
  Chromosome answer;
  AlgorithmState algorithm_state;
  PopulationUpdator population_updator;

  sc_event do_update, update_done;

  int get_random_number(int range)
  {
    return 1 + std::rand() / ((RAND_MAX + 1u) / range);
  }

  void calculate_fitness(Population & population)
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

      if (sum_of_weights > weight_limit.read())
        fitness = 0;

      chromosome.fitness = fitness;
    }
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

  Chromosome start_algorithm()
  {
    auto population = SoftwarePart::generate_random_population(
        size_of_population.read(), number_of_items.read());

    while (true)
    {
      calculate_fitness(population);

      SoftwarePart::update_algorithm_state(
          algorithm_state, get_best_fit(population).fitness, population);

      // SoftwarePart::print_population(population); // for debug purpose

      if (SoftwarePart::are_requirements_met(algorithm_state, finish_condition,
                                             get_best_fit(population).fitness))
        break;

      population_updator.population = &population;
      do_update.notify();
      wait(update_done);
    }

    return get_best_fit(population);
  }

  void foo() { answer = start_algorithm(); }

  typedef GeneticAlgorithm SC_CURRENT_USER_MODULE;
  GeneticAlgorithm(::sc_core::sc_module_name, const std::vector<Item> &i_items,
                   const FinishCondition &i_finish_condition)
      : items(i_items), finish_condition(i_finish_condition),
        population_updator("population_updator")
  {
    population_updator.p_cross_over(p_cross_over);
    population_updator.p_mutation(p_mutation);
    population_updator.do_update = &do_update;
    population_updator.update_done = &update_done;

    std::srand(
        std::time(nullptr)); // use current time as seed for random generator
    SC_THREAD(foo);
  }
};
