#include <systemc.h>

#include "ga_module.hh"


int sc_main(int argc, char *argv[])
{
  sc_signal<int> number_of_items;
  number_of_items.write(5);

  std::vector<int> values({1, 2, 1, 3, 1});
  std::vector<int> weights({2, 1, 4, 5, 5}); // 0.5 + 2 + 0.6 = 3.1
  std::vector<Item> items;
  for (int i = 0; i < values.size(); i++)
    items.emplace_back(values[i], weights[i]);

  sc_signal<double> p_co, p_mu;
  p_co.write(0.2);
  p_mu.write(0.1);

  sc_signal<int> size_of_population;
  size_of_population.write(4);

  sc_signal<int> weight_limit;
  weight_limit.write(8);

  FinishCondition finish_condition;
  finish_condition.finish_condition_type =
  FinishConditionType::REQUIRED_FITNESS;
  finish_condition.required_fitness = 1.2;


  GeneticAlgorithm ga("genetic algo", items, finish_condition);
  ga.size_of_population(size_of_population);
  ga.number_of_items(number_of_items);
  ga.p_cross_over(p_co);
  ga.p_mutation(p_mu);
  ga.weight_limit(weight_limit);
  auto &answer = ga.answer;

  sc_start();
  std::cout << answer << std::endl;


  return 0;
}
