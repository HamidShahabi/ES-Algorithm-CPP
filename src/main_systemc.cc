#include <systemc.h>

#include "ga_module.hh"

int sc_main(int argc, char *argv[])
{
  sc_signal<int> number_of_items;
  sc_signal<double> probability_cross_over, probability_mutation;
  sc_signal<int> size_of_population;
  sc_signal<int> weight_limit;
  FinishCondition finish_condition;
  probability_cross_over.write(0.2); // probability_cross_over
  probability_mutation.write(0.1);   // probability_mutation
  size_of_population.write(4);       // size of population

  std::vector<int> values({1, 2, 1, 3, 1});
  std::vector<int> weights({2, 1, 4, 5, 5}); // 0.5 + 2 + 0.6 = 3.1
  std::vector<Item> items;
  for (int i = 0; i < values.size(); i++)
    items.emplace_back(values[i], weights[i]);
  number_of_items.write(items.size());

  // finish condition
  finish_condition.finish_condition_type = FinishConditionType::REQUIRED_FITNESS;
  finish_condition.required_fitness = 1.2;

  // knap-sack extra condition
  weight_limit.write(8);

  GeneticAlgorithm genetic_algorithm("genetic algo", items, finish_condition);
  genetic_algorithm.size_of_population(size_of_population);
  genetic_algorithm.number_of_items(number_of_items);
  genetic_algorithm.probability_cross_over(probability_cross_over);
  genetic_algorithm.probability_mutation(probability_mutation);
  genetic_algorithm.weight_limit(weight_limit);

  auto &answer = genetic_algorithm.answer;

  sc_start();
  std::cout << answer << std::endl;

  return 0;
}
